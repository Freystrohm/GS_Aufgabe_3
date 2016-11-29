/*
 * bmpEinlesen.c
 *
 *  Created on: 04.11.2016
 *      Author: Johannes
 */

#include "bmpVerwalten.h"
#include <stdio.h>
#include <stdlib.h>

//#include <inttypes.h>

#define BIT8 8
#define BIT24 24
#define SIZEPAL 256
#define STDHEADSIZE 54

RGBTRIPLE* bildDatenLesen(FILE *file, BITMAPINFOHEADER infoHeader,
		BITMAPFILEHEADER fileheader);
void decompress(RGBTRIPLE *pbild, int hoehe, int breite);

void farbenSchreiben(FILE *file, RGBTRIPLE *pbild, BITMAPINFOHEADER infoHeader);

typedef struct tagRGBQUAD {
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
} RGBQUAD;

void hello(void) {
	printf("hallo aus einlesen");
}

bmpBild* einlesen(char const *filename) {
	FILE *file = fopen(filename, "rb");
	if (file == NULL) {
		return NULL;
	}
	bmpBild bild;

	//lesen FileHeader
	fread(&bild.fileHeader.bfType, sizeof(WORD), 1, file);
	fread(&bild.fileHeader.bfSize, sizeof(DWORD), 1, file);
	fread(&bild.fileHeader.bfReserved1, sizeof(WORD), 1, file);
	fread(&bild.fileHeader.bfReserved2, sizeof(WORD), 1, file);
	fread(&bild.fileHeader.bfOffBits, sizeof(DWORD), 1, file);

	//Lesen InfoHeader
	fread(&bild.infoHeader.biSize, sizeof(DWORD), 1, file);
	fread(&bild.infoHeader.biWidth, sizeof(LONG), 1, file);
	fread(&bild.infoHeader.biHeight, sizeof(LONG), 1, file);
	fread(&bild.infoHeader.biPlanes, sizeof(WORD), 1, file);
	fread(&bild.infoHeader.biBitCount, sizeof(WORD), 1, file);
	fread(&bild.infoHeader.biCompression, sizeof(DWORD), 1, file);
	fread(&bild.infoHeader.biSizeImage, sizeof(DWORD), 1, file);
	fread(&bild.infoHeader.biXPelsPerMeter, sizeof(LONG), 1, file);
	fread(&bild.infoHeader.biYPelsPerMeter, sizeof(LONG), 1, file);
	fread(&bild.infoHeader.biClrUsed, sizeof(DWORD), 1, file);
	fread(&bild.infoHeader.biClrImportant, sizeof(DWORD), 1, file);

	fseek(file, 0L, SEEK_END);
	int size = ftell(file);
	if (size != bild.fileHeader.bfSize) {
		return NULL;
	}
	//double sizepal = bild.fileHeader.bfOffBits - sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	bild.bildaten = bildDatenLesen(file, bild.infoHeader, bild.fileHeader);

	if (bild.bildaten == NULL) {
		return NULL;
	}
	bild.fileHeader.bfOffBits = STDHEADSIZE;
	bild.infoHeader.biBitCount = BIT24;
	bild.infoHeader.biCompression = 0;

	bmpBild *pbild;
	pbild = malloc(sizeof(bild));
	*pbild = bild;

	fclose(file);

	return pbild;
}

RGBTRIPLE* bildDatenLesen(FILE *file, BITMAPINFOHEADER infoHeader,
		BITMAPFILEHEADER fileheader) {
	RGBTRIPLE *pbild = malloc(
			sizeof(RGBTRIPLE) * infoHeader.biHeight * infoHeader.biWidth);
	fseek(file, fileheader.bfOffBits, SEEK_SET);

	if (infoHeader.biCompression == 1) {
		BYTE code[2] = { 1, 0 };
		for (int i = 0; !(code[0] == 0 && code[1] == 1); i += 2) {
			fread(code, sizeof(BYTE), 2, file);

			if (feof(file) && !(code[0] == 0 && code[1] == 1)) {
				return NULL;
			}

//			int cont = fread(&code[0], sizeof(BYTE), 1, file);
//			int error = ferror(file);
//			int end = feof(file);
//			fread(&code[1], sizeof(BYTE), 1, file);
//			if ((code[0] == 1 && code[1] == 208) || i == 492) {
//				printf("hier %d", i);
//			}

			pbild[i].rgbtRed = code[0];
			pbild[i + 1].rgbtRed = code[1];
//			printf("%d|%d", (int) code[0], (int) code[1]);
//			printf("\n");
//			fflush(stdout);
		}
		decompress(pbild, infoHeader.biHeight, infoHeader.biWidth);
	} else if (infoHeader.biBitCount == BIT8) {
		for (int i = 0; i < infoHeader.biHeight * infoHeader.biWidth; i++) {
			fread(&pbild[i].rgbtRed, sizeof(unsigned char), 1, file);
		}
	} else {
		fread(pbild, sizeof(RGBTRIPLE),
				infoHeader.biHeight * infoHeader.biWidth, file);
	}

	if (infoHeader.biBitCount == BIT8 && pbild != NULL) {
		farbenSchreiben(file, pbild, infoHeader);
	}
	return pbild;
}

void decompress(RGBTRIPLE *pbild, int hoehe, int breite) {
	unsigned char compresseddata[hoehe * breite], code[2] = { 1, 5 };
	for (int i = 0; i < hoehe * breite; i++) {
		compresseddata[i] = pbild[i].rgbtRed;
	}
	int bildpos = 0;
	for (int i = 0; code[0] != 0 || code[1] != 1; i += 2) {
		code[0] = compresseddata[i];
		code[1] = compresseddata[i + 1];

		if (bildpos % breite == 0 && code[0] != 0
				&& (code[1] != 0 || code[1] != 1)) {
			pbild = NULL;
			return;
		}

		if (code[0] == 0) {
			if (code[1] == 2) {
				i += 2;
				code[0] = compresseddata[i];
				code[1] = compresseddata[i + 1];
				bildpos += code[0] + code[1] * hoehe;
			} else if (code[1] > 2) {
				int zahl = code[1];
				for (int j = 0; j < zahl; j += 2) {
					i += 2;
					code[0] = compresseddata[i];
					code[1] = compresseddata[i + 1];
					pbild[bildpos].rgbtRed = code[0];
					bildpos++;
					if (zahl % 2 == 0 || zahl - j != 1) {
						pbild[bildpos].rgbtRed = code[1];
						bildpos++;
					}
				}
			} else {
				int zahl = code[0];
				for (int j = 0; j < zahl; j++) {
					pbild[bildpos].rgbtRed = code[1];
					bildpos++;
				}
			}
		}
	}

}

void farbenSchreiben(FILE *file, RGBTRIPLE *pbild, BITMAPINFOHEADER infoHeader) {
	RGBQUAD palette[SIZEPAL];
	fseek(file, STDHEADSIZE, SEEK_SET);
	uint8_t indize;
	for (int i = 0; i < SIZEPAL; i++) {
		fread(&palette[i].rgbBlue, sizeof(unsigned char), 1, file);
		fread(&palette[i].rgbGreen, sizeof(unsigned char), 1, file);
		fread(&palette[i].rgbRed, sizeof(unsigned char), 1, file);
		fread(&palette[i].rgbReserved, sizeof(unsigned char), 1, file);
	}

	for (int i = 0; i < infoHeader.biHeight * infoHeader.biWidth; i++) {

		indize = pbild[i].rgbtRed;
		pbild[i].rgbtBlue = palette[indize].rgbBlue;
		pbild[i].rgbtGreen = palette[indize].rgbGreen;
		pbild[i].rgbtRed = palette[indize].rgbRed;

	}

}

RGBTRIPLE* bildDatenLesenAlt(FILE *file, BITMAPINFOHEADER infoHeader,
		BITMAPFILEHEADER fileheader) {
	fseek(file, fileheader.bfOffBits, SEEK_SET); //Lese zeiger an positon nach den Headern setzen
	RGBTRIPLE bilddaten[infoHeader.biHeight][infoHeader.biWidth];
	//RGBTRIPLE bilddaten[480][640];
	RGBQUAD palette[SIZEPAL];
	if (infoHeader.biBitCount == BIT8) {
		//fseek(file, -(SIZEPAL*sizeof(unsigned char)*3), SEEK_CUR);
		fseek(file, STDHEADSIZE, SEEK_SET);
		uint8_t indize;
		for (int i = 0; i < SIZEPAL; i++) {
			fread(&palette[i].rgbBlue, sizeof(unsigned char), 1, file);
			fread(&palette[i].rgbGreen, sizeof(unsigned char), 1, file);
			fread(&palette[i].rgbRed, sizeof(unsigned char), 1, file);
			fread(&palette[i].rgbReserved, sizeof(unsigned char), 1, file);
		}
		fseek(file, fileheader.bfOffBits, SEEK_SET);
		for (int i = 0; i < infoHeader.biHeight; i++) {
			for (int j = 0; j < infoHeader.biWidth; j++) {
				fread(&indize, sizeof(uint8_t), 1, file);
				bilddaten[i][j].rgbtBlue = palette[indize].rgbBlue;
				bilddaten[i][j].rgbtGreen = palette[indize].rgbGreen;
				bilddaten[i][j].rgbtRed = palette[indize].rgbRed;
			}
		}
	} else if (infoHeader.biBitCount == BIT24) {

		//fread(bilddaten, sizeof(bilddaten), 1, file);

		for (int i = 0; i < infoHeader.biHeight; i++) {
			for (int j = 0; j < infoHeader.biWidth; j++) {
				fread(&bilddaten[i][j].rgbtBlue, sizeof(unsigned char), 1,
						file);
				fread(&bilddaten[i][j].rgbtGreen, sizeof(unsigned char), 1,
						file);
				fread(&bilddaten[i][j].rgbtRed, sizeof(unsigned char), 1, file);
			}
		}

	}

	RGBTRIPLE *pbild;
	pbild = malloc(sizeof(bilddaten));
	for (int i = 0; i < infoHeader.biHeight; i++) {
		for (int j = 0; j < infoHeader.biWidth; j++) {
			pbild[(i * infoHeader.biWidth) + j] = bilddaten[i][j];
		}
	}
	//pbild = bilddaten;

	return pbild;
}

void speichern(char const *filename, bmpBild *bild) {
	FILE *file = fopen(filename, "w+");
//schreiben FileHeader
	fwrite(&bild->fileHeader.bfType, sizeof(WORD), 1, file);
	fwrite(&bild->fileHeader.bfSize, sizeof(DWORD), 1, file);
	fwrite(&bild->fileHeader.bfReserved1, sizeof(WORD), 1, file);
	fwrite(&bild->fileHeader.bfReserved2, sizeof(WORD), 1, file);
	fwrite(&bild->fileHeader.bfOffBits, sizeof(DWORD), 1, file);

//Schreiben InfoHeader
	fwrite(&bild->infoHeader.biSize, sizeof(DWORD), 1, file);
	fwrite(&bild->infoHeader.biWidth, sizeof(LONG), 1, file);
	fwrite(&bild->infoHeader.biHeight, sizeof(LONG), 1, file);
	fwrite(&bild->infoHeader.biPlanes, sizeof(WORD), 1, file);
	fwrite(&bild->infoHeader.biBitCount, sizeof(WORD), 1, file);
	fwrite(&bild->infoHeader.biCompression, sizeof(DWORD), 1, file);
	fwrite(&bild->infoHeader.biSizeImage, sizeof(DWORD), 1, file);
	fwrite(&bild->infoHeader.biXPelsPerMeter, sizeof(LONG), 1, file);
	fwrite(&bild->infoHeader.biYPelsPerMeter, sizeof(LONG), 1, file);
	fwrite(&bild->infoHeader.biClrUsed, sizeof(DWORD), 1, file);
	fwrite(&bild->infoHeader.biClrImportant, sizeof(DWORD), 1, file);

//Schreiben der Bilddaten
	for (int i = 0; i < bild->infoHeader.biWidth * bild->infoHeader.biHeight;
			i++) {
		fwrite(&bild->bildaten[i].rgbtBlue, sizeof(unsigned char), 1, file);
		fwrite(&bild->bildaten[i].rgbtGreen, sizeof(unsigned char), 1, file);
		fwrite(&bild->bildaten[i].rgbtRed, sizeof(unsigned char), 1, file);
	}
	fclose(file);
}

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
		BITMAPFILEHEADER filheader);

typedef struct tagRGBQUAD
{
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
} RGBQUAD;

void hello(void)
{
	printf("hallo aus einlesen");
}

bmpBild* einlesen(char const *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
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

	//double sizepal = bild.fileHeader.bfOffBits - sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	bild.bildaten = bildDatenLesen(file, bild.infoHeader, bild.fileHeader);

	bild.fileHeader.bfOffBits =STDHEADSIZE;
	bild.infoHeader.biBitCount = BIT24;
	bild.infoHeader.biCompression = 0;

	bmpBild *pbild;
	pbild = malloc(sizeof(bild));
	*pbild = bild;

	fclose(file);

	return pbild;
}

RGBTRIPLE* bildDatenLesen(FILE *file, BITMAPINFOHEADER infoHeader,
		BITMAPFILEHEADER fileheader)
{
	fseek(file, fileheader.bfOffBits, SEEK_SET); //Lese zeiger an positon nach den Headern setzen
	RGBTRIPLE bilddaten[infoHeader.biHeight][infoHeader.biWidth];
	//RGBTRIPLE bilddaten[480][640];
	RGBQUAD palette[SIZEPAL];
	if (infoHeader.biBitCount == BIT8)
	{
		//fseek(file, -(SIZEPAL*sizeof(unsigned char)*3), SEEK_CUR);
		fseek(file, STDHEADSIZE, SEEK_SET);
		uint8_t indize;
		for (int i = 0; i < SIZEPAL; i++)
		{
			fread(&palette[i].rgbBlue, sizeof(unsigned char), 1, file);
			fread(&palette[i].rgbGreen, sizeof(unsigned char), 1, file);
			fread(&palette[i].rgbRed, sizeof(unsigned char), 1, file);
			fread(&palette[i].rgbReserved, sizeof(unsigned char), 1, file);
		}
		fseek(file, fileheader.bfOffBits, SEEK_SET);
		for (int i = 0; i < infoHeader.biHeight; i++)
		{
			for (int j = 0; j < infoHeader.biWidth; j++)
			{
				fread(&indize, sizeof(uint8_t), 1, file);
				bilddaten[i][j].rgbtBlue = palette[indize].rgbBlue;
				bilddaten[i][j].rgbtGreen = palette[indize].rgbGreen;
				bilddaten[i][j].rgbtRed = palette[indize].rgbRed;
			}
		}
	} else if (infoHeader.biBitCount == BIT24)
	{

		//fread(bilddaten, sizeof(bilddaten), 1, file);

		for (int i = 0; i < infoHeader.biHeight; i++)
		{
			for (int j = 0; j < infoHeader.biWidth; j++)
			{
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
	for (int i = 0; i < infoHeader.biHeight; i++)
	{
		for (int j = 0; j < infoHeader.biWidth; j++)
		{
			pbild[(i*infoHeader.biWidth) + j] = bilddaten[i][j];
		}
	}
	//pbild = bilddaten;

	return pbild;
}

void speichern(char const *filename, bmpBild *bild)
{
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
	for (int i = 0; i < bild->infoHeader.biWidth * bild->infoHeader.biHeight; i++)
	{
		fwrite(&bild->bildaten[i].rgbtBlue,
				sizeof(unsigned char), 1, file);
		fwrite(&bild->bildaten[i].rgbtGreen,
				sizeof(unsigned char), 1, file);
		fwrite(&bild->bildaten[i].rgbtRed,
				sizeof(unsigned char), 1, file);
	}
	fclose(file);
}

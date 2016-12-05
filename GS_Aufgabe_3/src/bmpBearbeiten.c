#include "bmpBearbeiten.h"
#include "bmpVerwalten.h"
#include <stdlib.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define ARRAY_GROESSE 15

void grassfire(unsigned char *pbild, int hoehe, int breite,
		unsigned char* ergebnisbild);
int bildLeer(unsigned char *binBild, int hoehe, int breite);
void objektLoeschen(unsigned char *objekt, unsigned char *binBild, LONG hoehe,
		LONG breite);
void blauerRahmen(bmpBild *bild, RECHTECKINFO *objekte, int zaehler);
void ausgeben(RECHTECKINFO *objekte, int zaehler);

void dilatiere(unsigned char *pbild, int hoehe, int breite) {
	unsigned char *temp = malloc(sizeof(unsigned char) * hoehe * breite);
	for (int i = 0; i < hoehe * breite; i++) {
		temp[i] = pbild[i];
	}

	for (int i = 0; i < hoehe; i++) {
		for (int j = 0; j < breite; j++) {
			if (temp[breite * i + j] == 255 && i == 0 && j == 0) //Ausnahme erster Pixel
					{
				pbild[breite * (i + 1) + j] = 255;
				pbild[breite * i + j + 1] = 255;
			} else if (temp[breite * i + j] == 255 && j == breite - 1
					&& i == hoehe - 1) //Ausnahme letzter Pixel
							{
				pbild[breite * (i - 1) + j] = 255;
				pbild[breite * i + j - 1] = 255;
			} else if (temp[breite * i + j] == 255 && j == 0 && i == hoehe - 1) //Ausnahme Ecke unten links
					{
				pbild[breite * (i - 1) + j] = 255;
				pbild[breite * i + j + 1] = 255;
			} else if (temp[breite * i + j] == 255 && j == breite - 1 && i == 0) //Ausnahme Ecke oben rechts
					{
				pbild[breite * (i + 1) + j] = 255;
				pbild[breite * i + j - 1] = 255;
			}

			else if (temp[breite * i + j] == 255 && i == 0) //Ausnahme erste Reihe
					{
				pbild[breite * (i + 1) + j] = 255;
				pbild[breite * i + j + 1] = 255;
				pbild[breite * i + j - 1] = 255;
			} else if (temp[breite * i + j] == 255 && j == 0) //Ausnahme erste Spalte
					{
				pbild[breite * (i + 1) + j] = 255;
				pbild[breite * (i - 1) + j] = 255;
				pbild[breite * i + j + 1] = 255;
			} else if (temp[breite * i + j] == 255 && i == hoehe - 1) //Ausnahem letzte Reihe
					{
				pbild[breite * (i - 1) + j] = 255;
				pbild[breite * i + j + 1] = 255;
				pbild[breite * i + j - 1] = 255;
			} else if (temp[breite * i + j] == 255 && j == breite - 1) //Ausnahme letzte Spalte
					{
				pbild[breite * (i + 1) + j] = 255;
				pbild[breite * (i - 1) + j] = 255;
				pbild[breite * i + j - 1] = 255;
			} else if (temp[breite * i + j] == 255) {
				pbild[breite * (i + 1) + j] = 255;
				pbild[breite * (i - 1) + j] = 255;
				pbild[breite * i + j + 1] = 255;
				pbild[breite * i + j - 1] = 255;
			}
		}
	}

	free(temp);
}

void objektFinden(unsigned char *pbild, int hoehe, int breite,
		unsigned char **ergebnis) {
	//unsigned char bildarray[hoehe][breite];

	unsigned char *ergebnisbild = malloc(
			sizeof(unsigned char) * hoehe * breite);
	*ergebnis = ergebnisbild;
	for (int i = 0; i < hoehe * breite; i++) {
		ergebnisbild[i] = 0;
	}

	/*for (int i = 0; i < hoehe; i++)
	 {
	 for (int j = 0; j < breite; j++)
	 {
	 bildarray[i][j] = pbild[i * breite + j];
	 }
	 }*/

	for (int i = 0; i < hoehe * breite; i++) {
		if (pbild[i] == 255) {
			ergebnisbild[i] = 255;
			grassfire(pbild, hoehe, breite, ergebnisbild);
			return;
		}
	}
}

void grassfire(unsigned char *pbild, int hoehe, int breite,
		unsigned char *ergebnisbild) {
	int akt = 1, letzt = 0;
	while (letzt != akt) {
		letzt = akt;
		akt = 0;
		dilatiere(ergebnisbild, hoehe, breite);
		for (int j = 0; j < hoehe * breite; j++) {
			if (ergebnisbild[j] == 255 && pbild[j] != 255) {
				ergebnisbild[j] = 0;
			}
			if (ergebnisbild[j] == 255) {
				akt++;
			}
		}
	}

}
RECHTECKINFO* ueberpruefen(unsigned char *objekt, int hoehe, int breite) {
	RECHTECKINFO *rechteckInfo;
	int groesseObjekt = 0;
	int hoeheObjekt = 0;
	int breiteObjekt = 0;
	int istRechteck = 0;
	int posX = -1;
	int posY = -1;
	for (int i = 0; i < hoehe; i++) {
		for (int j = 0; j < breite; j++) {
			if (objekt[breite * i + j] == 255) {
				posX = j;
				posY = i;
				break;
			}
		}
		if(posX != -1)
		{
			break;
		}
	}
	for (int i = posX; i < breite; i++) {
		if (objekt[breite * posY + i] == 255) {
			breiteObjekt++;
		}
	}
	for (int i = posY; i < hoehe; i++) {
		if (objekt[breite * i + posX] == 255) {
			hoeheObjekt++;
		}
	}
	for (int i = 0; i < hoehe; i++) {
		for (int j = 0; j < breite; j++) {
			if (objekt[breite * i + j] == 255) {
				groesseObjekt++;
			}
		}
	}

	if (groesseObjekt == (breiteObjekt * hoeheObjekt)) {
		istRechteck = TRUE;
	} else {
		istRechteck = FALSE;
	}

	rechteckInfo = malloc(sizeof(RECHTECKINFO));
	rechteckInfo->istRechteck = istRechteck;
	rechteckInfo->posX = posX;
	rechteckInfo->posY = posY;
	rechteckInfo->groesseRechteck = groesseObjekt;
	rechteckInfo->breiteRechteck = breiteObjekt;
	rechteckInfo->hoeheRechteck = hoeheObjekt;
	return rechteckInfo;
}

void bildBinarise(bmpBild *bild, unsigned char **binBild) {
	unsigned char *objektbild = malloc(
			sizeof(unsigned char) * bild->infoHeader.biHeight
					* bild->infoHeader.biWidth);

	for (int i = 0; i < bild->infoHeader.biHeight * bild->infoHeader.biWidth;
			i++) {
		if ((bild->bildaten[i].rgbtRed == 255
				|| bild->bildaten[i].rgbtGreen == 255)
				&& bild->bildaten[i].rgbtBlue == 0) {
			objektbild[i] = 255;
		} else {
			objektbild[i] = 0;
		}
	}
	*binBild = objektbild;
}

void bildBearbeiten(bmpBild *bild) {
	unsigned char *ergebnis;
	unsigned char *binBild;
	RECHTECKINFO objekte[ARRAY_GROESSE];
	RECHTECKINFO *pobjekt;
	int objektzaehler = 0;
	bildBinarise(bild, &binBild);
	while (bildLeer(binBild, bild->infoHeader.biHeight,
			bild->infoHeader.biWidth) == FALSE) {
		objektFinden(binBild, bild->infoHeader.biHeight,
				bild->infoHeader.biWidth, &ergebnis);
		objektLoeschen(ergebnis, binBild, bild->infoHeader.biHeight,
				bild->infoHeader.biWidth);
		pobjekt = ueberpruefen(ergebnis, (int) bild->infoHeader.biHeight,
				(int) bild->infoHeader.biWidth);
		if (pobjekt->istRechteck == TRUE) {
			objekte[objektzaehler] = *pobjekt;
			objektzaehler++;
		}
		free(pobjekt);
	}
	if(objektzaehler ==0)
	{
		printf("Kein Rechteck vorhanden");
		return;
	}
	blauerRahmen(bild, objekte, objektzaehler);
	ausgeben(objekte, objektzaehler);
}

void objektLoeschen(unsigned char *objekt, unsigned char *binBild, LONG hoehe,
		LONG breite) {
	for (int i = 0; i < hoehe * breite; i++) {
		if (objekt[i] == 255 && binBild[i] == 255) {
			binBild[i] = 0;
		}
	}
}

void blauerRahmen(bmpBild *bild, RECHTECKINFO *objekte, int zaehler) {
	int posXMin = objekte[0].posX;
	int posXMax = 0;
	int posYMin = objekte[0].posY;
	int posYMax = 0;
	for (int i = 0; i < zaehler; i++) {
		if (objekte[i].posX < posXMin)
			posXMin = objekte[i].posX;
		if (objekte[i].posY < posYMin)
			posYMin = objekte[i].posY;
		if (objekte[i].posX + objekte[i].breiteRechteck > posXMax)
			posXMax = objekte[i].posX + objekte[i].breiteRechteck;
		if (objekte[i].posY + objekte[i].hoeheRechteck > posYMax)
			posYMax = objekte[i].posY + objekte[i].hoeheRechteck;
	}
	for (int i = posXMin; i <= posXMax; i++) {
		bild->bildaten[bild->infoHeader.biWidth * posYMax + i].rgbtBlue = 255;
		bild->bildaten[bild->infoHeader.biWidth * posYMax + i].rgbtRed = 0;
		bild->bildaten[bild->infoHeader.biWidth * posYMax + i].rgbtGreen = 0;
	}
	for (int i = posYMax; i >= posYMin; i--) {
		bild->bildaten[bild->infoHeader.biWidth * i + posXMin].rgbtBlue = 255;
		bild->bildaten[bild->infoHeader.biWidth * i + posXMin].rgbtRed = 0;
		bild->bildaten[bild->infoHeader.biWidth * i + posXMin].rgbtGreen = 0;
	}
	for (int i = posXMax; i >= posXMin; i--) {
		bild->bildaten[bild->infoHeader.biWidth * posYMin + i].rgbtBlue = 255;
		bild->bildaten[bild->infoHeader.biWidth * posYMin + i].rgbtRed = 0;
		bild->bildaten[bild->infoHeader.biWidth * posYMin + i].rgbtGreen = 0;
	}
	for (int i = posYMin; i <= posYMax; i++) {
		bild->bildaten[bild->infoHeader.biWidth * i + posXMax].rgbtBlue = 255;
		bild->bildaten[bild->infoHeader.biWidth * i + posXMax].rgbtRed = 0;
		bild->bildaten[bild->infoHeader.biWidth * i + posXMax].rgbtGreen = 0;
	}
}
int bildLeer(unsigned char *binBild, int hoehe, int breite) {
	for (int i = 0; i < hoehe * breite; i++) {
		if (binBild[i] != 0) {
			return FALSE;
		}
	}
	return TRUE;
}
void ausgeben(RECHTECKINFO *objekte, int zaehler) {
	for (int i = 0; i < zaehler; i++) {
		printf("Groesse: %d\n", objekte[i].groesseRechteck);
		printf("Position: %d/%d\n", objekte[i].posX, objekte[i].posY);
		printf("Hoehe: %d\n", objekte[i].hoeheRechteck);
		printf("Breite: %d\n", objekte[i].breiteRechteck);
	}
}

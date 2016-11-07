/*
 ============================================================================
 Name        : GS_Aufgabe_3.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmpVerwalten.h"
#include "bmpBearbeiten.h"
int main(void)
{
	bmpBild *bild = einlesen("aufgabe3_bild1.bmp");

	unsigned char *objektbild = malloc(
			sizeof(unsigned char) * bild->infoHeader.biHeight
					* bild->infoHeader.biWidth);

	for (int i = 0; i < bild->infoHeader.biHeight * bild->infoHeader.biWidth;
			i++)
	{
		if ((bild->bildaten[i].rgbtRed == 255
				|| bild->bildaten[i].rgbtGreen == 255)
				&& bild->bildaten[i].rgbtBlue == 0)
		{
			objektbild[i] = 255;
		} else
		{
			objektbild[i] = 0;
		}
	}

	unsigned char *ergebnis = NULL;

	objektFinden(objektbild, bild->infoHeader.biHeight,
			bild->infoHeader.biWidth, &ergebnis);

	ergebnis = objektbild;
	for (int i = 0; i < bild->infoHeader.biHeight * bild->infoHeader.biWidth;
			i++)
	{
		if (ergebnis[i] == 255)
		{
			bild->bildaten[i].rgbtRed = 255;
			bild->bildaten[i].rgbtGreen = 0;
			bild->bildaten[i].rgbtBlue = 0;
		} else
		{
			bild->bildaten[i].rgbtRed = 0;
			bild->bildaten[i].rgbtGreen = 0;
			bild->bildaten[i].rgbtBlue = 0;
		}
	}

	speichern("test1.bmp", bild);
	int red = bild->bildaten->rgbtRed;
	int green = bild->bildaten->rgbtGreen;
	int blue = bild->bildaten->rgbtBlue;
	printf("Fertig %d %d %d", bild->bildaten->rgbtRed, green, blue);
	return 0;
}

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
	bmpBild *bild = einlesen("amrandrle_NO_BLUE_LINES.bmp");

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

	for (int i = 0; i < bild->infoHeader.biHeight * bild->infoHeader.biWidth;
			i++)
	{
		if(ergebnis[i] == 255)
		{
			objektbild[i]=0;
		}
		if(i % bild->infoHeader.biWidth == 0)
		{
			//objektbild[i]= 0;
		}
	}
	objektFinden(objektbild, bild->infoHeader.biHeight,
				bild->infoHeader.biWidth, &ergebnis);
	//ergebnis = objektbild;
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
	//bild->bildaten[0].rgbtGreen=255;
	//bild->bildaten[1].rgbtBlue=255;
	speichern("test.bmp", bild);

	int red = bild->bildaten->rgbtRed;
	int green = bild->bildaten->rgbtGreen;
	int blue = bild->bildaten->rgbtBlue;
	printf("Fertig %d %d %d", bild->bildaten->rgbtRed, green, blue);
	return 0;
}

void bildBinarise(bmpBild *bild, unsigned char **binBild)
{
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

		*binBild = objektbild;
}

void binBildWandeln(bmpBild *bild, unsigned char *binBild)
{
	for (int i = 0; i < bild->infoHeader.biHeight * bild->infoHeader.biWidth;
				i++)
		{
			if (binBild[i] == 255)
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
}

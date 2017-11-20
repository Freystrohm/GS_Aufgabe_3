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
	bmpBild *bild = einlesen("testbilderneu/128x480_8_bit_komprimiert_ohne_padding_bytes_zeile_zu_lang.bmp");
	if(bild == NULL)
	{
		printf("Bild ist Null");
		return 1;
	}

	bildBearbeiten(bild);
	speichern("test.bmp", bild);

	int red = bild->bildaten->rgbtRed;
	int green = bild->bildaten->rgbtGreen;
	int blue = bild->bildaten->rgbtBlue;
	printf("Fertig %d %d %d", bild->bildaten->rgbtRed, green, blue);
	return 0;
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

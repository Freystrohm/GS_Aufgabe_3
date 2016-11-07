#include "bmpBearbeiten.h"
#include <stdlib.h>

void grassfire(unsigned char *pbild, int hoehe, int breite,
		unsigned char* ergebnisbild);

void dilatiere(unsigned char *pbild, int hoehe, int breite)
{
	unsigned char *temp = malloc(sizeof(unsigned char) * hoehe * breite);
	for (int i = 0; i < hoehe * breite; i++)
	{
		temp[i] = pbild[i];
	}

	for (int i = 0; i < hoehe; i++)
	{
		for (int j = 0; j < breite; j++)
		{
			if (temp[breite * i + j] == 255 && i == 0 && j == 0) //Ausnahme erster Pixel
			{
				pbild[breite * (i + 1) + j] = 255;
				pbild[breite * i + j + 1] = 255;
			} else if (temp[breite * i + j] == 255 && j == breite - 1
					&& i == hoehe - 1) //Ausnahme letzter Pixel
			{
				pbild[breite * (i - 1) + j] = 255;
				pbild[breite * i + j - 1] = 255;
			} else if (temp[breite * i + j] == 255 && j == 0 && i == hoehe-1) //Ausnahme Ecke unten links
			{
				pbild[breite * (i - 1) + j] = 255;
				pbild[breite * i + j + 1] = 255;
			} else if (temp[breite * i + j] == 255 && j == breite -1 && i == 0) //Ausnahme Ecke oben rechts
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
			} else if (temp[breite * i + j] == 255 && j == breite - 1)//Ausnahme letzte Spalte
			{
				pbild[breite * (i + 1) + j] = 255;
				pbild[breite * (i - 1) + j] = 255;
				pbild[breite * i + j - 1] = 255;
			} else if (temp[breite * i + j] == 255)
			{
				pbild[breite * (i + 1) + j] = 255;
				pbild[breite * (i - 1) + j] = 255;
				pbild[breite * i + j + 1] = 255;
				pbild[breite * i + j - 1] = 255;
			}
		}
	}
}

void objektFinden(unsigned char *pbild, int hoehe, int breite,
		unsigned char **ergebnis)
{
	unsigned char bildarray[hoehe][breite];

	unsigned char *ergebnisbild = malloc(
			sizeof(unsigned char) * hoehe * breite);
	*ergebnis = ergebnisbild;
	for (int i = 0; i < hoehe * breite; i++)
	{
		ergebnisbild[i] = 0;
	}

	for (int i = 0; i < hoehe; i++)
	{
		for (int j = 0; j < breite; j++)
		{
			bildarray[i][j] = pbild[i * breite + j];
		}
	}

	for (int i = 0; i < hoehe * breite; i++)
	{
		if (pbild[i] == 255)
		{
			ergebnisbild[i] = 255;
			grassfire(pbild, hoehe, breite, ergebnisbild);
			return;
		}
	}
}

void grassfire(unsigned char *pbild, int hoehe, int breite,
		unsigned char *ergebnisbild)
{
	int akt = 1, letzt = 0;
	while (letzt != akt)
	{
		letzt = akt;
		akt = 0;
		dilatiere(ergebnisbild, hoehe, breite);
		for (int j = 0; j < hoehe * breite; j++)
		{
			if (ergebnisbild[j] == 255 && pbild[j] != 255)
			{
				ergebnisbild[j] = 0;
			}
			if (ergebnisbild[j] == 255)
			{
				akt++;
			}
		}
	}

}

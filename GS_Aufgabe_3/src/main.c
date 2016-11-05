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

int main(void) {
	bmpBild *bild = einlesen("aufgabe3_bild1.bmp");

	speichern("test1.bmp", bild);
	int red = bild->bildaten->rgbtRed;
	int green = bild->bildaten->rgbtGreen;
	int blue = bild->bildaten->rgbtBlue;
	printf("Fertig %d %d %d", bild->bildaten->rgbtRed, green, blue);
	return 0;
}

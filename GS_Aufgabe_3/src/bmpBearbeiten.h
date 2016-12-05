/*
 * bmpBearbeiten.h
 *
 *  Created on: 06.11.2016
 *      Author: Johannes
 */

#ifndef BMPBEARBEITEN_H_
#define BMPBEARBEITEN_H_

#include "bmpVerwalten.h"

void bildBearbeiten(bmpBild *bild);

typedef struct tagRECHTECKINFO
{
        int posX;
        int posY;
        int groesseRechteck;
        int hoeheRechteck;
        int breiteRechteck;
        int istRechteck;
}RECHTECKINFO;

#endif /* BMPBEARBEITEN_H_ */

/*
 * bmpEinlesen.h
 *
 *  Created on: 04.11.2016
 *      Author: Johannes
 */
#include <stdint.h>
#ifndef BMPVERWALTEN_H_
#define BMPVERWALTEN_H_

typedef int8_t CHAR;
typedef int16_t SHORT;
typedef int32_t LONG;
typedef uint32_t DWORD;
typedef int32_t BOOL;
typedef uint8_t BYTE;
typedef uint16_t WORD;

typedef struct tagRGBTRIPLE
{
	unsigned char rgbtBlue;
	unsigned char rgbtGreen;
	unsigned char rgbtRed;
}RGBTRIPLE;

typedef struct tagBITMAPFILEHEADER
{
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
}BITMAPINFOHEADER;

typedef struct bmpDatei
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	RGBTRIPLE *bildaten;
}bmpBild;
void hello(void);

bmpBild* einlesen(char const *filename);
void speichern(char const *filename, bmpBild *bild);

#endif /* BMPVERWALTEN_H_ */

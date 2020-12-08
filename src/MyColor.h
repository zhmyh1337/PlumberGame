#pragma once
#include <Vcl.Graphics.hpp>

typedef unsigned char BYTE;

struct MyColor
{
    // в bitmap.scanline первым идет синий
	BYTE b, g, r;

	MyColor();
	MyColor(BYTE r, BYTE g, BYTE b);
	MyColor(unsigned int clr);

	bool operator==(const MyColor& other);

	operator TColor() const;
	operator unsigned int() const;
};

const MyColor cWhite(255, 255, 255);
const MyColor cBlack(0, 0, 0);
const MyColor cRed(255, 0, 0);
const MyColor cGreen(0, 255, 0);
const MyColor cBlue(0, 0, 255);
const MyColor cFuchsia(255, 0, 255);
const MyColor cDarkred(84, 14, 24);
const MyColor cDarkGreen(0, 128, 0);
const MyColor cDeepSkyBlue(0, 191, 255);
const MyColor cSkyBlue(0, 122, 204);
const MyColor cYellow(255, 255, 0);
const MyColor cOrange(255, 165, 0);
const MyColor cDarkOrange(255, 140, 0);
const MyColor cPurple(125, 0, 255);
const MyColor cCyan(0, 255, 255);
const MyColor cPink(255, 20, 147);
const MyColor cGray(128, 128, 128);
const MyColor cDarkGray(73, 73, 73);

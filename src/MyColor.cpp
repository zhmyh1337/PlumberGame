#include "MyColor.h"

MyColor::MyColor()
	: b(0), g(0), r(0)
{
}
MyColor::MyColor(BYTE r, BYTE g, BYTE b)
	: b(b), g(g), r(r)
{
}
MyColor::MyColor(unsigned int clr)
	: b(clr >> 16 & 0xFF), g(clr >> 8 & 0xFF), r(clr & 0xFF)
{
}

bool MyColor::operator==(const MyColor& other)
{
	return r == other.r && g == other.g && b == other.b;
}

MyColor::operator TColor() const
{
	return (TColor)RGB(r, g, b);
}
MyColor::operator unsigned int() const
{
	return (b << 16) | (g << 8) | r;
}

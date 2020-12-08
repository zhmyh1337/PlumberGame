#include "Textures.h"
#include "Constants.h"
#include <algorithm>

TBitmap** Textures::preLoadedTextures;

const wchar_t* Textures::TypeToName(TTypes type)
{
	switch (type)
	{
	case PipeLine:
		return L"pipe_line.bmp";
	case PipeCorner:
		return L"pipe_corner.bmp";
	case PipePlus:
		return L"pipe_plus.bmp";
	case PipeTriple:
		return L"pipe_triple.bmp";
	case ColorSwitcher:
		return L"color_switcher.bmp";
	case Source:
		return L"source.bmp";
	case Stock:
		return L"stock.bmp";
	case Stone:
		return L"stone.bmp";
	case Unknown:
		return L"";
	}
}

Textures::TTypes Textures::CellTypeToTexture(CellType type)
{
	switch (type)
	{
	case CellType::PipeLine:
		return PipeLine;
	case CellType::PipeCorner:
		return PipeCorner;
	case CellType::PipePlus:
		return PipePlus;
	case CellType::PipeTriple:
		return PipeTriple;
	case CellType::ColorSwitcher:
		return ColorSwitcher;
	case CellType::Source:
		return Source;
	case CellType::Stock:
		return Stock;
	default:
		return Unknown;
    }
}

void Textures::PreLoadTextures()
{
	preLoadedTextures = new TBitmap*[texturesCount];
	for (unsigned int i = 0; i < texturesCount; ++i)
	{
		TBitmap* bmp = new TBitmap();
		TTypes type = (TTypes)i;
		bmp->LoadFromFile(UnicodeString(L"Resources\\") + TypeToName(type));
		bmp->Transparent = type != Stone;
		bmp->HandleType = bmDIB;
		bmp->PixelFormat = pf24bit;
		preLoadedTextures[i] = bmp;
	}
}

void Textures::BitmapAssignData(TBitmap* dest, TBitmap* src, Rotation rotation)
{
	dest->Transparent = src->Transparent;
	dest->HandleType = src->HandleType;
	dest->PixelFormat = src->PixelFormat;
	if (rotation == Rotation::Angle0 || rotation == Rotation::Angle180)
		dest->SetSize(src->Width, src->Height);
	else
		dest->SetSize(src->Height, src->Width);
}

void Textures::DrawReplacedColors(TCanvas* canvas, Rotation rotation, const TRect& rect,
	TTypes texture, const unsigned int colorCount, const MyColor* was, const MyColor* became)
{
	if (colorCount)
	{
		TBitmap* dest = new TBitmap();
		TBitmap* src = preLoadedTextures[texture];
		BitmapAssignData(dest, src, rotation);

		MyColor** scanLine1 = new MyColor*[src->Height];
		for (int i = 0; i < src->Height; ++i)
			scanLine1[i] = (MyColor*)src->ScanLine[i];

		unsigned int width = dest->Width;
		unsigned int height = dest->Height;
		for (int i = 0; i < height; ++i)
		{
			MyColor* const scanLine2 = (MyColor*)dest->ScanLine[i];

			for (int j = 0; j < width; ++j)
			{
				switch (rotation)
				{
				case Rotation::Angle0:
					scanLine2[j] = scanLine1[i][j];
					break;
				case Rotation::Angle90:
					scanLine2[j] = scanLine1[height - j - 1][i];
					break;
				case Rotation::Angle180:
					scanLine2[j] = scanLine1[height - i - 1][width - j - 1];
					break;
				case Rotation::Angle270:
					scanLine2[j] = scanLine1[j][width - i - 1];
					break;
				}

				for (int k = 0; k < colorCount; ++k)
				{
					if (scanLine2[j] == was[k])
					{
						scanLine2[j] = became[k];
					}
				}
			}
		}

		canvas->StretchDraw(rect, dest);
		delete dest;
		delete[] scanLine1;
	}
	else
	{
		canvas->StretchDraw(rect, preLoadedTextures[texture]);
	}
}

void Textures::DrawCell(TCanvas* dest, const std::vector<MyColor>& colorMap,
	const Cell& cell, const TRect& rect)
{
	if (cell.ground == Ground::Stone)
		DrawReplacedColors(dest, Rotation::Angle0, rect, Stone);

	if (cell.type == CellType::Empty)
		return;

	unsigned int colorCount = 0;
	bool isPipe = false;
	switch (cell.type)
	{
	case CellType::PipeLine:
	case CellType::PipeCorner:
	case CellType::PipeTriple:
		colorCount = 1;
		isPipe = true;
		break;
	case CellType::PipePlus:
		colorCount = 2;
		isPipe = true;
		break;
	case CellType::Source:
	case CellType::Stock:
		colorCount = 1;
		isPipe = false;
		break;
	case CellType::ColorSwitcher:
		colorCount = 2;
		isPipe = false;
		break;
	default:
		colorCount = 0;
		isPipe = false;
		break;
	}

	const MyColor* was = Colors::tReplaceOrder;
	MyColor became[Stuff::tMaxColorCount];

	if (isPipe)
	{
		if (colorCount > 0 && (colorMap.size() > cell.clr1 || !cell.activated1))
			became[0] = cell.activated1 ? colorMap[cell.clr1] : Colors::gPipeInactive;
		if (colorCount > 1 && (colorMap.size() > cell.clr2 || !cell.activated2))
			became[1] = cell.activated2 ? colorMap[cell.clr2] : Colors::gPipeInactive;
	}
	else
	{
		if (colorCount > 0 && colorMap.size() > cell.clr1)
			became[0] = colorMap[cell.clr1];
		if (colorCount > 1 && colorMap.size() > cell.clr2)
			became[1] = colorMap[cell.clr2];
	}

	TTypes texture = CellTypeToTexture(cell.type);
	if (texture != Unknown)
		DrawReplacedColors(dest, cell.rotation, rect, texture, colorCount, was, became);
}


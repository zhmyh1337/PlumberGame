#pragma once
#include "Cell.h"
#include <vector>

class Textures
{
private:
	enum TTypes : unsigned int
	{
		PipeLine = 0,
		PipeCorner,
		PipePlus,
		PipeTriple,
		ColorSwitcher,
		Source,
		Stock,
		Stone,
        Unknown
	};

	static const unsigned int texturesCount = 8;
	static TBitmap** preLoadedTextures;

	static const wchar_t* TypeToName(TTypes type);
	static TTypes CellTypeToTexture(CellType type);
	static void BitmapAssignData(TBitmap* dest, TBitmap* src, Rotation rotation);
	static void DrawReplacedColors(TCanvas* canvas, Rotation rotation, const TRect& rect, TTypes texture,
		const unsigned int colorCount = 0, const MyColor* was = nullptr, const MyColor* became = nullptr);
public:
	static void PreLoadTextures();
	static void DrawCell(TCanvas* dest, const std::vector<MyColor>& colorMap,
		const Cell& cell, const TRect& rect);
};

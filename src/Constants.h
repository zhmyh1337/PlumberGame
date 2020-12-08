#pragma once
#include "MyColor.h"
#include "Cell.h"

namespace Colors
{
	// grids
	static const MyColor grLine = cBlack;

	// level choice
	static const MyColor lcGridBack = cWhite;
	static const MyColor lcGridSelect = cRed;
	static const MyColor lcFont = cBlack;
	static const MyColor lcFontCompleted = MyColor(10, 200, 60);
	static const MyColor lcFontNotCompleted = MyColor(200, 20, 10);

	// game
	static const MyColor gGridBack = cWhite;
	static const MyColor gGridSelect = cRed;
	static const MyColor gPipeInactive = cGray;

	// textures
	static const MyColor tReplaceOrder[] = { MyColor(0, 0, 254), MyColor(254, 0, 0) };

	// editor
	static const MyColor eUniversalColor = cDarkGray;
	static const unsigned int ePresetColorsCount = 8;
	static const MyColor ePresetColors[] = { cBlue, cRed, cGreen, cOrange,
		cPurple, cCyan, cFuchsia, cYellow };
}

namespace Offsets
{
	// grids
	static const int grLineWidth = 1;

	// forms
	static const int fReservedWidth = 30;
	static const int fReservedHeight = 120;

	// level choice
	static const int lcGridButton = 20;
	static const int lcGridSelectWidth = 2;
	static const int lcGridCrossWidth = 3;
	static const int lcText1 = 10;
	static const int lcText2 = 30;
	static const int lcText3 = 50;
	static const int lcText4 = 70;

	// game
	static const int gGridSelectWidth = 2;
	static const int gTextInfo = 20;
	static const int gTextInfoButton = 30;
	static const int gInfoButton = 10;
	static const int gMain = 10;
	static const int gPanelBottom = 10;
	static const int gFirstPanelLabel = 5;
    static const unsigned int gMinimumWidth = 450;
}

namespace Stuff
{
	// textures
	static const unsigned int tMaxColorCount = 2;

	// level choice
	static const int lcInfoFontSize = 7;

	// game
	static const bool gHighlightLeakFrom = true;
    static const int gTickInterval = 300;
    static const float gDragSensivity = 2.0f;
	static const unsigned int gBuildingPaletteCount = 7;
	static const Cell gBuildingPalette[] = { Cell(CellType::PipeCorner, Rotation::Angle0),
		Cell(CellType::PipeCorner, Rotation::Angle90), Cell(CellType::PipeCorner, Rotation::Angle180),
		Cell(CellType::PipeCorner, Rotation::Angle270), Cell(CellType::PipeLine, Rotation::Angle0),
		Cell(CellType::PipeLine, Rotation::Angle90), Cell(CellType::PipePlus, Rotation::Angle0)
	};
	static const unsigned int gClassicRandomCount = 7;
	static const Cell gClassicRandom[] = { Cell(CellType::PipeCorner, Rotation::Angle0),
		Cell(CellType::PipeCorner, Rotation::Angle90), Cell(CellType::PipeCorner, Rotation::Angle180),
		Cell(CellType::PipeCorner, Rotation::Angle270), Cell(CellType::PipeLine, Rotation::Angle0),
		Cell(CellType::PipeLine, Rotation::Angle90), Cell(CellType::PipePlus, Rotation::Angle0)
	};
	static const Cell gEmptyCell = Cell(CellType::Empty, Rotation::Angle0);
	static const unsigned int gSpawnBombCounter = 8;

	// editor
	static const unsigned int ePaletteCount = 8;
	static const Cell ePalette[] = { Cell(CellType::Empty, Ground::Stone),
		Cell(CellType::PipeLine), Cell(CellType::PipeCorner),
		Cell(CellType::PipePlus), Cell(CellType::PipeTriple),
		Cell(CellType::Source), Cell(CellType::Stock),
		Cell(CellType::ColorSwitcher)
	};
}

namespace Strings
{
	// fonts
	static const wchar_t* fLevelChoice = L"Calibri";
}


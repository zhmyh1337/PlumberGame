#pragma once
#include <Vcl.Grids.hpp>
#include "MyColor.h"

struct MyPoint
{
	int x = -1, y = -1, col = -1, row = -1;
};

class Utilities
{
public:
	static int NormalizeGridCol(int col, const TDrawGrid* grid);
	static int NormalizeGridRow(int row, const TDrawGrid* grid);
	static void BalanceGrid(TDrawGrid* grid, unsigned int workingWidth,
		unsigned int workingHeight);
	static unsigned int LinearFrom2D(unsigned int col, unsigned int row,
		unsigned int colCount);
	static void LinearTo2D(unsigned int linear, unsigned int colCount,
		int& col, int& row);
	static void SetGridSelection(TDrawGrid* grid, int col, int row);
	static void DrawBorderedRect(TCanvas* canvas, const TRect& rect,
		MyColor color = cBlack, int width = 1);
	static void CenterWindow(TForm* form);
	static void CheckGridDrag(TDrawGrid* grid, const MyPoint& middleMouse,
		const TShiftState& Shift, const TPoint& point);
	static void ScreenToGridCell(TDrawGrid* grid, const TPoint& point, int& col, int& row);
	static void StringsResize(TStrings* strings, unsigned int size);
    static void Exception();
};

#include "Utilities.h"
#include "Constants.h"
#include <algorithm>

int Utilities::NormalizeGridCol(int col, const TDrawGrid* grid)
{
	const int visibleCols = grid->Width / grid->DefaultColWidth;
	return std::max(0, std::min(col, grid->ColCount - visibleCols));
}

int Utilities::NormalizeGridRow(int row, const TDrawGrid* grid)
{
	const int visibleRows = grid->Height / grid->DefaultRowHeight;
	return std::max(0, std::min(row, grid->RowCount - visibleRows));
}

void Utilities::BalanceGrid(TDrawGrid* grid, unsigned int workingWidth,
	unsigned int workingHeight)
{
	const unsigned int visibleCols = std::min((unsigned int)grid->ColCount,
		workingWidth / grid->DefaultColWidth);
	const unsigned int visibleRows = std::min((unsigned int)grid->RowCount,
		workingHeight / grid->DefaultRowHeight);
	grid->Width = visibleCols * grid->DefaultColWidth;
	grid->Height = visibleRows * grid->DefaultRowHeight;
}

unsigned Utilities::LinearFrom2D(unsigned int col, unsigned int row,
	unsigned int colCount)
{
	return row * colCount + col;
}

void Utilities::LinearTo2D(unsigned int linear, unsigned int colCount,
	int& col, int& row)
{
	col = linear % colCount;
    row = linear / colCount;
}

void Utilities::SetGridSelection(TDrawGrid* grid, int col, int row)
{
	TGridRect rect;
	rect.Left = col;
	rect.Right = col;
	rect.Top = row;
	rect.Bottom = row;
	grid->Selection = rect;
}

void Utilities::DrawBorderedRect(TCanvas* canvas, const TRect& rect,
	MyColor color, int width)
{
	canvas->Brush->Style = bsSolid;
	canvas->Brush->Color = color;
	// left
	canvas->FillRect(TRect(rect.left, rect.top, rect.left + width, rect.bottom));
	// right
	canvas->FillRect(TRect(rect.right - width, rect.top, rect.right, rect.bottom));
	// top
	canvas->FillRect(TRect(rect.left, rect.top, rect.right, rect.top + width));
	// bottom
	canvas->FillRect(TRect(rect.left, rect.bottom - width, rect.right, rect.bottom));
}

void Utilities::CenterWindow(TForm* form)
{
	form->Left = Screen->Width / 2 - form->Width / 2;
	form->Top = Screen->Height / 2 - form->Height / 2;
}

void Utilities::CheckGridDrag(TDrawGrid* grid, const MyPoint& middleMouse,
	const TShiftState& Shift, const TPoint& point)
{
    if (Shift.Contains(ssMiddle))
	{
		if (middleMouse.x != -1 && middleMouse.y != -1 &&
			middleMouse.col != -1 && middleMouse.row != -1)
		{
			int dx = middleMouse.x - point.X;
			int dy = middleMouse.y - point.Y;
			int newCol = NormalizeGridCol(middleMouse.col +
				Stuff::gDragSensivity * dx / grid->DefaultColWidth, grid);
			int newRow = NormalizeGridRow(middleMouse.row +
				Stuff::gDragSensivity * dy / grid->DefaultRowHeight, grid);
			if (grid->LeftCol != newCol || grid->TopRow != newRow)
			{
				grid->LeftCol = newCol;
				grid->TopRow = newRow;
				grid->Refresh();
			}
		}
	}
}

void Utilities::ScreenToGridCell(TDrawGrid* grid, const TPoint& point, int& col, int& row)
{
	TPoint clientPoint = grid->ScreenToClient(point);
	grid->MouseToCell(clientPoint.X, clientPoint.Y, col, row);
}

void Utilities::StringsResize(TStrings* strings, unsigned int size)
{
	const unsigned int curSize = strings->Count;
	if (curSize > size)
	{
		const unsigned int iterations = curSize - size;
		for (unsigned int i = 0; i < iterations; ++i)
			strings->Delete(strings->Count - 1);
	}
	else if (curSize < size)
	{
		const unsigned int iterations = size - curSize;
		for (unsigned int i = 0; i < iterations; ++i)
			strings->Append(L"");
	}
}

void Utilities::Exception()
{
	Application->MessageBox(L"Ошибка ввода.\nПерепроверьте данные.", L"Ошибка");
}


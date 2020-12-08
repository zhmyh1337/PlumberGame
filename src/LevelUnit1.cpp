//---------------------------------------------------------------------------

#include <vcl.h>
#include <vector>
#include <algorithm>
#include <cstring>
#pragma hdrstop

#include "LevelUnit1.h"
#include "MainUnit1.h"
#include "GameUnit1.h"
#include "LevelPreview.h"
#include "Constants.h"
#include "Utilities.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLevelForm *LevelForm;
static std::vector<LevelPreview> levels;
//---------------------------------------------------------------------------
__fastcall TLevelForm::TLevelForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TLevelForm::Button2Click(TObject *Sender)
{
	MainForm->Show();
	LevelForm->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TLevelForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    MainForm->Show();
	Action = caHide;
}
//---------------------------------------------------------------------------
void __fastcall TLevelForm::FormShow(TObject *Sender)
{
	LevelForm->ActiveControl = DrawGrid1;

	TSearchRec sr;
	levels.clear();
	for (bool end = FindFirst("Levels\\*.game", faDirectory, sr); !end; end = FindNext(sr))
	{
		levels.emplace_back((L"Levels\\" + sr.Name).c_str());
		levels.back().LoadFromFile();
	}
	FindClose(sr);
	std::sort(levels.begin(), levels.end());

	DrawGrid1->TopRow = 0;
	DrawGrid1->LeftCol = 0;
    Utilities::SetGridSelection(DrawGrid1, 0, 0);

	const unsigned int rows = ((((int)levels.size()) - 1) / 5) + 1;
	DrawGrid1->RowCount = rows;
	const unsigned int workingArea = Screen->Height - Offsets::fReservedHeight -
		DrawGrid1->Top - Offsets::lcGridButton * 2 - Button1->Height;
	const unsigned int visibleRows = std::min(rows, workingArea / DrawGrid1->DefaultRowHeight);
	DrawGrid1->Height = visibleRows * DrawGrid1->DefaultRowHeight;
	Button1->Top = DrawGrid1->Top + DrawGrid1->Height + Offsets::lcGridButton;
	Button2->Top = Button1->Top;
	LevelForm->ClientHeight = Button1->Top + Button1->Height + Offsets::lcGridButton;

	Utilities::CenterWindow(LevelForm);
}
//---------------------------------------------------------------------------
void __fastcall TLevelForm::DrawGrid1MouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
	DrawGrid1->TopRow = Utilities::NormalizeGridRow(DrawGrid1->TopRow + 1, DrawGrid1);
	Handled = true;
    DrawGrid1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TLevelForm::DrawGrid1MouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
    DrawGrid1->TopRow = Utilities::NormalizeGridRow(DrawGrid1->TopRow - 1, DrawGrid1);
	Handled = true;
	DrawGrid1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TLevelForm::DrawGrid1DrawCell(TObject *Sender, int ACol, int ARow,
		  TRect &Rect, TGridDrawState State)
{
	DrawGrid1->Canvas->Brush->Style = bsSolid;
	DrawGrid1->Canvas->Brush->Color = Colors::lcGridBack;
	DrawGrid1->Canvas->FillRect(Rect);

	unsigned int index = Utilities::LinearFrom2D(ACol, ARow, DrawGrid1->ColCount);
	if (index >= levels.size())
	{
		DrawGrid1->Canvas->Pen->Color = Colors::grLine;
		DrawGrid1->Canvas->Pen->Width = Offsets::lcGridCrossWidth;
		DrawGrid1->Canvas->MoveTo(Rect.left, Rect.top);
		DrawGrid1->Canvas->LineTo(Rect.right, Rect.bottom);
		DrawGrid1->Canvas->MoveTo(Rect.right, Rect.top);
		DrawGrid1->Canvas->LineTo(Rect.left, Rect.bottom);
	}
	else
	{
		DrawGrid1->Canvas->Brush->Style = bsClear;
		DrawGrid1->Canvas->Font->Color = Colors::lcFont;
		DrawGrid1->Canvas->Font->Size = Stuff::lcInfoFontSize;
		DrawGrid1->Canvas->Font->Style = TFontStyles() << fsBold;
		wcscpy(DrawGrid1->Canvas->Font->Name.c_str(), Strings::fLevelChoice);
		SetTextAlign(DrawGrid1->Canvas->Handle, TA_CENTER);
		DrawGrid1->Canvas->TextOut((Rect.left + Rect.right) / 2,
			Rect.top + Offsets::lcText1, L"Уровень " + UIntToStr(levels[index].levelId));
		DrawGrid1->Canvas->TextOut((Rect.left + Rect.right) / 2,
			Rect.top + Offsets::lcText2, levels[index].GetTypeName());
		DrawGrid1->Canvas->TextOut((Rect.left + Rect.right) / 2,
			Rect.top + Offsets::lcText3, levels[index].GetDifficultyName());
		DrawGrid1->Canvas->Font->Color = levels[index].completed ?
			Colors::lcFontCompleted : Colors::lcFontNotCompleted;
        DrawGrid1->Canvas->TextOut((Rect.left + Rect.right) / 2,
			Rect.top + Offsets::lcText4, levels[index].GetIsCompletedName());
    }

	TRect rect1(Rect.left - (ACol != DrawGrid1->LeftCol),
		Rect.top - (ARow != DrawGrid1->TopRow), Rect.right, Rect.bottom);
	Utilities::DrawBorderedRect(DrawGrid1->Canvas, rect1, Colors::grLine, Offsets::grLineWidth);

	if (State.Contains(gdSelected))
	{
		TRect rect2(rect1.left + 1, rect1.top + 1, rect1.right - 1, rect1.bottom - 1);
		Utilities::DrawBorderedRect(DrawGrid1->Canvas, rect2, Colors::lcGridSelect, Offsets::lcGridSelectWidth);
	}
}
//---------------------------------------------------------------------------
void __fastcall TLevelForm::DrawGrid1TopLeftChanged(TObject *Sender)
{
    DrawGrid1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TLevelForm::EnterLevel()
{
	unsigned int index = Utilities::LinearFrom2D(DrawGrid1->Selection.Left,
		DrawGrid1->Selection.Top, DrawGrid1->ColCount);
	if (index < levels.size())
	{
		currentLevel = Level(levels[index].GetPath().c_str());
		GameForm->Show();
		LevelForm->Hide();
	}
}
//---------------------------------------------------------------------------
void __fastcall TLevelForm::Button1Click(TObject *Sender)
{
	EnterLevel();
}
//---------------------------------------------------------------------------
void __fastcall TLevelForm::DrawGrid1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN)
	{
		EnterLevel();
    }
}
//---------------------------------------------------------------------------
void __fastcall TLevelForm::DrawGrid1DblClick(TObject *Sender)
{
    EnterLevel();
}
//---------------------------------------------------------------------------

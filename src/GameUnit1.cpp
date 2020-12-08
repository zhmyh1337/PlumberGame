//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GameUnit1.h"
#include "LevelUnit1.h"
#include "EditorUnit1.h"
#include "Constants.h"
#include "Utilities.h"
#include "Textures.h"
#include <algorithm>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGameForm *GameForm;
Level currentLevel(false);

// от какой точки началось движение мыши
static MyPoint leftMouse;
static MyPoint middleMouse;
static MyPoint rightMouse;
static bool leftFromPalette = false;
//---------------------------------------------------------------------------
__fastcall TGameForm::TGameForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::FormInitialization()
{
	Button1->Visible = currentLevel.difficulty == LevelDifficulty::Instruction;

	switch (currentLevel.type)
	{
	case LevelType::Classic:
		DrawGrid2->Visible = true;
		DrawGrid2->ColCount = 1;
		DrawGrid2->RowCount = 1;
		DrawGrid2->Width = DrawGrid2->DefaultColWidth * DrawGrid2->ColCount;
		DrawGrid2->Height = DrawGrid2->DefaultRowHeight * DrawGrid2->RowCount;
		Label2->Visible = true;
		break;
	case LevelType::Building:
		DrawGrid2->Visible = true;
		DrawGrid2->ColCount = Stuff::gBuildingPaletteCount;
		DrawGrid2->RowCount = 1;
		DrawGrid2->Width = DrawGrid2->DefaultColWidth * DrawGrid2->ColCount;
		DrawGrid2->Height = DrawGrid2->DefaultRowHeight * DrawGrid2->RowCount;
		Label2->Visible = false;
		break;
	case LevelType::Moving:
	case LevelType::Rotating:
		DrawGrid2->Visible = false;
        Label2->Visible = false;
		break;
	}

	const unsigned int mainOffset = DrawGrid1->Top;

	// width
	const unsigned int reservedAreaX = mainOffset * 3 + Panel1->Width;
	const unsigned int workingAreaX = Screen->Width - reservedAreaX - Offsets::fReservedWidth;

	// height
	const unsigned int reservedAreaY = mainOffset * (2 + DrawGrid2->Visible) +
		(DrawGrid2->Visible ? DrawGrid2->Height : 0);
	const unsigned int workingAreaY = Screen->Height - reservedAreaY - Offsets::fReservedHeight;
	unsigned int cur = Offsets::gFirstPanelLabel;
	Label1->Top = cur;
	if (Label2->Visible)
	{
		cur += Offsets::gTextInfo;
		Label2->Top = cur;
	}
	cur += Offsets::gTextInfoButton;
	if (Button1->Visible)
	{
		Button1->Top = cur;
		cur += Offsets::gInfoButton + Button1->Height;
	}
	Button2->Top = cur;
	cur += Offsets::gInfoButton + Button2->Height;
	Button3->Top = cur;
	Panel1->Height = cur + Button3->Height + Offsets::gPanelBottom;

	// balancing grids
	DrawGrid1->RowCount = currentLevel.rowCount;
	DrawGrid1->ColCount = currentLevel.colCount;
	Utilities::BalanceGrid(DrawGrid1, workingAreaX, workingAreaY);
    DrawGrid2->Top = DrawGrid1->Top + DrawGrid1->Height + mainOffset;

	DrawGrid1->TopRow = 0;
	DrawGrid1->LeftCol = 0;
	Utilities::SetGridSelection(DrawGrid1, 0, 0);
	if (DrawGrid2->Visible)
	{
		DrawGrid2->TopRow = 0;
		DrawGrid2->LeftCol = 0;
		Utilities::SetGridSelection(DrawGrid2, 0, 0);
	}

	GameForm->ClientWidth = std::max(Offsets::gMinimumWidth,
		std::max(DrawGrid1->Width + reservedAreaX,
		DrawGrid2->Visible ? DrawGrid2->Width + 2 * mainOffset : 0));
	GameForm->ClientHeight = std::max(DrawGrid1->Height + reservedAreaY,
		Panel1->Height + mainOffset * 2);

	Panel1->Left = GameForm->ClientWidth - Panel1->Width - mainOffset;
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::LevelInitialization()
{
	switch (currentLevel.type)
	{
	case LevelType::Classic:
		currentLevel.RegenerateCell();
		currentLevel.bombsIncCounter = 0;
		break;
	default:
		break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::FormShow(TObject *Sender)
{
	GameForm->ActiveControl = DrawGrid1;
	LevelInitialization();
	FormInitialization();
	BombsLabelUpdate();
	TimeLabelUpdate();
	Utilities::CenterWindow(GameForm);

	if (currentLevel.fromEditor)
		GameForm->Caption = (UnicodeString)L"Просмотр уровня (" +
			currentLevel.GetTypeName() + L")";
	else
		GameForm->Caption = L"Уровень " + UIntToStr(currentLevel.levelId) +
			L" (" + currentLevel.GetTypeName() + L")";

	leftMouse = MyPoint();
	middleMouse = MyPoint();
	rightMouse = MyPoint();

	Button2->Enabled = true;
	Timer1->Interval = 1000;
	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::DrawGrid1DrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State)
{
	const unsigned int cell = Utilities::LinearFrom2D(ACol, ARow, currentLevel.colCount);

	DrawGrid1->Canvas->Brush->Color = Colors::gGridBack;
	DrawGrid1->Canvas->FillRect(Rect);

	Textures::DrawCell(DrawGrid1->Canvas, currentLevel.colorMap,
		currentLevel.cells[cell], Rect);

	TRect rect1(Rect.left - (ACol != DrawGrid1->LeftCol),
		Rect.top - (ARow != DrawGrid1->TopRow), Rect.right, Rect.bottom);
	Utilities::DrawBorderedRect(DrawGrid1->Canvas, rect1, Colors::grLine, Offsets::grLineWidth);

	if (currentLevel.highlighted[cell])
	{
		TRect rect2(rect1.left + 1, rect1.top + 1, rect1.right - 1, rect1.bottom - 1);
		Utilities::DrawBorderedRect(DrawGrid1->Canvas, rect2, Colors::gGridSelect, Offsets::gGridSelectWidth);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (Application->MessageBox(L"Покинуть уровень (прогресс будет потерян)?",
		L"Подтверждение", MB_YESNO) == IDYES)
	{
		if (currentLevel.fromEditor)
			EditorForm->Show();
		else
            LevelForm->Show();
		Action = caHide;
    }
	else
		Action = caNone;
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::DrawGrid2DrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State)
{
	DrawGrid2->Canvas->Brush->Color = Colors::gGridBack;
	DrawGrid2->Canvas->FillRect(Rect);

	if (currentLevel.type == LevelType::Building)
		Textures::DrawCell(DrawGrid2->Canvas, { }, Stuff::gBuildingPalette[ACol], Rect);
	else if (currentLevel.type == LevelType::Classic)
		Textures::DrawCell(DrawGrid2->Canvas, { }, currentLevel.randomCell, Rect);

	TRect rect1(Rect.left - (ACol != DrawGrid2->LeftCol),
		Rect.top - (ARow != DrawGrid2->TopRow), Rect.right, Rect.bottom);
	Utilities::DrawBorderedRect(DrawGrid2->Canvas, rect1, Colors::grLine, Offsets::grLineWidth);

    if (currentLevel.type == LevelType::Building && State.Contains(gdSelected))
	{
		TRect rect2(rect1.left + 1, rect1.top + 1, rect1.right - 1, rect1.bottom - 1);
		Utilities::DrawBorderedRect(DrawGrid2->Canvas, rect2, Colors::gGridSelect, Offsets::gGridSelectWidth);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::DrawGrid1MouseWheelDown(TObject *Sender, TShiftState Shift,
	TPoint &MousePos, bool &Handled)
{
	Handled = true;
	if (Shift.Contains(ssMiddle))
		return;

	DrawGrid1->TopRow = Utilities::NormalizeGridRow(DrawGrid1->TopRow + 1, DrawGrid1);
	DrawGrid1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::DrawGrid1MouseWheelUp(TObject *Sender, TShiftState Shift,
	TPoint &MousePos, bool &Handled)
{
	Handled = true;
	if (Shift.Contains(ssMiddle))
		return;

	DrawGrid1->TopRow = Utilities::NormalizeGridRow(DrawGrid1->TopRow - 1, DrawGrid1);
	DrawGrid1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::BombsLabelUpdate()
{
	Label2->Caption = L"Взрывчатка - " + UIntToStr(currentLevel.bombsCount);
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::TimeLabelUpdate()
{
	const unsigned int minutes = currentLevel.time / 60;
	const unsigned int seconds = currentLevel.time % 60;
	Label1->Caption = L"Осталось времени - " + UIntToStr(minutes) +
		":" + (seconds < 10 ? "0" : "") + UIntToStr(seconds);
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::DrawGrid1MouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	GlobalMouseDown(Button, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::DrawGrid2MouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	GlobalMouseDown(Button, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::DrawGrid1MouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
	GlobalMouseMove(Shift);
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::FormMouseMove(TObject *Sender, TShiftState Shift, int X,
	int Y)
{
	GlobalMouseMove(Shift);
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::DrawGrid2MouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
	GlobalMouseMove(Shift);
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::DrawGrid2MouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	GlobalMouseUp(Button, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::DrawGrid1MouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	GlobalMouseUp(Button, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::FormHide(TObject *Sender)
{
    Timer1->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::Button3Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::Button1Click(TObject *Sender)
{
	Application->MessageBox(currentLevel.GetLevelHint(), L"Подсказка", MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::GlobalMouseDown(TMouseButton Button, TShiftState Shift)
{
	if (currentLevel.secondPhase && Button != mbMiddle)
		return;

	int col1 = -1, row1 = -1, col2 = -1, row2 = -1;
	const int X = Mouse->CursorPos.X;
	const int Y = Mouse->CursorPos.Y;
	Utilities::ScreenToGridCell(DrawGrid1, Mouse->CursorPos, col1, row1);
	Utilities::ScreenToGridCell(DrawGrid2, Mouse->CursorPos, col2, row2);

    if (col2 != -1 && row2 != -1) // DrawGrid2
	{
		if (Button == mbLeft)
		{
			leftMouse = { X, Y, col2, row2 };
			leftFromPalette = true;
		}
	}
	else if (col1 != -1 && row1 != -1) // DrawGrid1
	{
		switch (Button)
		{
		case mbMiddle:
			middleMouse = { X, Y, DrawGrid1->LeftCol, DrawGrid1->TopRow };
			break;

		case mbLeft:
			leftMouse = { X, Y, col1, row1 };
			leftFromPalette = false;
			break;

		case mbRight:
			rightMouse = { X, Y, col1, row1 };
			break;
        }
	}
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::GlobalMouseUp(TMouseButton Button, TShiftState Shift)
{
	if (currentLevel.secondPhase && Button != mbMiddle)
		return;

	int col1 = -1, row1 = -1, col2 = -1, row2 = -1;
	const int X = Mouse->CursorPos.X;
	const int Y = Mouse->CursorPos.Y;
	Utilities::ScreenToGridCell(DrawGrid1, Mouse->CursorPos, col1, row1);
	Utilities::ScreenToGridCell(DrawGrid2, Mouse->CursorPos, col2, row2);

	if (col2 != -1 && row2 != -1) // DrawGrid2
	{
		if (Button == mbLeft)
			leftMouse = MyPoint();
	}
	else if (col1 != -1 && row1 != -1) // DrawGrid1
	{
		switch (Button)
		{
		case mbMiddle:
			middleMouse = MyPoint();
			break;

		case mbLeft:
			if (leftMouse.col == -1 || leftMouse.row == -1)
				break;

			switch (currentLevel.type)
			{
			case LevelType::Classic:
				if (leftFromPalette || (leftMouse.col == col1 && leftMouse.row == row1))
				{
					unsigned int cellId = Utilities::LinearFrom2D(col1, row1, currentLevel.colCount);
					if (currentLevel.cells[cellId].type == CellType::Empty &&
						currentLevel.cells[cellId].ground == Ground::Empty)
					{
						currentLevel.cells[cellId] = currentLevel.randomCell;
						currentLevel.RegenerateCell();
						if (++currentLevel.bombsIncCounter == Stuff::gSpawnBombCounter)
						{
							currentLevel.bombsIncCounter = 0;
							currentLevel.bombsCount++;
							BombsLabelUpdate();
						}
						DrawGrid1->Refresh();
						DrawGrid2->Refresh();
					}
				}
				break;

			case LevelType::Building:
			{
				unsigned int cellTo = Utilities::LinearFrom2D(col1,
					row1, currentLevel.colCount);
                if (currentLevel.cells[cellTo].type == CellType::Empty &&
					currentLevel.cells[cellTo].ground == Ground::Empty &&
					(leftFromPalette || (DrawGrid2->Selection.Left != -1 &&
					DrawGrid2->Selection.Top != -1)))
				{
					unsigned int cellFrom = leftFromPalette ? leftMouse.col :
						DrawGrid2->Selection.Left;
					currentLevel.cells[cellTo] = Stuff::gBuildingPalette[cellFrom];
					DrawGrid1->Refresh();
				}

				break;
			}

			case LevelType::Moving:
				if (!leftFromPalette && (leftMouse.col != col1 || leftMouse.row != row1))
				{
					unsigned int cell1 = Utilities::LinearFrom2D(leftMouse.col,
						leftMouse.row, currentLevel.colCount);
					unsigned int cell2 = Utilities::LinearFrom2D(col1, row1, currentLevel.colCount);
					if (currentLevel.cells[cell1].ground == Ground::Empty &&
						currentLevel.cells[cell2].ground == Ground::Empty)
					{
						std::swap(currentLevel.cells[cell1], currentLevel.cells[cell2]);
                        DrawGrid1->Refresh();
                    }
				}
				break;

			case LevelType::Rotating:
				if (!leftFromPalette && leftMouse.col == col1 && leftMouse.row == row1)
				{
					unsigned int cell = Utilities::LinearFrom2D(col1, row1, currentLevel.colCount);
					if (currentLevel.cells[cell].ground == Ground::Empty &&
						currentLevel.cells[cell].type != CellType::Empty/* &&
						currentLevel.cells[cell].type != CellType::PipePlus &&
						currentLevel.cells[cell].type != CellType::Stock*/)
					{
						currentLevel.cells[cell].DoRotate();
                        DrawGrid1->Refresh();
                    }
				}
				break;

			default:
				break;
			}

			leftMouse = MyPoint();
			break;

		case mbRight:
			if (rightMouse.col == -1 || rightMouse.row == -1)
				break;

			if (rightMouse.col == col1 && rightMouse.row == row1)
			{
				const unsigned int cellId = Utilities::LinearFrom2D(col1, row1, currentLevel.colCount);
				if (currentLevel.cells[cellId].type != CellType::Empty &&
					currentLevel.cells[cellId].ground == Ground::Empty &&
					(currentLevel.type == LevelType::Building ||
					(currentLevel.type == LevelType::Classic && currentLevel.bombsCount > 0)))
				{
					currentLevel.cells[cellId] = Stuff::gEmptyCell;
					if (currentLevel.type == LevelType::Classic)
					{
						currentLevel.bombsCount--;
						BombsLabelUpdate();
					}
					DrawGrid1->Refresh();
				}
			}

			rightMouse = MyPoint();
			break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::GlobalMouseMove(TShiftState Shift)
{
	Utilities::CheckGridDrag(DrawGrid1, middleMouse, Shift, Mouse->CursorPos);
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::StartSecondPhase()
{
	Timer1->Enabled = false;
	Timer1->Interval = Stuff::gTickInterval;
	currentLevel.secondPhase = true;
	Button2->Enabled = false;
	currentLevel.time = 0;
	TimeLabelUpdate();
	currentLevel.SwitchToSecondPhase();
	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::Timer1Timer(TObject *Sender)
{
	if (currentLevel.secondPhase) // потекла вода
	{
		LevelState state = currentLevel.GameTick();
        if (state != LevelState::NotFinished)
			Timer1->Enabled = false;
		DrawGrid1->Refresh();
		if (state == LevelState::NotFinished)
			return;

		const wchar_t* reason;

		switch (state)
		{
		case LevelState::Completed:
			if (!currentLevel.completed && !currentLevel.fromEditor)
				currentLevel.UpdateCompleted();
			Application->MessageBox(L"Уровень пройден!", L"Успешно");
			if (currentLevel.fromEditor)
				EditorForm->Show();
			else
				LevelForm->Show();
			GameForm->Hide();
			return;

		case LevelState::LostStocks:
			reason = L"Не все стоки заполнены водой.\n";
			break;

		case LevelState::LostLeaked:
			reason = L"Вода вытекла.\n";
			break;

		case LevelState::LostMixed:
			reason = L"Вода смешалась.\n";
			break;

		case LevelState::LostStockError:
			reason = L"Не универсальный сток заполнен водой другого цвета.\n";
			break;

		default:
			reason = L"";
			break;
		}

		if (Application->MessageBox(((UnicodeString)reason + L"Переиграть?").c_str(),
			L"Уровень не пройден", MB_YESNO) == IDYES)
		{
			currentLevel.Reload();
			this->FormShow(this);
			GameForm->Refresh();
			DrawGrid1->Refresh();
            DrawGrid2->Refresh();
		}
		else
		{
            if (currentLevel.fromEditor)
				EditorForm->Show();
			else
				LevelForm->Show();
			GameForm->Hide();
        }
	}
	else
	{
		if (--currentLevel.time == 0)
			StartSecondPhase();
		else
			TimeLabelUpdate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGameForm::Button2Click(TObject *Sender)
{
	StartSecondPhase();
}
//---------------------------------------------------------------------------


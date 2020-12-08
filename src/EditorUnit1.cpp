//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainUnit1.h"
#include "EditorUnit1.h"
#include "GameUnit1.h"
#include "Utilities.h"
#include "Textures.h"
#include "MyColor.h"
#include "Constants.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditorForm *EditorForm;

Level editLevel;
static Cell cells[Stuff::ePaletteCount];
// от какой точки началось движение мыши
static MyPoint leftMouse;
static MyPoint middleMouse;
static MyPoint rightMouse;
static bool leftFromPalette = false;
//---------------------------------------------------------------------------
__fastcall TEditorForm::TEditorForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (Application->MessageBox(L"Покинуть редактор?",
		L"Подтверждение", MB_YESNO) == IDYES)
	{
		MainForm->Show();
		Action = caHide;
	}
	else
		Action = caNone;
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::OnResizeUpdate()
{
	const unsigned int newSize = editLevel.colCount * editLevel.rowCount;
	editLevel.highlighted.resize(newSize);
	editLevel.cells.clear();
	editLevel.cells.resize(newSize);
	FormPositioning();
	Utilities::CenterWindow(EditorForm);
	DrawGrid1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::OnPaletteColorUpdate()
{
    const unsigned int clr1 = StrToUInt(LabeledEdit6->Text);
	const unsigned int clr2 = StrToUInt(LabeledEdit7->Text);
	for (unsigned int i = 0; i < Stuff::ePaletteCount; ++i)
	{
		cells[i].clr1 = clr1;
		cells[i].clr2 = clr2;
	}
	DrawGrid2->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::OnResizeColorsUpdate(unsigned int newCnt)
{
	unsigned int wasCnt = editLevel.colorsCount;
	editLevel.colorMap.resize(newCnt);
	editLevel.colorsCount = newCnt;
	Utilities::StringsResize(ComboBox3->Items, editLevel.colorsCount);
	if (wasCnt < newCnt)
        editLevel.FillDefaultColors(wasCnt, newCnt - 1);
	if (ComboBox3->ItemIndex == -1)
		ComboBox3->ItemIndex = 0;

    if (StrToUInt(LabeledEdit6->Text) >= editLevel.colorsCount)
		LabeledEdit6->Text = UIntToStr(0u);
	if (StrToUInt(LabeledEdit7->Text) >= editLevel.colorsCount)
		LabeledEdit7->Text = UIntToStr(0u);

    OnPaletteColorUpdate();

	for (unsigned int i = 0; i < editLevel.cells.size(); ++i)
	{
		Cell& cell = editLevel.cells[i];
		if (cell.clr1 >= editLevel.colorsCount)
			cell.clr1 = 0u;
		if (cell.clr2 >= editLevel.colorsCount)
			cell.clr2 = 0u;
    }

	DrawGrid1->Refresh();
	ComboBox3->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::FormPositioning()
{
	const unsigned int mainOffset = DrawGrid1->Top;

    // width
	const unsigned int reservedAreaX = mainOffset * 3 +
		std::max(Panel1->Width, Panel2->Width);
	const unsigned int workingAreaX = Screen->Width - reservedAreaX - Offsets::fReservedWidth;

	// height
	const unsigned int reservedAreaY = mainOffset * 3 + DrawGrid2->Height;
	const unsigned int workingAreaY = Screen->Height - reservedAreaY - Offsets::fReservedHeight;

	// balancing grids
	DrawGrid1->RowCount = editLevel.rowCount;
	DrawGrid1->ColCount = editLevel.colCount;
    Utilities::BalanceGrid(DrawGrid1, workingAreaX, workingAreaY);
	DrawGrid2->Top = DrawGrid1->Top + DrawGrid1->Height + mainOffset;

    Panel2->Top = DrawGrid2->Top - mainOffset - Panel2->Height;

	EditorForm->ClientWidth = std::max(DrawGrid1->Width + reservedAreaX,
		DrawGrid2->Width + 2 * mainOffset);
	EditorForm->ClientHeight = std::max(DrawGrid1->Height + reservedAreaY,
		Panel1->Height + Panel2->Height + mainOffset * 5);

	Panel1->Left = EditorForm->ClientWidth - Panel1->Width - mainOffset;
	Panel2->Left = EditorForm->ClientWidth - Panel2->Width - mainOffset;

    DrawGrid1->TopRow = 0;
	DrawGrid1->LeftCol = 0;
	Utilities::SetGridSelection(DrawGrid1, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::FormUpdate()
{
	ComboBox1->ItemIndex = (int)editLevel.type;
	ComboBox2->ItemIndex = (int)editLevel.difficulty;
	Utilities::StringsResize(ComboBox3->Items, editLevel.colorsCount);
    if (ComboBox3->ItemIndex == -1)
		ComboBox3->ItemIndex = 0;
	LabeledEdit1->Text = UIntToStr(editLevel.levelId);
	LabeledEdit2->Text = UIntToStr(editLevel.time);
	LabeledEdit3->Text = UIntToStr(editLevel.colCount);
	LabeledEdit4->Text = UIntToStr(editLevel.rowCount);
	LabeledEdit5->Text = UIntToStr(editLevel.colorsCount);

	/*
	if (StrToUInt(LabeledEdit6->Text) >= editLevel.colorsCount)
		LabeledEdit6->Text = UIntToStr(0u);
	if (StrToUInt(LabeledEdit7->Text) >= editLevel.colorsCount)
		LabeledEdit7->Text = UIntToStr(0u);
	*/
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::FormInitialize()
{
	FormPositioning();
	FormUpdate();
	ComboBox3->ItemIndex = 0;

	const unsigned int clr1 = 0;
	const unsigned int clr2 = 0;
	LabeledEdit6->Text = UIntToStr(clr1);
	LabeledEdit7->Text = UIntToStr(clr2);
	for (unsigned int i = 0; i < Stuff::ePaletteCount; ++i)
	{
		cells[i] = Stuff::ePalette[i];
		cells[i].clr1 = clr1;
		cells[i].clr2 = clr2;
	}

	DrawGrid2->TopRow = 0;
	DrawGrid2->LeftCol = 0;
	Utilities::SetGridSelection(DrawGrid2, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::AfterInit()
{
	leftMouse = MyPoint();
	middleMouse = MyPoint();
	rightMouse = MyPoint();

	EditorForm->ActiveControl = DrawGrid2;
	Utilities::CenterWindow(EditorForm);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::FormShow(TObject *Sender)
{
	if (currentLevel.fromEditor) // back from the game
	{
        currentLevel.fromEditor = false;
		FormUpdate();
	}
	else // from the main menu
	{
		FormInitialize();
	}

    AfterInit();
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::ComboBox3Change(TObject *Sender)
{
    if (ColorDialog1->Execute())
	{
		editLevel.colorMap[ComboBox3->ItemIndex] = ColorDialog1->Color;
		ComboBox3->Refresh();
		DrawGrid1->Refresh();
        DrawGrid2->Refresh();
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::ComboBox3DrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
    TComboBox* comboBox = (TComboBox*)Control;
	TCanvas* canvas = comboBox->Canvas;
	int offset = Rect.Bottom - Rect.Top;

	canvas->Brush->Style = bsSolid;
	canvas->Brush->Color = State.Contains(odSelected) &&
		!State.Contains(odComboBoxEdit) ? clHighlight : clWhite;
	canvas->FillRect(Rect);

	canvas->Brush->Color = editLevel.colorMap[Index];
	canvas->Pen->Style = psSolid;
	canvas->Pen->Color = clBlack;
	canvas->Pen->Width = 1;
	TRect rect1 = TRect(Rect.Left + 1, Rect.top + 1,
		Rect.left + offset - 1, Rect.Top + offset - 1);
	canvas->Rectangle(rect1);

	canvas->Brush->Style = bsClear;
	canvas->Font->Color = clBlack;
	TRect rect2 = TRect(Rect.left + offset, Rect.top, Rect.right, Rect.bottom);
	canvas->TextRect(rect2, rect2.Left + 3, rect2.Top, L"Цвет " + IntToStr(Index));
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::Button1Click(TObject *Sender)
{
	if (OpenDialog1->Execute())
	{
		editLevel = Level(OpenDialog1->FileName.c_str(), true);
        editLevel.completed = false;
		FormInitialize();
		AfterInit();
		ComboBox3->Refresh();
		// возможно, не нужно
		DrawGrid1->Refresh();
		DrawGrid2->Refresh();
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::Button2Click(TObject *Sender)
{
	SaveDialog1->FileName = L"Level" + UIntToStr(editLevel.levelId);
	if (SaveDialog1->Execute())
	{
		editLevel.completed = false;
		editLevel.WriteToFile(SaveDialog1->FileName.c_str());
		Application->MessageBox(L"Уровень сохранен.", L"Успешно");
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::DrawGrid1DrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State)
{
	const unsigned int cell = Utilities::LinearFrom2D(ACol, ARow, editLevel.colCount);

	DrawGrid1->Canvas->Brush->Color = Colors::gGridBack;
	DrawGrid1->Canvas->FillRect(Rect);

	Textures::DrawCell(DrawGrid1->Canvas, editLevel.colorMap,
		editLevel.cells[cell], Rect);

	TRect rect1(Rect.left - (ACol != DrawGrid1->LeftCol),
		Rect.top - (ARow != DrawGrid1->TopRow), Rect.right, Rect.bottom);
	Utilities::DrawBorderedRect(DrawGrid1->Canvas, rect1, Colors::grLine, Offsets::grLineWidth);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::DrawGrid2DrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State)
{
	DrawGrid2->Canvas->Brush->Color = Colors::gGridBack;
	DrawGrid2->Canvas->FillRect(Rect);

	Textures::DrawCell(DrawGrid2->Canvas, editLevel.colorMap, cells[ACol], Rect);

	TRect rect1(Rect.left - (ACol != DrawGrid2->LeftCol),
		Rect.top - (ARow != DrawGrid2->TopRow), Rect.right, Rect.bottom);
	Utilities::DrawBorderedRect(DrawGrid2->Canvas, rect1, Colors::grLine, Offsets::grLineWidth);

    if (State.Contains(gdSelected))
	{
		TRect rect2(rect1.left + 1, rect1.top + 1, rect1.right - 1, rect1.bottom - 1);
		Utilities::DrawBorderedRect(DrawGrid2->Canvas, rect2, Colors::gGridSelect, Offsets::gGridSelectWidth);
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::Button3Click(TObject *Sender)
{
	for (int i = 0; i < Stuff::ePaletteCount; ++i)
		cells[i].DoRotate();
    DrawGrid2->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::Button4Click(TObject *Sender)
{
	currentLevel = editLevel;
	GameForm->Show();
	EditorForm->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::GlobalMouseDown(TMouseButton Button, TShiftState Shift)
{
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
void __fastcall TEditorForm::GlobalMouseUp(TMouseButton Button, TShiftState Shift)
{
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
		{
			if (leftMouse.col == -1 || leftMouse.row == -1)
				break;

			unsigned int cellTo = Utilities::LinearFrom2D(col1,
				row1, editLevel.colCount);
			if (leftFromPalette || (DrawGrid2->Selection.Left != -1 &&
				DrawGrid2->Selection.Top != -1))
			{
				unsigned int cellFrom = leftFromPalette ? leftMouse.col :
					DrawGrid2->Selection.Left;
				Cell& cell = editLevel.cells[cellTo];
				const Cell& newCell = cells[cellFrom];
				Ground ground = cell.ground;
				if (newCell.type == CellType::Empty)
				{
					cell.ground = ground == Ground::Empty ? Ground::Stone :
						Ground::Empty;
				}
				else
				{
                    cell = newCell;
					cell.ground = ground;
                }
				DrawGrid1->Refresh();
			}

			leftMouse = MyPoint();
			break;
		}

		case mbRight:
			if (rightMouse.col == -1 || rightMouse.row == -1)
				break;

			if (rightMouse.col == col1 && rightMouse.row == row1)
			{
				const unsigned int cellTo = Utilities::LinearFrom2D(col1, row1, editLevel.colCount);
				Cell& cell = editLevel.cells[cellTo];
				Ground ground = cell.ground;
				cell = Stuff::gEmptyCell;
                cell.ground = ground;
				DrawGrid1->Refresh();
			}

			rightMouse = MyPoint();
			break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::GlobalMouseMove(TShiftState Shift)
{
    Utilities::CheckGridDrag(DrawGrid1, middleMouse, Shift, Mouse->CursorPos);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::FormMouseMove(TObject *Sender, TShiftState Shift, int X,
	int Y)
{
	GlobalMouseMove(Shift);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::DrawGrid1MouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
	GlobalMouseMove(Shift);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::DrawGrid1MouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	GlobalMouseDown(Button, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::DrawGrid1MouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
    GlobalMouseUp(Button, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::DrawGrid2MouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
	GlobalMouseMove(Shift);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::DrawGrid2MouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	GlobalMouseDown(Button, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::DrawGrid2MouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	GlobalMouseUp(Button, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::DrawGrid1MouseWheelDown(TObject *Sender, TShiftState Shift,
	TPoint &MousePos, bool &Handled)
{
    Handled = true;
	if (Shift.Contains(ssMiddle))
		return;

	DrawGrid1->TopRow = Utilities::NormalizeGridRow(DrawGrid1->TopRow + 1, DrawGrid1);
	DrawGrid1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::DrawGrid1MouseWheelUp(TObject *Sender, TShiftState Shift,
	TPoint &MousePos, bool &Handled)
{
    Handled = true;
	if (Shift.Contains(ssMiddle))
		return;

	DrawGrid1->TopRow = Utilities::NormalizeGridRow(DrawGrid1->TopRow - 1, DrawGrid1);
	DrawGrid1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::ComboBox1Change(TObject *Sender)
{
	if (ComboBox1->ItemIndex != -1)
		editLevel.type = (LevelType)ComboBox1->ItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::ComboBox2Change(TObject *Sender)
{
	if (ComboBox2->ItemIndex != -1)
		editLevel.difficulty = (LevelDifficulty)ComboBox2->ItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::LabeledEdit1Update(TObject *Sender)
{
	try {
		editLevel.levelId = StrToUInt(((TLabeledEdit*)Sender)->Text);
	}
	catch (...) {
		Utilities::Exception();
		EditorForm->ActiveControl = (TWinControl*)Sender;
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::LabeledEdit1KeyPress(TObject *Sender, System::WideChar &Key)
{
    if (Key == VK_RETURN)
		LabeledEdit1Update(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::LabeledEdit2Update(TObject *Sender)
{
	try {
		editLevel.time = StrToUInt(((TLabeledEdit*)Sender)->Text);
	}
	catch (...) {
		Utilities::Exception();
		EditorForm->ActiveControl = (TWinControl*)Sender;
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::LabeledEdit2KeyPress(TObject *Sender, System::WideChar &Key)
{
    if (Key == VK_RETURN)
		LabeledEdit2Update(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::LabeledEdit3Update(TObject *Sender)
{
	try {
		unsigned int ret = StrToUInt(((TLabeledEdit*)Sender)->Text);
		if (!ret)
			throw 0;
		if (editLevel.colCount != ret)
		{
			editLevel.colCount = ret;
			OnResizeUpdate();
        }
	}
	catch (...) {
		Utilities::Exception();
		EditorForm->ActiveControl = (TWinControl*)Sender;
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::LabeledEdit3KeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == VK_RETURN)
		LabeledEdit3Update(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::LabeledEdit4Update(TObject *Sender)
{
	try {
		unsigned int ret = StrToUInt(((TLabeledEdit*)Sender)->Text);
		if (!ret)
			throw 0;
		if (editLevel.rowCount != ret)
		{
			editLevel.rowCount = ret;
			OnResizeUpdate();
        }
	}
	catch (...) {
		Utilities::Exception();
		EditorForm->ActiveControl = (TWinControl*)Sender;
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::LabeledEdit4KeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == VK_RETURN)
		LabeledEdit4Update(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::LabeledEdit5Update(TObject *Sender)
{
    try {
		unsigned int ret = StrToUInt(((TLabeledEdit*)Sender)->Text);
		if (!ret)
			throw 0;
		if (editLevel.colorsCount != ret)
		{
			OnResizeColorsUpdate(ret);
			ComboBox3->Refresh();
		}
	}
	catch (...) {
		Utilities::Exception();
		EditorForm->ActiveControl = (TWinControl*)Sender;
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::LabeledEdit5KeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == VK_RETURN)
		LabeledEdit5Update(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::LabeledEdit6Update(TObject *Sender)
{
	try {
		unsigned int ret = StrToUInt(((TLabeledEdit*)Sender)->Text);
		if (ret >= editLevel.colorsCount)
			throw 0;
		OnPaletteColorUpdate();
	}
	catch (...) {
		Utilities::Exception();
		EditorForm->ActiveControl = (TWinControl*)Sender;
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::LabeledEdit6KeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == VK_RETURN)
		LabeledEdit6Update(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::LabeledEdit7Update(TObject *Sender)
{
    try {
		unsigned int ret = StrToUInt(((TLabeledEdit*)Sender)->Text);
		if (ret >= editLevel.colorsCount)
			throw 0;
		OnPaletteColorUpdate();
	}
	catch (...) {
		Utilities::Exception();
		EditorForm->ActiveControl = (TWinControl*)Sender;
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::LabeledEdit7KeyPress(TObject *Sender, System::WideChar &Key)
{
    if (Key == VK_RETURN)
		LabeledEdit7Update(Sender);
}
//---------------------------------------------------------------------------


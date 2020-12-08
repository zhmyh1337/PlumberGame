//---------------------------------------------------------------------------

#ifndef EditorUnit1H
#define EditorUnit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "Level.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TEditorForm : public TForm
{
__published:	// IDE-managed Components
	TDrawGrid *DrawGrid1;
	TDrawGrid *DrawGrid2;
	TPanel *Panel1;
	TComboBox *ComboBox1;
	TComboBox *ComboBox2;
	TLabeledEdit *LabeledEdit2;
	TLabeledEdit *LabeledEdit1;
	TLabeledEdit *LabeledEdit3;
	TLabeledEdit *LabeledEdit4;
	TLabeledEdit *LabeledEdit5;
	TComboBox *ComboBox3;
	TColorDialog *ColorDialog1;
	TButton *Button1;
	TButton *Button2;
	TPanel *Panel2;
	TLabeledEdit *LabeledEdit6;
	TLabeledEdit *LabeledEdit7;
	TButton *Button3;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TButton *Button4;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ComboBox3Change(TObject *Sender);
	void __fastcall ComboBox3DrawItem(TWinControl *Control, int Index, TRect &Rect,
		  TOwnerDrawState State);
	void __fastcall GlobalMouseDown(TMouseButton Button, TShiftState Shift);
	void __fastcall GlobalMouseUp(TMouseButton Button, TShiftState Shift);
	void __fastcall GlobalMouseMove(TShiftState Shift);
	void __fastcall OnResizeUpdate();
	void __fastcall OnResizeColorsUpdate(unsigned int newCnt);
    void __fastcall OnPaletteColorUpdate();
	void __fastcall FormUpdate();
	void __fastcall FormInitialize();
	void __fastcall FormPositioning();
    void __fastcall AfterInit();
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall DrawGrid1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall DrawGrid2DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall DrawGrid1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall DrawGrid1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall DrawGrid1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall DrawGrid2MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall DrawGrid2MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall DrawGrid2MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall DrawGrid1MouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall DrawGrid1MouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall ComboBox1Change(TObject *Sender);
	void __fastcall ComboBox2Change(TObject *Sender);
	void __fastcall LabeledEdit1Update(TObject *Sender);
	void __fastcall LabeledEdit2Update(TObject *Sender);
	void __fastcall LabeledEdit3Update(TObject *Sender);
	void __fastcall LabeledEdit3KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall LabeledEdit2KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall LabeledEdit1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall LabeledEdit4Update(TObject *Sender);
	void __fastcall LabeledEdit4KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall LabeledEdit5Update(TObject *Sender);
	void __fastcall LabeledEdit5KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall LabeledEdit6Update(TObject *Sender);
	void __fastcall LabeledEdit6KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall LabeledEdit7Update(TObject *Sender);
	void __fastcall LabeledEdit7KeyPress(TObject *Sender, System::WideChar &Key);







private:	// User declarations
public:		// User declarations
	__fastcall TEditorForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEditorForm *EditorForm;
extern Level editLevel;
//---------------------------------------------------------------------------
#endif

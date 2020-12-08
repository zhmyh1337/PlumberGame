//---------------------------------------------------------------------------

#ifndef GameUnit1H
#define GameUnit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "Level.h"
//---------------------------------------------------------------------------
class TGameForm : public TForm
{
__published:	// IDE-managed Components
	TDrawGrid *DrawGrid1;
	TDrawGrid *DrawGrid2;
	TPanel *Panel1;
	TLabel *Label1;
	TLabel *Label2;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TTimer *Timer1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormInitialization();
    void __fastcall LevelInitialization();
	void __fastcall DrawGrid1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall DrawGrid2DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall DrawGrid1MouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall DrawGrid1MouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall DrawGrid1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall DrawGrid1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall DrawGrid1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall BombsLabelUpdate();
    void __fastcall TimeLabelUpdate();
	void __fastcall DrawGrid2MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall DrawGrid2MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall GlobalMouseDown(TMouseButton Button, TShiftState Shift);
	void __fastcall GlobalMouseUp(TMouseButton Button, TShiftState Shift);
    void __fastcall GlobalMouseMove(TShiftState Shift);
	void __fastcall DrawGrid2MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall StartSecondPhase();

private:	// User declarations
public:		// User declarations
	__fastcall TGameForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGameForm *GameForm;
extern Level currentLevel;
//---------------------------------------------------------------------------
#endif

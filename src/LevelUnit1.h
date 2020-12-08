//---------------------------------------------------------------------------

#ifndef LevelUnit1H
#define LevelUnit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TLevelForm : public TForm
{
__published:	// IDE-managed Components
	TDrawGrid *DrawGrid1;
	TButton *Button1;
	TButton *Button2;
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall DrawGrid1MouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall DrawGrid1MouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall DrawGrid1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall DrawGrid1TopLeftChanged(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall DrawGrid1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EnterLevel();
	void __fastcall DrawGrid1DblClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TLevelForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLevelForm *LevelForm;
//---------------------------------------------------------------------------
#endif

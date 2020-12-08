//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainUnit1.h"
#include "LevelUnit1.h"
#include "EditorUnit1.h"
#include "Utilities.h"
#include "Textures.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Button1Click(TObject *Sender)
{
    LevelForm->Show();
	MainForm->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormShow(TObject *Sender)
{
	MainForm->ActiveControl = Button1;
	Utilities::CenterWindow(MainForm);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
    Textures::PreLoadTextures();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Button2Click(TObject *Sender)
{
	editLevel = Level();
	EditorForm->Show();
    MainForm->Hide();
}
//---------------------------------------------------------------------------


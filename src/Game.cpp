//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("MainUnit1.cpp", MainForm);
USEFORM("LevelUnit1.cpp", LevelForm);
USEFORM("GameUnit1.cpp", GameForm);
USEFORM("EditorUnit1.cpp", EditorForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = false;
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TLevelForm), &LevelForm);
		Application->CreateForm(__classid(TGameForm), &GameForm);
		Application->CreateForm(__classid(TEditorForm), &EditorForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

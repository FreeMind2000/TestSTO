//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit_frmAbout.h"
#include <Strutils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAbout *frmAbout;
//---------------------------------------------------------------------------
__fastcall TfrmAbout::TfrmAbout(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmAbout::Button_HelpClick(TObject *Sender)
{
	AnsiString path = "\"" + Strutils::ReplaceStr(Application->ExeName, ".exe", ".mht") + "\"";

	ShellExecute(NULL,"open", path.c_str(), NULL, NULL,SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

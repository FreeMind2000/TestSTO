//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit_frmHelpSut.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Unit_frmHelpExp1"
#pragma resource "*.dfm"
TfrmHelpSut *frmHelpSut;
//---------------------------------------------------------------------------
__fastcall TfrmHelpSut::TfrmHelpSut(TComponent* Owner)
	: TfrmHelpExp1(Owner)
{
}
//---------------------------------------------------------------------------

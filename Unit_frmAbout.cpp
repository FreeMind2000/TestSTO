//---------------------------------------------------------------------------
// < TestSTO >
//---------------------------------------------------------------------------
// Программа для визуализации уравнений движений
// классической и релятивистской физики, иллюстрирующая
// противоречие в Специальной Теории Относительности А.Эйнштейна
//---------------------------------------------------------------------------
// Автор: Александр Меркелов (free_mind2@list.ru)
// Форум программы: http://www.free---mind.mirbb.net/t7-topic
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit_frmAbout.h"
#include <Strutils.hpp>
#include "Unit_frmHello.h"
#include "Unit_frmMain.h"
#include "Unit_frmHelpSut.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAbout *frmAbout;
//---------------------------------------------------------------------------
__fastcall TfrmAbout::TfrmAbout(TComponent* Owner)
	: TForm(Owner)
{
	unsigned mask = SendMessage(m_RichEdit_1->Handle, EM_GETEVENTMASK, 0, 0);
	SendMessage(m_RichEdit_1->Handle, EM_SETEVENTMASK, 0, mask | ENM_LINK);
	SendMessage(m_RichEdit_1->Handle, EM_AUTOURLDETECT, true, 0);

	m_RichEdit_1->Text = m_RichEdit_1->Text + " ";
}

//---------------------------------------------------------------------------
void __fastcall TfrmAbout::WndProc(Messages::TMessage &Message)
{ 
	if(Message.Msg == WM_NOTIFY)
	{
		if(((LPNMHDR)Message.LParam)->code == EN_LINK)
		{
			ENLINK* p = (ENLINK *)Message.LParam;
			if(p->msg == WM_LBUTTONDOWN)
			{
				SendMessage(m_RichEdit_1->Handle, EM_EXSETSEL, 0, (LPARAM)&(p->chrg));

				ShellExecute(Handle, "open", m_RichEdit_1->SelText.c_str(),
					0, 0, SW_SHOWNORMAL);
			}
		}
	}

	TForm::WndProc(Message);
}

//---------------------------------------------------------------------------
void __fastcall TfrmAbout::Button_HelpClick(TObject *Sender)
{
	AnsiString path = "\"" + Strutils::ReplaceStr(Application->ExeName, ".exe", ".mht") + "\"";

	ShellExecute(NULL,"open", path.c_str(), NULL, NULL,SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmAbout::Button_SutClick(TObject *Sender)
{
	frmHelpSut->ShowModal();	
}
//---------------------------------------------------------------------------


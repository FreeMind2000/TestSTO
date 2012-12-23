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

#include "Unit_frmHello.h"
#include "Unit_frmMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmHello *frmHello;
//---------------------------------------------------------------------------
__fastcall TfrmHello::TfrmHello(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmHello::FormShow(TObject *Sender)
{
	AnsiString start_page = "file:///" + ExtractFileDir(Application->ExeName)
		+ "\\TestSTO.mht";

	m_WebBrowser_1->Navigate(start_page);
}
//---------------------------------------------------------------------------
void __fastcall TfrmHello::Button_NextClick(TObject *Sender)
{
	frmMain->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TfrmHello::Button_BackClick(TObject *Sender)
{
	try
	{
		m_WebBrowser_1->GoBack();
	}
	catch(...){}
}
//---------------------------------------------------------------------------
void __fastcall TfrmHello::Button_ForwardClick(TObject *Sender)
{
	try
	{
		m_WebBrowser_1->GoForward();
	}
	catch(...){}
}
//---------------------------------------------------------------------------


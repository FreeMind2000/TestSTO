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

#ifndef Unit_frmHelloH
#define Unit_frmHelloH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <OleCtrls.hpp>
#include <SHDocVw.hpp>
//---------------------------------------------------------------------------
class TfrmHello : public TForm
{
__published:	// IDE-managed Components
	TWebBrowser *m_WebBrowser_1;
	TButton *Button_Next;
	TButton *Button_Back;
	TButton *Button_Forward;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button_NextClick(TObject *Sender);
	void __fastcall Button_BackClick(TObject *Sender);
	void __fastcall Button_ForwardClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmHello(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmHello *frmHello;
//---------------------------------------------------------------------------
#endif

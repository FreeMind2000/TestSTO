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
#ifndef Unit_frmAboutH
#define Unit_frmAboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TfrmAbout : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *m_RichEdit_1;
	TButton *Button_Help;
	TImage *m_Image_1;
	TButton *Button_Sut;
	void __fastcall Button_HelpClick(TObject *Sender);
	void __fastcall Button_SutClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmAbout(TComponent* Owner);

	// переопределение
	void __fastcall WndProc(Messages::TMessage &Message);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAbout *frmAbout;
//---------------------------------------------------------------------------
#endif

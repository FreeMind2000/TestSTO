//---------------------------------------------------------------------------
// < TestSTO >
//---------------------------------------------------------------------------
// ��������� ��� ������������ ��������� ��������
// ������������ � �������������� ������, ��������������
// ������������ � ����������� ������ ��������������� �.���������
//---------------------------------------------------------------------------
// �����: ��������� �������� (free_mind2@list.ru)
// ����� ���������: http://www.free---mind.mirbb.net/t7-topic
//---------------------------------------------------------------------------

#ifndef Unit_frmSettingsH
#define Unit_frmSettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmSettings : public TForm
{
__published:	// IDE-managed Components
	TValueListEditor *m_VLE_1;
	TButton *Button_OK;
	TButton *Button_Cancel;
	TRichEdit *m_RichEdit_Descript;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button_OKClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmSettings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSettings *frmSettings;
//---------------------------------------------------------------------------
#endif

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
#ifndef Unit_frmHelpExp1H
#define Unit_frmHelpExp1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmHelpExp1 : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *m_RichEdit_1;
	TButton *Button_1;
private:	// User declarations
public:		// User declarations
	__fastcall TfrmHelpExp1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmHelpExp1 *frmHelpExp1;
//---------------------------------------------------------------------------
#endif

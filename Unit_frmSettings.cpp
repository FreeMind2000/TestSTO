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
#include <vcl.h>
#pragma hdrstop

#include "Unit_frmSettings.h"
#include "Unit_frmMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSettings *frmSettings;
//---------------------------------------------------------------------------
__fastcall TfrmSettings::TfrmSettings(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSettings::FormShow(TObject *Sender)
{
	m_VLE_1->Values["�������� �����"] = FloatToStr(g_light_c);
	m_VLE_1->Values["�������� ������ (��� K') ������������ ��� K"] = FloatToStr(frmMain->m_CurExperiment->move_iso->coord_begin_point.v);
	m_VLE_1->Values["��� ������� (dt) � ��� �"] = FloatToStr(frmMain->m_CurExperiment->time_step);
	m_VLE_1->Values["����� (t) ����� ������������ � ��� K"] = FloatToStr(frmMain->m_CurExperiment->time_end);

	Valedit::TItemProp *ItemProp = m_VLE_1->ItemProps["�������� �����"];
	ItemProp->ReadOnly = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSettings::Button_OKClick(TObject *Sender)
{
	//g_light_c = StrToFloat(m_VLE_1->Values["�������� �����"]);
	frmMain->m_CurExperiment->move_iso->coord_begin_point.v = StrToFloat(m_VLE_1->Values["�������� ������ (��� K') ������������ ��� K"]);
	frmMain->m_CurExperiment->time_step = StrToFloat(m_VLE_1->Values["��� ������� (dt) � ��� �"]);
	frmMain->m_CurExperiment->time_end = StrToFloat(m_VLE_1->Values["����� (t) ����� ������������ � ��� K"]);


	// ������������� ����������� � ������ �����������
//	frmMain->m_CurExperiment->time_cur = frmMain->m_CurExperiment->time_end;
//	frmMain->m_CurExperiment->Run();
//	frmMain->m_CurExperiment->Show();

	// ����, ��� ��������� ������������ �� ��������� ���������������� �� ����
	frmMain->m_ComboBox_Experiment->Tag = 1;

	// ������������� ����������� � ������ �����������
	frmMain->m_ComboBox_ExperimentChange(NULL);

	this->Close();
}
//---------------------------------------------------------------------------

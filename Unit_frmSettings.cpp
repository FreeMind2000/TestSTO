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
	m_VLE_1->Values["Скорость света"] = FloatToStr(g_light_c);
	m_VLE_1->Values["Скорость поезда (ИСО K') относительно ИСО K"] = FloatToStr(frmMain->m_CurExperiment->move_iso->coord_begin_point.v);
	m_VLE_1->Values["Шаг времени (dt) в ИСО К"] = FloatToStr(frmMain->m_CurExperiment->time_step);
	m_VLE_1->Values["Время (t) конца эксперимента в ИСО K"] = FloatToStr(frmMain->m_CurExperiment->time_end);

	Valedit::TItemProp *ItemProp = m_VLE_1->ItemProps["Скорость света"];
	ItemProp->ReadOnly = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSettings::Button_OKClick(TObject *Sender)
{
	//g_light_c = StrToFloat(m_VLE_1->Values["Скорость света"]);
	frmMain->m_CurExperiment->move_iso->coord_begin_point.v = StrToFloat(m_VLE_1->Values["Скорость поезда (ИСО K') относительно ИСО K"]);
	frmMain->m_CurExperiment->time_step = StrToFloat(m_VLE_1->Values["Шаг времени (dt) в ИСО К"]);
	frmMain->m_CurExperiment->time_end = StrToFloat(m_VLE_1->Values["Время (t) конца эксперимента в ИСО K"]);


	// Перезапускаем эксперимент с новыми параметрами
//	frmMain->m_CurExperiment->time_cur = frmMain->m_CurExperiment->time_end;
//	frmMain->m_CurExperiment->Run();
//	frmMain->m_CurExperiment->Show();

	// флаг, что параметры эксперимента по умолчанию инициализировать не надо
	frmMain->m_ComboBox_Experiment->Tag = 1;

	// Перезапускаем эксперимент с новыми параметрами
	frmMain->m_ComboBox_ExperimentChange(NULL);

	this->Close();
}
//---------------------------------------------------------------------------

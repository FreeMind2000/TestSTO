//---------------------------------------------------------------------------
// < TestSTO >
//---------------------------------------------------------------------------
// �����: ��������� �������� (free_mind2@list.ru)
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit_frmMain.h"
#include <math.h>
#include "Unit_frmAbout.h"
#include "Unit_frmSettings.h"
#include "Unit_frmHelpExp1.h"
#include "Unit_frmHelpExp2.h"
#include "Unit_frmHelpExp3.h"
#include "Unit_frmHelpExp4.h"
#include "Unit_frmHelpExp5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
	m_CurExperiment = NULL;

	Application->HintHidePause = 10000;

	AnsiString str;
	str.printf("�������������� ������ (C=const=%0.2f ������������ ����� ���)", g_light_c);
	Panel_3->Caption = str;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormShow(TObject *Sender)
{
	// �������������� ����������� �� ������
	m_ComboBox_ExperimentChange(NULL);
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
	// ������� ������ �����������
	if (m_CurExperiment)
	{
		delete m_CurExperiment;
		m_CurExperiment = NULL;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::m_ComboBox_ExperimentChange(TObject *Sender)
{
	// ������� ������ �����������
	if (m_CurExperiment)
	{
		delete m_CurExperiment;
		m_CurExperiment = NULL;
	}

	// ������� �����
	if (m_ComboBox_Experiment->ItemIndex == 0)
	{
		m_CurExperiment = new MExperimentEasyMove;
	}
	else
	if (m_ComboBox_Experiment->ItemIndex == 1)
	{
		m_CurExperiment = new MExperimentLastArgumentOfKing;
	}
	else
	if (m_ComboBox_Experiment->ItemIndex == 2)
	{
		m_CurExperiment = new MExperimentLastArgumentOfKing2;
	}
	else
	if (m_ComboBox_Experiment->ItemIndex == 3)
	{
		m_CurExperiment = new MExperimentEnshtainTimers;
	}
	else
	if (m_ComboBox_Experiment->ItemIndex == 4)
	{
		m_CurExperiment = new MExperimentKingTimers;
	}

	// ������
	m_CurExperiment->Init();
	m_CurExperiment->Show();
	frmMain->m_Memo_Dbg->Lines->Add("----------");

	// ���� ������ - ���� ������ ���
	if (frmMain->Tag == 1)
	if (m_CurExperiment->m_frmHelp->Tag == 0)
	{
		m_CurExperiment->m_frmHelp->Tag = 1;
		m_CurExperiment->m_frmHelp->ShowModal();
	}
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::m_PaintBox_AbsSTOPaint(TObject *Sender)
{
	// ���������
	if (m_CurExperiment)
	{
		m_CurExperiment->Show();
	}
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::Button_StartExperimentClick(TObject *Sender)
{
	if (m_CurExperiment)
	{
		m_CurExperiment->Run();
		m_CurExperiment->Show();
		frmMain->m_Memo_Dbg->Lines->Add("----------");		
	}
}


//===========================================================================
//
// ������� ������
//
//===========================================================================


//---------------------------------------------------------------------------
// ������������
//---------------------------------------------------------------------------
void __fastcall MMatPoint::Show
(
)
{
	// ������� X
	int base_x = (int)(iso->visual_dx + iso->coord_begin_point.x + x + 0.5);

	// ��� ������
	iso->paint->Canvas->Pixels[base_x][visual_dy] = clWhite;

	// �����
	iso->paint->Canvas->Brush->Color = color;
//	int r = 10;
	iso->paint->Canvas->FrameRect(
		TRect(base_x-visual_rx, visual_dy-visual_ry, base_x+visual_rx+1, visual_dy+visual_ry+1));

	// ���������
	if (this->caption_type)
	{
		AnsiString caption;

		if (caption_type == t_ct_full)
        {
			caption.printf("%s (x=%0.2f, v=%0.2f, t=%0.2f)", name, x, v, t);
		}
		else
		if (caption_type == t_ct_name)
		{
			caption.printf("%s", name);
		}
		else
		if (caption_type == t_ct_name_time)
		{
			caption.printf("%s (x=%0.2f, t=%0.2f)", name, x, t);
		}

		iso->paint->Canvas->Font->Color = caption_color; //(TColor)RGB(0,255,100);
		iso->paint->Canvas->Brush->Color = clBlack;

		if (this->caption_up)
		{
			iso->paint->Canvas->TextOutA(base_x - 10, visual_dy - visual_ry - 15, caption);
		}
		else
		{
			iso->paint->Canvas->TextOutA(base_x - 10, visual_dy + visual_ry + 5, caption);
		}
	}

}

//---------------------------------------------------------------------------
// ������� ������� � ����������� ���
// t_move_abs: x = x0 + v * t
//---------------------------------------------------------------------------
void __fastcall MMatPoint::Move
(
	const int in_t_move,		// ��� ��������
	const double in_new_time	// ����� t ��� �������
)
{
	switch (in_t_move)
	{
		case MIso::t_move_abs:
		{
			MoveAbsFormula(in_new_time);
			break;
		}
	}
}

//---------------------------------------------------------------------------
// ������� �������� abs - ����������� x �� ������ t
// t_move_abs: x = x0 + v * t
//---------------------------------------------------------------------------
void __fastcall MMatPoint::MoveAbsFormula
(
	const double in_new_time	// ����� t ��� �������
)
{
	// x � ������� x ���������� �����
	// x0 � ��������� x ���������� �����
	// v ��������� �����
	// t � ������� ����� ��� �����

//	t = in_new_time;
//	x = x0 + v * t;

	if (in_new_time > 0)
	{
		t = t + frmMain->m_CurExperiment->time_step;
		x += v * frmMain->m_CurExperiment->time_step;
	}
}

//---------------------------------------------------------------------------
// ������ �������������� �������� � ���������� ���
//---------------------------------------------------------------------------
void __fastcall MMatPoint::Transform
(
	const int in_t_transform,	// ��� �������������
	const double in_move_v,		// ������� �������� ����� ��� ������������ �������
	MMatObject& out_obj			// ������ � ������� ����������� ��������� ��������������
)
{
	switch (in_t_transform)
	{
		case MIso::t_transform_sto:
		{
			TransformStoFormula(in_move_v, (MMatPoint&)out_obj);
			break;
		}
	}
}

////---------------------------------------------------------------------------
//// ������ �������������� �������� � ���������� ���
////---------------------------------------------------------------------------
//void __fastcall MMatPoint::TransformStoFormula
//(
//	const double in_move_v,		// ������� �������� ����� ��� ������������ �������
//	MMatPoint& out_obj			// ������ � ������� ����������� ��������� ��������������
//)
//{
//
//	// ������� �������� ���������
//	const double& m = in_move_v;
//	double mm = m * m;
//	double cc = g_light_c * g_light_c;
//
//	try
//	{
//		// v' = �������������� �������� ���������
//		// ��� �������� ����� ��� K ==> K'
//		out_obj.v = (v - m) / (1 - (v * m)/cc);
//	}
//	catch(...){}
//
//	try
//	{
//		// t' = �������������� �����
//		// ��� �������� ����� ��� K ==> K'
//		out_obj.t = t * sqrt(1 - (mm)/(cc));
//	}
//	catch(...){}
//
//	try
//	{
//		// �������� x0' = �������������� ���������
//		// ��� �������� ����� ��� K ==> K'
//		double x00 = (x0) / sqrt(1 - (mm)/(cc));
//
//		// ������� x' �� ����� ����������� ������ ������������ ��� K'
//		// x' = x0' + (v' * t')
//		out_obj.x = x00 + out_obj.v * out_obj.t;
//	}
//	catch(...){}
//
//
//	AnsiString dbg;
//	dbg.printf("[%s (x=%0.2f, v=%0.2f, t=%0.2f)] ==> [%s (x=%0.2f, v=%0.2f, t=%0.2f)]",
//		name, x, v, t, out_obj.name, out_obj.x, out_obj.v, out_obj.t);
//	frmMain->m_Memo_Dbg->Lines->Add(dbg);
//}

//---------------------------------------------------------------------------
// ������ �������������� �������� � ���������� ��� 2 - ������
//---------------------------------------------------------------------------
void __fastcall MMatPoint::TransformStoFormula
(
	const double in_move_v,		// ������� �������� ����� ��� ������������ �������
	MMatPoint& out_obj			// ������ � ������� ����������� ��������� ��������������
)
{

	// ������� �������� ���������
	const double& m = in_move_v;
	double mm = m * m;
	double cc = g_light_c * g_light_c;

	try
	{
		// v' = �������������� �������� ���������
		// ��� �������� ����� ��� K ==> K'
		out_obj.v = (v - m) / (1 - (v * m)/cc);
	}
	catch(...){}

	try
	{
		// t' = �������������� �����
		// ��� �������� ����� ��� K ==> K'
		out_obj.t = (t - (m * x)/cc) / sqrt(1 - (mm)/(cc));
	}
	catch(...){}

	try
	{
		// �������� x' = �������������� ���������
		// ��� �������� ����� ��� K ==> K'
		out_obj.x = (x - (m * t)) / sqrt(1 - (mm)/(cc));
	}
	catch(...){}


	AnsiString dbg;
	dbg.printf("[%s (x=%0.2f, v=%0.2f, t=%0.2f)] ==> [%s (x=%0.2f, v=%0.2f, t=%0.2f)]",
		name, x, v, t, out_obj.name, out_obj.x, out_obj.v, out_obj.t);
	frmMain->m_Memo_Dbg->Lines->Add(dbg);
}


//---------------------------------------------------------------------------
// ��������� ��������� �������
//---------------------------------------------------------------------------
void __fastcall MSensor2::Run
(
)
{
	int k_work = 0;
	//double sensor_zone = 0.001;	// ���� ���������������� �������
									// (�.�. ��� ������� delta t - ������ ����� �������������
									// ����� ����� � ������� ��������� ������)
	double sensor_zone = this->visual_rx; 

	// ���������� ��� ������� ��������� � ����� ���
	// � ������� ���-�� �������� ������ ���� �����������
	// � ��� �� �����, ��� � ������
	for (int i=this->iso->obj_list.CicleCount(); i>0; i--)
	{
		// ������� ������
		MMatPoint& cur_obj = (MMatPoint&)this->iso->obj_list.GetCurObj();

		// ���� ������ ����
		if (cur_obj.name.SubString(1,1) == this->type)
		{
			// ���� � ����� �����
			if ((cur_obj.x <= (this->x + sensor_zone)) && (cur_obj.x >= (this->x - sensor_zone)))
			if ((cur_obj.t <= (this->t + sensor_zone)) && (cur_obj.t >= (this->t - sensor_zone)))
			{
				k_work++;
			}
		}

		this->iso->obj_list.MoveNext();
	}

	// ���� ��������� ��� �� ����������
	if (this->sost == MSensor2::t_sost_undefined)
	{
		// ��������� ���������
		if (k_work >= 2)
		{
			this->sost = MSensor2::t_sost_on;
		}
		else
		if (k_work == 1)
		{
			this->sost = MSensor2::t_sost_off;
		}
		else
		{
			// �� ������ ���������
		}
	}
}

//---------------------------------------------------------------------------
// ������������
//---------------------------------------------------------------------------
void __fastcall MSensor2::Show
(
)
{
	AnsiString str_msg;
	TColor color_sost;

	switch (this->sost)
	{
		case MSensor2::t_sost_on:
			str_msg = "ON - ��-��-�!!!";
			color_sost = clRed;
		break;
		case MSensor2::t_sost_off:
			str_msg = "OFF";
			color_sost = clGreen;
		break;
		case MSensor2::t_sost_undefined:
			str_msg = "UNDEFINED";
			color_sost = this->color;
		break;
	}

	// ������� X
	int base_x = (int)(iso->visual_dx + iso->coord_begin_point.x + this->x + 0.5);

	// �����
	iso->paint->Canvas->Brush->Color = color_sost;
//	int r = 10;
	iso->paint->Canvas->FillRect(
		TRect(base_x-visual_rx, visual_dy-visual_ry, base_x+visual_rx+1, visual_dy+visual_ry+1));

	// ����. �����
	iso->paint->Canvas->Pen->Color = color_sost;
	iso->paint->Canvas->MoveTo(base_x, visual_dy);
	iso->paint->Canvas->LineTo(base_x, visual_dy - visual_ry - 70);


	// ���������
	AnsiString caption;
	caption.printf("%s (x=%0.2f, v=%0.2f, t=%0.2f) ��������� = %s",
		name, x, v, t, str_msg);

	iso->paint->Canvas->Font->Color = caption_color; //(TColor)RGB(0,255,100);
	iso->paint->Canvas->Brush->Color = color_sost;

	if (this->caption_up)
	{
		iso->paint->Canvas->TextOutA(base_x - 10, visual_dy - visual_ry - 70, caption);
	}
	else
	{
		iso->paint->Canvas->TextOutA(base_x - 10, visual_dy - visual_ry - 100, caption);	
	}	
}

//---------------------------------------------------------------------------
// ��������� ��������� �������
//---------------------------------------------------------------------------
void __fastcall MSensorMirror::Run
(
)
{
	int k_work = 0;
	//double sensor_zone = 0.001;	// ���� ���������������� �������
									// (�.�. ��� ������� delta t - ������ ����� �������������
									// ����� ����� � ������� ��������� ������)
	double sensor_zone = 0.01;//this->visual_rx;

	// ���������� ��� ������� ��������� � ����� ���
	// � ������� ���-�� �������� ������ ���� �����������
	// � ��� �� �����, ��� � ������
	for (int i=this->iso->obj_list.CicleCount(); i>0; i--)
	{
		// ������� ������
		MMatPoint& cur_obj = (MMatPoint&)this->iso->obj_list.GetCurObj();

		// ���� ������ ����
		if (cur_obj.name.SubString(1,1) == this->type)
		{
			// ���� � ����� �����
			if ((cur_obj.x <= (this->x + sensor_zone)) && (cur_obj.x >= (this->x - sensor_zone)))
			if ((cur_obj.t <= (this->t + sensor_zone)) && (cur_obj.t >= (this->t - sensor_zone)))
			{
				k_work++;

				// ������ ����������� ��������
				cur_obj.v = -cur_obj.v;
			}
		}

		this->iso->obj_list.MoveNext();
	}

	// ���� ��� �� �������
	if (this->sost != MSensorMirror::t_sost_no_count)
	{
		// ��������� ���������
		if (k_work >= 1)
		{
			this->sost++;
		}
		else
		{
			// �� ������ ���������
		}
	}
}

//---------------------------------------------------------------------------
// ������������
//---------------------------------------------------------------------------
void __fastcall MSensorMirror::Show
(
)
{
	AnsiString str_msg;
	TColor color_sost;

	if (this->sost == t_sost_no_count)
    {
		str_msg.printf("�������");
		color_sost = clGreen;
	}
	else
	{
		str_msg.printf("%d", this->sost);
		color_sost = clGreen;
	}

	// ������� X
	int base_x = (int)(iso->visual_dx + iso->coord_begin_point.x + this->x + 0.5);

	// �����
	iso->paint->Canvas->Brush->Color = color_sost;
//	int r = 10;
	iso->paint->Canvas->FillRect(
		TRect(base_x-visual_rx, visual_dy-visual_ry, base_x+visual_rx+1, visual_dy+visual_ry+1));

	// ����. �����
	iso->paint->Canvas->Pen->Color = color_sost;
	iso->paint->Canvas->MoveTo(base_x, visual_dy);
	iso->paint->Canvas->LineTo(base_x, visual_dy - visual_ry - 70);


	// ���������
	AnsiString caption;
	caption.printf("%s (x=%0.2f, v=%0.2f, t=%0.2f) ��������� = %s",
		name, x, v, t, str_msg);

	iso->paint->Canvas->Font->Color = caption_color; //(TColor)RGB(0,255,100);
	iso->paint->Canvas->Brush->Color = color_sost;

	if (this->caption_up)
	{
		iso->paint->Canvas->TextOutA(base_x - 10, visual_dy - visual_ry - 70, caption);
	}
	else
	{
		iso->paint->Canvas->TextOutA(base_x - 10, visual_dy - visual_ry - 100, caption);	
	}
}

//---------------------------------------------------------------------------
// ��������� ��������� �������
//---------------------------------------------------------------------------
void __fastcall MSensorStrelka::Run
(
)
{
	int k_work = 0;
	//double sensor_zone = 0.001;	// ���� ���������������� �������
									// (�.�. ��� ������� delta t - ������ ����� �������������
									// ����� ����� � ������� ��������� ������)
	double sensor_zone = 0.01;//this->visual_rx;

	// ���������� ��� ������� ��������� � ����� ���
	// � ������� ���-�� �������� ������ ���� �����������
	// � ��� �� �����, ��� � ������
	for (int i=this->iso->obj_list.CicleCount(); i>0; i--)
	{
		// ������� ������
		MMatPoint& cur_obj = (MMatPoint&)this->iso->obj_list.GetCurObj();

		// ���� ������ ����
		if (cur_obj.name.SubString(1,1) == this->type)
		{
			// ���� � ����� �����
			if ((cur_obj.x <= (this->x + sensor_zone)) && (cur_obj.x >= (this->x - sensor_zone)))
			if ((cur_obj.t <= (this->t + sensor_zone)) && (cur_obj.t >= (this->t - sensor_zone)))
			{
				k_work++;

				// ������ ����������� ��������
				//cur_obj.v = -cur_obj.v;
				cur_obj.caption_color = clYellow;
			}
		}

		this->iso->obj_list.MoveNext();
	}

	// ���� ��� �� �������
	if (this->sost != MSensorStrelka::t_sost_no_count)
	{
		// ��������� ���������
		if (k_work >= 1)
		{
			this->sost++;
			sost_update = true;
		}
		else
		{
			// �� ������ ���������
			sost_update = false;			
		}
	}
}

//---------------------------------------------------------------------------
// ������������
//---------------------------------------------------------------------------
void __fastcall MSensorStrelka::Show
(
)
{
	AnsiString str_msg;
	TColor color_sost;

	if (this->sost_update)
	{
		str_msg.printf("%d ���������!", this->sost);
		color_sost = clMaroon;
	}
	else
	{
		str_msg.printf("%d", this->sost);
		color_sost = clGreen;
	}


	// ������� X
	int base_x = (int)(iso->visual_dx + iso->coord_begin_point.x + this->x + 0.5);

	// �����
	iso->paint->Canvas->Brush->Color = color_sost;
//	int r = 10;
	iso->paint->Canvas->FillRect(
		TRect(base_x-visual_rx, visual_dy-visual_ry, base_x+visual_rx+1, visual_dy+visual_ry+1));

	// ����. �����
	iso->paint->Canvas->Pen->Color = color_sost;
	iso->paint->Canvas->MoveTo(base_x, visual_dy);
	iso->paint->Canvas->LineTo(base_x, visual_dy - visual_ry - 70);

	// ��� ������
	iso->paint->Canvas->Pixels[base_x][visual_dy] = clWhite;	

	// ���������
	AnsiString caption;
	caption.printf("%s (x=%0.2f, v=%0.2f, t=%0.2f) ���������: T=%s",
		name, x, v, t, str_msg);

	iso->paint->Canvas->Font->Color = caption_color; //(TColor)RGB(0,255,100);
	iso->paint->Canvas->Brush->Color = color_sost;

	if (this->caption_up)
	{
		iso->paint->Canvas->TextOutA(base_x - 10, visual_dy - visual_ry - 70, caption);
	}
	else
	{
		iso->paint->Canvas->TextOutA(base_x - 10, visual_dy - visual_ry - 100, caption);	
	}
}

//---------------------------------------------------------------------------
// ������������
//---------------------------------------------------------------------------
void __fastcall MIso::Show
(
)
{
	// ������� X
	int base_x = (int)(visual_dx + coord_begin_point.x + 0.5);

	// ����
	paint->Canvas->Pen->Color = color;
	paint->Canvas->Brush->Color = color;

	// ����� - ������ ���������
	int r = 3;
	//paint->Canvas->FrameRect(TRect(base_x-r, visual_dy-r, base_x+r+1, visual_dy+r+1));
	paint->Canvas->FrameRect(TRect(base_x-r, visual_dy-r, base_x+r+1, visual_dy+r+1));

	// ������������ �����
	paint->Canvas->MoveTo(base_x, visual_dy);
	paint->Canvas->LineTo(base_x, visual_dy - ver_line_len);
	// �������������� �����
	paint->Canvas->MoveTo(base_x, visual_dy);
	paint->Canvas->LineTo(base_x + hor_line_len, visual_dy);

	// ��� ������
	this->paint->Canvas->Pixels[base_x][visual_dy] = clWhite;	

	// ���������
	AnsiString caption;
	caption.printf("%s (v=%0.2f)", name, coord_begin_point.v);
	paint->Canvas->Font->Color = clWhite;
	paint->Canvas->Brush->Color = clBlack;
	paint->Canvas->TextOutA(base_x - 10, visual_dy+5, caption);

	// ������� ������ ��������� ��������
	for (int i=obj_list.CicleCount(); i>0; i--)
	{
		obj_list.GetCurObj().Show();

		obj_list.MoveNext();
	}
}

//---------------------------------------------------------------------------
// ������� ������� � ����������� ���
// t_move_abs: x = x0 + v * t
//---------------------------------------------------------------------------
void __fastcall MIso::MoveObjects
(
	const int in_t_move,		// ��� ��������
	const double in_new_time	// ����� t ��� �������
)
{
	// ����������� �������� ��� ������ = 0
	this->coord_begin_point.x = 0;
	this->coord_begin_point.t = in_new_time;

	// ������� ������ ��������� ��������
	for (int i=obj_list.CicleCount(); i>0; i--)
	{
		obj_list.GetCurObj().Move(in_t_move, in_new_time);

		obj_list.MoveNext();
	}
}

//---------------------------------------------------------------------------
// ������ �������������� �������� � ���������� ���
//---------------------------------------------------------------------------
void __fastcall MIso::TransformObjects
(
	const int in_t_transform,	// ��� �������������
	MIso* inout_iso          	// ��� � ������� ����������� ��������� �������������
)
{
	//-----
	// ����������� ��������� ������ ��������� inout_iso ������������ ������� ��� (this)
	if (frmMain->m_CheckBox_Experiment_FixMoveIso->Checked == false)
	{
		inout_iso->coord_begin_point.x = inout_iso->coord_begin_point.v * this->coord_begin_point.t;
	}
	else
	{
		// ����������� �������� ��� ������ = 0
		inout_iso->coord_begin_point.x = 0;
	}



	//-----
	// ��������������� ���������� ������� ����������� ������� (this)
	// � ���������� ��� inout_iso ���������� ������������ ���

	// ���������� ������ ��������� �������� � ������� ���
	// � ����������� � ���������� ���
	// (���-�� �������� � ������� ���������� ������ ���� ���������)
	inout_iso->obj_list.MoveFirst();
	for (int i=obj_list.CicleCount(); i>0; i--)
	{
		obj_list.GetCurObj().Transform(in_t_transform,
			inout_iso->coord_begin_point.v - this->coord_begin_point.v,
			inout_iso->obj_list.GetCurObj());

		obj_list.MoveNext();
		inout_iso->obj_list.MoveNext();
	}
}

//---------------------------------------------------------------------------
// ���������� ������� (��������� ������������ ��������)
//---------------------------------------------------------------------------
void __fastcall MIso::RunObjects
(
)
{
	// �.�. �� ���������� ������ ������ Run - ��� CurObj ��������
	// ������� ���������� ���������/������������ ��� ������� CurObj ����� MoveNext
	MListObj<MMatObject> *cur_obj;

	// ���������� ������ ��������� �������� � ������� ���
	for (int i=obj_list.CicleCount(); i>0; i--)
	{
		cur_obj = obj_list.CurObj;

		// � ������ ������� ��������� �-��� ���������
		obj_list.GetCurObj().Run();

		obj_list.CurObj = cur_obj;

		obj_list.MoveNext();
	}
}

//---------------------------------------------------------------------------
// ������������
//---------------------------------------------------------------------------
void __fastcall MExperimentBase::Show
(
)
{
	frmMain->m_PaintBox_AbsSTO->Canvas->Brush->Color = clBlack;
	frmMain->m_PaintBox_AbsSTO->Canvas->FillRect(
		TRect(0,0, frmMain->m_PaintBox_AbsSTO->Width, frmMain->m_PaintBox_AbsSTO->Height));

	frmMain->m_PaintBox_MoveSTO->Canvas->Brush->Color = clBlack;
	frmMain->m_PaintBox_MoveSTO->Canvas->FillRect(
		TRect(0,0, frmMain->m_PaintBox_MoveSTO->Width, frmMain->m_PaintBox_MoveSTO->Height));


	if (abs_iso)
	{
		abs_iso->Show();
	}
	if (move_iso)
	{
		move_iso->Show();
	}
}

//---------------------------------------------------------------------------
// ��������� � ������ ���������� �� �����
//---------------------------------------------------------------------------
void __fastcall MExperimentBase::Run
(
)
{
	// �������� ������� �����
	time_cur += time_step;

	// ���� ����� ��������� - ������
	if (time_cur > time_end)
	{
//		// ����������������� ������� � ������� t0
//		time_cur = time_start;
//		this->Reset();
//		frmMain->m_Memo_Dbg->Clear();

		// ����������������� ������� � ������� t0
		frmMain->m_ComboBox_Experiment->Tag = 1;	// ��������� ��������� ���������� �� ��������� (�.�. ����. �������)
		frmMain->m_ComboBox_ExperimentChange(NULL);
//		frmMain->m_Memo_Dbg->Clear();
		return;
	}

	// ������� ������� � ����������� ���
	// x = x0 + v * t
	abs_iso->MoveObjects(MIso::t_move_abs, time_cur);

	// ������� ����� ���
	// ���������� ����� ���������� abs_iso
	// � ������� ���������� ��������������
	// x' = (x - v*t) / sqrt(1 - (v*v)/(c*c))
	abs_iso->TransformObjects(MIso::t_transform_sto, move_iso);
	//move_iso = abs_iso->TransformToNewIso(v_move, MIso::t_transform_sto);

	// ���������� ������� � ���������� ���
	// (��������� ������������ �������� ��������� � ������ ���)
	abs_iso->RunObjects();

	// ���������� ������� � ���������� ���
	// (��������� ������������ �������� ��������� � ������ ���)
	move_iso->RunObjects();
}

//---------------------------------------------------------------------------
// ������������� MExperimentEasyMove
//---------------------------------------------------------------------------
void __fastcall MExperimentEasyMove::Init
(
)
{
	frmMain->m_Memo_Dbg->Clear();

	// �������� ������
	double v_vagon;

	//-----
	// �������������� ����������� ���� �� ����������
	// ����, ��� ��������� ������������ �� ��������� ���������������� �� ����
	if (frmMain->m_ComboBox_Experiment->Tag != 1)
	{
		time_start = 0;
		time_end = 50;
		time_step = 1;
		time_cur = time_start;
		this->m_frmHelp = frmHelpExp1;
		v_vagon = 20;

		// �������� ��������� �� ����� ��������
		frmSettings->m_VLE_1->Values["�������� �����"] = FloatToStr(g_light_c);
		frmSettings->m_VLE_1->Values["�������� ������ (��� K') ������������ ��� K"] = FloatToStr(v_vagon);
		frmSettings->m_VLE_1->Values["��� ������� (dt) � ��� �"] = FloatToStr(frmMain->m_CurExperiment->time_step);
		frmSettings->m_VLE_1->Values["����� (t) ����� ������������ � ��� K"] = FloatToStr(frmMain->m_CurExperiment->time_end);

	}
	// �������������� ��������� �� ���� ����������
	else
	{
    	// �������� ��������� �� ����� ��������
		time_start = 0;
		frmMain->m_CurExperiment->time_step = StrToFloat(frmSettings->m_VLE_1->Values["��� ������� (dt) � ��� �"]);
		frmMain->m_CurExperiment->time_end = StrToFloat(frmSettings->m_VLE_1->Values["����� (t) ����� ������������ � ��� K"]);

		time_cur = time_start;
		this->m_frmHelp = frmHelpExp1;
		v_vagon = StrToFloat(frmSettings->m_VLE_1->Values["�������� ������ (��� K') ������������ ��� K"]);


		// ���������� ����
		frmMain->m_ComboBox_Experiment->Tag = 0;
	}


	//-----
	// ������� ���������� ���
	abs_iso = new MIso;

	abs_iso->name = "K";
	abs_iso->color = clGreen;
	abs_iso->paint = frmMain->m_PaintBox_AbsSTO;
	abs_iso->visual_dx = 20;
	abs_iso->visual_dy = abs_iso->paint->Height - 30;
	abs_iso->ver_line_len = abs_iso->paint->Height;
	abs_iso->hor_line_len = abs_iso->paint->Width - 20*2;

	abs_iso->coord_begin_point.x0 = 0;
	abs_iso->coord_begin_point.x = 0;
	abs_iso->coord_begin_point.v = 0;
	abs_iso->coord_begin_point.t = 0;

	//-----
	// ��������� ������� ������������
	MMatPoint* new_mat_point;

	// �������� ������
	//double v_vagon = 20;
	// ������ �� ������� ������������ �����
	int vagon_visual_dy = abs_iso->visual_dy - 50;
	int vagon_visual_rx = 10;
	int vagon_visual_ry = 30;

	// ����� A1 - ������ ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A1";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = true;
	new_mat_point->x0 = 0;
	new_mat_point->x = new_mat_point->x0;
	new_mat_point->v = v_vagon;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	// ����� As - �������� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "As";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = false;
	new_mat_point->x0 = 120;
	new_mat_point->x = new_mat_point->x0;
	new_mat_point->v = v_vagon;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	// ����� A2 - ����� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A2";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;	
	new_mat_point->caption_up = true;
	new_mat_point->x0 = 240;
	new_mat_point->x = new_mat_point->x0;	
	new_mat_point->v = v_vagon;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);



	//-----
	// ������� ���������� ���
	move_iso = new MIso;

	move_iso->name = "K'";
	move_iso->color = clRed;
	move_iso->paint = frmMain->m_PaintBox_MoveSTO;
	move_iso->visual_dx = abs_iso->visual_dx;
	move_iso->visual_dy = move_iso->paint->Height - 30;;
	move_iso->ver_line_len = move_iso->paint->Height;
	move_iso->hor_line_len = 450;

	move_iso->coord_begin_point.x0 = 0;
	move_iso->coord_begin_point.x = 0;
	move_iso->coord_begin_point.v = v_vagon; // �������� ���������� ���
	move_iso->coord_begin_point.t = 0;

	//-----
	// �������� ������� �� abs_iso

	// ����� A1' - ������ ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A1'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = true;
//	new_mat_point->x0 = 0;
//	new_mat_point->v = 0;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� As' - �������� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "As'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = false;
//	new_mat_point->x0 = 120;
//	new_mat_point->v = 0;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� A2' - ����� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A2'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = true;
//	new_mat_point->x0 = 240;
//	new_mat_point->v = 0;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	//-----
	// ������� ������� ����������
	abs_iso->MoveObjects(MIso::t_move_abs, time_start);

	
	// ��������� ���������� abs_iso � ����������� ���
	// move_iso
	abs_iso->TransformObjects(MIso::t_transform_sto, move_iso);
}

//---------------------------------------------------------------------------
// ����� ��������� ��������
//---------------------------------------------------------------------------
void __fastcall MExperimentEasyMove::Reset
(
)
{
	// ����� �������� abs_iso
	// + �������� �������� �������� ������ ��� K' (�� ����� �������� � ����������)
	for (int i=abs_iso->obj_list.CicleCount(); i>0; i--)
	{
		// ������ A - A1,As,A2 = �����
		if (abs_iso->obj_list.GetCurObj().name.SubString(1,1) == "A")
		{
			MMatPoint& mp = (MMatPoint&)abs_iso->obj_list.GetCurObj();

			// �������� ������ = �������� ���������� ��� K'
			mp.v = this->move_iso->coord_begin_point.v;
		}

		// � ���� ������������ �������� ���
		// ������ �� ����������

		// �������� ������� �� �������� = C
		// �� �� ������...

		//
		abs_iso->obj_list.MoveNext();
	}

	// ����� �������� move_iso
	// � ���� ������������ �������� ���
	// ������ �� ����������
}

//---------------------------------------------------------------------------
// ������������� MExperimentLastArgumentOfKing
//---------------------------------------------------------------------------
void __fastcall MExperimentLastArgumentOfKing::Init
(
)
{
	frmMain->m_Memo_Dbg->Clear();

	// �������� ������
	double v_vagon;

	//-----
	// �������������� ����������� ���� �� ����������
	// ����, ��� ��������� ������������ �� ��������� ���������������� �� ����
	if (frmMain->m_ComboBox_Experiment->Tag != 1)
	{
		time_start = 0;
		time_end = 34;//100;
		time_step = 0.2;//10;
		time_cur = time_start;
		this->m_frmHelp = frmHelpExp2;
		v_vagon = 20;

		// �������� ��������� �� ����� ��������
		frmSettings->m_VLE_1->Values["�������� �����"] = FloatToStr(g_light_c);
		frmSettings->m_VLE_1->Values["�������� ������ (��� K') ������������ ��� K"] = FloatToStr(v_vagon);
		frmSettings->m_VLE_1->Values["��� ������� (dt) � ��� �"] = FloatToStr(frmMain->m_CurExperiment->time_step);
		frmSettings->m_VLE_1->Values["����� (t) ����� ������������ � ��� K"] = FloatToStr(frmMain->m_CurExperiment->time_end);

	}
	// �������������� ��������� �� ���� ����������
	else
	{
    	// �������� ��������� �� ����� ��������
		time_start = 0;
		frmMain->m_CurExperiment->time_step = StrToFloat(frmSettings->m_VLE_1->Values["��� ������� (dt) � ��� �"]);
		frmMain->m_CurExperiment->time_end = StrToFloat(frmSettings->m_VLE_1->Values["����� (t) ����� ������������ � ��� K"]);

		time_cur = time_start;
		this->m_frmHelp = frmHelpExp2;
		v_vagon = StrToFloat(frmSettings->m_VLE_1->Values["�������� ������ (��� K') ������������ ��� K"]);


		// ���������� ����
		frmMain->m_ComboBox_Experiment->Tag = 0;
	}

	//-----
	// ������� ���������� ���
	abs_iso = new MIso;

	abs_iso->name = "K";
	abs_iso->color = clGreen;
	abs_iso->paint = frmMain->m_PaintBox_AbsSTO;
	abs_iso->visual_dx = 20;
	abs_iso->visual_dy = abs_iso->paint->Height - 30;
	abs_iso->ver_line_len = abs_iso->paint->Height;
	abs_iso->hor_line_len = abs_iso->paint->Width - 40;

	abs_iso->coord_begin_point.x0 = 0;
	abs_iso->coord_begin_point.v = 0;
	abs_iso->coord_begin_point.t = 0;

	//-----
	// ��������� ������� ������������
	MMatPoint* new_mat_point;

	//-----
	// ������ (������)
	//-----
	int foton_visual_dy = abs_iso->visual_dy - 50;
	int foton_visual_rx = 5;
	int foton_visual_ry = 5;

	// ����� F1 - ����� ������� �� ������
	// �������������� � ������ � ����� A1 (� ������ ������)
	new_mat_point = new MMatPoint;
	new_mat_point->name = "F1";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clYellow;
	new_mat_point->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_mat_point->visual_dy = foton_visual_dy;
	new_mat_point->visual_rx = foton_visual_rx;
	new_mat_point->visual_ry = foton_visual_ry;
	new_mat_point->caption_up = true;
	new_mat_point->x0 = 0;
	new_mat_point->x = new_mat_point->x0;
	new_mat_point->v = g_light_c;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	// ����� F2 - ����� ������� �� ������
	// �������������� � ������ � ����� A2 (� ����� ������)
	new_mat_point = new MMatPoint;
	new_mat_point->name = "F2";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clYellow;
	new_mat_point->caption_color = (TColor)RGB(235,235,115);//clYellow;	
	new_mat_point->visual_dy = foton_visual_dy;
	new_mat_point->visual_rx = foton_visual_rx;
	new_mat_point->visual_ry = foton_visual_ry;
	new_mat_point->caption_up = false;
	new_mat_point->x0 = 240;
	new_mat_point->x = new_mat_point->x0;	
	new_mat_point->v = -g_light_c;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	//-----
	// �����
	//-----
	// �������� ������
	//double v_vagon = 20;
	// ������ �� ������� ������������ �����
	int vagon_visual_dy = abs_iso->visual_dy - 50;
	int vagon_visual_rx = 10;
	int vagon_visual_ry = 30;

	// ����� A1 - ������ ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A1";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = true;
	new_mat_point->x0 = 0;
	new_mat_point->x = new_mat_point->x0;	
	new_mat_point->v = v_vagon;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	// ����� As - �������� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "As";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = false;
	new_mat_point->x0 = 120;
	new_mat_point->x = new_mat_point->x0;	
	new_mat_point->v = v_vagon;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	// ����� A2 - ����� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A2";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = true;
	new_mat_point->x0 = 240;
	new_mat_point->x = new_mat_point->x0;	
	new_mat_point->v = v_vagon;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	//-----
	// ����� ����������
	//-----
	// �������� ������
	//double v_vagon = 20;
	// ������ �� ������� ������������ �����
	int vagon0_visual_dy = abs_iso->visual_dy - 50;
	int vagon0_visual_rx = 10;
	int vagon0_visual_ry = 30;

	// ����� B1 - ������ ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "B1";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clLtGray;
	new_mat_point->visual_dy = vagon0_visual_dy;
	new_mat_point->visual_rx = vagon0_visual_rx;
	new_mat_point->visual_ry = vagon0_visual_ry;
	new_mat_point->caption_up = true;
	new_mat_point->caption_type = MMatObject::t_ct_none;
	new_mat_point->x0 = 0;
	new_mat_point->x = new_mat_point->x0;	
	new_mat_point->v = 0;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	// ����� Bs - �������� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "Bs";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clLtGray;
	new_mat_point->visual_dy = vagon0_visual_dy;
	new_mat_point->visual_rx = vagon0_visual_rx;
	new_mat_point->visual_ry = vagon0_visual_ry;
	new_mat_point->caption_up = false;
	new_mat_point->caption_type = MMatObject::t_ct_none;
	new_mat_point->x0 = 120;
	new_mat_point->x = new_mat_point->x0;	
	new_mat_point->v = 0;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	// ����� B2 - ����� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "B2";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clLtGray;
	new_mat_point->visual_dy = vagon0_visual_dy;
	new_mat_point->visual_rx = vagon0_visual_rx;
	new_mat_point->visual_ry = vagon0_visual_ry;
	new_mat_point->caption_up = true;
	new_mat_point->caption_type = MMatObject::t_ct_none;	
	new_mat_point->x0 = 240;
	new_mat_point->x = new_mat_point->x0;	
	new_mat_point->v = 0;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	//-----
	// ������� (�������������� ������)
	//-----
	MSensor2* new_sensor;
	int sensor_visual_dy = abs_iso->visual_dy - 50;
	int sensor_visual_rx = 7;
	int sensor_visual_ry = 10;

	// ������ D1 - ������������ ��������������� ������� ����� �� ������/����� ������
	// �������������� � ������ � ����� As (� �������� ������)
	new_sensor = new MSensor2;
	new_sensor->name = "D1";
	new_sensor->iso = abs_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = true;
	new_sensor->type = "F";	// ����������� ������ �� ������
	new_sensor->sost = MSensor2::t_sost_undefined;	// � ������ ��������� = UNDEFINED
	new_sensor->x0 = 120;
	new_sensor->x = new_sensor->x0;
	new_sensor->v = v_vagon;
	new_sensor->t = 0;
	abs_iso->obj_list.Add(new_sensor);


	//-----
	// ������� ���������� ���
	move_iso = new MIso;

	move_iso->name = "K'";
	move_iso->color = clRed;
	move_iso->paint = frmMain->m_PaintBox_MoveSTO;
	move_iso->visual_dx = abs_iso->visual_dx;
	move_iso->visual_dy = move_iso->paint->Height - 30;;
	move_iso->ver_line_len = move_iso->paint->Height;
	move_iso->hor_line_len = 450;

	move_iso->coord_begin_point.x0 = 0;
	move_iso->coord_begin_point.v = v_vagon; // �������� ���������� ���
	move_iso->coord_begin_point.t = 0;

	//-----
	// �������� ������� �� abs_iso

	// ����� F1' - ����� ������� �� ������
	// �������������� � ������ � ����� A1 (� ������ ������)
	new_mat_point = new MMatPoint;
	new_mat_point->name = "F1'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clYellow;
	new_mat_point->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_mat_point->visual_dy = foton_visual_dy;
	new_mat_point->visual_rx = foton_visual_rx;
	new_mat_point->visual_ry = foton_visual_ry;
	new_mat_point->caption_up = true;
//	new_mat_point->x0 = 0;
//	new_mat_point->v = g_light_c;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� F2' - ����� ������� �� ������
	// �������������� � ������ � ����� A2 (� ����� ������)
	new_mat_point = new MMatPoint;
	new_mat_point->name = "F2'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clYellow;
	new_mat_point->caption_color = (TColor)RGB(235,235,115);//clYellow;	
	new_mat_point->visual_dy = foton_visual_dy;
	new_mat_point->visual_rx = foton_visual_rx;
	new_mat_point->visual_ry = foton_visual_ry;
	new_mat_point->caption_up = false;
//	new_mat_point->x0 = 240;
//	new_mat_point->v = -g_light_c;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� A1' - ������ ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A1'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = true;
//	new_mat_point->x0 = 0;
//	new_mat_point->v = 0;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� As' - �������� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "As'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = false;
//	new_mat_point->x0 = 120;
//	new_mat_point->v = 0;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� A2' - ����� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A2'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;	
	new_mat_point->caption_up = true;
//	new_mat_point->x0 = 240;
//	new_mat_point->v = 0;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� B1' - ������ ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "B1'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clLtGray;
	new_mat_point->visual_dy = vagon0_visual_dy;
	new_mat_point->visual_rx = vagon0_visual_rx;
	new_mat_point->visual_ry = vagon0_visual_ry;
	new_mat_point->caption_up = true;
	new_mat_point->caption_type = MMatObject::t_ct_none;
//	new_mat_point->x0 = 0;
//	new_mat_point->x = new_mat_point->x0;
//	new_mat_point->v = 0;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� Bs' - �������� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "Bs'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clLtGray;
	new_mat_point->visual_dy = vagon0_visual_dy;
	new_mat_point->visual_rx = vagon0_visual_rx;
	new_mat_point->visual_ry = vagon0_visual_ry;
	new_mat_point->caption_up = false;
	new_mat_point->caption_type = MMatObject::t_ct_none;
//	new_mat_point->x0 = 120;
//	new_mat_point->x = new_mat_point->x0;
//	new_mat_point->v = 0;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� B2' - ����� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "B2'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clLtGray;
	new_mat_point->visual_dy = vagon0_visual_dy;
	new_mat_point->visual_rx = vagon0_visual_rx;
	new_mat_point->visual_ry = vagon0_visual_ry;
	new_mat_point->caption_up = true;
	new_mat_point->caption_type = MMatObject::t_ct_none;	
//	new_mat_point->x0 = 240;
//	new_mat_point->x = new_mat_point->x0;
//	new_mat_point->v = 0;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ������ D1' - ������������ ��������������� ������� ����� �� ������/����� ������
	// �������������� � ������ � ����� As (� �������� ������)
	new_sensor = new MSensor2;
	new_sensor->name = "D1'";
	new_sensor->iso = move_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = true;
	new_sensor->type = "F";	// ����������� ������ �� ������
	new_sensor->sost = MSensor2::t_sost_undefined;	// � ������ ��������� = UNDEFINED
//	new_sensor->x0 = 0;
//	new_sensor->v = v_vagon;
//	new_sensor->t = 0;
	move_iso->obj_list.Add(new_sensor);



	//-----
	// ������� ������� ����������
	abs_iso->MoveObjects(MIso::t_move_abs, time_start);

	
	// ��������� ���������� abs_iso � ����������� ���
	// move_iso
	abs_iso->TransformObjects(MIso::t_transform_sto, move_iso);
}

//---------------------------------------------------------------------------
// ����� ��������� ��������
//---------------------------------------------------------------------------
void __fastcall MExperimentLastArgumentOfKing::Reset
(
)
{
	// ����� �������� abs_iso
	// + �������� �������� �������� ������ ��� K' (�� ����� �������� � ����������)
	for (int i=abs_iso->obj_list.CicleCount(); i>0; i--)
	{
		if (abs_iso->obj_list.GetCurObj().name == "D1")
		{
			MSensor2& d1 = (MSensor2&)abs_iso->obj_list.GetCurObj();

			//
			d1.sost = MSensor2::t_sost_undefined;

			// �������� ������� = �������� ������ = �������� ���������� ��� K'
			d1.v = this->move_iso->coord_begin_point.v;
		}

		// ������ A - A1,As,A2 = �����
		if (abs_iso->obj_list.GetCurObj().name.SubString(1,1) == "A")
		{
			MMatPoint& mp = (MMatPoint&)abs_iso->obj_list.GetCurObj();

			// �������� ������ = �������� ���������� ��� K'
			mp.v = this->move_iso->coord_begin_point.v;
		}

		// �������� ������� �� �������� = C
		// �� �� ������...

		//
		abs_iso->obj_list.MoveNext();
	}

	// ����� �������� move_iso
	for (int i=move_iso->obj_list.CicleCount(); i>0; i--)
	{
		if (move_iso->obj_list.GetCurObj().name == "D1'")
		{
			MSensor2& d1 = (MSensor2&)move_iso->obj_list.GetCurObj();

			d1.sost = MSensor2::t_sost_undefined;
		}

		// �������� � ���������� ���� ��������
		// ������������� �� ������� �������������� ��� K � ��� K' � Run()
		// ��� �� �������...

		move_iso->obj_list.MoveNext();
	}

}

//---------------------------------------------------------------------------
// ������������� MExperimentLastArgumentOfKing2
//---------------------------------------------------------------------------
void __fastcall MExperimentLastArgumentOfKing2::Init
(
)
{
	frmMain->m_Memo_Dbg->Clear();

	// �������� ������
	double v_vagon;

	//-----
	// �������������� ����������� ���� �� ����������
	// ����, ��� ��������� ������������ �� ��������� ���������������� �� ����
	if (frmMain->m_ComboBox_Experiment->Tag != 1)
	{
		time_start = 0;
		time_end = 34;//100;
		time_step = 0.2;//10;
		time_cur = time_start;
		this->m_frmHelp = frmHelpExp3;
		v_vagon = 20;

		// �������� ��������� �� ����� ��������
		frmSettings->m_VLE_1->Values["�������� �����"] = FloatToStr(g_light_c);
		frmSettings->m_VLE_1->Values["�������� ������ (��� K') ������������ ��� K"] = FloatToStr(v_vagon);
		frmSettings->m_VLE_1->Values["��� ������� (dt) � ��� �"] = FloatToStr(frmMain->m_CurExperiment->time_step);
		frmSettings->m_VLE_1->Values["����� (t) ����� ������������ � ��� K"] = FloatToStr(frmMain->m_CurExperiment->time_end);

	}
	// �������������� ��������� �� ���� ����������
	else
	{
    	// �������� ��������� �� ����� ��������
		time_start = 0;
		frmMain->m_CurExperiment->time_step = StrToFloat(frmSettings->m_VLE_1->Values["��� ������� (dt) � ��� �"]);
		frmMain->m_CurExperiment->time_end = StrToFloat(frmSettings->m_VLE_1->Values["����� (t) ����� ������������ � ��� K"]);

		time_cur = time_start;
		this->m_frmHelp = frmHelpExp2;
		v_vagon = StrToFloat(frmSettings->m_VLE_1->Values["�������� ������ (��� K') ������������ ��� K"]);


		// ���������� ����
		frmMain->m_ComboBox_Experiment->Tag = 0;
	}

	//-----
	// ������� ���������� ���
	abs_iso = new MIso;

	abs_iso->name = "K";
	abs_iso->color = clGreen;
	abs_iso->paint = frmMain->m_PaintBox_AbsSTO;
	abs_iso->visual_dx = 20;
	abs_iso->visual_dy = abs_iso->paint->Height - 30;
	abs_iso->ver_line_len = abs_iso->paint->Height;
	abs_iso->hor_line_len = abs_iso->paint->Width - 40;

	abs_iso->coord_begin_point.x0 = 0;
	abs_iso->coord_begin_point.v = 0;
	abs_iso->coord_begin_point.t = 0;

	//-----
	// ��������� ������� ������������
	MMatPoint* new_mat_point;

	//-----
	// ������ (������)
	//-----
	int foton_visual_dy = abs_iso->visual_dy - 50;
	int foton_visual_rx = 5;
	int foton_visual_ry = 5;

	// ����� F1 - ����� ������� �� ������
	// �������������� � ������ � ����� A1 (� ������ ������)
	new_mat_point = new MMatPoint;
	new_mat_point->name = "F1";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clYellow;
	new_mat_point->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_mat_point->visual_dy = foton_visual_dy;
	new_mat_point->visual_rx = foton_visual_rx;
	new_mat_point->visual_ry = foton_visual_ry;
	new_mat_point->caption_up = true;
	new_mat_point->x0 = 120;
	new_mat_point->x = new_mat_point->x0;
	new_mat_point->v = g_light_c;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	// ����� F2 - ����� ������� �� ������
	// �������������� � ������ � ����� A2 (� ����� ������)
	new_mat_point = new MMatPoint;
	new_mat_point->name = "F2";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clYellow;
	new_mat_point->caption_color = (TColor)RGB(235,235,115);//clYellow;	
	new_mat_point->visual_dy = foton_visual_dy;
	new_mat_point->visual_rx = foton_visual_rx;
	new_mat_point->visual_ry = foton_visual_ry;
	new_mat_point->caption_up = false;
	new_mat_point->x0 = 120;
	new_mat_point->x = new_mat_point->x0;	
	new_mat_point->v = -g_light_c;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	//-----
	// �����
	//-----
	// �������� ������
	//double v_vagon = 20;
	// ������ �� ������� ������������ �����
	int vagon_visual_dy = abs_iso->visual_dy - 50;
	int vagon_visual_rx = 10;
	int vagon_visual_ry = 30;

	// ����� A1 - ������ ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A1";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = true;
	new_mat_point->x0 = 0;
	new_mat_point->x = new_mat_point->x0;	
	new_mat_point->v = v_vagon;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	// ����� As - �������� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "As";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = false;
	new_mat_point->x0 = 120;
	new_mat_point->x = new_mat_point->x0;	
	new_mat_point->v = v_vagon;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	// ����� A2 - ����� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A2";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = true;
	new_mat_point->x0 = 240;
	new_mat_point->x = new_mat_point->x0;	
	new_mat_point->v = v_vagon;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	//-----
	// ������� (�������������� ������)
	//-----
	MSensor2* new_sensor;
	int sensor_visual_dy = abs_iso->visual_dy - 50;
	int sensor_visual_rx = 7;
	int sensor_visual_ry = 10;

	// ������ D1 - ������������ ��������������� ������� ������ �� �������� ������
	// �������������� � ������ � ����� A1 (� ������ ������)
	new_sensor = new MSensor2;
	new_sensor->name = "D1";
	new_sensor->iso = abs_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = true;
	new_sensor->type = "F";	// ����������� ������ �� ������
	new_sensor->sost = MSensor2::t_sost_undefined;	// � ������ ��������� = UNDEFINED
	new_sensor->x0 = 0;
	new_sensor->x = new_sensor->x0;
	new_sensor->v = v_vagon;
	new_sensor->t = 0;
	abs_iso->obj_list.Add(new_sensor);

	// ������ D2 - ������������ ��������������� ������� ������ �� �������� ������
	// �������������� � ������ � ����� A2 (� ����� ������)
	new_sensor = new MSensor2;
	new_sensor->name = "D2";
	new_sensor->iso = abs_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = false;
	new_sensor->type = "F";	// ����������� ������ �� ������
	new_sensor->sost = MSensor2::t_sost_undefined;	// � ������ ��������� = UNDEFINED
	new_sensor->x0 = 240;
	new_sensor->x = new_sensor->x0;
	new_sensor->v = v_vagon;
	new_sensor->t = 0;
	abs_iso->obj_list.Add(new_sensor);


	//-----
	// ������� ���������� ���
	move_iso = new MIso;

	move_iso->name = "K'";
	move_iso->color = clRed;
	move_iso->paint = frmMain->m_PaintBox_MoveSTO;
	move_iso->visual_dx = abs_iso->visual_dx;
	move_iso->visual_dy = move_iso->paint->Height - 30;;
	move_iso->ver_line_len = move_iso->paint->Height;
	move_iso->hor_line_len = 450;

	move_iso->coord_begin_point.x0 = 0;
	move_iso->coord_begin_point.v = v_vagon; // �������� ���������� ���
	move_iso->coord_begin_point.t = 0;

	//-----
	// �������� ������� �� abs_iso

	// ����� F1' - ����� ������� �� ������
	// �������������� � ������ � ����� A1 (� ������ ������)
	new_mat_point = new MMatPoint;
	new_mat_point->name = "F1'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clYellow;
	new_mat_point->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_mat_point->visual_dy = foton_visual_dy;
	new_mat_point->visual_rx = foton_visual_rx;
	new_mat_point->visual_ry = foton_visual_ry;
	new_mat_point->caption_up = true;
//	new_mat_point->x0 = 0;
//	new_mat_point->v = g_light_c;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� F2' - ����� ������� �� ������
	// �������������� � ������ � ����� A2 (� ����� ������)
	new_mat_point = new MMatPoint;
	new_mat_point->name = "F2'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clYellow;
	new_mat_point->caption_color = (TColor)RGB(235,235,115);//clYellow;	
	new_mat_point->visual_dy = foton_visual_dy;
	new_mat_point->visual_rx = foton_visual_rx;
	new_mat_point->visual_ry = foton_visual_ry;
	new_mat_point->caption_up = false;
//	new_mat_point->x0 = 240;
//	new_mat_point->v = -g_light_c;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� A1' - ������ ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A1'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = true;
//	new_mat_point->x0 = 0;
//	new_mat_point->v = 0;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� As' - �������� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "As'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = false;
//	new_mat_point->x0 = 120;
//	new_mat_point->v = 0;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� A2' - ����� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A2'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;	
	new_mat_point->caption_up = true;
//	new_mat_point->x0 = 240;
//	new_mat_point->v = 0;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ������ D1' - ������������ ��������������� ������� ������ �� �������� ������
	// �������������� � ������ � ����� A1 (� ������ ������)
	new_sensor = new MSensor2;
	new_sensor->name = "D1'";
	new_sensor->iso = move_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = true;
	new_sensor->type = "F";	// ����������� ������ �� ������
	new_sensor->sost = MSensor2::t_sost_undefined;	// � ������ ��������� = UNDEFINED
//	new_sensor->x0 = 0;
//	new_sensor->x = new_sensor->x0;
//	new_sensor->v = v_vagon;
//	new_sensor->t = 0;
	move_iso->obj_list.Add(new_sensor);

	// ������ D2' - ������������ ��������������� ������� ������ �� �������� ������
	// �������������� � ������ � ����� A2 (� ����� ������)
	new_sensor = new MSensor2;
	new_sensor->name = "D2'";
	new_sensor->iso = move_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = false;
	new_sensor->type = "F";	// ����������� ������ �� ������
	new_sensor->sost = MSensor2::t_sost_undefined;	// � ������ ��������� = UNDEFINED
//	new_sensor->x0 = 240;
//	new_sensor->x = new_sensor->x0;
//	new_sensor->v = v_vagon;
//	new_sensor->t = 0;
	move_iso->obj_list.Add(new_sensor);



	//-----
	// ������� ������� ����������
	abs_iso->MoveObjects(MIso::t_move_abs, time_start);

	
	// ��������� ���������� abs_iso � ����������� ���
	// move_iso
	abs_iso->TransformObjects(MIso::t_transform_sto, move_iso);
}

//---------------------------------------------------------------------------
// ������������� MExperimentEnshtainTimers
//---------------------------------------------------------------------------
void __fastcall MExperimentEnshtainTimers::Init
(
)
{
	frmMain->m_Memo_Dbg->Clear();

	// �������� ������
	double v_vagon;

	//-----
	// �������������� ����������� ���� �� ����������
	// ����, ��� ��������� ������������ �� ��������� ���������������� �� ����
	if (frmMain->m_ComboBox_Experiment->Tag != 1)
	{
		time_start = 0;
		time_end = 34;//100;
		time_step = 0.2;//10;
		time_cur = time_start;
		this->m_frmHelp = frmHelpExp4;
		v_vagon = 10;
		  
		// �������� ��������� �� ����� ��������
		frmSettings->m_VLE_1->Values["�������� �����"] = FloatToStr(g_light_c);
		frmSettings->m_VLE_1->Values["�������� ������ (��� K') ������������ ��� K"] = FloatToStr(v_vagon);
		frmSettings->m_VLE_1->Values["��� ������� (dt) � ��� �"] = FloatToStr(frmMain->m_CurExperiment->time_step);
		frmSettings->m_VLE_1->Values["����� (t) ����� ������������ � ��� K"] = FloatToStr(frmMain->m_CurExperiment->time_end);
	}
	// �������������� ��������� �� ���� ����������
	else
	{
		time_start = 0;
		frmMain->m_CurExperiment->time_step = StrToFloat(frmSettings->m_VLE_1->Values["��� ������� (dt) � ��� �"]);
		frmMain->m_CurExperiment->time_end = StrToFloat(frmSettings->m_VLE_1->Values["����� (t) ����� ������������ � ��� K"]);

		time_cur = time_start;
		this->m_frmHelp = frmHelpExp2;
		v_vagon = StrToFloat(frmSettings->m_VLE_1->Values["�������� ������ (��� K') ������������ ��� K"]);


		// ���������� ����
		frmMain->m_ComboBox_Experiment->Tag = 0;
	}


	//-----
	// ������� ���������� ���
	abs_iso = new MIso;

	abs_iso->name = "K";
	abs_iso->color = clGreen;
	abs_iso->paint = frmMain->m_PaintBox_AbsSTO;
	abs_iso->visual_dx = 20;
	abs_iso->visual_dy = abs_iso->paint->Height - 30;
	abs_iso->ver_line_len = abs_iso->paint->Height;
	abs_iso->hor_line_len = abs_iso->paint->Width - 40;

	abs_iso->coord_begin_point.x0 = 0;
	abs_iso->coord_begin_point.v = 0;
	abs_iso->coord_begin_point.t = 0;

	//-----
	// ��������� ������� ������������
	MMatPoint* new_mat_point;

	//-----
	// ������� (�������������� ������)
	//-----
	int sensor_visual_dy = abs_iso->visual_dy - 50;
	int sensor_visual_rx = 7;
	int sensor_visual_ry = 10;
	MSensorMirror* new_sensor;

	//-----
	// ������ (������)
	//-----
	int foton_visual_dy = abs_iso->visual_dy - 50;
	int foton_visual_rx = 5;
	int foton_visual_ry = 5;

	// ����� F1 - ����� ������� �� ������
	// �������������� � ������ � ����� A1 (� ������ ������)
	new_mat_point = new MMatPoint;
	new_mat_point->name = "F1";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clYellow;
	new_mat_point->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_mat_point->visual_dy = foton_visual_dy;
	new_mat_point->visual_rx = foton_visual_rx;
	new_mat_point->visual_ry = foton_visual_ry;
	new_mat_point->caption_up = true;
	new_mat_point->x0 = 0;
	new_mat_point->x = new_mat_point->x0;
	new_mat_point->v = g_light_c;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	//-----
	// ����������� ����������� ����
	//-----

	double timer_pos = 750;

	// ����� F2 - ����� ��� ����������� ����������� �����
	// � ���� �� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "F2";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clYellow;
	new_mat_point->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_mat_point->visual_dy = foton_visual_dy;
	new_mat_point->visual_rx = foton_visual_rx;
	new_mat_point->visual_ry = foton_visual_ry;
	new_mat_point->caption_up = false;
	new_mat_point->x0 = timer_pos;
	new_mat_point->x = new_mat_point->x0;
	new_mat_point->v = g_light_c;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	// ������ D3 - �������-������� ����������� �����
	// ����������� �����
	new_sensor = new MSensorMirror;
	new_sensor->name = "D3";
	new_sensor->iso = abs_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = true;
	new_sensor->type = "F";	// ����������� ������ �� ������
	new_sensor->sost = MSensorMirror::t_sost_undefined;	// � ������ ��������� = UNDEFINED
	new_sensor->x0 = timer_pos;
	new_sensor->x = new_sensor->x0;
	new_sensor->v = 0;
	new_sensor->t = 0;
	abs_iso->obj_list.Add(new_sensor);

	// ������ D4 - �������-���������� ����������� �����
	// ����������� ������
	new_sensor = new MSensorMirror;
	new_sensor->name = "D4";
	new_sensor->iso = abs_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = false;
	new_sensor->type = "F";	// ����������� ������ �� ������
	new_sensor->sost = MSensorMirror::t_sost_no_count;	// � ������ ��������� = t_sost_no_count
	new_sensor->x0 = timer_pos + 120;
	new_sensor->x = new_sensor->x0;
	new_sensor->v = 0;
	new_sensor->t = 0;
	abs_iso->obj_list.Add(new_sensor);



	//-----
	// �����
	//-----
	// �������� ������
	//double v_vagon = 20;
	// ������ �� ������� ������������ �����
	int vagon_visual_dy = abs_iso->visual_dy - 50;
	int vagon_visual_rx = 10;
	int vagon_visual_ry = 30;

	// ����� A1 - ������ ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A1";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = true;
	new_mat_point->x0 = 0;
	new_mat_point->x = new_mat_point->x0;
	new_mat_point->v = v_vagon;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	// ����� As - �������� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "As";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = false;
	new_mat_point->x0 = 120;
	new_mat_point->x = new_mat_point->x0;
	new_mat_point->v = v_vagon;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	// ����� A2 - ����� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A2";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = true;
	new_mat_point->x0 = 240;
	new_mat_point->x = new_mat_point->x0;
	new_mat_point->v = v_vagon;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	//-----
	// ������� (�������������� ������)
	//-----

	// ������ D1 - �������-������� ����������� �����
	// �������������� � ������ � ����� A1 (� ������ ������)	
	new_sensor = new MSensorMirror;
	new_sensor->name = "D1";
	new_sensor->iso = abs_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = true;
	new_sensor->type = "F";	// ����������� ������ �� ������
	new_sensor->sost = MSensorMirror::t_sost_undefined;	// � ������ ��������� = UNDEFINED
	new_sensor->x0 = 0;
	new_sensor->x = new_sensor->x0;
	new_sensor->v = v_vagon;
	new_sensor->t = 0;
	abs_iso->obj_list.Add(new_sensor);

	// ������ D2 - �������-���������� ����������� �����
	// �������������� � ������ � ����� As (� �������� ������)
	new_sensor = new MSensorMirror;
	new_sensor->name = "D2";
	new_sensor->iso = abs_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = false;
	new_sensor->type = "F";	// ����������� ������ �� ������
	new_sensor->sost = MSensorMirror::t_sost_no_count;	// � ������ ��������� = t_sost_no_count
	new_sensor->x0 = 120;
	new_sensor->x = new_sensor->x0;
	new_sensor->v = v_vagon;
	new_sensor->t = 0;
	abs_iso->obj_list.Add(new_sensor);

	//-----
	// ������� ���������� ���
	move_iso = new MIso;

	move_iso->name = "K'";
	move_iso->color = clRed;
	move_iso->paint = frmMain->m_PaintBox_MoveSTO;
	move_iso->visual_dx = abs_iso->visual_dx;
	move_iso->visual_dy = move_iso->paint->Height - 30;;
	move_iso->ver_line_len = move_iso->paint->Height;
	move_iso->hor_line_len = 450;

	move_iso->coord_begin_point.x0 = 0;
	move_iso->coord_begin_point.v = v_vagon; // �������� ���������� ���
	move_iso->coord_begin_point.t = 0;

	//-----
	// �������� ������� �� abs_iso

	// ����� F1' - ����� ������� �� ������
	// �������������� � ������ � ����� A1 (� ������ ������)
	new_mat_point = new MMatPoint;
	new_mat_point->name = "F1'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clYellow;
	new_mat_point->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_mat_point->visual_dy = foton_visual_dy;
	new_mat_point->visual_rx = foton_visual_rx;
	new_mat_point->visual_ry = foton_visual_ry;
	new_mat_point->caption_up = true;
//	new_mat_point->x0 = 0;
//	new_mat_point->v = g_light_c;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� F2' - ����� ��� ����������� ����������� �����
	// � ���� �� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "F2'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clYellow;
	new_mat_point->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_mat_point->visual_dy = foton_visual_dy;
	new_mat_point->visual_rx = foton_visual_rx;
	new_mat_point->visual_ry = foton_visual_ry;
	new_mat_point->caption_up = false;
//	new_mat_point->x0 = timer_pos + 120;
//	new_mat_point->x = new_mat_point->x0;
//	new_mat_point->v = -g_light_c;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ������ D3' - �������-������� ����������� �����
	// ����������� �����
	new_sensor = new MSensorMirror;
	new_sensor->name = "D3'";
	new_sensor->iso = move_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = true;
	new_sensor->type = "F";	// ����������� ������ �� ������
	new_sensor->sost = MSensorMirror::t_sost_undefined;	// � ������ ��������� = UNDEFINED
//	new_sensor->x0 = timer_pos;
//	new_sensor->x = new_sensor->x0;
//	new_sensor->v = 0;
//	new_sensor->t = 0;
	move_iso->obj_list.Add(new_sensor);

	// ������ D4' - �������-���������� ����������� �����
	// ����������� ������
	new_sensor = new MSensorMirror;
	new_sensor->name = "D4'";
	new_sensor->iso = move_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = false;
	new_sensor->type = "F";	// ����������� ������ �� ������
	new_sensor->sost = MSensorMirror::t_sost_no_count;	// � ������ ��������� = t_sost_no_count
//	new_sensor->x0 = timer_pos + 120;
//	new_sensor->x = new_sensor->x0;
//	new_sensor->v = 0;
//	new_sensor->t = 0;
	move_iso->obj_list.Add(new_sensor);

	// ����� A1' - ������ ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A1'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = true;
//	new_mat_point->x0 = 0;
//	new_mat_point->v = 0;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� As' - �������� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "As'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;
	new_mat_point->caption_up = false;
//	new_mat_point->x0 = 120;
//	new_mat_point->v = 0;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ����� A2' - ����� ������
	new_mat_point = new MMatPoint;
	new_mat_point->name = "A2'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clBlue;
	new_mat_point->visual_dy = vagon_visual_dy;
	new_mat_point->visual_rx = vagon_visual_rx;
	new_mat_point->visual_ry = vagon_visual_ry;	
	new_mat_point->caption_up = true;
//	new_mat_point->x0 = 240;
//	new_mat_point->v = 0;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// ������ D1' - �������-������� ����������� �����
	// �������������� � ������ � ����� A1 (� ������ ������)
	new_sensor = new MSensorMirror;
	new_sensor->name = "D1'";
	new_sensor->iso = move_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = true;
	new_sensor->type = "F";	// ����������� ������ �� ������
	new_sensor->sost = MSensorMirror::t_sost_undefined;	// � ������ ��������� = UNDEFINED
	new_sensor->x0 = 0;
	new_sensor->x = new_sensor->x0;
	new_sensor->v = v_vagon;
	new_sensor->t = 0;
	move_iso->obj_list.Add(new_sensor);

	// ������ D2' - �������-���������� ����������� �����
	// �������������� � ������ � ����� As (� �������� ������)
	new_sensor = new MSensorMirror;
	new_sensor->name = "D2'";
	new_sensor->iso = move_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = false;
	new_sensor->type = "F";	// ����������� ������ �� ������
	new_sensor->sost = MSensorMirror::t_sost_no_count;	// � ������ ��������� = t_sost_no_count
	new_sensor->x0 = 120;
	new_sensor->x = new_sensor->x0;
	new_sensor->v = v_vagon;
	new_sensor->t = 0;
	move_iso->obj_list.Add(new_sensor);

	//-----
	// ������� ������� ����������
	abs_iso->MoveObjects(MIso::t_move_abs, time_start);

	
	// ��������� ���������� abs_iso � ����������� ���
	// move_iso
	abs_iso->TransformObjects(MIso::t_transform_sto, move_iso);
}

//---------------------------------------------------------------------------
// ����� ��������� ��������
//---------------------------------------------------------------------------
void __fastcall MExperimentEnshtainTimers::Reset
(
)
{
	// ����� �������� abs_iso
	// + �������� �������� �������� ������ ��� K' (�� ����� �������� � ����������)
	for (int i=abs_iso->obj_list.CicleCount(); i>0; i--)
	{
		// ���������� x � x0 ��� ���x
		MMatPoint& p = (MMatPoint&)abs_iso->obj_list.GetCurObj();
		p.x = p.x0;

		if (abs_iso->obj_list.GetCurObj().name == "D1")
		{
			MSensorMirror& d1 = (MSensorMirror&)abs_iso->obj_list.GetCurObj();

			//
			d1.sost = MSensorMirror::t_sost_undefined;

			// �������� ������� = �������� ������ = �������� ���������� ��� K'
			d1.v = this->move_iso->coord_begin_point.v;
		}
		if (abs_iso->obj_list.GetCurObj().name == "D2")
		{
			MSensorMirror& d1 = (MSensorMirror&)abs_iso->obj_list.GetCurObj();

			//
			d1.sost = MSensorMirror::t_sost_no_count;

			// �������� ������� = �������� ������ = �������� ���������� ��� K'
			d1.v = this->move_iso->coord_begin_point.v;
		}
		if (abs_iso->obj_list.GetCurObj().name == "D3")
		{
			MSensorMirror& d1 = (MSensorMirror&)abs_iso->obj_list.GetCurObj();

			//
			d1.sost = MSensorMirror::t_sost_undefined;

			// �������� ������� = �������� ������ = �������� ���������� ��� K'
			d1.v = g_light_c;
		}
		if (abs_iso->obj_list.GetCurObj().name == "D4")
		{
			MSensorMirror& d1 = (MSensorMirror&)abs_iso->obj_list.GetCurObj();

			//
			d1.sost = MSensorMirror::t_sost_no_count;

			// �������� ������� = �������� ������ = �������� ���������� ��� K'
			d1.v = g_light_c;
		}


		// ������ A - A1,As,A2 = �����
		if (abs_iso->obj_list.GetCurObj().name.SubString(1,1) == "A")
		{
			MMatPoint& mp = (MMatPoint&)abs_iso->obj_list.GetCurObj();

			// �������� ������ = �������� ���������� ��� K'
			mp.v = this->move_iso->coord_begin_point.v;
		}

		// �������� ������� �� �������� = C
		// �� �� ������...

		//
		abs_iso->obj_list.MoveNext();
	}

	// ����� �������� move_iso
	for (int i=move_iso->obj_list.CicleCount(); i>0; i--)
	{
		if (move_iso->obj_list.GetCurObj().name == "D1")
		{
			MSensorMirror& d1 = (MSensorMirror&)move_iso->obj_list.GetCurObj();

			//
			d1.sost = MSensorMirror::t_sost_undefined;

			// �������� ������� = �������� ������ = �������� ���������� ��� K'
			d1.v = this->move_iso->coord_begin_point.v;
		}
		if (move_iso->obj_list.GetCurObj().name == "D2")
		{
			MSensorMirror& d1 = (MSensorMirror&)move_iso->obj_list.GetCurObj();

			//
			d1.sost = MSensorMirror::t_sost_no_count;

			// �������� ������� = �������� ������ = �������� ���������� ��� K'
			d1.v = this->move_iso->coord_begin_point.v;
		}
		if (move_iso->obj_list.GetCurObj().name == "D3")
		{
			MSensorMirror& d1 = (MSensorMirror&)move_iso->obj_list.GetCurObj();

			//
			d1.sost = MSensorMirror::t_sost_undefined;

			// �������� ������� = �������� ������ = �������� ���������� ��� K'
			d1.v = g_light_c;
		}
		if (move_iso->obj_list.GetCurObj().name == "D4")
		{
			MSensorMirror& d1 = (MSensorMirror&)move_iso->obj_list.GetCurObj();

			//
			d1.sost = MSensorMirror::t_sost_no_count;

			// �������� ������� = �������� ������ = �������� ���������� ��� K'
			d1.v = g_light_c;
		}

		// �������� � ���������� ���� ��������
		// ������������� �� ������� �������������� ��� K � ��� K' � Run()
		// ��� �� �������...

		move_iso->obj_list.MoveNext();
	}

}

//---------------------------------------------------------------------------
// ������������� MExperimentKingTimers
//---------------------------------------------------------------------------
void __fastcall MExperimentKingTimers::Init
(
)
{
	frmMain->m_Memo_Dbg->Clear();

	// �������� ������
	double v_vagon;

	//-----
	// �������������� ����������� ���� �� ����������
	// ����, ��� ��������� ������������ �� ��������� ���������������� �� ����
	if (frmMain->m_ComboBox_Experiment->Tag != 1)
	{
		time_start = 0;
		time_end = 34;//100;
		time_step = 0.2;//10;
		time_cur = time_start;
		this->m_frmHelp = frmHelpExp5;
		v_vagon = 10;
		  
		// �������� ��������� �� ����� ��������
		frmSettings->m_VLE_1->Values["�������� �����"] = FloatToStr(g_light_c);
		frmSettings->m_VLE_1->Values["�������� ������ (��� K') ������������ ��� K"] = FloatToStr(v_vagon);
		frmSettings->m_VLE_1->Values["��� ������� (dt) � ��� �"] = FloatToStr(frmMain->m_CurExperiment->time_step);
		frmSettings->m_VLE_1->Values["����� (t) ����� ������������ � ��� K"] = FloatToStr(frmMain->m_CurExperiment->time_end);
	}
	// �������������� ��������� �� ���� ����������
	else
	{
		time_start = 0;
		frmMain->m_CurExperiment->time_step = StrToFloat(frmSettings->m_VLE_1->Values["��� ������� (dt) � ��� �"]);
		frmMain->m_CurExperiment->time_end = StrToFloat(frmSettings->m_VLE_1->Values["����� (t) ����� ������������ � ��� K"]);

		time_cur = time_start;
		this->m_frmHelp = frmHelpExp2;
		v_vagon = StrToFloat(frmSettings->m_VLE_1->Values["�������� ������ (��� K') ������������ ��� K"]);


		// ���������� ����
		frmMain->m_ComboBox_Experiment->Tag = 0;
	}


	//-----
	// ������� ���������� ���
	abs_iso = new MIso;

	abs_iso->name = "K";
	abs_iso->color = clGreen;
	abs_iso->paint = frmMain->m_PaintBox_AbsSTO;
	abs_iso->visual_dx = 20;
	abs_iso->visual_dy = abs_iso->paint->Height - 30;
	abs_iso->ver_line_len = abs_iso->paint->Height;
	abs_iso->hor_line_len = abs_iso->paint->Width - 40;

	abs_iso->coord_begin_point.x0 = 0;
	abs_iso->coord_begin_point.v = 0;
	abs_iso->coord_begin_point.t = 0;

	//-----
	// ��������� ������� ������������
	MMatPoint* new_mat_point;

	//-----
	// ������ (������)
	//-----
	int foton_visual_dy = abs_iso->visual_dy - 50;
	int foton_visual_rx = 5;
	int foton_visual_ry = 5;

	// ����� F1 - ����� ������� �� ������
	// �������������� � ������ � ����� A1 (� ������ ������)
	new_mat_point = new MMatPoint;
	new_mat_point->name = "F1";
	new_mat_point->iso = abs_iso;
	new_mat_point->color = clYellow;
	new_mat_point->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_mat_point->visual_dy = foton_visual_dy;
	new_mat_point->visual_rx = foton_visual_rx;
	new_mat_point->visual_ry = foton_visual_ry;
	new_mat_point->caption_up = true;
	new_mat_point->x0 = 0;
	new_mat_point->x = new_mat_point->x0;
	new_mat_point->v = g_light_c;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);	

	//-----
	// ��������� ����� ����� ������� (�������)
	//-----
	// �������� ������
	//double v_vagon = 20;
	// ������ �� ������� ������������ �����
	int sec1_visual_dy = abs_iso->visual_dy - 50;
	int sec1_visual_rx = 1;
	int sec1_visual_ry = 10;
	int sec1_start_x = 0;
	int sec1_dx = 30;
	int sec1_k = 21;
	int sec5_visual_rx = 1;
	int sec5_visual_ry = 30;
	int cur_x;

	// 60 ��
	cur_x = sec1_start_x;
	for (int i = 0; i < sec1_k; i++)
	{
		if ((i % 5) == 0)
		{
			new_mat_point = new MMatPoint;
			new_mat_point->name = "s" + IntToStr(i);
			new_mat_point->iso = abs_iso;
			new_mat_point->color = clBlue;
			new_mat_point->visual_dy = sec1_visual_dy;
			new_mat_point->visual_rx = sec5_visual_rx;
			new_mat_point->visual_ry = sec5_visual_ry;
			new_mat_point->caption_up = false;
			new_mat_point->caption_type = MMatObject::t_ct_name_time;
			if (i==0) new_mat_point->caption_color = clYellow;
			new_mat_point->x0 = cur_x;
			new_mat_point->x = new_mat_point->x0;
			new_mat_point->v = 0;
			new_mat_point->t = 0;
			abs_iso->obj_list.Add(new_mat_point);
		}
		else
		{
			new_mat_point = new MMatPoint;
			new_mat_point->name = "s" + IntToStr(i);
			new_mat_point->iso = abs_iso;
			new_mat_point->color = clBlue;
			new_mat_point->visual_dy = sec1_visual_dy;
			new_mat_point->visual_rx = sec1_visual_rx;
			new_mat_point->visual_ry = sec1_visual_ry;
			new_mat_point->caption_up = false;
			new_mat_point->caption_type = MMatObject::t_ct_name;
			new_mat_point->x0 = cur_x;
			new_mat_point->x = new_mat_point->x0;
			new_mat_point->v = 0;
			new_mat_point->t = 0;
			abs_iso->obj_list.Add(new_mat_point);
		}

		cur_x += sec1_dx;
	}

	//-----
	// ������-������� (�������������� ������)
	//-----
	MSensorStrelka* new_strelka;
	int strelka_visual_dy = abs_iso->visual_dy - 50;
	int strelka_visual_rx = 7;
	int strelka_visual_ry = 10;

	// ������ D1 - ������������ ����� (���-�� �������)
	new_strelka = new MSensorStrelka;
	new_strelka->name = "D1";
	new_strelka->iso = abs_iso;
	new_strelka->color = (TColor)RGB(70,70,70);
	new_strelka->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_strelka->visual_dy = strelka_visual_dy;
	new_strelka->visual_rx = strelka_visual_rx;
	new_strelka->visual_ry = strelka_visual_ry;
	new_strelka->caption_up = true;
	new_strelka->type = "s";	// ����������� ������ �� ����� �����
	new_strelka->sost = MSensorStrelka::t_sost_undefined;	// � ������ ��������� = UNDEFINED
	new_strelka->x0 = 0;
	new_strelka->x = new_strelka->x0;
	new_strelka->v = v_vagon;
	new_strelka->t = 0;
	abs_iso->obj_list.Add(new_strelka);

//	//-----
//	// ������ - �������
//	//-----
//	double mirror_pos = (sec1_k - 1) * sec1_dx;
//	double mirror_visual_dy = strelka_visual_dy;
//	double mirror_visual_rx = strelka_visual_rx;
//	double mirror_visual_ry = strelka_visual_ry;
//
//	// ������ D2 - �������-������� ����������� �����
//	// ����������� ������ � ����� �����
//	MSensorMirror* new_mirror = new MSensorMirror;
//	new_mirror->name = "D2";
//	new_mirror->iso = abs_iso;
//	new_mirror->color = (TColor)RGB(70,70,70);
//	new_mirror->caption_color = (TColor)RGB(235,235,115);//clYellow;
//	new_mirror->visual_dy = mirror_visual_dy;
//	new_mirror->visual_rx = mirror_visual_rx;
//	new_mirror->visual_ry = mirror_visual_ry;
//	new_mirror->caption_up = true;
//	new_mirror->type = "F";	// ����������� ������ �� ������
//	new_mirror->sost = MSensorMirror::t_sost_no_count;	// � ������ ��������� = �������
//	new_mirror->x0 = mirror_pos;
//	new_mirror->x = new_mirror->x0;
//	new_mirror->v = 0;
//	new_mirror->t = 0;
//	abs_iso->obj_list.Add(new_mirror);


	//-----
	// ������� ���������� ���
	move_iso = new MIso;

	move_iso->name = "K'";
	move_iso->color = clRed;
	move_iso->paint = frmMain->m_PaintBox_MoveSTO;
	move_iso->visual_dx = abs_iso->visual_dx;
	move_iso->visual_dy = move_iso->paint->Height - 30;;
	move_iso->ver_line_len = move_iso->paint->Height;
	move_iso->hor_line_len = 450;

	move_iso->coord_begin_point.x0 = 0;
	move_iso->coord_begin_point.v = v_vagon; // �������� ���������� ���
	move_iso->coord_begin_point.t = 0;

	//-----
	// �������� ������� �� abs_iso

	// ����� F1' - ����� ������� �� ������
	// �������������� � ������ � ����� A1 (� ������ ������)
	new_mat_point = new MMatPoint;
	new_mat_point->name = "F1'";
	new_mat_point->iso = move_iso;
	new_mat_point->color = clYellow;
	new_mat_point->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_mat_point->visual_dy = foton_visual_dy;
	new_mat_point->visual_rx = foton_visual_rx;
	new_mat_point->visual_ry = foton_visual_ry;
	new_mat_point->caption_up = true;
//	new_mat_point->x0 = 0;
//	new_mat_point->x = new_mat_point->x0;
//	new_mat_point->v = g_light_c;
//	new_mat_point->t = 0;
	move_iso->obj_list.Add(new_mat_point);

	// 60 ��
	cur_x = sec1_start_x;
	for (int i = 0; i < sec1_k; i++)
	{
		if ((i % 5) == 0)
		{
			new_mat_point = new MMatPoint;
			new_mat_point->name = "s'" + IntToStr(i);
			new_mat_point->iso = move_iso;
			new_mat_point->color = clBlue;
			new_mat_point->visual_dy = sec1_visual_dy;
			new_mat_point->visual_rx = sec5_visual_rx;
			new_mat_point->visual_ry = sec5_visual_ry;
			new_mat_point->caption_up = false;
			new_mat_point->caption_type = MMatObject::t_ct_name_time;
			if (i==0) new_mat_point->caption_color = clYellow;
//			new_mat_point->x0 = cur_x;
//			new_mat_point->x = new_mat_point->x0;
//			new_mat_point->v = v_vagon;
//			new_mat_point->t = 0;
			move_iso->obj_list.Add(new_mat_point);
		}
		else
		{
			new_mat_point = new MMatPoint;
			new_mat_point->name = "s'" + IntToStr(i);
			new_mat_point->iso = move_iso;
			new_mat_point->color = clBlue;
			new_mat_point->visual_dy = sec1_visual_dy;
			new_mat_point->visual_rx = sec1_visual_rx;
			new_mat_point->visual_ry = sec1_visual_ry;
			new_mat_point->caption_up = false;
			new_mat_point->caption_type = MMatObject::t_ct_name;
//			new_mat_point->x0 = cur_x;
//			new_mat_point->x = new_mat_point->x0;
//			new_mat_point->v = v_vagon;
//			new_mat_point->t = 0;
			move_iso->obj_list.Add(new_mat_point);
		}

		cur_x += sec1_dx;
	}

	// ������ D1' - ������������ ����� (���-�� �������)
	new_strelka = new MSensorStrelka;
	new_strelka->name = "D1'";
	new_strelka->iso = move_iso;
	new_strelka->color = (TColor)RGB(70,70,70);
	new_strelka->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_strelka->visual_dy = strelka_visual_dy;
	new_strelka->visual_rx = strelka_visual_rx;
	new_strelka->visual_ry = strelka_visual_ry;
	new_strelka->caption_up = true;
	new_strelka->type = "s";	// ����������� ������ �� ����� �����
	new_strelka->sost = MSensorStrelka::t_sost_undefined;	// � ������ ��������� = UNDEFINED
//	new_strelka->x0 = 120;
//	new_strelka->x = new_strelka->x0;
//	new_strelka->v = v_vagon;
//	new_strelka->t = 0;
	move_iso->obj_list.Add(new_strelka);

//	// ������ D2' - �������-������� ����������� �����
//	// ����������� ������ � ����� �����
//	new_mirror = new MSensorMirror;
//	new_mirror->name = "D2'";
//	new_mirror->iso = move_iso;
//	new_mirror->color = (TColor)RGB(70,70,70);
//	new_mirror->caption_color = (TColor)RGB(235,235,115);//clYellow;
//	new_mirror->visual_dy = mirror_visual_dy;
//	new_mirror->visual_rx = mirror_visual_rx;
//	new_mirror->visual_ry = mirror_visual_ry;
//	new_mirror->caption_up = true;
//	new_mirror->type = "F";	// ����������� ������ �� ������
//	new_mirror->sost = MSensorMirror::t_sost_no_count;	// � ������ ��������� = �������
////	new_mirror->x0 = mirror_pos;
////	new_mirror->x = new_mirror->x0;
////	new_mirror->v = 0;
////	new_mirror->t = 0;
//	move_iso->obj_list.Add(new_mirror);



	//-----
	// ������� ������� ����������
	abs_iso->MoveObjects(MIso::t_move_abs, time_start);

	
	// ��������� ���������� abs_iso � ����������� ���
	// move_iso
	abs_iso->TransformObjects(MIso::t_transform_sto, move_iso);
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::Button_AboutClick(TObject *Sender)
{
	frmAbout->ShowModal();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::m_CheckBox_Experiment_FixMoveIsoClick(TObject *Sender)
{
	if (m_CurExperiment)
	{
		if (m_CheckBox_Experiment_FixMoveIso->Checked == false)
		{
			m_CurExperiment->move_iso->coord_begin_point.x =
				m_CurExperiment->move_iso->coord_begin_point.v * m_CurExperiment->abs_iso->coord_begin_point.t;
		}
		else
		{
			// ����������� �������� ��� ������ = 0
			m_CurExperiment->move_iso->coord_begin_point.x = 0;
		}

		m_CurExperiment->Show();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Button_ExpSettingsClick(TObject *Sender)
{
	if (m_CurExperiment)
	{
		frmSettings->ShowModal();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormHide(TObject *Sender)
{
	Application->Terminate();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Button_ExpHelpClick(TObject *Sender)
{
	if (m_CurExperiment)
	{
		if (m_CurExperiment->m_frmHelp)
        {
			m_CurExperiment->m_frmHelp->ShowModal();
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormActivate(TObject *Sender)
{
	if (m_CurExperiment)
    {
		// ���� ������ - ���� ������ ���
		if (m_CurExperiment->m_frmHelp->Tag == 0)
		{
			frmMain->Tag = 1;

			m_CurExperiment->m_frmHelp->Tag = 1;
			m_CurExperiment->m_frmHelp->ShowModal();
		}
	}
}
//---------------------------------------------------------------------------


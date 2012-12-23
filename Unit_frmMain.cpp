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
	{
		m_CurExperiment = new MExperimentLastArgumentOfKing;
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
	int base_x = iso->visual_dx + iso->coord_begin_point.x + x;

	// ��� ������
	iso->paint->Canvas->Pixels[base_x][visual_dy] = clWhite;

	// �����
	iso->paint->Canvas->Brush->Color = color;
//	int r = 10;
	iso->paint->Canvas->FrameRect(
		TRect(base_x-visual_rx, visual_dy-visual_ry, base_x+visual_rx+1, visual_dy+visual_ry+1));

	// ���������
	AnsiString caption;
	caption.printf("%s (x=%0.2f, v=%0.2f, t=%0.2f)", name, x, v, t);

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

	t = in_new_time;
	x = x0 + v * t;
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

//---------------------------------------------------------------------------
// ������ �������������� �������� � ���������� ���
//---------------------------------------------------------------------------
void __fastcall MMatPoint::TransformStoFormula
(
	const double in_move_v,		// ������� �������� ����� ��� ������������ �������
	MMatPoint& out_obj			// ������ � ������� ����������� ��������� ��������������
)
{

	// ������� �������� ���������
	double vv = in_move_v * in_move_v;
	double cc = g_light_c * g_light_c;

	try
	{
		// v' = �������������� �������� ���������
		// ��� �������� ����� ��� K ==> K'
		out_obj.v = (v - in_move_v) / (1 - (v * in_move_v)/cc);
	}
	catch(...){}

	try
	{
		// t' = �������������� �����
		// ��� �������� ����� ��� K ==> K'
		out_obj.t = t * sqrt(1 - (vv)/(cc));
	}
	catch(...){}

	try
	{
		// �������� x0' = �������������� ���������
		// ��� �������� ����� ��� K ==> K'
		double x00 = (x0) / sqrt(1 - (vv)/(cc));

		// ������� x' �� ����� ����������� ������ ������������ ��� K'
		// x' = x0' + (v' * t')
		out_obj.x = x00 + out_obj.v * out_obj.t;
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
	int base_x = iso->visual_dx + iso->coord_begin_point.x + this->x;

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

	//if (this->caption_up)
	{
		iso->paint->Canvas->TextOutA(base_x - 10, visual_dy - visual_ry - 70, caption);
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
	int base_x = visual_dx + coord_begin_point.x;

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
		// ����������������� ������� � ������� t0
		time_cur = time_start;
		this->Reset();
		frmMain->m_Memo_Dbg->Clear();
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

	//-----
	// �������������� �����������
	time_start = 0;
	time_end = 50;
	time_step = 1;
	time_cur = time_start;
	this->m_frmHelp = frmHelpExp1;

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
	double v_vagon = 20;
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

	//-----
	// �������������� �����������
	time_start = 0;
	time_end = 34;//100;
	time_step = 0.2;//10;
	time_cur = time_start;
	this->m_frmHelp = frmHelpExp2;	

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
	new_mat_point->v = -g_light_c;
	new_mat_point->t = 0;
	abs_iso->obj_list.Add(new_mat_point);

	//-----
	// �����
	//-----
	// �������� ������
	double v_vagon = 20;
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


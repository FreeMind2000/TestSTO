//---------------------------------------------------------------------------
// < TestSTO >
//---------------------------------------------------------------------------
// Автор: Александр Меркелов (free_mind2@list.ru)
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
	str.printf("Релятивистская физика (C=const=%0.2f относительно любых ИСО)", g_light_c);
	Panel_3->Caption = str;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormShow(TObject *Sender)
{
	// Инициализируем эксперимент из списка
	m_ComboBox_ExperimentChange(NULL);
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
	// Удаляем старый эксперимент
	if (m_CurExperiment)
	{
		delete m_CurExperiment;
		m_CurExperiment = NULL;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::m_ComboBox_ExperimentChange(TObject *Sender)
{
	// Удаляем старый эксперимент
	if (m_CurExperiment)
	{
		delete m_CurExperiment;
		m_CurExperiment = NULL;
	}

	// создаем новый
	if (m_ComboBox_Experiment->ItemIndex == 0)
	{
		m_CurExperiment = new MExperimentEasyMove;
	}
	else
	{
		m_CurExperiment = new MExperimentLastArgumentOfKing;
	}

	// инитим
	m_CurExperiment->Init();
	m_CurExperiment->Show();
	frmMain->m_Memo_Dbg->Lines->Add("----------");

	// окно помощи - если первый раз
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
	// Отрисовка
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
// Рабочие классы
//
//===========================================================================


//---------------------------------------------------------------------------
// Визуализация
//---------------------------------------------------------------------------
void __fastcall MMatPoint::Show
(
)
{
	// базовый X
	int base_x = iso->visual_dx + iso->coord_begin_point.x + x;

	// сам объект
	iso->paint->Canvas->Pixels[base_x][visual_dy] = clWhite;

	// фрейм
	iso->paint->Canvas->Brush->Color = color;
//	int r = 10;
	iso->paint->Canvas->FrameRect(
		TRect(base_x-visual_rx, visual_dy-visual_ry, base_x+visual_rx+1, visual_dy+visual_ry+1));

	// заголовок
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
// двигаем объекты в неподвижной ИСО
// t_move_abs: x = x0 + v * t
//---------------------------------------------------------------------------
void __fastcall MMatPoint::Move
(
	const int in_t_move,		// тип движения
	const double in_new_time	// новое t для расчета
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
// формула движения abs - расчитываем x по новому t
// t_move_abs: x = x0 + v * t
//---------------------------------------------------------------------------
void __fastcall MMatPoint::MoveAbsFormula
(
	const double in_new_time	// новое t для расчета
)
{
	// x – текущая x координата точки
	// x0 – начальная x координата точки
	// v –скорость точки
	// t – текущее время для точки

	t = in_new_time;
	x = x0 + v * t;
}

//---------------------------------------------------------------------------
// Делаем преобразование объектов в движущуюся ИСО
//---------------------------------------------------------------------------
void __fastcall MMatPoint::Transform
(
	const int in_t_transform,	// тип трансформации
	const double in_move_v,		// скрость движения новой ИСО относительно текущей
	MMatObject& out_obj			// объект в который сохраняется результат преобразования
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
// Делаем преобразование объектов в движущуюся ИСО
//---------------------------------------------------------------------------
void __fastcall MMatPoint::TransformStoFormula
(
	const double in_move_v,		// скрость движения новой ИСО относительно текущей
	MMatPoint& out_obj			// объект в который сохраняется результат преобразования
)
{

	// считаем квадраты скоростей
	double vv = in_move_v * in_move_v;
	double cc = g_light_c * g_light_c;

	try
	{
		// v' = релятивистское сложение скоростей
		// для перехода между ИСО K ==> K'
		out_obj.v = (v - in_move_v) / (1 - (v * in_move_v)/cc);
	}
	catch(...){}

	try
	{
		// t' = релятивистское время
		// для перехода между ИСО K ==> K'
		out_obj.t = t * sqrt(1 - (vv)/(cc));
	}
	catch(...){}

	try
	{
		// получаем x0' = релятивистское растояние
		// для перехода между ИСО K ==> K'
		double x00 = (x0) / sqrt(1 - (vv)/(cc));

		// Считаем x' по ранее расчитанным данным относительно ИСО K'
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
// Запускаем активацию объекта
//---------------------------------------------------------------------------
void __fastcall MSensor2::Run
(
)
{
	int k_work = 0;
	//double sensor_zone = 0.001;	// зона чувствительности датчика
									// (т.к. при больших delta t - фотоны могут перепрыгивать
									// через точку в которой находится датчик)
	double sensor_zone = this->visual_rx; 

	// перебираем все объекты связанные с нашей ИСО
	// и считаем кол-во объектов нашего типа находящихся
	// в той же точке, что и датчик
	for (int i=this->iso->obj_list.CicleCount(); i>0; i--)
	{
		// текущий объект
		MMatPoint& cur_obj = (MMatPoint&)this->iso->obj_list.GetCurObj();

		// если нашего типа
		if (cur_obj.name.SubString(1,1) == this->type)
		{
			// если в нашей точке
			if ((cur_obj.x <= (this->x + sensor_zone)) && (cur_obj.x >= (this->x - sensor_zone)))
			if ((cur_obj.t <= (this->t + sensor_zone)) && (cur_obj.t >= (this->t - sensor_zone)))
			{
				k_work++;
			}
		}

		this->iso->obj_list.MoveNext();
	}

	// если состояние еще не определено
	if (this->sost == MSensor2::t_sost_undefined)
	{
		// проверяем результат
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
			// не меняем состояние
		}
	}
}

//---------------------------------------------------------------------------
// Визуализация
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
			str_msg = "ON - Ба-ба-х!!!";
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

	// базовый X
	int base_x = iso->visual_dx + iso->coord_begin_point.x + this->x;

	// фрейм
	iso->paint->Canvas->Brush->Color = color_sost;
//	int r = 10;
	iso->paint->Canvas->FillRect(
		TRect(base_x-visual_rx, visual_dy-visual_ry, base_x+visual_rx+1, visual_dy+visual_ry+1));

	// верт. линия
	iso->paint->Canvas->Pen->Color = color_sost;
	iso->paint->Canvas->MoveTo(base_x, visual_dy);
	iso->paint->Canvas->LineTo(base_x, visual_dy - visual_ry - 70);


	// заголовок
	AnsiString caption;
	caption.printf("%s (x=%0.2f, v=%0.2f, t=%0.2f) Состояние = %s",
		name, x, v, t, str_msg);

	iso->paint->Canvas->Font->Color = caption_color; //(TColor)RGB(0,255,100);
	iso->paint->Canvas->Brush->Color = color_sost;

	//if (this->caption_up)
	{
		iso->paint->Canvas->TextOutA(base_x - 10, visual_dy - visual_ry - 70, caption);
	}
}

//---------------------------------------------------------------------------
// Визуализация
//---------------------------------------------------------------------------
void __fastcall MIso::Show
(
)
{
	// базовый X
	int base_x = visual_dx + coord_begin_point.x;

	// цвет
	paint->Canvas->Pen->Color = color;
	paint->Canvas->Brush->Color = color;

	// точка - начало координат
	int r = 3;
	//paint->Canvas->FrameRect(TRect(base_x-r, visual_dy-r, base_x+r+1, visual_dy+r+1));
	paint->Canvas->FrameRect(TRect(base_x-r, visual_dy-r, base_x+r+1, visual_dy+r+1));

	// вертикальная линия
	paint->Canvas->MoveTo(base_x, visual_dy);
	paint->Canvas->LineTo(base_x, visual_dy - ver_line_len);
	// горизонтальная линия
	paint->Canvas->MoveTo(base_x, visual_dy);
	paint->Canvas->LineTo(base_x + hor_line_len, visual_dy);

	// заголовок
	AnsiString caption;
	caption.printf("%s (v=%0.2f)", name, coord_begin_point.v);
	paint->Canvas->Font->Color = clWhite;
	paint->Canvas->Brush->Color = clBlack;
	paint->Canvas->TextOutA(base_x - 10, visual_dy+5, caption);

	// выводим список связанных объектов
	for (int i=obj_list.CicleCount(); i>0; i--)
	{
		obj_list.GetCurObj().Show();

		obj_list.MoveNext();
	}
}

//---------------------------------------------------------------------------
// двигаем объекты в неподвижной ИСО
// t_move_abs: x = x0 + v * t
//---------------------------------------------------------------------------
void __fastcall MIso::MoveObjects
(
	const int in_t_move,		// тип движения
	const double in_new_time	// новое t для расчета
)
{
	// собственная скорость ИСО всегда = 0
	this->coord_begin_point.x = 0;
	this->coord_begin_point.t = in_new_time;

	// выводим список связанных объектов
	for (int i=obj_list.CicleCount(); i>0; i--)
	{
		obj_list.GetCurObj().Move(in_t_move, in_new_time);

		obj_list.MoveNext();
	}
}

//---------------------------------------------------------------------------
// Делаем преобразование объектов в движущуюся ИСО
//---------------------------------------------------------------------------
void __fastcall MIso::TransformObjects
(
	const int in_t_transform,	// тип трансформации
	MIso* inout_iso          	// ИСО в которую сохраняется результат трансформации
)
{
	//-----
	// Расчитываем положение начала координат inout_iso относительно текущей ИСО (this)
	if (frmMain->m_CheckBox_Experiment_FixMoveIso->Checked == false)
	{
		inout_iso->coord_begin_point.x = inout_iso->coord_begin_point.v * this->coord_begin_point.t;
	}
	else
	{
		// собственная скорость ИСО всегда = 0
		inout_iso->coord_begin_point.x = 0;
	}



	//-----
	// Преобразовываем координаты текущей неподвижной системы (this)
	// в координаты ИСО inout_iso движущейся относительно нее

	// перебираем список связанных объектов с текущей ИСО
	// и параллельно с движущейся ИСО
	// (кол-во объектов и порядок добавления должны быть ОДИНАКОВЫ)
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
// Активируем объекты (проверяем срабатывание датчиков)
//---------------------------------------------------------------------------
void __fastcall MIso::RunObjects
(
)
{
	// т.к. мы перебираем список внутри Run - его CurObj меняется
	// поэтому необходимо запомнить/восстановить его текущий CurObj перед MoveNext
	MListObj<MMatObject> *cur_obj;

	// перебираем список связанных объектов с текущей ИСО
	for (int i=obj_list.CicleCount(); i>0; i--)
	{
		cur_obj = obj_list.CurObj;

		// в каждом объекте запускаем ф-цию активации
		obj_list.GetCurObj().Run();

		obj_list.CurObj = cur_obj;

		obj_list.MoveNext();
	}
}

//---------------------------------------------------------------------------
// Визуализация
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
// Запускаем с учетом параметров на форме
//---------------------------------------------------------------------------
void __fastcall MExperimentBase::Run
(
)
{
	// изменяем текущее время
	time_cur += time_step;

	// если время кончилось - циклим
	if (time_cur > time_end)
	{
		// Переинициализация системы с момента t0
		time_cur = time_start;
		this->Reset();
		frmMain->m_Memo_Dbg->Clear();
	}

	// двигаем объекты в неподвижной ИСО
	// x = x0 + v * t
	abs_iso->MoveObjects(MIso::t_move_abs, time_cur);

	// создаем новую ИСО
	// преобразуя новые координаты abs_iso
	// с помощью Лоренцевых преобразований
	// x' = (x - v*t) / sqrt(1 - (v*v)/(c*c))
	abs_iso->TransformObjects(MIso::t_transform_sto, move_iso);
	//move_iso = abs_iso->TransformToNewIso(v_move, MIso::t_transform_sto);

	// активируем объекты в покоящейся ИСО
	// (проверяем срабатывание датчиков связанных с данной ИСО)
	abs_iso->RunObjects();

	// активируем объекты в движущейся ИСО
	// (проверяем срабатывание датчиков связанных с данной ИСО)
	move_iso->RunObjects();
}

//---------------------------------------------------------------------------
// Инициализация MExperimentEasyMove
//---------------------------------------------------------------------------
void __fastcall MExperimentEasyMove::Init
(
)
{
	frmMain->m_Memo_Dbg->Clear();

	//-----
	// инициализируем эксперимент
	time_start = 0;
	time_end = 50;
	time_step = 1;
	time_cur = time_start;
	this->m_frmHelp = frmHelpExp1;

	//-----
	// создаем покоящуюся ИСО
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
	// добавляем объекты эксперимента
	MMatPoint* new_mat_point;

	// скорость вагона
	double v_vagon = 20;
	// высота на которой отображается вагон
	int vagon_visual_dy = abs_iso->visual_dy - 50;
	int vagon_visual_rx = 10;
	int vagon_visual_ry = 30;

	// Точка A1 - начало вагона
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

	// Точка As - середина вагона
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

	// Точка A2 - конец вагона
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
	// создаем движущуюся ИСО
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
	move_iso->coord_begin_point.v = v_vagon; // скорость движущейся ИСО
	move_iso->coord_begin_point.t = 0;

	//-----
	// копируем объекты из abs_iso

	// Точка A1' - начало вагона
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

	// Точка As' - середина вагона
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

	// Точка A2' - конец вагона
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
	// считаем текущие координаты
	abs_iso->MoveObjects(MIso::t_move_abs, time_start);

	
	// переводим координаты abs_iso в двигающуюся ИСО
	// move_iso
	abs_iso->TransformObjects(MIso::t_transform_sto, move_iso);
}

//---------------------------------------------------------------------------
// Сброс состояния датчиков
//---------------------------------------------------------------------------
void __fastcall MExperimentEasyMove::Reset
(
)
{
	// сброс объектов abs_iso
	// + устновка скорости движения поезда ИСО K' (ее можно изменить в параметрах)
	for (int i=abs_iso->obj_list.CicleCount(); i>0; i--)
	{
		// объекы A - A1,As,A2 = поезд
		if (abs_iso->obj_list.GetCurObj().name.SubString(1,1) == "A")
		{
			MMatPoint& mp = (MMatPoint&)abs_iso->obj_list.GetCurObj();

			// скорость поезда = скорость движущейся ИСО K'
			mp.v = this->move_iso->coord_begin_point.v;
		}

		// в этом эксперименте датчиков нет
		// ничего не сбрасываем

		// скорость фотонов не меняется = C
		// их не трогам...

		//
		abs_iso->obj_list.MoveNext();
	}

	// сброс объектов move_iso
	// в этом эксперименте датчиков нет
	// ничего не сбрасываем
}

//---------------------------------------------------------------------------
// Инициализация MExperimentLastArgumentOfKing
//---------------------------------------------------------------------------
void __fastcall MExperimentLastArgumentOfKing::Init
(
)
{
	frmMain->m_Memo_Dbg->Clear();

	//-----
	// инициализируем эксперимент
	time_start = 0;
	time_end = 34;//100;
	time_step = 0.2;//10;
	time_cur = time_start;
	this->m_frmHelp = frmHelpExp2;	

	//-----
	// создаем покоящуюся ИСО
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
	// добавляем объекты эксперимента
	MMatPoint* new_mat_point;

	//-----
	// Фотоны (фонари)
	//-----
	int foton_visual_dy = abs_iso->visual_dy - 50;
	int foton_visual_rx = 5;
	int foton_visual_ry = 5;

	// Точка F1 - фотон летящий из фонаря
	// расположенного в вагоне в точке A1 (в начале вагона)
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

	// Точка F2 - фотон летящий из фонаря
	// расположенного в вагоне в точке A2 (в конце вагона)
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
	// Вагон
	//-----
	// скорость вагона
	double v_vagon = 20;
	// высота на которой отображается вагон
	int vagon_visual_dy = abs_iso->visual_dy - 50;
	int vagon_visual_rx = 10;
	int vagon_visual_ry = 30;

	// Точка A1 - начало вагона
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

	// Точка As - середина вагона
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

	// Точка A2 - конец вагона
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
	// Датчики (регистрирующий прибор)
	//-----
	MSensor2* new_sensor;
	int sensor_visual_dy = abs_iso->visual_dy - 50;
	int sensor_visual_rx = 7;
	int sensor_visual_ry = 10;

	// Датчик D1 - регистрирует одновременность прихода фотон из начала/конца вагона
	// расположенного в вагоне в точке As (в середине вагона)
	new_sensor = new MSensor2;
	new_sensor->name = "D1";
	new_sensor->iso = abs_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = true;
	new_sensor->type = "F";	// срабатывает только на фотоны
	new_sensor->sost = MSensor2::t_sost_undefined;	// в начале состояние = UNDEFINED
	new_sensor->x0 = 120;
	new_sensor->v = v_vagon;
	new_sensor->t = 0;
	abs_iso->obj_list.Add(new_sensor);


	//-----
	// создаем движущуюся ИСО
	move_iso = new MIso;

	move_iso->name = "K'";
	move_iso->color = clRed;
	move_iso->paint = frmMain->m_PaintBox_MoveSTO;
	move_iso->visual_dx = abs_iso->visual_dx;
	move_iso->visual_dy = move_iso->paint->Height - 30;;
	move_iso->ver_line_len = move_iso->paint->Height;
	move_iso->hor_line_len = 450;

	move_iso->coord_begin_point.x0 = 0;
	move_iso->coord_begin_point.v = v_vagon; // скорость движущейся ИСО
	move_iso->coord_begin_point.t = 0;

	//-----
	// копируем объекты из abs_iso

	// Точка F1' - фотон летящий из фонаря
	// расположенного в вагоне в точке A1 (в начале вагона)
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

	// Точка F2' - фотон летящий из фонаря
	// расположенного в вагоне в точке A2 (в конце вагона)
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

	// Точка A1' - начало вагона
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

	// Точка As' - середина вагона
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

	// Точка A2' - конец вагона
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

	// Датчик D1' - регистрирует одновременность прихода фотон из начала/конца вагона
	// расположенного в вагоне в точке As (в середине вагона)
	new_sensor = new MSensor2;
	new_sensor->name = "D1'";
	new_sensor->iso = move_iso;
	new_sensor->color = (TColor)RGB(70,70,70);
	new_sensor->caption_color = (TColor)RGB(235,235,115);//clYellow;
	new_sensor->visual_dy = sensor_visual_dy;
	new_sensor->visual_rx = sensor_visual_rx;
	new_sensor->visual_ry = sensor_visual_ry;
	new_sensor->caption_up = true;
	new_sensor->type = "F";	// срабатывает только на фотоны
	new_sensor->sost = MSensor2::t_sost_undefined;	// в начале состояние = UNDEFINED
//	new_sensor->x0 = 0;
//	new_sensor->v = v_vagon;
//	new_sensor->t = 0;
	move_iso->obj_list.Add(new_sensor);



	//-----
	// считаем текущие координаты
	abs_iso->MoveObjects(MIso::t_move_abs, time_start);

	
	// переводим координаты abs_iso в двигающуюся ИСО
	// move_iso
	abs_iso->TransformObjects(MIso::t_transform_sto, move_iso);
}

//---------------------------------------------------------------------------
// Сброс состояния датчиков
//---------------------------------------------------------------------------
void __fastcall MExperimentLastArgumentOfKing::Reset
(
)
{
	// сброс объектов abs_iso
	// + устновка скорости движения поезда ИСО K' (ее можно изменить в параметрах)
	for (int i=abs_iso->obj_list.CicleCount(); i>0; i--)
	{
		if (abs_iso->obj_list.GetCurObj().name == "D1")
		{
			MSensor2& d1 = (MSensor2&)abs_iso->obj_list.GetCurObj();

			//
			d1.sost = MSensor2::t_sost_undefined;

			// скорость датчика = скорость поезда = скорость движущейся ИСО K'
			d1.v = this->move_iso->coord_begin_point.v;
		}

		// объекы A - A1,As,A2 = поезд
		if (abs_iso->obj_list.GetCurObj().name.SubString(1,1) == "A")
		{
			MMatPoint& mp = (MMatPoint&)abs_iso->obj_list.GetCurObj();

			// скорость поезда = скорость движущейся ИСО K'
			mp.v = this->move_iso->coord_begin_point.v;
		}

		// скорость фотонов не меняется = C
		// их не трогам...

		//
		abs_iso->obj_list.MoveNext();
	}

	// сброс объектов move_iso
	for (int i=move_iso->obj_list.CicleCount(); i>0; i--)
	{
		if (move_iso->obj_list.GetCurObj().name == "D1'")
		{
			MSensor2& d1 = (MSensor2&)move_iso->obj_list.GetCurObj();

			d1.sost = MSensor2::t_sost_undefined;
		}

		// скорости и координаты всех объектов
		// расчитываются по формуле преобразования ИСО K в ИСО K' в Run()
		// тут не трогаем...

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
			// собственная скорость ИСО всегда = 0
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
		// окно помощи - если первый раз
		if (m_CurExperiment->m_frmHelp->Tag == 0)
		{
			frmMain->Tag = 1;

			m_CurExperiment->m_frmHelp->Tag = 1;
			m_CurExperiment->m_frmHelp->ShowModal();
		}
	}
}
//---------------------------------------------------------------------------


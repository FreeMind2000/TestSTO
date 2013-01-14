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
//---------------------------------------------------------------------------
USEFORM("Unit_frmMain.cpp", frmMain);
USEFORM("Unit_frmAbout.cpp", frmAbout);
USEFORM("Unit_frmSettings.cpp", frmSettings);
USEFORM("Unit_frmHello.cpp", frmHello);
USEFORM("Unit_frmHelpExp1.cpp", frmHelpExp1);
USEFORM("Unit_frmHelpExp2.cpp", frmHelpExp2);
USEFORM("Unit_frmHelpSut.cpp", frmHelpSut);
USEFORM("Unit_frmHelpExp3.cpp", frmHelpExp3);
USEFORM("Unit_frmHelpExp4.cpp", frmHelpExp4);
USEFORM("Unit_frmHelpExp5.cpp", frmHelpExp5);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		SetApplicationMainFormOnTaskBar(Application, true);
		Application->CreateForm(__classid(TfrmHello), &frmHello);
		Application->CreateForm(__classid(TfrmSettings), &frmSettings);
		Application->CreateForm(__classid(TfrmHelpExp1), &frmHelpExp1);
		Application->CreateForm(__classid(TfrmHelpExp2), &frmHelpExp2);
		Application->CreateForm(__classid(TfrmMain), &frmMain);
		Application->CreateForm(__classid(TfrmAbout), &frmAbout);
		Application->CreateForm(__classid(TfrmHelpSut), &frmHelpSut);
		Application->CreateForm(__classid(TfrmHelpExp3), &frmHelpExp3);
		Application->CreateForm(__classid(TfrmHelpExp4), &frmHelpExp4);
		Application->CreateForm(__classid(TfrmHelpExp5), &frmHelpExp5);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

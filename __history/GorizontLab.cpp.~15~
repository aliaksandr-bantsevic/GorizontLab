//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("FPort_adjust.cpp", Form_Port_adjust);
USEFORM("FPlace_adjust.cpp", Form_Place_adjust);
USEFORM("FMain.cpp", Form_General);
USEFORM("FSensor_adjust.cpp", Form_Sensor_adjust);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TForm_General), &Form_General);
		Application->CreateForm(__classid(TForm_Port_adjust), &Form_Port_adjust);
		Application->CreateForm(__classid(TForm_Place_adjust), &Form_Place_adjust);
		Application->CreateForm(__classid(TForm_Sensor_adjust), &Form_Sensor_adjust);
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

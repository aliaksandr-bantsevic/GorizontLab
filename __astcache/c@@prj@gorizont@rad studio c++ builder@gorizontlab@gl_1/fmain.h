//---------------------------------------------------------------------------

#ifndef FMainH
#define FMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>

#include <Vcl.Menus.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <Vcl.Dialogs.hpp>
#include <list>

#include "defs.h"

#include "GL_System.h"
#include "GL_Port.h"
#include "GL_Sensor.h"

#include "FPort_adjust.h"
#include "FPlace_adjust.h"
#include "FSensor_adjust.h"
#include "MainMonitorThread.h"

#define SYS_MODE_ADJ 0
#define SYS_MODE_RUN 1

//---------------------------------------------------------------------------
class TForm_General : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar;
	TStatusBar *StatusBar;
	TPanel *Panel_Browser;
	TSplitter *Splitter1;
	TPanel *Panel_Work;
	TPanel *Panel_Data;
	TSplitter *Splitter2;
	TPanel *Panel_Console;
	TToolButton *ToolButton1;
	TImageList *ImageList_Tool_bar;
	TTreeView *TreeView_Browser;
	TPageControl *PageControl_Data;
	TPageControl *PageControl_Console;
	TTimer *Timer_General_1s;
	TPopupMenu *PopupMenu;
	TMenuItem *N_AddPlace;
	TMenuItem *N_DeletePlace;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TToolButton *ToolButton4;
	TToolButton *ToolButton5;
	TToolButton *ToolButton6;
	TTimer *Timer_Init_app;
	TImageList *ImageList_Tree_browser;
	TMenuItem *N_AddPort;
	TMenuItem *N_DeletePort;
	TMenuItem *N_AddSensor;
	TMenuItem *N_DeleteSensor;
	TMenuItem *N_Port;
	TMenuItem *N_Sensor;
	TMenuItem *N_AdjustPlace;
	TMenuItem *N_AdjustPort;
	TMenuItem *N_AdjustSensor;
	TMenuItem *N_AdjustSystem;
	TXMLDocument *XMLDocument_conf;
	TFileOpenDialog *FileOpenDialog;
	TFileSaveDialog *FileSaveDialog;
	TSaveDialog *SaveDialog;
	TOpenDialog *OpenDialog;
	TTabSheet *TabSheet_console;
	TListBox *ListBox_console;
	TTabSheet *TabSheet_ports;
	TTabSheet *TabSheet_sensors;
	TTabSheet *TabSheet_data;
	TListView *ListView_ports;
	TListView *ListView_sensors;
	TListView *ListView_data;
	TToolButton *ToolButton_start;
	void __fastcall Timer_General_1sTimer(TObject *Sender);
	void __fastcall Timer_Init_appTimer(TObject *Sender);
	void __fastcall ToolButton1Click(TObject *Sender);
	void __fastcall TreeView_BrowserClick(TObject *Sender);
	void __fastcall PopupMenuPopup(TObject *Sender);
	void __fastcall N_AddPortClick(TObject *Sender);
	void __fastcall N_AddPlaceClick(TObject *Sender);
	void __fastcall N_AddSensorClick(TObject *Sender);
	void __fastcall ToolButton4Click(TObject *Sender);
	void __fastcall ToolButton3Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall N_AdjustSensorClick(TObject *Sender);
	void __fastcall N_DeleteSensorClick(TObject *Sender);
	void __fastcall N_DeletePortClick(TObject *Sender);
	void __fastcall N_DeletePlaceClick(TObject *Sender);
	void __fastcall ToolButton2Click(TObject *Sender);
	void __fastcall ToolButton5Click(TObject *Sender);
	void __fastcall ToolButton6Click(TObject *Sender);
	void __fastcall N_AdjustPortClick(TObject *Sender);
	void __fastcall TreeView_BrowserDblClick(TObject *Sender);
	void __fastcall TreeView_BrowserMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ToolButton_startClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TForm_General(TComponent* Owner);

private:     //Window property management

	void InitMainWindow(void);
	void DevideMainWindow(int browser_part, int data_part);

	void InitApplication(void);
    void HidePopupMenu(void);
	void ShowSystemElement(WideString s);

	void AddPlace(void);
	void AddPort(void);
	void AddSensor(void);
	void AdjustSensor(void);
	void SetGeneralCaption(void);
	void AdjustPort(void);
	void refresh_system_dashboard(void);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_General *Form_General;
//---------------------------------------------------------------------------
#endif

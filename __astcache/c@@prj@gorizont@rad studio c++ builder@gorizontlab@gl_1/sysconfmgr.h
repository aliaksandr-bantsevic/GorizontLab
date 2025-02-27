﻿//---------------------------------------------------------------------------

#ifndef SysConfMgrH
#define SysConfMgrH

#include "defs.h"
#include <vcl.h>

#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
//---------------------------------------------------------------------------

class TSysConfMgr
{

public:

	TSysConfMgr();
	~TSysConfMgr();

private:

	WideString curDir;
	WideString curSysName;
	WideString confFold;
	WideString baseFold;
	WideString curConfPath;
	WideString curBasePath;
	WideString logFilePath;

	TXMLDocument* xmlDoc;

private:

   void IniCurPath(void);
   void SelCurSysName(void);
   TIniFile * ini;

public:

	void SetXMLDoc(TXMLDocument* doc);
	TXMLDocument* GetXMLDoc(void);
	TCHAR* GetXMLDocPath(void);
	void SaveCurSysName(void);
	int CreateConf(TSaveDialog* dlg);
	int OpenConf(TOpenDialog* dlg);
	int AcceptConf(TCHAR* newconf);
	int SaveConf(TSaveDialog* dlg);
	TCHAR* GetLogFilePath(void);
    TCHAR* GetCurConfPath(void);
    TCHAR* GetCurBasePath(void);
};




#endif

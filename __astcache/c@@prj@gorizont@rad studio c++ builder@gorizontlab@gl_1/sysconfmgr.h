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

	TXMLDocument* xmlDoc;

private:

   void IniCurPath(void);
   void SelCurSysName(void);

public:

	void SetXMLDoc(TXMLDocument* doc);
	TXMLDocument* GetXMLDoc(void);
    TCHAR* GetXMLDocPath(void);
};




#endif
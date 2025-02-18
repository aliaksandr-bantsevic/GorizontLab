﻿//---------------------------------------------------------------------------

#pragma hdrstop

#include "SysConfMgr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

extern void SetGeneralCaption(TCHAR* s);

TSysConfMgr::TSysConfMgr()
{
   IniCurPath();
}

TSysConfMgr::~TSysConfMgr()
{

}

void TSysConfMgr::SelCurSysName(void)
{

	TCHAR tdir [1024];
	::GetCurrentDirectoryW(1024, tdir);

	curDir = tdir;

	wcscat(tdir, L"\\GorizontLab.gcf");

	ini = new TIniFile(tdir);

	//curSysName = L"GorizontLab";

	curSysName = ini->ReadString("GENERAL","CONFNAME", L"GorizontLab");

}

void TSysConfMgr::IniCurPath(void)
{
	SelCurSysName();
	
	//TCHAR tdir [1024];
	//::GetCurrentDirectoryW(1024, tdir);

	//curDir = tdir;

	curConfPath = curDir + L"\\" + L"Conf"  L"\\";
	::CreateDirectoryW(curConfPath.c_bstr(), 0);

	confFold = curConfPath;

	curConfPath = curConfPath + curSysName + L".xml";

	curBasePath =  curDir + L"\\" + L"Base"  L"\\";
	::CreateDirectoryW(curBasePath.c_bstr(), 0);

	baseFold = curBasePath;

	curBasePath = curBasePath + curSysName + L"\\";
    ::CreateDirectoryW(curBasePath.c_bstr(), 0);


	logFilePath = curDir + L"\\" + L"GorizontLab.log";

	Sleep(1);
}

void TSysConfMgr::SetXMLDoc(TXMLDocument* doc)
{
   xmlDoc = doc;
}

TXMLDocument* TSysConfMgr::GetXMLDoc(void)
{
   return xmlDoc;
}

TCHAR* TSysConfMgr::GetXMLDocPath(void)
{
   return  curConfPath.c_bstr();
}

TCHAR* TSysConfMgr::GetLogFilePath(void)
{
   return  logFilePath.c_bstr();
}

void TSysConfMgr::SaveCurSysName(void)
{
	ini->WriteString("GENERAL","CONFNAME", curSysName);
}

int TSysConfMgr::CreateConf(TSaveDialog* dlg)
{
	 wchar_t fres[1024];
	 wchar_t cres[1024];

	 dlg->InitialDir = confFold;
	 dlg->Filter = L"*.xml|*.xml";
	 dlg->FileName = L"NewSystemGL";
	 dlg->Title = L"Cоздать новую систему";
	 if (dlg->Execute()!=IDOK) return -1;

	 wcscpy(fres, (wchar_t*)dlg->FileName.c_str());
	 wcscpy(cres, (wchar_t*)dlg->FileName.c_str());

	 int i = 0;

	 for (i = wcslen(cres); i >=0 ; i--) {

		if (cres[i] == '\\') break;
	 }

	 TCHAR cnew [1024];
	 wcscpy(cnew, &cres[i+1]);

	 i = 0;

	 for (i = wcslen(cnew); i >=0 ; i--) {

		if (cnew[i] == '.')
		{
			cnew[i] = 0;
			break;
		}
	 }

	 AcceptConf(cnew);

	 return 0;
}

int TSysConfMgr::OpenConf(TOpenDialog* dlg)
{

	 WideString fres(L"");

	 dlg->InitialDir = confFold;
	 dlg->Filter = L"*.xml|*.xml";
	 dlg->FileName = curSysName;

	 dlg->Title = L"Открыть конфигурацию";
	 if (dlg->Execute()!=IDOK) return -1;

	 fres = dlg->FileName;

	 TCHAR cres[1024];
	 wcscpy(cres,fres.c_bstr());

	 int i = 0;

	 for (i = wcslen(cres); i >=0 ; i--) {

		if (cres[i] == '\\') break;
	 }

	 TCHAR cnew [1024];
	 wcscpy(cnew, &cres[i+1]);

	 i = 0;

	 for (i = wcslen(cnew); i >=0 ; i--) {

		if (cnew[i] == '.')
		{
			cnew[i] = '\0';
			break;
		}
	 }

	 AcceptConf(cnew);


   return 0;
}

int TSysConfMgr::AcceptConf(TCHAR* newconf)
{
   curSysName = newconf;
   SaveCurSysName();
   IniCurPath();

   return 0;
}

int TSysConfMgr::SaveConf(TSaveDialog* dlg)
{

	 WideString fres(L"");
	 int i = 0;

	 dlg->InitialDir = confFold;
	 dlg->Filter = L"*.xml|*.xml";

	 dlg->FileName = L"NewSystemGL";

	 dlg->Title = L"Сохранить конфигурацию как ...";
	 if (dlg->Execute()!=IDOK) return -1;

	 fres = dlg->FileName;

	 TCHAR fpathold [1024];
	 TCHAR fpathnew [1024];

	 wcscpy(fpathold, confFold.c_bstr());
	 wcscpy(fpathnew, fres.c_bstr());

	 int ll = wcslen(fpathnew) -1;

	 if (
		  (fpathnew[ll] == 'l')&&
		  (fpathnew[ll-1] == 'm')&&
		  (fpathnew[ll-2] == 'x')&&
		  (fpathnew[ll-3] == '.')

		)
	 {
		  fpathnew[ll-3] = 0;

		  TCHAR fn[1024];
		  wcscpy(fn, dlg->FileName.c_str());
		  int kk = wcslen(fn);
		  fn[kk-5] = 0;
		  dlg->FileName = fn;
	 }

	 wcscat(fpathold, curSysName.c_bstr());
	 //wcscat(fpathnew, fres.c_bstr());

	 wcscat(fpathold, L".xml");
	 wcscat(fpathnew, L".xml");

	 FILE* fn =  _wfopen(fpathnew,L"rb");

	 if (fn != NULL)
	 {

		fclose(fn);
		Application->MessageBoxW(L"Данная конфигурация уже существует!",L"ВНИМАНИЕ!",0);
		return -1;
	 }

     fclose (fn);

	 WideString sfold = fpathold;
	 WideString sfnew = fpathnew;

	 ::CopyFile(fpathold, fpathnew, false);

	 int ii = wcslen(fpathnew) - 1;
	 TCHAR nsn [1024];

	 while(fpathnew[ii] != '\\')
	 {
		 TCHAR tc = fpathnew[ii];
		 if (tc == '\\')
		 {
			break;
		 }
		 ii--;
	 }

	 int j = 0;

	 for(int i = ii; i<wcslen(fpathnew) - 4; i++)
	 {
		if (fpathnew[i] != '\\')
		{
		 	nsn[j] = fpathnew[i]; j++;
		}
	 }

	 nsn[j] = 0;

	 AcceptConf(nsn);

	return 0;
}

TCHAR* TSysConfMgr::GetCurConfPath(void)
{
	return curConfPath.c_bstr();
}

TCHAR* TSysConfMgr::GetCurBasePath(void)
{
	return curBasePath.c_bstr();
}

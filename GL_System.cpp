//---------------------------------------------------------------------------

#pragma hdrstop

#include "GL_System.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <list>
#include "GL_Place.h"
#include "GL_Port.h"
#include "GL_Sensor.h"

#include <System.SysUtils.hpp>
//---------------------------------------------------------------------------

extern TDateTime g_monitor_second_timer;

TGLSystem::TGLSystem()
{
   name = L"GorizontLab Monitoring System";
   mark = L"...";
   node = NULL;
}

TGLSystem::TGLSystem(TTreeView* t, TXMLDocument* xmlDoc)
{
   name = L"GorizontLab Monitoring System";
   mark = L"...";
   tree = t;
   node = this->tree->Items->Add(NULL, this->name);

   SysConfMgr = new TSysConfMgr();
   SysConfMgr->SetXMLDoc(xmlDoc);
   DBMgr = new TDataBaseMgr();
   BBFMgr = new TBBFMgr();

   sens_uid_max = 1;
   ports_list = NULL;
   sensors_list = NULL;
   data_list = NULL;
}

TGLSystem::~TGLSystem()
{

}

TGLPlace* TGLSystem::add_place(WideString nm)
{
	int nmb = place_list.count() + 1;
	pl = new TGLPlace(nm, NULL, place_list.count() + 1);

	//the place shall have unique name & number in the list
	if (place_list.add(pl) != 0)
	{
		delete pl;
		pl = NULL;
		//return -1;  //fail to add the place
	}
	else
	{
	//  create the place title in the browser & create tree node
		WideString ss;
		ss.printf(L"%d [", nmb);
		ss = ss+nm;
		ss = ss+L"]";
		TTreeNode* ndd = tree->Items->AddChild(node, ss); ndd->ImageIndex = 1; ndd->SelectedIndex = 1;

		pl->SetTree(tree);
		pl->SetNode(ndd);

		////// !!!

		//pl->add_port(L"COM_1");
		//pl->add_port(L"COM_2");
		//pl->add_port(L"COM_3");

		node->Expand(true);
		//return 0;
	}

	return pl;
}

TTreeNode* TGLSystem::get_node(void)
{
	return node;
}

void TGLSystem::View(BYTE state)
{
	node->ImageIndex = 0;
	node->SelectedIndex = 0;  node->SelectedIndex = 0;
}

/*
TGLPlace* TGLSystem::GetPlace(int n)
{
	TGLPlace* pl = NULL;

	pl = place_list.find(n);

	return pl;
}
*/

TObject* TGLSystem::GetBrowserElement(TTreeNode* nd, int* type)
{
	*type = OBJ_TYPE_NONE;
	TObject* res = NULL;

	cur_pl = NULL;
	cur_pr = NULL;
	cur_sn = NULL;

	if (nd == this->node)
	{
		*type = OBJ_TYPE_SYST;
		return  (TObject*)this;
	}
	else if ((res = (TObject*)place_list.find(nd)) != NULL)
	{
		*type = OBJ_TYPE_PLCE;
		cur_pl = (TGLPlace*)res;
		return res;
	}
	else
	{
		TGLPlace* pl = NULL;
		TGLPort* pr = NULL;
		TGLSensor* sn = NULL;

		for (auto itpl : place_list.m_list)
		{
			if ((res = (TObject*)itpl->port_list.find(nd)) != NULL)
			{
				*type = OBJ_TYPE_PORT;

				cur_pl = itpl;
				cur_pr = (TGLPort*) res;

				return res;
			}

			for (auto itpr : itpl->port_list.m_list)
			{
				if ((res = (TObject*)itpr->sensor_list.find(nd)) != NULL)
				{
					*type = OBJ_TYPE_SNSR;

					cur_pl = (TGLPlace*) itpl;
					cur_pr = (TGLPort*) itpr;
					cur_sn = (TGLSensor*) res;

					return res;
				}
			}

		}
	}

	return res;
}

void TGLSystem::ProcBrowser�lick(TTreeNode* nd, int* tp)
{
	int type = OBJ_TYPE_NONE;
	TObject* obj = NULL;
	WideString ss;

	cur_pl = NULL;
	cur_pr = NULL;
	cur_sn = NULL;
	 
	 obj = GetBrowserElement(nd, &type);
	 {

		switch (type)
		{
			case OBJ_TYPE_SYST:

				ss.printf(L"�������: %s", this->name.c_bstr());

			break;

			case OBJ_TYPE_PLCE:

				cur_pl = (TGLPlace*)obj;
				ss.printf(L"�����: %d %s", cur_pl->num, cur_pl->name.c_bstr());

			break;

			case OBJ_TYPE_PORT:

				cur_pr = (TGLPort*)obj;
				ss.printf(L"����: %d.%d %s", cur_pr->plnum, cur_pr->num, cur_pr->name.c_bstr());

			break;

			case OBJ_TYPE_SNSR:

				cur_sn = (TGLSensor*)obj;
				ss.printf(L"������: %d.%d.%d %s", cur_sn->plnum, cur_sn->prnum, cur_sn->num, cur_sn->name.c_bstr());

			break;

			default:;

		}

	 }

	*tp =  type;
	//ShowMessage(ss);
    g_ws_msg = ss;

}

TGLPort* TGLSystem::add_port(WideString nm)
{
   if (cur_pl != NULL)
   {
	   return cur_pl->add_port(nm);
   }

   return NULL;
}

TGLSensor* TGLSystem::add_sensor(WideString nm, int uid)
{
	TGLSensor* sn = NULL;

   if ((cur_pr != NULL) &&  (cur_pl != NULL))
   {
	   sn = cur_pr->add_sensor(nm, cur_pl->num, 0);
	   if (sn)
	   {
		   //sens_uid_max++;
		   return sn;
       }
   }

   return NULL;
}

int TGLSystem::SaveConf(void)
{
   TXMLDocument* XMLDoc = SysConfMgr->GetXMLDoc();

   if (XMLDoc != nullptr)
   {
		XMLDoc->ChildNodes->Clear();
   }

   XMLDoc->Options = XMLDoc->Options << doNodeAutoIndent;
   XMLDoc->Version = "1.0";
   XMLDoc->Encoding = "UTF-16"; // ������������� ��������� �� UTF-16

   _di_IXMLNode RootNode = XMLDoc->AddChild("system");
   RootNode->Attributes["name"] = name.c_bstr(); // ������������� �������

   int plid = 0;
   int prid = 0;
   int snid = 0;

   WideString sid ("");
   WideString spar ("");

   _di_IXMLNode PlacesListNode = RootNode->AddChild("objects_list");


	for (auto itpl : place_list.m_list)
	{
		//_di_IXMLNode ObjectNode = RootNode->AddChild("object");

		_di_IXMLNode ObjectNode = PlacesListNode->AddChild("object");

		sid.printf(L"%d", ++plid);
		ObjectNode->Attributes["ID"] = sid;
		ObjectNode->AddChild("name")->Text = itpl->name;

		prid = 0;

		_di_IXMLNode PortsListNode = ObjectNode->AddChild("ports_list");

		for (auto itpr : itpl->port_list.m_list)
		{
			//_di_IXMLNode PortNode = ObjectNode->AddChild("port");

			_di_IXMLNode PortNode = PortsListNode->AddChild("port");

			sid.printf(L"%d.%d", plid, ++prid);

			PortNode->Attributes["ID"] = sid;
			PortNode->AddChild("name")->Text = itpr->name;

			spar.printf(L"%d", itpr->get_sys_port_num());
			PortNode->AddChild("sys_port_num")->Text = spar;

			spar.printf(L"%d", itpr->get_baud());
			PortNode->AddChild("baud_rate")->Text = spar;

			spar.printf(L"%d", itpr->get_on());
			PortNode->AddChild("on")->Text = spar;

			PortNode->AddChild("mark")->Text = itpr->get_mark();

			spar.printf(L"%d", itpr->get_type());
			PortNode->AddChild("type")->Text = spar;

			spar.printf(L"%d", itpr->delay_set.tout_rd);
			PortNode->AddChild("timeout_read")->Text = spar;

			spar.printf(L"%d", itpr->delay_set.tout_wr);
			PortNode->AddChild("timeout_write")->Text = spar;

			spar.printf(L"%d", itpr->delay_set.delay_default);
			PortNode->AddChild("delay_default")->Text = spar;

			spar.printf(L"%d", itpr->delay_set.delay_cmd_exec);
			PortNode->AddChild("delay_cmd_exec")->Text = spar;

			snid = 0;

			_di_IXMLNode SensorsListNode = PortNode->AddChild("sensors_list");

			for (auto itsn : itpr->sensor_list.m_list)
			{
				//_di_IXMLNode SensorNode = PortNode->AddChild("sensor");

				_di_IXMLNode SensorNode = SensorsListNode->AddChild("sensor");

				sid.printf(L"%d.%d.%d", plid, prid, ++snid);
				SensorNode->Attributes["ID"] = sid;
				SensorNode->AddChild("name")->Text = itsn->GetName();
				WideString s("");

				s.printf(L"%03d", itsn->get_uid());
				SensorNode->AddChild("uid")->Text = s;

				s.printf(L"%03d", itsn->get_addr());
				SensorNode->AddChild("addr")->Text = s;

				s.printf(L"%d", itsn->get_on());
				SensorNode->AddChild("on")->Text = s;

				s.printf(L"%d", itsn->get_sensor_type());
				SensorNode->AddChild("sensor_type")->Text = s;

				s.printf(L"%d", itsn->get_protocol_type());
				SensorNode->AddChild("protocol_type")->Text = s;

				s.printf(L"%d", itsn->data_stream_rate);
				SensorNode->AddChild("data_stream_rate")->Text = s;

				SensorNode->AddChild("mark")->Text = itsn->get_mark();
				SensorNode->AddChild("name")->Text = itsn->get_name();


			}
		}

	}

   TCHAR* path = SysConfMgr->GetXMLDocPath();
   TCHAR pathb [1024];
   wcscpy(pathb, SysConfMgr->GetXMLDocPath());
   wcscat(pathb, L".back");

   ::DeleteFile(pathb);
   ::CopyFile(path, pathb, true);
   ::DeleteFile(path);

   Sleep(100);

   XMLDoc->SaveToFile(path);

   SysConfMgr->SaveCurSysName();

   console(L"�������", L"��������� ������� ������������");

   return 0;
}

int TGLSystem::LoadConf(void)
{
   tree->Visible = false;

   TXMLDocument* XMLDoc = SysConfMgr->GetXMLDoc();

   int plid = 0;
   int prid = 0;
   int snid = 0;

   WideString sid ("");

   cur_pl = NULL;
   cur_pr = NULL;
   cur_sn = NULL;

   m_port_proc_thread_list.clear();

   try
   {
	  TCHAR* path = SysConfMgr->GetXMLDocPath();
	  _di_IXMLDocument xmlDoc;

	try
	{
			xmlDoc = LoadXMLDocument(path);
	}
	catch (...)
	{
		 ShowMessage(L"������������ �� ������� � ����� ������� ������!");
		 return -1;
	}

	 _di_IXMLNode rootNode = xmlDoc->DocumentElement;

	 //no any tags for systyem level

	 _di_IXMLNode ObjectsListNode = rootNode->ChildNodes->FindNode("objects_list");

	 if (ObjectsListNode)
	 {
		for (int i = 0; i < ObjectsListNode->ChildNodes->Count; i++)
		{

			_di_IXMLNode PlaceNode = ObjectsListNode->ChildNodes->Nodes[i];
			_di_IXMLNode PlaceNameNode = PlaceNode->ChildNodes->FindNode("name");

			if (PlaceNameNode)
			{
				cur_pl = add_place (PlaceNameNode->Text);
			}
			else
			{
				//std::cout << "��� name �� ������ � ���� objectNode." << std::endl;
			}

			//here load other place params

			_di_IXMLNode PortsListNode = PlaceNode->ChildNodes->FindNode("ports_list");

			for (int i = 0; i < PortsListNode->ChildNodes->Count; i++)
			{
				_di_IXMLNode PortNode = PortsListNode->ChildNodes->Nodes[i];
				_di_IXMLNode PortNameNode = PortNode->ChildNodes->FindNode("name");

				if (PortNode)
				{
					cur_pr = add_port (PortNameNode->Text);

					TPortProcThread* ppt = new TPortProcThread(true, cur_pr);
					m_port_proc_thread_list.push_back(ppt);

					Sleep(1);
				}
				else
				{
				//std::cout << "��� name �� ������ � ���� objectNode." << std::endl;
				}

				_di_IXMLNode PorSysNumNode = PortNode->ChildNodes->FindNode("sys_port_num");
				if ( PorSysNumNode)
				{
					cur_pr->set_sys_port_num(PorSysNumNode->Text.ToInt());
				}

				_di_IXMLNode PorBaudNode = PortNode->ChildNodes->FindNode("baud_rate");
				if ( PorBaudNode)
				{
					cur_pr->set_baud((DWORD)PorBaudNode->Text.ToInt());
				}

				_di_IXMLNode PorOnNode = PortNode->ChildNodes->FindNode("on");
				if ( PorOnNode)
				{
					cur_pr->set_on(PorOnNode->Text.ToInt());
				}

				_di_IXMLNode PorMarkNode = PortNode->ChildNodes->FindNode("mark");
				if ( PorMarkNode)
				{
					cur_pr->set_mark(PorMarkNode->Text.c_str());
				}

				_di_IXMLNode PorTypeNode = PortNode->ChildNodes->FindNode("type");
				if ( PorTypeNode)
				{
					cur_pr->set_type(PorTypeNode->Text.ToInt());
				}

				_di_IXMLNode toutrdNode = PortNode->ChildNodes->FindNode("timeout_read");
				if ( toutrdNode)
				{
					cur_pr->delay_set.tout_rd = toutrdNode->Text.ToInt();
				}


				_di_IXMLNode touwrNode = PortNode->ChildNodes->FindNode("timeout_write");
				if ( touwrNode)
				{
					cur_pr->delay_set.tout_wr = touwrNode->Text.ToInt();
				}

				_di_IXMLNode dldfNode = PortNode->ChildNodes->FindNode("delay_default");
				if ( dldfNode)
				{
					cur_pr->delay_set.delay_default = dldfNode->Text.ToInt();
				}

				_di_IXMLNode dlexNode = PortNode->ChildNodes->FindNode("delay_cmd_exec");
				if (dlexNode)
				{
					cur_pr->delay_set.delay_cmd_exec = dlexNode->Text.ToInt();
				}

				cur_pr->set_com();
				//load other port attr here

				_di_IXMLNode SensorsListNode = PortNode->ChildNodes->FindNode("sensors_list");

				for (int i = 0; i < SensorsListNode->ChildNodes->Count; i++)
				{
					_di_IXMLNode SensorNode = SensorsListNode->ChildNodes->Nodes[i];
					_di_IXMLNode SensorNameNode = SensorNode->ChildNodes->FindNode("name");

					if (SensorNameNode)
					{
						cur_sn = add_sensor (SensorNameNode->Text, 0);
					}
					else
					{

					}

					_di_IXMLNode uidNode = SensorNode->ChildNodes->FindNode("uid");

					if (uidNode)
					{
						cur_sn->set_uid (StrToIntDef(uidNode->Text, 0));
						if (sens_uid_max <= cur_sn->get_uid())
						{
							sens_uid_max = cur_sn->get_uid() + 1;
						}
					}
					else
					{

					}

					_di_IXMLNode addrNode = SensorNode->ChildNodes->FindNode("addr");
					if (addrNode)
					{
						cur_sn->set_addr (StrToIntDef(addrNode->Text, 0));
					}


					_di_IXMLNode onNode = SensorNode->ChildNodes->FindNode("on");
					if (onNode)
					{
						cur_sn->set_on (StrToIntDef(onNode->Text, 0));
					}

					_di_IXMLNode markNode = SensorNode->ChildNodes->FindNode("mark");
					if (markNode)
					{
						cur_sn->set_mark (markNode->Text.c_str());
					}

					_di_IXMLNode nameNode = SensorNode->ChildNodes->FindNode("name");
					if (nameNode)
					{
						cur_sn->set_name (nameNode->Text.c_str());
					}

					_di_IXMLNode sensor_typeNode = SensorNode->ChildNodes->FindNode("sensor_type");
					if (sensor_typeNode)
					{
						cur_sn->set_sensor_type (sensor_typeNode->Text.ToInt());
					}

					_di_IXMLNode protocol_typeNode = SensorNode->ChildNodes->FindNode("protocol_type");
					if (protocol_typeNode)
					{
						cur_sn->set_protocol_type (protocol_typeNode->Text.ToInt());
					}

					_di_IXMLNode streamrateeNode = SensorNode->ChildNodes->FindNode("data_stream_rate");
					if (streamrateeNode)
					{
						cur_sn->data_stream_rate = streamrateeNode->Text.ToInt();
					}

                    cur_sn->init_sensor();
				}


			}
		}


		Sleep(1);
	 }


	 tree->Visible = true;

	 ReDraw();


	 return 0;//

	 int tmp = rootNode->ChildNodes->Count;

	 for (int i = 0; i < rootNode->ChildNodes->Count; i++)
	 {
		_di_IXMLNode objectNode = rootNode->ChildNodes->Nodes[i];
		_di_IXMLNode nameNode = objectNode->ChildNodes->FindNode("name");
		if (nameNode)
		{
			cur_pl = add_place (nameNode->Text);
		}
		else
		{
			//std::cout << "��� name �� ������ � ���� objectNode." << std::endl;
		}

		tmp = objectNode->ChildNodes->Count;

		for (int i = 0; i < objectNode->ChildNodes->Count; i++)
		{
			_di_IXMLNode portNode = objectNode->ChildNodes->Nodes[i];
			_di_IXMLNode nameNode = portNode->ChildNodes->FindNode("name");
			if (nameNode)
			{
				cur_pr = add_port (nameNode->Text);

				TPortProcThread* ppt = new TPortProcThread(true, cur_pr);
				m_port_proc_thread_list.push_back(ppt);

				Sleep(1);
			}
			else
			{
			//std::cout << "��� name �� ������ � ���� objectNode." << std::endl;
			}

			tmp = portNode->ChildNodes->Count;

			for (int i = 0; i < portNode->ChildNodes->Count; i++)
			{
				TGLSensor* sn = NULL;

				_di_IXMLNode sensorNode = portNode->ChildNodes->Nodes[i];
				_di_IXMLNode nameNode = sensorNode->ChildNodes->FindNode("name");
				_di_IXMLNode uidNode = sensorNode->ChildNodes->FindNode("uid");

				if (sensorNode)
				{
                   for (int i = 0; i < portNode->ChildNodes->Count; i++)
				   {

                   }
				}
				else
				{

                }

				if (nameNode)
				{
					sn = add_sensor (nameNode->Text, 0);
				}
				else
				{

				}

				if (uidNode)
				{
					sn->set_uid (StrToIntDef(uidNode->Text, 0));
					if (sens_uid_max <= sn->get_uid())
					{
						sens_uid_max = sn->get_uid() + 1;
					}
				}
				else
				{

				}
				//_di_IXMLNode baudNode = sensorNode->ChildNodes->FindNode("baud");
				//if (baudNode)
				//{
				//	sn->SetBaud(StrToIntDef(baudNode->Text, 0));
				//}
				//else
				//{
				//std::cout << "��� name �� ������ � ���� objectNode." << std::endl;
				//}

					   /*
				//try
				//{
					_di_IXMLNode uidNode = sensorNode->ChildNodes->FindNode("uid");
					if (uidNode)
					{
						sn->set_uid(StrToIntDef(uidNode->Text, 0));

						if (sn->get_uid() > sens_uid_max) sens_uid_max = sn->get_uid() + 1;
					}
					else
					{
						sn->set_uid(0);
					//std::cout << "��� name �� ������ � ���� objectNode." << std::endl;
					}   */
				//}
				//catch (...)
				//{

				//}
			}

		}
	 }

	 //bool rflag = false;

	 for (auto ipl: place_list.m_list)
	 {
		for (auto ipr: ipl->port_list.m_list)
		{
			for (auto isn: ipr->sensor_list.m_list)
			{
			   if (true)
			   {
				  if (isn->get_uid() == 0)
				  {
					 isn->set_uid(sens_uid_max++);
				  }
			   }
			}
		}
	 }

	 //if (rflag)

	 tree->Visible = true;

	 ReDraw();

	 Sleep(1);
   }
   catch(...)
   {
	   //ShowMessage(L"������������ �� ������� � ����� ������� ������!");
   }



   return 0;
}

TGLSensor* TGLSystem::GetCurSn(void)
{
	return cur_sn;
}

int TGLSystem::ReDraw(void)
{
	 for (auto ipl: place_list.m_list)
	 {
		for (auto ipr: ipl->port_list.m_list)
		{
			for (auto isn: ipr->sensor_list.m_list)
			{
			   if (true)
			   {
				  if (isn->get_uid() == 0)
				  {
					 isn->set_uid(sens_uid_max++);
				  }
			   }
			}
		}
	 }

	tree->Visible = false;

	tree->Items->Clear();

	node = this->tree->Items->Add(NULL, this->name);

	int plnum = 0;
	int prnum = 0;
	int snnum = 0;

	for (auto itpl : place_list.m_list)
	{
		itpl->ReDraw(node, ++plnum);

		prnum = 0;

		for (auto itpr : itpl->port_list.m_list)
		{
			itpr->ReDraw(itpl->GetNode(), plnum, ++prnum);

			snnum = 0;

			for (auto itsn : itpr->sensor_list.m_list)
			{
				itsn->ReDraw(itpr->GetNode(), plnum, prnum, ++snnum);
			}


		}
	}

	tree->Visible = true;

	return 0;
}

int TGLSystem::delete_sensor(void)
{

	cur_pr->sensor_list.remove(cur_sn->name);

	//delete cur_sn;

	ReDraw();

	return 0;
}

int TGLSystem::delete_port(void)
{

	cur_pl->port_list.remove(cur_pr->name);

	//delete cur_pr;

	ReDraw();

	return 0;
}

int TGLSystem::delete_place(void)
{

	place_list.remove(cur_pl->name);

	//for (auto itpr : cur_pl->port_list.m_list)
	//{
	//	delete itpr;
	//}

	//delete cur_pl;

	ReDraw();

	return 0;
}

TCHAR* TGLSystem::GetConfPath(void)
{
	return SysConfMgr->GetXMLDocPath();
}

int TGLSystem::CreateConf(TSaveDialog* dlg)
{
	if (SysConfMgr->CreateConf(dlg) != 0) return -1;

	Clear();
	SaveConf();
	LoadConf();
	//ReDraw();

	WideString s = L"������� ������������ ";

	s = s + SysConfMgr->GetCurConfPath();

	console(L"�������", s);

	return 0;
}

int TGLSystem::OpenConf(TOpenDialog* dlg)
{
	if (SysConfMgr->OpenConf(dlg) != 0) return -1;

	Clear();
	LoadConf();
	//ReDraw();

	WideString s = L"��������� ������������ ";

	s = s + SysConfMgr->GetCurConfPath();

	console(L"�������", s);

   return 0;
}

int TGLSystem::Clear(void)
{

   place_list.m_list.clear();

   tree->Items->Clear();
   node = this->tree->Items->Add(NULL, this->name);

   return 0;
}

int TGLSystem::SaveConf(TSaveDialog* dlg)
{
	if (SysConfMgr->SaveConf(dlg) != 0) return -1;

	Clear();
	LoadConf();
	//ReDraw();
	WideString s = L"��������� ������������ ";

	s = s + SysConfMgr->GetCurConfPath();

	console(L"�������", s);

   return 0;
}

void TGLSystem::run_engine_start ()
{
	for (auto ppt : m_port_proc_thread_list)
	{
	   ppt->start();
	}
}

void TGLSystem::run_engine_suspend ()
{
	for (auto ppt : m_port_proc_thread_list)
	{
	   ppt->suspend();
	}
}

void TGLSystem::run_engine_resume ()
{
	for (auto ppt : m_port_proc_thread_list)
	{
	   ppt->resume();
	}
}

void TGLSystem::set_console(TListBox* list)
{
	list_console = list;
}

void TGLSystem::console(WideString  obj, WideString  msg)
{
   WideString s = FormatDateTime(L"[dd-mm-yyyy hh:nn:ss] ", Now());
   s = s + L"[" + obj + L"] ";
   s = s + msg;

   list_console->Items->Add(s);

   FILE* f = _wfopen(SysConfMgr->GetLogFilePath(), L"rb");

   if (f == NULL)
   {
		BYTE sgn [] = {0xff, 0xfe};

		fclose(f);
		FILE* f = _wfopen(SysConfMgr->GetLogFilePath(), L"wb");
		fwrite(sgn, 2, 1, f);
		fclose(f);

   }

   f = _wfopen(SysConfMgr->GetLogFilePath(), L"ab");

   s = s + L"\r\n";

   fwrite(s.c_bstr(), wcslen(s.c_bstr())*2, 1, f);

   fclose(f);
}

int TGLSystem::open_DB(void)
{
   if (DBMgr->is_base_open() == false)
   {
		return DBMgr->open_base(SysConfMgr->GetCurBasePath());
   }

   return 0;
}

double x = 23.456;
double y = 569.567;

#pragma synchronous=OFF
#pragma journal_mode=WAL

int TGLSystem::store_sensor_data(TDateTime t)
{

	DBMgr->BEGIN_TRANSACTION();

	for (auto itpl : place_list.m_list)
	{
		for (auto itpr : itpl->port_list.m_list)
		{
			for (auto itsn : itpr->sensor_list.m_list)
			{
               itsn->subst(x,y);

			   DBMgr->save_sensor_data_s(t, itsn);

			   x += 1.23; y += 4.56;
			}
		}
	}

	DBMgr->END_TRANSACTION();
}

std::list<dt_sensor_data_record_s> TGLSystem::read_sensor_data_s(TGLSensor* sn, TDateTime t1, TDateTime t2)
{
   return DBMgr->read_sensor_data_s(sn, t1, t2);
}

TGLSensor* TGLSystem::get_cur_sensor(void)
{
	return cur_sn;
}

int TGLSystem::get_sens_uid_max(void)
{
	return sens_uid_max;
}

int TGLSystem::bbf_save_sensor_data_s(TDateTime t, TGLSensor* sn)
{

	BBFMgr->save_sensor_data_s(SysConfMgr->GetCurBasePath(), t, sn);

	return 0;
}

int TGLSystem::bbf_store_sensor_data(TDateTime t)
{

	for (auto itpl : place_list.m_list)
	{
		for (auto itpr : itpl->port_list.m_list)
		{
			for (auto itsn : itpr->sensor_list.m_list)
			{
               itsn->subst(x,y);

			   bbf_save_sensor_data_s(t, itsn);

			   x += 1.23; y += 4.56;
			}
		}
	}


}

std::list<dt_sensor_data_record_s> TGLSystem::bbf_read_sensor_data_s(TGLSensor* sn, TDateTime t1, TDateTime t2)
{
   return BBFMgr->read_sensor_data_s(SysConfMgr->GetCurBasePath(), sn, t1, t2);
}

TGLPort* TGLSystem::GetCurPr(void)
{
	return cur_pr;
}

void TGLSystem::cycle(void)
{
	for (auto itpl: place_list.m_list)
	{
		  for (auto itpr: itpl->port_list.m_list)
		  {
			 itpr->start_cycle();
			 itpr->cycle();
		  }
	}

	update_data(g_monitor_second_timer);
}

void TGLSystem::view_ports_status(TListView* list)
{
   if (ports_list)
   {
        return;
   }

   ports_list = list;

   list->Items->Clear();
   list->Columns->Clear();

   TGLPort* pr = NULL;
   WideString s;
   TListColumn* col;
   TListItem *item;

   col = list->Columns->Add();
   col->Caption = L"����";
   col->Width = 100;

   col = list->Columns->Add();
   col->Caption = L"ID";
   col->Width = 50;

   col = list->Columns->Add();
   col->Caption = L"������";
   col->Width = 150;

   col = list->Columns->Add();
   col->Caption = L"Err tou";
   col->Width = 100;

   col = list->Columns->Add();
   col->Caption = L"Err crc";
   col->Width = 100;

   col = list->Columns->Add();
   col->Caption = L"���� Tx";
   col->Width = 100;

   col = list->Columns->Add();
   col->Caption = L"���� Rx";
   col->Width = 100;

   col = list->Columns->Add();
   col->Caption = L"T cur, ms";
   col->Width = 100;

   col = list->Columns->Add();
   col->Caption = L"T mid, ms";
   col->Width = 100;

   col = list->Columns->Add();
   col->Caption = L"Info";
   col->Width = 1000;

   for (auto itpl : place_list.m_list)
   {

	  for (auto itpr : itpl->port_list.m_list)
	  {
		  prt_st* st = itpr->get_state();

		  item = list->Items->Add();
		  itpr->set_list_item(item);
		  item->Caption = itpr->name;

		  item->SubItems->Add(itpr->get_str_ID());

		  if (itpr->get_on() == false)
		  {
			 s = L"��������";
		  }
		  else if (itpr->get_is_open_in_cycle())
		  {
			 s = L"������";
		  }
		  else
		  {
			s = L"�� ��������";
		  }

		  item->SubItems->Add(s);

		  s.printf(L"%d", st->err_tou);
		  item->SubItems->Add(s);

		  s.printf(L"%d", st->err_crc);
		  item->SubItems->Add(s);

		  s.printf(L"%d", st->bytes_tx);
		  item->SubItems->Add(s);

		  s.printf(L"%d", st->bytes_rx);
		  item->SubItems->Add(s);

		  s.printf(L"%.2f", st->t_req);
		  item->SubItems->Add(s);

		   s.printf(L"%.2f", st->t_req_mid);
		  item->SubItems->Add(s);

		  item->SubItems->Add(itpr->get_mark());
	  }


   }
}

void TGLSystem::update_view_ports_status()
{
   if (ports_list == NULL)
   {
		return;
   }

   int itm_idx = 0;
   int sub_idx = 0;
   WideString s("");

   TListView* list = ports_list;
   TListItem *item;

   for (auto itpl : place_list.m_list)
   {
	  for (auto itpr : itpl->port_list.m_list)
	  {
		  prt_st* st = itpr->get_state();

		  item = itpr->get_list_item();

		  if (itpr->get_on() == false)
		  {
			 s = L"��������";
		  }
		  else if (itpr->get_is_open_in_cycle())
		  {
			 s = L"������";
		  }
		  else
		  {
			s = L"�� ��������";
		  }

		  sub_idx = 1;

		  item->SubItems->Strings[sub_idx] = s; sub_idx++;

		  s.printf(L"%d", st->err_tou);
		  item->SubItems->Strings[sub_idx] = s; sub_idx++;

		  s.printf(L"%d", st->err_crc);
		  item->SubItems->Strings[sub_idx] = s; sub_idx++;

		  s.printf(L"%d", st->bytes_tx);
		  item->SubItems->Strings[sub_idx] = s; sub_idx++;

		  s.printf(L"%d", st->bytes_rx);
		  item->SubItems->Strings[sub_idx] = s; sub_idx++;

		  s.printf(L"%.2f", st->t_req);
		  item->SubItems->Strings[sub_idx] = s; sub_idx++;

		  s.printf(L"%.2f", st->t_req_mid);
		  item->SubItems->Strings[sub_idx] = s; sub_idx++;
	  }
   }

}

void TGLSystem::view_sensors_status(TListView* list)
{
   if (sensors_list)
   {
        return;
   }

   sensors_list = list;

   list->Items->Clear();
   list->Columns->Clear();

   TGLSensor* sn = NULL;
   WideString s;
   TListColumn* col;
   TListItem *item;

   col = list->Columns->Add();
   col->Caption = L"������";
   col->Width = 100;

   col = list->Columns->Add();
   col->Caption = L"ID";
   col->Width = 50;

   col = list->Columns->Add();
   col->Caption = L"UID";
   col->Width = 50;

   col = list->Columns->Add();
   col->Caption = L"������";
   col->Width = 150;

   col = list->Columns->Add();
   col->Caption = L"Err tou";
   col->Width = 100;

   col = list->Columns->Add();
   col->Caption = L"Err crc";
   col->Width = 100;

   col = list->Columns->Add();
   col->Caption = L"T cur, ms";
   col->Width = 100;

   col = list->Columns->Add();
   col->Caption = L"T mid, ms";
   col->Width = 100;

   col = list->Columns->Add();
   col->Caption = L"Info";
   col->Width = 1000;


   for (auto itpl : place_list.m_list)
   {

	  for (auto itpr : itpl->port_list.m_list)
	  {

		  for (auto itsn : itpr->sensor_list.m_list)
		  {
			  sns_st* st = itsn->get_sn_state();

			  item = list->Items->Add();
			  itsn->set_list_item(item);
			  item->Caption = itsn->name;

			  item->SubItems->Add(itsn->get_str_ID());

			  s.printf(L"%03d", itsn->get_uid());
			  item->SubItems->Add(s);

			  if (itsn->get_on() != 1)
			  {
				 s = L"��������";
			  }
			  else
			  {
				 s = L"�������";
			  }

			  item->SubItems->Add(s);

			  s.printf(L"%d", st->err_tou);
			  item->SubItems->Add(s);

			  s.printf(L"%d", st->err_crc);
			  item->SubItems->Add(s);

			  s.printf(L"%.2f", st->t_req);
			  item->SubItems->Add(s);

			   s.printf(L"%.2f", st->t_req_mid);
			  item->SubItems->Add(s);

			  item->SubItems->Add(itsn->get_mark());
		  }

	  }


   }
}

void TGLSystem::update_view_sensors_status()
{
   if (sensors_list == NULL)
   {
		return;
   }

   int itm_idx = 0;
   int sub_idx = 0;
   WideString s("");

   TListView* list = sensors_list;
   TListItem *item;

   for (auto itpl : place_list.m_list)
   {
	  for (auto itpr : itpl->port_list.m_list)
	  {

		  for (auto itsn : itpr->sensor_list.m_list)
		  {
			  sns_st* st = itsn->get_sn_state();

			  item = itsn->get_list_item();

			  sub_idx = 2;

			  if (itsn->get_on() != 1)
			  {
				 s = L"��������";
			  }
			  else
			  {
				 s = L"�������";
			  }

			  if (item == NULL) {

                continue;
			  }

			  item->SubItems->Strings[sub_idx] = s; sub_idx++;

			  s.printf(L"%d", st->err_tou);
			  item->SubItems->Strings[sub_idx] = s; sub_idx++;

			  s.printf(L"%d", st->err_crc);
			  item->SubItems->Strings[sub_idx] = s; sub_idx++;

			  s.printf(L"%.2f", st->t_req);
			  item->SubItems->Strings[sub_idx] = s; sub_idx++;

			  s.printf(L"%.2f", st->t_req_mid);
			  item->SubItems->Strings[sub_idx] = s; sub_idx++;

		  }
	  }
   }

}

void TGLSystem::update_data(TDateTime t)
{
	for (auto itpl : place_list.m_list)
	{
	  for (auto itpr : itpl->port_list.m_list)
	  {
		  for (auto itsn : itpr->sensor_list.m_list)
		  {
			  for (auto itds : itsn->data_stream_list)
			  {
					if (itpr->get_on()&&itsn->get_on())
					{
						itds->update(t);
					}
			  }
          }
	  }
	}

}

void TGLSystem::view_data_status(TListView* list)
{
   if (data_list)
   {
		return;
   }

   data_list = list;

   list->Items->Clear();
   list->Columns->Clear();

   TGLSensor* sn = NULL;
   WideString s;
   TListColumn* col;
   TListItem *item;

   col = list->Columns->Add();
   col->Caption = L"������";
   col->Width = 200;

   col = list->Columns->Add();
   col->Caption = L"������";
   col->Width = 150;

   col = list->Columns->Add();
   col->Caption = L"raw data";
   col->Width = 100;

   col = list->Columns->Add();
   col->Caption = L"calc data";
   col->Width = 100;

   col = list->Columns->Add();
   col->Caption = L"�������";
   col->Width = 100;

   col = list->Columns->Add();
   col->Caption = L"Timestamp";
   col->Width = 200;

   col = list->Columns->Add();
   col->Caption = L"Info";
   col->Width = 1000;


   for (auto itpl : place_list.m_list)
   {
	  for (auto itpr : itpl->port_list.m_list)
	  {
		  for (auto itsn : itpr->sensor_list.m_list)
		  {

			  itsn->data_stream_setup();

			  for (auto itds : itsn->data_stream_list)
			  {
				  item = list->Items->Add();
				  itds->item = item;
				  item->Caption = itds->name;

				  if (itsn->get_on()&&itpr->get_on())
				  {
						s.printf(L"Run");
				  }
				  else
				  {
					   s.printf(L"Suspended");
				  }

				  item->SubItems->Add(s);

				  s.printf(L"%.02f", *itds->raw);
				  item->SubItems->Add(s);

				  if (itds->val != DATA_LOST_DBL_CONSTANT) s.printf(L"%.02f", itds->val);
				  else s = L"no data";
				  item->SubItems->Add(s);

				  item->SubItems->Add(itds->units);

				  item->SubItems->Add(L"");

				  item->SubItems->Add(itds->mark);
			  }
		  }
	  }
   }
}

void TGLSystem::update_view_data_status()
{
   if (data_list == NULL)
   {
		return;
   }

   int itm_idx = 0;
   int sub_idx = 0;
   WideString s("");

   TListView* list = data_list;

   for (auto itpl : place_list.m_list)
   {
	  for (auto itpr : itpl->port_list.m_list)
	  {
		  for (auto itsn : itpr->sensor_list.m_list)
		  {
				if (itsn->refresh_data_flag == true)  {

				for (auto itds : itsn->data_stream_list)
				{

				  int idx = 0;
				  TListItem *item = itds->item;


				  if (itsn->get_on()&&itpr->get_on())
				  {
						s.printf(L"Run");
				  }
				  else
				  {
					   s.printf(L"Suspended");
				  }

                  item->SubItems->Strings[idx] = s; idx++;

				  s.printf(L"%.02f", *itds->raw);
				  item->SubItems->Strings[idx] = s; idx++;

				  if (itds->val != DATA_LOST_DBL_CONSTANT) s.printf(L"%.02f", itds->val);
				  else s = L"no data";
				  item->SubItems->Strings[idx] = s; idx++;

				  item->SubItems->Strings[idx] = itds->units; idx++;

				  s = FormatDateTime(L"yyyy-mm-dd hh:mm:ss:zzz",itds->last_update_timestamp);
				  item->SubItems->Strings[idx] = s; idx++;

				}  }

		  }
	  }
   }

}


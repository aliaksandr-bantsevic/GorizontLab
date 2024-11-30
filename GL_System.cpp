//---------------------------------------------------------------------------

#pragma hdrstop

#include "GL_System.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <list>
#include "GL_Place.h"
#include "GL_Port.h"
#include "GL_Sensor.h"
//---------------------------------------------------------------------------

TGLSystem::TGLSystem()
{
   name = L"GorizontLab Monitoring System";
   mark = L"...";
   node = NULL;
}

TGLSystem::TGLSystem(TTreeView* t)
{
   name = L"GorizontLab Monitoring System";
   mark = L"...";
   tree = t;
   node = this->tree->Items->Add(NULL, this->name);
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

	if (nd == this->node)
	{
		*type = OBJ_TYPE_SYST;
		return  (TObject*)this;
	}
	else if ((res = (TObject*)place_list.find(nd)) != NULL)
	{
		*type = OBJ_TYPE_PLCE;
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
				return res;
			}

			for (auto itpr : itpl->port_list.m_list)
			{
				if ((res = (TObject*)itpr->sensor_list.find(nd)) != NULL)
				{
					*type = OBJ_TYPE_SNSR;
					return res;
				}
			}

		}
	}

	return res;
}

void TGLSystem::ProcBrowserÑlick(TTreeNode* nd, int* tp)
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

				ss.printf(L"ÑÈÑÒÅÌÀ: %s", this->name.c_bstr());

			break;

			case OBJ_TYPE_PLCE:

				cur_pl = (TGLPlace*)obj;
				ss.printf(L"ÌÅÑÒÎ: %d %s", cur_pl->num, cur_pl->name.c_bstr());

			break;

			case OBJ_TYPE_PORT:

				cur_pr = (TGLPort*)obj;
				ss.printf(L"ÏÎÐÒ: %d.%d %s", cur_pr->plnum, cur_pr->num, cur_pr->name.c_bstr());

			break;

			case OBJ_TYPE_SNSR:

				cur_sn = (TGLSensor*)obj;
				ss.printf(L"ÄÀÒ×ÈÊ: %d.%d.%d %s", cur_sn->plnum, cur_sn->prnum, cur_sn->num, cur_sn->name.c_bstr());

			break;

			default:;

		}

	 }

	*tp =  type;
	//ShowMessage(ss);
    g_ws_msg = ss;

}

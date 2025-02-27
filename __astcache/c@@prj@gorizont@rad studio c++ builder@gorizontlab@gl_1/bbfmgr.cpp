﻿//---------------------------------------------------------------------------

#pragma hdrstop

#include "BBFMgr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TBBFMgr::TBBFMgr()
{

}
TBBFMgr::~TBBFMgr()
{

}


int TBBFMgr::save_sensor_data_s(TCHAR* bdir, TDateTime t, TGLSensor* sn)
{

	TCHAR tcdir[1024];

	unsigned short y,m,d;
	_stprintf(tcdir, L"%sSENSOR_DATA_S\\", bdir);
	::CreateDirectoryW(tcdir,0);

	t.DecodeDate(&y, &m, &d);

	_stprintf(tcdir, L"%sSENSOR_DATA_S\\%03d\\", bdir, sn->get_uid());
	::CreateDirectoryW(tcdir,0);

	_stprintf(tcdir, L"%sSENSOR_DATA_S\\%03d\\SENSOR_DATA_S_%03d_%04d_%02d.bds", bdir,sn->get_uid(), sn->get_uid(), y, m);

	FILE* f = _tfopen(tcdir, L"ab");

	if (f)
	{
	   dt_sensor_data_record_s rec;
	   rec.t = t;
	   rec.x = sn->get_rawX();
	   rec.y = sn->get_rawY();

	   fwrite(&rec, 1, sizeof(rec), f);
	}

	fclose (f);

	return 0;
}

std::list<dt_sensor_data_record_s> TBBFMgr::read_sensor_data_s(TCHAR* bdir, TGLSensor* sn, TDateTime t1, TDateTime t2)
{
   std::list<dt_sensor_data_record_s> list;
   list.clear();

   	TCHAR tcdir[1024];

	unsigned short y,m,d;
	unsigned short y1,m1,d1;
	unsigned short y2,m2,d2;

	_stprintf(tcdir, L"%sSENSOR_DATA_S\\", bdir);
	::CreateDirectoryW(tcdir,0);

	t1.DecodeDate(&y1, &m1, &d1);
	t1.DecodeDate(&y2, &m2, &d2);

	_stprintf(tcdir, L"%sSENSOR_DATA_S\\%03d\\", bdir, sn->get_uid());
	::CreateDirectoryW(tcdir,0);

	for (y = y1; y <= y2; y++)
	{
		int mm = 1;
		int mmm = 12;

		if (y == y1)
		{
			mm = m1;
		}

		if (y == y2)
		{
			mmm = m1;
		}

		for (m = mm; (m <= mmm) ; m++)
		{

			_stprintf(tcdir, L"%sSENSOR_DATA_S\\%03d\\SENSOR_DATA_S_%03d_%04d_%02d.bds", bdir,sn->get_uid(), sn->get_uid(), y, m);

			FILE* f = _tfopen(tcdir, L"rb");

			if (f)
			{
			   dt_sensor_data_record_s rec;

			   //while (fread(&rec, 1, sizeof(rec), f))
			   while(1)
			   {
					int res = fread(&rec, 1, sizeof(rec), f);
					if (res)
					{
						if ((rec.t >= t1) && (rec.t <= t2))
						{
						   list.push_back(rec);
                        }
					}
					else
					{
						break;
					}
			   }

			}

			fclose (f);

			if ((y == y2)&&(m == m2))
			{
				break;
			}
		}
	}

   return list;
}

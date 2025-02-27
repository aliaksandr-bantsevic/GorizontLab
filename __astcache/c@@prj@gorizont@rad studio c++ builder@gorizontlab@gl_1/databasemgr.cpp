﻿//---------------------------------------------------------------------------

#pragma hdrstop

#include "DataBaseMgr.h"
#include <windows.h>
#include <System.SysUtils.hpp>
#include <string.h>
#include <iostream>
#include <cstdio> // Включение заголовочного файла для sprintf

//---------------------------------------------------------------------------
#pragma package(smart_init)


TDataBaseMgr::TDataBaseMgr()
{
   l_sql_db = NULL;
   is_open = false;
}

TDataBaseMgr::~TDataBaseMgr()
{
   sqlite3_close(l_sql_db);
}

int TDataBaseMgr::open_base(TCHAR* path)
{
   int res = -1;

   TCHAR pb[1024];

   wcscpy(pb, path);
   wcscat(pb, L"data_base.db");

   char cb [1024];

   for (int i = 0; i <wcslen(pb); i++)
   {
		cb[i] = (char)pb[i]; cb[i+1] = 0;
   }

   res = sqlite3_open(cb, &l_sql_db);

   if (res == SQLITE_OK) is_open = true;

   return res;
}

bool TDataBaseMgr::is_base_open(void)
{
   return is_open;
}

int TDataBaseMgr::save_sensor_data_s(TDateTime t, TGLSensor* sn)
{
	int res = 0;
	char* zErrMsg = 0;

	TDateTime tt = t;
	AnsiString stime("");
	stime = FormatDateTime(L"yyyy-mm-dd hh:nn:ss:zzz", tt); //ISO 8601.

	if (is_open)
	{
		String ssname("");
		ssname.printf(L" DATA_SENSOR_%d_%d_%d_S ", sn->plnum, sn->prnum, sn->num);

		//stime = FormatDateTime(L"yyyy-mm-dd hh:nn:ss:zzz", tt); //ISO 8601.

		char sql[1024];

		//String ssql("");

		/*ssql.printf(L"CREATE TABLE IF NOT EXISTS %s ("  \
					"ID INTEGER PRIMARY KEY AUTOINCREMENT," \
					"STIME          TEXT    NOT NULL," \
					"TIME           REAL," \
					"X              REAL," \
					"Y              REAL", sname); */

	   // Создаем строку SQL-запроса
		std::wstring sname = ssname.c_str();  // Пример имени таблицы
		std::wstring ssql = L"CREATE TABLE IF NOT EXISTS " + sname +
					   //L" (ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
					   //L"(STIME TEXT NOT NULL, " +
					   L"(TIME REAL, " +
                       L"X REAL, " +
					   L"Y REAL);";

	   //sprintf(sql, "%S", ssql.c_str());

       std::wcstombs(sql, ssql.c_str(), 1024);

	   res = sqlite3_exec(l_sql_db, sql, NULL, 0, &zErrMsg);
	   sqlite3_free(zErrMsg);
	   // Создание строки SQL-запроса
	   int id = 1;

	   ssql = L"INSERT INTO " + sname +
					   //L" (ID, STIME, TIME, X, Y) VALUES (" +
					   //std::to_wstring(id) + L", ";// +
					   //L" (STIME, TIME, X, Y) VALUES (";
					   L" (TIME, X, Y) VALUES (";
					   char cc[1024]; String ss("");
					   //sprintf(cc, "\'%s\', ", stime.c_str());
					   //ss = (String)cc;
					   //ssql += ss;
					   sprintf(cc, "%f, ", tt);
					   ss = (String)cc;
					   ssql += ss;
					   sprintf(cc, "%f, ", sn->get_rawX());
					   ss = (String)cc;
					   ssql += ss;
					   sprintf(cc, "%f)", sn->get_rawY());
					   ss = (String)cc;
					   ssql += ss;
					   /*
					   String ss;
					   ss.printf(L"%S ', ", sname); ssql += ss;
					   ss.printf(L"%.f, ",time); ssql += ss;
					   ss.printf(L"%.f, ",sn->get_rawX()); ssql += ss;
					   ss.printf(L"%.f);",sn->get_rawY()); ssql += ss;
					   */
					   //stime + L"', " +
					   //std::to_wstring(time) + L", " +
					   //std::to_wstring(sn->get_rawX()) + L", " +
					   //std::to_wstring(sn->get_rawY()) + L");";
	// Преобразование из std::wstring в char[]
	std::wcstombs(sql, ssql.c_str(), 1024);
	// Выполнение SQL-запроса
	res = sqlite3_exec(l_sql_db, sql, NULL, 0, &zErrMsg);
    sqlite3_free(zErrMsg);

	   return 0;
	}
	else
	{
		return -2;
    }
}

int TDataBaseMgr::BEGIN_TRANSACTION (void)
{
	char* zErrMsg = 0;
	return sqlite3_exec(l_sql_db, "BEGIN TRANSACTION;", NULL, NULL, &zErrMsg);
}

int TDataBaseMgr::END_TRANSACTION (void)
{
	char* zErrMsg = 0;
	return sqlite3_exec(l_sql_db, "END TRANSACTION;", NULL, NULL, &zErrMsg);
}

std::list<dt_sensor_data_record_s> TDataBaseMgr::read_sensor_data_s(TGLSensor* sn, TDateTime t1, TDateTime t2)
{
   int res = 0;
   dt_sensor_data_record_s record;
   char sql [1024];
   std::list<dt_sensor_data_record_s> list;
   list.clear();

   if (is_base_open() == true)
   {
	   sprintf(sql,"SELECT * FROM DATA_SENSOR_%d_%d_%d_S WHERE TIME BETWEEN %f AND %f", sn->plnum, sn->prnum, sn->num, t1, t2);

	   // Выполнение SQL-запроса
		sqlite3_stmt* stmt;
		res = sqlite3_prepare_v2(l_sql_db, sql, -1, &stmt, 0);
		if (res != SQLITE_OK)
		{
			//std::cerr << "Ошибка подготовки SQL-запроса: " << sqlite3_errmsg(l_sql_db) << std::endl;
		}
		else
		{

			BEGIN_TRANSACTION();
			while (sqlite3_step(stmt) == SQLITE_ROW)
			{
				// Получение значения столбцов TIME X и Y
				record.t = sqlite3_column_double(stmt, 0);
				record.x = sqlite3_column_double(stmt, 1); // Индекс столбца X
				record.y = sqlite3_column_double(stmt, 2); // Индекс столбца Y
				//std::cout << "X: " << x << ", Y: " << y << std::endl;
				list.push_back(record);
			}
            END_TRANSACTION();
		}

		 // Освобождение ресурсов
		sqlite3_finalize(stmt);
   }
   return list;
}


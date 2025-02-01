﻿//---------------------------------------------------------------------------


#pragma hdrstop

#include "Protocol_211.h"
#include <math.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

TProtocol_211::TProtocol_211(void)
{
  sensorsList=new TIND3SensorsList;
  sensorADDR=0;
  memset(packetBUF,0,PACKETBUF_LEN);
  accept_data_func=NULL;
  sensor_IND3=NULL;
}

TProtocol_211::~TProtocol_211(void)
{
  sensorsList->Clear();
}

//--------------------------------------------------------------------------


int TProtocol_211::CheckSumCalculate()
{
        //TODO: Add your source code here

		MessageBoxW(NULL,L"Check sumcalculate!",L"ВНИМАНИЕ",0);
        return 0;
}

int  TProtocol_211::packet_proc(unsigned char* buf, int len)
{
		int idx=0;
		memset (packetBUF,0,PACKETBUF_LEN);

//Проверяем разделитель пакета

		if (buf[0]!=PACKET_START)
						 return -1;//Нет разделителя пакета

//        if (buf[len-1]!=PACKET_START)
//                         return -1;//Нет разделителя пакета

//Проверяем тип протокола
		protocol_211_id pr_id=(protocol_211_id)buf[1];

        if (
				(pr_id!=MAIN_PR_211)&&
                (pr_id!=ADDN_PR_211)
           )
	   return -2; //неправильный тип протокола

//Проверяем тип пакета

        main_packet_211_id main_pc_id=MEAS_VAL;

        if (pr_id==MAIN_PR_211)
        {
                main_pc_id=(main_packet_211_id)buf[2];

                if (
                        (main_pc_id!=MEAS_VAL)&&
                        (main_pc_id!=MEAS_VER)&&
                        (main_pc_id!=PACK_ERR)
                   )
          return -3; //неправильный тип пакета
		}

//Считаем реальную длину пакета

       idx=1;

       while (buf[idx]!=0x7E)
	   {
           idx++;
           if (idx>1000) return -1;//Нет разделителя пакета в конце
       }

       idx++;
	   len=idx;

       addn_packet_211_id addn_pc_id=GET_BAUD;

        if (pr_id==ADDN_PR_211)
		{
                addn_pc_id=((addn_packet_211_id)buf[2]);

                if (
                        (addn_pc_id!=GET_BAUD)&&
						(addn_pc_id!=SET_BAUD)&&
                        (addn_pc_id!=GET_NAME)&&
                        (addn_pc_id!=SET_NAME)&&
                        (addn_pc_id!=GET_ZOFF)&&
                        (addn_pc_id!=SET_ZOFF)&&
						(addn_pc_id!=SET_ADDR)&&
                        (addn_pc_id!=GET_SVER)&&
                        (addn_pc_id!=GET_SERN)&&
                        (addn_pc_id!=GET_TACT)&&
                        (addn_pc_id!=SET_TACT)&&
						(addn_pc_id!=GET_PERD)&&
                        (addn_pc_id!=SET_PERD)
                 )

		  return -3; //неправильный тип пакета
        }

//Забираем пакет к себе в буфер
memcpy(packetBUF,buf,len);
packetLEN=len;

//Раскодируем ESCAPE последовательности в принятом пакете
this->EscapeBytesDecode(packetBUF,&packetLEN,1);

//Теперь можно проверить контрольную сумму

	   if (ChceckSumCheck()!=0)
       {
        return -6; //ошибка контрольной суммы
       }

//Адрес измерителя лежит в байте 3
        sensorADDR=packetBUF[3];
//Извлекаем образ датчикаиз списка
		sensor_IND3=sensorsList->GetIND3Sensor(sensorADDR);
//датчика c таким адресом нет в списке
        if (sensor_IND3==NULL)
        {
			return -5;
        }

//Обработка пакетов основного протокола
		if(pr_id==MAIN_PR_211)
        {

		   request_TYPE=(ind3_common_request_type)(int)main_pc_id;
		   //request_TYPE = main_pc_id;

           switch(main_pc_id)
           {

                case  MEAS_VAL:
                   AcceptSensorMeasVal();
                break;

                case  MEAS_VER:
                   AcceptSensorMeasVer();
                break;

                case PACK_ERR:
                   AcceptSensorLastErr();
                break;

           }
        }

//Обработка пакетов дополнительного протокола
        if(pr_id==ADDN_PR_211)
        {

		  request_TYPE=(ind3_common_request_type)(int)(addn_pc_id|(0x10));

           switch(addn_pc_id)
           {
				case GET_BAUD:
                   AcceptSensorBaudRate();
                break;

				//case SET_BAUD:
                //break;

                case GET_NAME:
					AcceptSensorName();
                break;

                //case SET_NAME:
				//break;

                case GET_ZOFF:
                    AcceptSensorZeroOffset();
				break;

                //case SET_ZOFF:
                //break;

                //case SET_ADDR:
                //break;

				case GET_SVER:
                    AcceptSensorSoftVer();
                break;

				case GET_SERN:
                     AcceptSensorSerialNumber();
                break;

				case GET_TACT:
                     AcceptSensorAveragingTacts();
                break;

				//case SET_TACT:
                //break;

                case GET_PERD:
					  AcceptSensorAveragingPeriod();
                break;

                //case SET_PERD:
				//break;
            }

        }

//Вызываем обработчик принятых данных в выхывающем объекте
       if (accept_data_func!=NULL) (*accept_data_func)();

	   return 0;
}

int TProtocol_211::ChceckSumCheck(void)
{
		if (ChceckSumCalculate()!=packetBUF[packetLEN-2])
		{
			 return -1; //неправлиная контрольная сумма
		}
        else
		{
			 return 0 ;
		}
}

/*
Контрольная сумма (1 байт) равна сумме по модулю 2 всех байт пакета. Escape-последовательности декодированы и в расчете контрольной суммы участия не принимают.
Таким образом, контрольная сумма рассчитывается по формуле 1:
CheckSum=ProtocolID?PacketID?Address???Data_i
*/

int TProtocol_211::ChceckSumCalculate(void)
{
        unsigned char chsum=0;

        for (int i=1; i<packetLEN-2; i++)
        {
                 chsum=chsum^packetBUF[i];
        }

        return chsum;
}

/*
Обработка основного пакета съема показаний датчика
*/

int TProtocol_211::AcceptSensorMeasVal(void)
{

//D5.7 знак угла по оси X
  unsigned char signX=(packetBUF[DATAOFFSET+5])&(1<<7);

//D5.6 размерность углапо оси X
  unsigned char unitsX=(packetBUF[DATAOFFSET+5])&(1<<6);

//D5.5 - D4.0 целая часть угла по X
  unsigned int intX=((unsigned int) (packetBUF[DATAOFFSET+5])&0x3F)<<8;
               intX+=(unsigned int)packetBUF[DATAOFFSET+4];

//D3.7 - D3.0 дробная часть угла по X
  unsigned int flX=(unsigned int) (packetBUF[DATAOFFSET+3]);

//D2.7 знак угла по оси Y
  unsigned char signY=(packetBUF[DATAOFFSET+2])&(1<<7);

//D2.6 размерность угла по оси Y
  unsigned char unitsY=(packetBUF[DATAOFFSET+2])&(1<<6);

//D2.5-D1.0 - целая часть угла Y
  unsigned int intY=((unsigned int) (packetBUF[DATAOFFSET+2])&0x3F)<<8;
               intY+=(unsigned int)packetBUF[DATAOFFSET+1];

//D3.7 - D3.0 дробная часть угла по X
  unsigned int flY=(unsigned int) (packetBUF[DATAOFFSET+0]);

//Устанавливаем датчику единицы по Х
  if (unitsX==0)        sensor_IND3->SetUnitsX(ANGLE_SECONDS);
  else                  sensor_IND3->SetUnitsX(ANGLE_MINUTES);

//Устанавливаем датчику единицы по Y
  if (unitsY==0)        sensor_IND3->SetUnitsY(ANGLE_SECONDS);
  else                  sensor_IND3->SetUnitsY(ANGLE_MINUTES);

//Вычисляем и устанавливаем значение угла по X
  double angle = (double)intX+((double)flX/(double)0x100);

//Приводимк 3знакам послезапятой
//  angle*=1000; angle=floor(angle); angle/=1000;

  if (signX) angle*=-1;
  sensor_IND3->SetAngleX(angle);

//Вычисляеми устанавливаем значение угла пл Y
  angle = (double)intY+((double)flY/(double)0x100);

//Приводимк 3знакам послезапятой
//  angle*=1000; angle=floor(angle); angle/=1000;

  if (signY) angle*=-1;
  sensor_IND3->SetAngleY(angle);

  return 0;
}


//Обработка пакета от датчика

int TProtocol_211::AcceptSensorMeasVer(void)
{
   sensor_IND3->SetSensVer((char*)&packetBUF[DATAOFFSET]);
   return 0;
}

int TProtocol_211::AcceptSensorLastErr(void)
{
   sensor_IND3->SetSensLastErr((ind3_sensor_err)packetBUF[DATAOFFSET]);
   return 0;
}

int TProtocol_211::AcceptSensorBaudRate(void)
{
    sensor_IND3->SetSensBaudRate((ind3_baudrate) packetBUF[DATAOFFSET]);
    return 0;
}

int TProtocol_211::AcceptSensorSoftVer(void)
{
    unsigned short ver=((unsigned short)packetBUF[DATAOFFSET+1])<<8;
    ver+=(unsigned short)packetBUF[DATAOFFSET];

    sensor_IND3->SetSensSoftVer(ver);
    return 0;
}

int TProtocol_211::AcceptSensorName(void)
{
    //!!!sensor_IND3->SetSensName(&packetBUF[DATAOFFSET],packetLEN-DATAOFFSET-2);
    return 0;
}



/*
Обрабтка пакета со значениями смещения нуля
*/
int TProtocol_211::AcceptSensorZeroOffset()
{

//D5.7 знак угла по оси X
  unsigned char signX=(packetBUF[DATAOFFSET+5])&(1<<7);

//D5.6 размерность углапо оси X
  unsigned char unitsX=(packetBUF[DATAOFFSET+5])&(1<<6);

//D5.5 - D4.0 целая часть угла по X
  unsigned int intX=((unsigned int) (packetBUF[DATAOFFSET+5])&0x3F)<<8;
               intX+=(unsigned int)packetBUF[DATAOFFSET+4];

//D3.7 - D3.0 дробная часть угла по X
  unsigned int flX=(unsigned int) (packetBUF[DATAOFFSET+3]);

//D2.7 знак угла по оси Y
  unsigned char signY=(packetBUF[DATAOFFSET+2])&(1<<7);

//D2.6 размерность угла по оси Y
  unsigned char unitsY=(packetBUF[DATAOFFSET+2])&(1<<6);

//D2.5-D1.0 - целая часть угла Y
  unsigned int intY=((unsigned int) (packetBUF[DATAOFFSET+2])&0x3F)<<8;
               intY+=(unsigned int)packetBUF[DATAOFFSET+1];

//D3.7 - D3.0 дробная часть угла по X
  unsigned int flY=(unsigned int) (packetBUF[DATAOFFSET+0]);

//Устанавливаем датчику единицы по Х
  if (unitsX==0)        sensor_IND3->SetUnitsOffsetX(ANGLE_SECONDS);
  else                  sensor_IND3->SetUnitsOffsetX(ANGLE_MINUTES);

//Устанавливаем датчику единицы по Y
  if (unitsY==0)        sensor_IND3->SetUnitsOffsetY(ANGLE_SECONDS);
  else                  sensor_IND3->SetUnitsOffsetY(ANGLE_MINUTES);

//Вычисляеми устанавливаем значение угла пл X
  double angle = (double)intX+((double)flX/(double)0x100);
  if (signX) angle*=-1;
  sensor_IND3->SetAngleOffsetX(angle);

//Вычисляеми устанавливаем значение угла пл Y
  angle = (double)intY+((double)flY/(double)0x100);
  if (signY) angle*=-1;
  sensor_IND3->SetAngleOffsetY(angle);


  return 0;

}

/*
Обработка пакета с заводским номером измерителя
*/

int TProtocol_211::AcceptSensorSerialNumber(void)
{
     unsigned int serial3=((unsigned int)packetBUF[7])<<24;
     unsigned int serial2=((unsigned int)packetBUF[6])<<16;
     unsigned int serial1=((unsigned int)packetBUF[5])<<8;
     unsigned int serial0=((unsigned int)packetBUF[4])<<0;

     unsigned int serial=serial3+serial2+serial1+serial0;

     sensor_IND3->SetSensSerial(serial);

     return 0;
}

void TProtocol_211::AcceptSensorAveragingTacts(void)
{
     sensor_IND3->SetSensAveragingTacts(packetBUF[4]);
}

void TProtocol_211::AcceptSensorAveragingPeriod(void)
{
     sensor_IND3->SetSensAveragingPeriod(packetBUF[4]);
}


//------------------Запросы к измерителю----------------------------------------

void TProtocol_211::ClearRequestBuf(void)
{
        memset(request_packetBUF,0,REQUESTBUFLEN);
        request_packetLEN=0;
}

//Основной протокол пакета запроса
void TProtocol_211::RequestMainMeterPacket(unsigned char addr, main_packet_211_id packet, unsigned char* buf, int* len)
{
//Инициализация
      int idx=0; unsigned char chsum=0; ClearRequestBuf();

//Начало пакета
      request_packetBUF[idx]=PACKET_START; idx++;
//Тип протокола
      request_packetBUF[idx]=MAIN_PR_211;       chsum=chsum^request_packetBUF[idx]; idx++;
//Тип пакета
      request_packetBUF[idx]=packet;            chsum=chsum^request_packetBUF[idx]; idx++;
//Адркс измерителя
      request_packetBUF[idx]=addr;              chsum=chsum^request_packetBUF[idx]; idx++;
//Контрольная сумма
      request_packetBUF[idx]=chsum;                                                 idx++;
//Конец пакета
      request_packetBUF[idx]=PACKET_END;                                            idx++;

      request_packetLEN=idx;

//Кодируем ESCAP Eпоследовательности
       this->EscapeBytesEncode(request_packetBUF,&request_packetLEN,1);

//Копируем пакет и длину вызвавшей процедуре
        memcpy(buf,request_packetBUF,request_packetLEN); *len=request_packetLEN;
}


//Дополнительный протокол пакета запроса
void TProtocol_211::RequestAddnMeterPacket(unsigned char addr, addn_packet_211_id packet, unsigned char* buf, int* len)
{
//Инициализация
      int idx=0; unsigned char chsum=0; ClearRequestBuf();

//Начало пакета
      request_packetBUF[idx]=PACKET_START; idx++;
//Тип протокола
      request_packetBUF[idx]=ADDN_PR_211;       chsum=chsum^request_packetBUF[idx]; idx++;
//Тип пакета
      request_packetBUF[idx]=packet;            chsum=chsum^request_packetBUF[idx]; idx++;
//Адрес измерителя
      request_packetBUF[idx]=addr;              chsum=chsum^request_packetBUF[idx]; idx++;
//Контрольная сумма
      request_packetBUF[idx]=chsum;                                                 idx++;
//Конец пакета
      request_packetBUF[idx]=PACKET_END;                                            idx++;

      request_packetLEN=idx;

//Кодируем ESCAPE Eпоследовательности
       this->EscapeBytesEncode(request_packetBUF,&request_packetLEN,1);
//Копируем пакет и длину вызвавшей процедуре
        memcpy(buf,request_packetBUF,request_packetLEN); *len=request_packetLEN;
}

//-----------------------------Обработка Escape байт---------------------------------

int TProtocol_211::EscapeBytesEncode(unsigned char* buf, int* len, int frame)
{
        unsigned char tmpbuf[PACKETBUF_LEN];
        memset(tmpbuf,0,PACKETBUF_LEN);
        int idx=0;
        int idxstart=0;

//Если отдали на кодирование не произвольный фрагмента а корректный фрейм нулевой байт всегда 0x7E
        if (frame)
        {
                tmpbuf[0]=buf[0];
                idx++;
                idxstart++;
        }

        for (int i=idxstart; i<(*len); i++)
        {
//Если отдали на кодирование не произвольный фрагмента а корректный фрейм последний байт всегда 0x7E
//Дошли до конца исходногопакета - пишемконец пакетаи выходим

              if (frame)
              {
                 if (i==(*len-1))
                 {
                    tmpbuf[idx]=0x7E; idx++;
                    break;
                 }

              }

              if ((buf[i]!=PACKET_START)&&(buf[i]!=ESCAPE_BYTE))
              {
                    tmpbuf[idx]=buf[i];      idx++;
              }
              else
              {

                    tmpbuf[idx]=ESCAPE_BYTE; idx++;

                    if (buf[i]==PACKET_START)
                    {
                        tmpbuf[idx]=CODE7E_BYTE; idx++;
                    }

                    if (buf[i]==ESCAPE_BYTE)
                    {
                        tmpbuf[idx]=CODE7D_BYTE; idx++;
                    }
              }

        }

        (*len)=idx;
        memcpy(buf,tmpbuf,idx);

        return 0;
 }


 int TProtocol_211::EscapeBytesDecode(unsigned char* buf, int* len, int frame)
 {
        unsigned char tmpbuf[PACKETBUF_LEN];
        memset(tmpbuf,0,PACKETBUF_LEN);
        int idx=0;
        int idxstart=0;
        int sleep=0;

//Если отдали на кодирование не произвольный фрагмент а корректный фрейм нулевой байт всегда 0xE7
        if (frame)
        {
                tmpbuf[0]=buf[0];
                idx++;
                idxstart++;
        }

        for (int i=1; i<(*len); i++)
        {
//Если отдали на кодирование не произвольный фрагмент а корректный фрейм последний байт всегда 0xE7
//Дошли до конца исходного пакета - пишем конец пакета и выходим

              if (frame)
              {
                 if (i==(*len-1))
                 {
                    tmpbuf[idx]=0x7E; idx++;
                    break;
                 }

              }

              if (sleep)
              {
                    sleep=0;
                    continue;
              }

              if (buf[i]!=ESCAPE_BYTE)
              {
                    tmpbuf[idx]=buf[i];
              }
              else
              {

                    if (buf[i+1]==CODE7E_BYTE)
                    {
                        tmpbuf[idx]=0x7E;
                    }

                    if (buf[i+1]==CODE7D_BYTE)
                    {
                        tmpbuf[idx]=0x7D;
                    }

                    sleep=1;

              }

               idx++;

        }

        (*len)=idx;
        memcpy(buf,tmpbuf,idx);

        return 0;
 }

//-------------------------------------------------------------------------------


/////////////////////////ВНЕШНИЙ ПРОГРАММНЫЙ ИНТЕРФЕЙС//////////////////////////


int TProtocol_211::AddIND3Sensor(unsigned char addr)
{
        return this->sensorsList->AddIND3Sensor(addr);
}

TIND3sensor* TProtocol_211::GetIND3Sensor(unsigned char addr)
{
     return   this->sensorsList->GetIND3Sensor(addr);
}


////////////////////ФОРМИРОВАНИЕ ПАКЕТОВ ЗАПРОСА В СТОРОНУ ДАТЧИКА//////////////

//---------------------------Основные пакета запроса-------------------------------------------

void TProtocol_211::RequestMeasureResult(unsigned char addr, unsigned char* buf, int* len)
{
        RequestMainMeterPacket(addr, MEAS_VAL, buf, len);
}

void TProtocol_211::RequestMeterVerion(unsigned char addr, unsigned char* buf, int* len)
{
        RequestMainMeterPacket(addr, MEAS_VER, buf, len);

}

//---------------------------Дополнительные пакета запроса-------------------------------------------
void TProtocol_211::RequestSensorBaudRate(unsigned char addr, unsigned char* buf, int* len)
{
        RequestAddnMeterPacket(addr, GET_BAUD, buf, len);
}

void TProtocol_211::RequestSensorMeterName(unsigned char addr, unsigned char* buf, int* len)
{
        RequestAddnMeterPacket(addr, GET_NAME, buf, len);

}

void TProtocol_211::RequestSensorZeroOffset(unsigned char addr, unsigned char* buf, int* len)
{
        RequestAddnMeterPacket(addr, GET_ZOFF, buf, len);
}

void TProtocol_211::RequestSensorSoftVersion(unsigned char addr, unsigned char* buf, int* len)
{
        RequestAddnMeterPacket(addr, GET_SVER,buf, len);
}

void TProtocol_211::RequestSensorMeterSerialNumber(unsigned char addr, unsigned char* buf, int* len)
{
        RequestAddnMeterPacket(addr, GET_SERN, buf, len);
}

void TProtocol_211::RequestSensorAveragingTacts(unsigned char addr, unsigned char* buf, int* len)
{
        RequestAddnMeterPacket(addr, GET_TACT, buf, len);
}

void TProtocol_211::RequestSensorAveragingPeriod(unsigned char addr, unsigned char* buf, int* len)
{
        RequestAddnMeterPacket(addr, GET_PERD, buf, len);
}

//////////////ФОРМИРОВАНИЕ ПАКЕТОВ УСТАНОВКИ ПАРАМЕТРОВ В СТОРОНУ ДАТЧИКА////////////////////////////////


void TProtocol_211::SetSensorBaudRate(unsigned char addr, ind3_baudrate baud, unsigned char* buf, int* len)
{

//Инициализация
      int idx=0; unsigned char chsum=0; ClearRequestBuf();

//Начало пакета
      request_packetBUF[idx]=PACKET_START; idx++;
//Тип протокола
      request_packetBUF[idx]=ADDN_PR_211;       chsum=chsum^request_packetBUF[idx]; idx++;
//Тип пакета
      request_packetBUF[idx]=SET_BAUD;          chsum=chsum^request_packetBUF[idx]; idx++;
//Адрес измерителя
      request_packetBUF[idx]=addr;              chsum=chsum^request_packetBUF[idx]; idx++;
//заданнаяскорость обмена
      request_packetBUF[idx]=baud;              chsum=chsum^request_packetBUF[idx]; idx++;
//Контрольная сумма
      request_packetBUF[idx]=chsum;                                                 idx++;
//Конец пакета
      request_packetBUF[idx]=PACKET_END;                                            idx++;

      request_packetLEN=idx;

//Кодируем ESCAPE Eпоследовательности
       this->EscapeBytesEncode(request_packetBUF,&request_packetLEN,1);

//Копируем пакет и длину вызвавшей процедуре
        memcpy(buf,request_packetBUF,request_packetLEN); *len=request_packetLEN;

}

void TProtocol_211::SetSensorName(unsigned char addr, char* name, unsigned char* buf, int* len)
{

//Инициализация
      int idx=0; unsigned char chsum=0; ClearRequestBuf();

//Начало пакета
      request_packetBUF[idx]=PACKET_START; idx++;
//Тип протокола
      request_packetBUF[idx]=ADDN_PR_211;       chsum=chsum^request_packetBUF[idx]; idx++;
//Тип пакета
      request_packetBUF[idx]=SET_NAME;          chsum=chsum^request_packetBUF[idx]; idx++;
//Адрес измерителя
      request_packetBUF[idx]=addr;              chsum=chsum^request_packetBUF[idx]; idx++;

//новое имя

      int j=0;

      while(name[j]!=0)
      {
          request_packetBUF[idx]=name[j];       chsum=chsum^request_packetBUF[idx]; idx++;
          j++;
          //неболее 16 символов
          if (j>=16) break;
      }

//Контрольная сумма
      request_packetBUF[idx]=chsum;                                                 idx++;
//Конец пакета
      request_packetBUF[idx]=PACKET_END;                                            idx++;

      request_packetLEN=idx;

//Кодируем ESCAP Eпоследовательности
       this->EscapeBytesEncode(request_packetBUF,&request_packetLEN,1);

//Копируем пакет и длину вызвавшей процедуре
        memcpy(buf,request_packetBUF,request_packetLEN); *len=request_packetLEN;
}


void TProtocol_211::SetsensorZeroOffset(unsigned char addr, double angle_X, double angle_Y, angle_units units_X, angle_units units_Y, unsigned char* buf, int* len)
{

//Инициализация
      int idx=0; unsigned char chsum=0; ClearRequestBuf();

//Начало пакета
      request_packetBUF[idx]=PACKET_START; idx++;
//Тип протокола
      request_packetBUF[idx]=ADDN_PR_211;       chsum=chsum^request_packetBUF[idx]; idx++;
//Тип пакета
      request_packetBUF[idx]=SET_ZOFF;          chsum=chsum^request_packetBUF[idx]; idx++;
//Адркс измерителя
      request_packetBUF[idx]=addr;              chsum=chsum^request_packetBUF[idx]; idx++;

//фиксируем знак по X Y
unsigned char sign_X=0;
unsigned char sign_Y=0;

if (angle_X<0)
{
        sign_X=1;
        angle_X*=-1;
}

if (angle_Y<0)
{
        sign_Y=1;
        angle_Y*=-1;
}

//целая часть
double intpart=0;
//дробная часть
double fltpart=0;

unsigned short tmp=0;

//выделяем целую и дробную по Y
fltpart=modf(angle_Y,&intpart);

//дробную часть приводим к трем знакам после запятой и множим на 1000
fltpart*=1000;
//дробную часть приводим к 256 долям
fltpart*=256;
fltpart/=1000;

//пакуем Y
//D0.7-D0.0 - дробная часть ?_Y
    request_packetBUF[idx]=fltpart;            chsum=chsum^request_packetBUF[idx]; idx++;

//D2.5-D1.0 - целая часть ?_Y

//младший байт Y
tmp=((unsigned short)intpart)&0xFF;
    request_packetBUF[idx]=(unsigned char)tmp;                 chsum=chsum^request_packetBUF[idx]; idx++;

//старший байт Y
tmp=(((unsigned short)intpart)>>8)&0xFF;
    request_packetBUF[idx]=(unsigned char)tmp;                 //пока стоим на том же байте

//пакуем размерность по Y

   if (units_Y==ANGLE_MINUTES)  request_packetBUF[idx]|= (1<<6);
   else                         request_packetBUF[idx]&=~(1<<6);

//пакуем знак по Y

   if (sign_Y>0)                request_packetBUF[idx]|= (1<<7);
   else                         request_packetBUF[idx]&=~(1<<7);

//теперь двигаем индекс
   chsum=chsum^request_packetBUF[idx]; idx++;

//выделяем целую и дробную по X
fltpart=modf(angle_X,&intpart);

//дробную часть приводим к трем знакампосле запятой и множим на 1000
fltpart*=1000;
//дробнуючасть приводим к 256 долям
fltpart*=256;
fltpart/=1000;

//пакуем X
//D3.7-D3.0 - дробная часть ?_X
    request_packetBUF[idx]=fltpart;            chsum=chsum^request_packetBUF[idx]; idx++;

//D5.5-D4.0 - целая часть ?_X

//младший байт X
tmp=((unsigned short)intpart)&0xFF;
    request_packetBUF[idx]=(unsigned char)tmp;                 chsum=chsum^request_packetBUF[idx]; idx++;

//старшийбайт X
tmp=(((unsigned short)intpart)>>8)&0xFF;
    request_packetBUF[idx]=(unsigned char)tmp;                 //пока стоим на том же байте

//пакуем размерность по X

   if (units_X==ANGLE_MINUTES)  request_packetBUF[idx]|= (1<<6);
   else                         request_packetBUF[idx]&=~(1<<6);

//пакуем знак по X

   if (sign_X>0)                request_packetBUF[idx]|= (1<<7);
   else                         request_packetBUF[idx]&=~(1<<7);

//теперь двигаем индекс
   chsum=chsum^request_packetBUF[idx]; idx++;


//Контрольная сумма
      request_packetBUF[idx]=chsum;                                                 idx++;
//Конец пакета
      request_packetBUF[idx]=PACKET_END;                                            idx++;

      request_packetLEN=idx;

//Кодируем ESCAP Eпоследовательности
       this->EscapeBytesEncode(request_packetBUF,&request_packetLEN,1);

//Копируем пакет и длину вызвавшей процедуре
        memcpy(buf,request_packetBUF,request_packetLEN); *len=request_packetLEN;
}


void TProtocol_211:: SetSensorNewAddress(unsigned char addr, unsigned char newaddr, unsigned char* buf, int* len)
{

 //Инициализация
      int idx=0; unsigned char chsum=0; ClearRequestBuf();

//Начало пакета
      request_packetBUF[idx]=PACKET_START; idx++;
//Тип протокола
      request_packetBUF[idx]=ADDN_PR_211;       chsum=chsum^request_packetBUF[idx]; idx++;
//Тип пакета
      request_packetBUF[idx]=SET_ADDR;          chsum=chsum^request_packetBUF[idx]; idx++;
//Адркс измерителя
      request_packetBUF[idx]=addr;              chsum=chsum^request_packetBUF[idx]; idx++;

//новый адрес
      request_packetBUF[idx]=newaddr;              chsum=chsum^request_packetBUF[idx]; idx++;
//Контрольная сумма
      request_packetBUF[idx]=chsum;                                                 idx++;
//Конец пакета
      request_packetBUF[idx]=PACKET_END;                                            idx++;

      request_packetLEN=idx;

//Кодируем ESCAP Eпоследовательности
       this->EscapeBytesEncode(request_packetBUF,&request_packetLEN,1);

//Копируем пакет и длину вызвавшей процедуре
        memcpy(buf,request_packetBUF,request_packetLEN); *len=request_packetLEN;

}

void TProtocol_211::SetSensorAveragingTacts(unsigned char addr, ind3_averaging_tacts tacts, unsigned char* buf, int* len)
{

 //Инициализация
      int idx=0; unsigned char chsum=0; ClearRequestBuf();

//Начало пакета
      request_packetBUF[idx]=PACKET_START; idx++;
//Тип протокола
      request_packetBUF[idx]=ADDN_PR_211;       chsum=chsum^request_packetBUF[idx]; idx++;
//Тип пакета
      request_packetBUF[idx]=SET_TACT;          chsum=chsum^request_packetBUF[idx]; idx++;
//Адрес измерителя
      request_packetBUF[idx]=addr;              chsum=chsum^request_packetBUF[idx]; idx++;

//новый адрес
      request_packetBUF[idx]=tacts;              chsum=chsum^request_packetBUF[idx]; idx++;
//Контрольная сумма
      request_packetBUF[idx]=chsum;                                                 idx++;
//Конец пакета
      request_packetBUF[idx]=PACKET_END;                                            idx++;

      request_packetLEN=idx;

//Кодируем ESCAP Eпоследовательности
       this->EscapeBytesEncode(request_packetBUF,&request_packetLEN,1);

//Копируем пакет и длину вызвавшей процедуре
        memcpy(buf,request_packetBUF,request_packetLEN); *len=request_packetLEN;

}

void TProtocol_211::SetSensorAveragingPeriod(unsigned char addr, ind3_averaging_period period, unsigned char* buf, int* len)
{

 //Инициализация
      int idx=0; unsigned char chsum=0; ClearRequestBuf();

//Начало пакета
      request_packetBUF[idx]=PACKET_START; idx++;
//Тип протокола
      request_packetBUF[idx]=ADDN_PR_211;       chsum=chsum^request_packetBUF[idx]; idx++;
//Тип пакета
      request_packetBUF[idx]=SET_PERD;          chsum=chsum^request_packetBUF[idx]; idx++;
//Адрес измерителя
      request_packetBUF[idx]=addr;              chsum=chsum^request_packetBUF[idx]; idx++;

//новый адрес
      request_packetBUF[idx]=period;              chsum=chsum^request_packetBUF[idx]; idx++;
//Контрольная сумма
      request_packetBUF[idx]=chsum;                                                 idx++;
//Конец пакета
      request_packetBUF[idx]=PACKET_END;                                            idx++;

      request_packetLEN=idx;

//Кодируем ESCAP Eпоследовательности
       this->EscapeBytesEncode(request_packetBUF,&request_packetLEN,1);

//Копируем пакет и длину вызвавшей процедуре
        memcpy(buf,request_packetBUF,request_packetLEN); *len=request_packetLEN;
}

//////////////////////////ЧТЕНИЕ ДАННЫХ ИЗ ОБРАЗА ДАТЧИКА///////////////////////

int TProtocol_211::ReadIND3SensorMeasureResult(unsigned char addr, double* angle_X, double* &angle_Y, unsigned char* units_X, unsigned char* units_Y)
{

    TIND3sensor* sensor=this->GetIND3Sensor(addr);

    if (sensor==NULL) return -1; //адрес отсутствует всписке

    *angle_X=sensor->GetSensAngleX();
    *angle_Y=sensor->GetSensAngleY();
    *units_X=sensor->GetSensUnitsX();
    *units_Y=sensor->GetSensUnitsY();

    return 0;
}

//Прочитать версию измерителя внешней процедурой
int TProtocol_211::ReadIND3SensorMeterVerion(unsigned char addr, char* version)
{
    TIND3sensor* sensor=this->GetIND3Sensor(addr);
    if (sensor==NULL) return -1; //адрес отсутствует всписке

    memcpy(version, sensor->GetSensMeterVersion(),5);

    return 0;
}

//Прочитать скорость обмена внешней процедурой
int TProtocol_211:: ReadIND3SensorBaudRate(unsigned char addr, int* baud)
{

    TIND3sensor* sensor=this->GetIND3Sensor(addr);
    if (sensor==NULL) return -1; //адрес отсутствует всписке

    *baud=sensor->GetSensBaudRate();

    return 0;

}

//Прочитать имя измерителя внешней процедурой
int TProtocol_211::ReadIND3SensorMeterName(unsigned char addr, char* name)
{
    TIND3sensor* sensor=this->GetIND3Sensor(addr);
    if (sensor==NULL) return -1; //адрес отсутствует всписке

    memcpy(name, sensor->GetSensName(),16);

    return 0;
}


//Прочитать смещение нуля внешней процедурой
int TProtocol_211::ReadIND3SensorZeroOffset(unsigned char addr, double* angle_X, double* &angle_Y, unsigned char* units_X, unsigned char* units_Y)
{
    TIND3sensor* sensor=this->GetIND3Sensor(addr);
    if (sensor==NULL) return -1; //адрес отсутствует всписке

        *angle_X=sensor->GetSensZeroOffsetX();
        *angle_Y=sensor->GetSensZeroOffsetY();
        *units_X=sensor->GetSensZeroOffsetUnitsX();
        *units_Y=sensor->GetSensZeroOffsetUnitsY();

    return 0;
}

//Прочитать номер редакции ПО внешней процедурой
int TProtocol_211::ReadIND3SensorSoftVersion(unsigned char addr, unsigned short* ver)
{

    TIND3sensor* sensor=this->GetIND3Sensor(addr);
    if (sensor==NULL) return -1; //адрес отсутствует всписке

    *ver=sensor->GetSensSoftVer();

    return 0;

}

//Прочитать заводской номер измерителя внешней процедурой
int TProtocol_211::ReadIND3SensorSerialNumber(unsigned char addr, unsigned int* serial)
{

    TIND3sensor* sensor=this->GetIND3Sensor(addr);
    if (sensor==NULL) return -1; //адрес отсутствует всписке

    *serial=sensor->GetSensSerialNumber();

    return 0;

}

//Прочитать количество тактов усреднения внешней процедурой
int TProtocol_211::ReadIND3SensorAveragingTacts(unsigned char addr, int* tacts)
{
    TIND3sensor* sensor=this->GetIND3Sensor(addr);
    if (sensor==NULL) return -1; //адрес отсутствует всписке


    switch(sensor->GetSensAveragingTacts())
    {

        case averaging_tacts_1:
                *tacts=1;
        break;

	case averaging_tacts_2:
                *tacts=2;
        break;

	case averaging_tacts_4:
                *tacts=4;
        break;

	case averaging_tacts_8:
                *tacts=8;
        break;

	case averaging_tacts_16:
                *tacts=16;
        break;

	case averaging_tacts_32:
                *tacts=32;
        break;

        default:
                *tacts=0;

    }

    return 0;

}


//Прочитать период усреднения внешней процедурой
int TProtocol_211::ReadIND3SensorAveragingPeriod(unsigned char addr, int* period)
{
    TIND3sensor* sensor=this->GetIND3Sensor(addr);
    if (sensor==NULL) return -1; //адрес отсутствует всписке


    switch(sensor->GetSensAveragingPeriod())
    {

        case averaging_period_10:
                *period=10;
        break;

	case averaging_period_20:
                *period=20;
        break;

	case averaging_period_50:
                *period=50;
        break;

	case averaging_period_100:
                *period=100;
        break;

        default:
                *period=0;

    }

    return 0;

}

int TProtocol_211::ReadIND3SensorLastErr(unsigned char addr, unsigned char* err)
{
        TIND3sensor* sensor=this->GetIND3Sensor(addr);
        if (sensor==NULL) return -1; //адрес отсутствует в списке

        *err=(unsigned char)sensor->GetSensLastErr();

        return 0;
}

/////////////////////////////////////////////////////////////////////////////////

//----------------------ДАТЧИК ИН-Д3--------------------------------------------

  __fastcall TIND3sensor::TIND3sensor()
  {
   strcpy(SensorIND3.sensor_vers,"");
   SensorIND3.units_x=ANGLE_MINUTES;


  }

  __fastcall TIND3sensor::~TIND3sensor()
  {



  }

  void TIND3sensor::SetAddr(unsigned char addr)
  {
        SensorIND3.sensor_addr=addr;
  }

  void TIND3sensor::SetUnitsX(angle_units unit)
  {
       SensorIND3.units_x=unit;
  }

  void TIND3sensor::SetUnitsY(angle_units unit)
  {
       SensorIND3.units_y=unit;
  }

  void TIND3sensor::SetAngleX(double angle)
  {
       SensorIND3.angle_x=angle;
  }

  void TIND3sensor::SetAngleY(double angle)
  {
       SensorIND3.angle_y=angle;
  }

  void TIND3sensor::SetSensVer(char* version)
  {

       SensorIND3.sensor_vers[5]=0;
       for (int i=0; i<5; i++)
       {
         SensorIND3.sensor_vers[i]=version[i];
       }

  }

  void TIND3sensor::SetSensLastErr(ind3_sensor_err err)
  {
       SensorIND3.last_err=err;
  }

   void TIND3sensor::SetSensBaudRate(ind3_baudrate baud)
   {
       SensorIND3.baud_rate=baud;
   }

   void TIND3sensor::SetSensSoftVer(unsigned int version)
   {
       SensorIND3.soft_version=version;
   }

   void TIND3sensor::SetSensName(char* name, int len)
   {
       memset(SensorIND3.sensor_name,0,16);
       memcpy(SensorIND3.sensor_name,name,len);
   }

 //выдать адрес датчика
    unsigned char TIND3sensor::GetSensAddr(void)
    {
        return this->SensorIND3.sensor_addr;
    }

    double TIND3sensor::GetSensAngleX(void)
    {
        return this->SensorIND3.angle_x;
    }

    double TIND3sensor::GetSensAngleY(void)
    {
        return this->SensorIND3.angle_y;
    }

    int TIND3sensor::GetSensUnitsX(void)
    {
        return this->SensorIND3.units_x;
    }

    int TIND3sensor::GetSensUnitsY(void)
    {
        return this->SensorIND3.units_y;
    }

     char*  TIND3sensor::GetSensMeterVersion(void)
     {
        return this->SensorIND3.sensor_vers;
     }

     ind3_sensor_err  TIND3sensor::GetSensLastErr(void)
     {
        return this->SensorIND3.last_err;
     }

     int  TIND3sensor::GetSensBaudRate(void)
     {
        int baud=0;

        switch (this->SensorIND3.baud_rate)
        {
               case 1:
                baud=1200;
               break;

               case 2:
                baud=2400;
               break;

               case 3:
                baud=4800;
               break;

               case 4:
                baud=9600;
               break;

               case 5:
                baud=19200;
               break;

               case 6:
                baud=38400;
               break;

               case 7:
                baud=57600;
               break;

               case 8:
                baud=115200;
               break;


               default:
               baud=0;

        }

        return baud;
     }


  char* TIND3sensor::GetSensName(void)
  {
        return  this->SensorIND3.sensor_name;
  }

  void TIND3sensor::SetAngleOffsetX(double angle)
  {
        this->SensorIND3.zoffset_x=angle;
  }

  void TIND3sensor::SetAngleOffsetY(double angle)
  {
        this->SensorIND3.zoffset_y=angle;
  }

  void TIND3sensor::SetUnitsOffsetX(angle_units unit)
  {
       SensorIND3.units_zoffset_x=unit;
  }

  void TIND3sensor::SetUnitsOffsetY(angle_units unit)
  {
       SensorIND3.units_zoffset_y=unit;
  }

  double TIND3sensor::GetSensZeroOffsetX(void)
  {
        return SensorIND3.zoffset_x;
  }

  double TIND3sensor::GetSensZeroOffsetY(void)
  {
        return SensorIND3.zoffset_y;
  }

  int TIND3sensor::GetSensZeroOffsetUnitsX(void)
  {
        return (int) SensorIND3.units_zoffset_x;
  }

  int TIND3sensor::GetSensZeroOffsetUnitsY(void)
  {
        return (int) SensorIND3.units_zoffset_y;
  }

  unsigned short TIND3sensor::GetSensSoftVer(void)
  {
        return SensorIND3.soft_version;
  }

  unsigned int TIND3sensor::GetSensSerialNumber(void)
  {
        return SensorIND3.sensor_serial;
  }


  void TIND3sensor::SetSensSerial(unsigned int serial)
  {
         SensorIND3.sensor_serial=serial;
  }


  void TIND3sensor::SetSensAveragingTacts(unsigned char tacts)
  {

          SensorIND3.averaging_tacts=tacts;
  }

  ind3_averaging_tacts TIND3sensor::GetSensAveragingTacts(void)
  {

          return (ind3_averaging_tacts)SensorIND3.averaging_tacts;
  }

  void TIND3sensor::SetSensAveragingPeriod(unsigned char period)
  {

          SensorIND3.averaging_period=period;
  }

  ind3_averaging_period TIND3sensor::GetSensAveragingPeriod(void)
  {

          return (ind3_averaging_period) SensorIND3.averaging_period;
  }


//-------------Запись указателя на датчик в списке-------------------------------

  TIND3sensor_ptr::TIND3sensor_ptr(void)
   {
        sensor=NULL;
        next=NULL;
   }

  TIND3sensor_ptr::~TIND3sensor_ptr(void)
  {

  }

//---------------------Список датчиков------------------------------------------

TIND3SensorsList::TIND3SensorsList(void)
{
     top=NULL;
     bottom=NULL;
}


TIND3SensorsList::~TIND3SensorsList(void)
{
        Clear();
}

//Добавить в список датчик с заданным адресом
int TIND3SensorsList::AddIND3Sensor(unsigned char addr)
{
    if (SensorExist(addr)) return -1;

    TIND3sensor* sensor = new TIND3sensor;
    sensor->SetAddr(addr);
    TIND3sensor_ptr* ptr = new TIND3sensor_ptr;

    ptr->sensor=sensor;

    if (top==NULL)
    {
        top=ptr;
        bottom=ptr;
    }
    else
    {
        bottom->next=ptr;
        bottom=ptr;
    }

    return 0;
}

//Наличие датчика в списке
int TIND3SensorsList::SensorExist(unsigned char addr)
{

   TIND3sensor_ptr* ptr=top;
   unsigned char sadr;

   while (ptr)
   {
        sadr=ptr->sensor->GetSensAddr();
        if (sadr==addr) return 1;
        ptr=ptr->next;
   }

   return 0;
}

//Найти в списке датчик по адресу
TIND3sensor*  TIND3SensorsList::GetIND3Sensor(unsigned char addr)
{

   TIND3sensor_ptr* ptr=top;

   while (ptr)
   {
//        int a=ptr->sensor->GetAddr();

        if (ptr->sensor->GetSensAddr()==addr)
        {
                 return ptr->sensor;
        }
        ptr=ptr->next;
   }

   return NULL;

}

//Очистка списка датчиков
void  TIND3SensorsList::Clear(void)
{

   TIND3sensor_ptr* ptr=top;
   TIND3sensor_ptr* next=top;

   while (next)
   {
        ptr=next;
        next=ptr->next;
        delete ptr;
   }

   top=NULL;
   bottom=NULL;

}

//------------------------------------------------------------------------------


void TProtocol_211::RequestSaveSettingsNVM(unsigned char addr, unsigned char* buf, int* len)
{
//TODO: Add your source code here

//Инициализация
      int idx=0; unsigned char chsum=0; ClearRequestBuf();

//Начало пакета
      request_packetBUF[idx]=PACKET_START; idx++;
//Тип протокола
      request_packetBUF[idx]=PACK_PR_NVM;       chsum=chsum^request_packetBUF[idx]; idx++;
//Тип пакета
      request_packetBUF[idx]=0x04;              chsum=chsum^request_packetBUF[idx]; idx++;
//Адркс измерителя
      request_packetBUF[idx]=addr;              chsum=chsum^request_packetBUF[idx]; idx++;
//Контрольная сумма
      request_packetBUF[idx]=chsum^0x5A;                                                 idx++;
//Конец пакета
      request_packetBUF[idx]=PACKET_END;                                            idx++;

      request_packetLEN=idx;

//Кодируем ESCAP Eпоследовательности
       this->EscapeBytesEncode(request_packetBUF,&request_packetLEN,1);

//Копируем пакет и длину вызвавшей процедуре
        memcpy(buf,request_packetBUF,request_packetLEN); *len=request_packetLEN;

}

void TProtocol_211::ClearSensorsList()
{
        this->sensorsList->Clear();
}

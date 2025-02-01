//---------------------------------------------------------------------------

#pragma hdrstop

#include "Protocol_211.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void TProtocol_211::setProtocol(void)
{
   protocol_type = PROTOCOL_TYPE_IND3;
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

//Основной протокол пакета запроса
void TProtocol_211::RequestMainMeterPacket(unsigned char addr, main_packet_211_id packet)
{
//Инициализация
	 clear_tx(); unsigned char chsum=0;
//Начало пакета
	  buftx[tx_idx] = PACKET_START; tx_idx++;
//Тип протокола
	  buftx[tx_idx] = MAIN_PR_211;           chsum=chsum^buftx[tx_idx]; tx_idx++;
//Тип пакета
	  buftx[tx_idx] = packet;                chsum=chsum^buftx[tx_idx]; tx_idx++;
//Адрес измерителя
	  buftx[tx_idx] = addr;                  chsum=chsum^buftx[tx_idx]; tx_idx++;
//Контрольная сумма
	  buftx[tx_idx] = chsum;                                            tx_idx++;
//Конец пакета
	  buftx[tx_idx] = PACKET_END;                                       tx_idx++;

//Кодируем ESCAP Eпоследовательности
	 this->EscapeBytesEncode(buftx, &tx_idx, 1);

}

/*
	заполняет запрос на углы x y
	запрос сформирован в buftx
	длина пакета сформирована в tx_idx

*/
int TProtocol_211::request_curr_XY(BYTE addr)
{
	RequestMainMeterPacket(addr, MEAS_VAL);

	return 0;
}

int TProtocol_211::accept_response_curr_XY(BYTE addr)
{

     return 0;
}

/*
  процессинг принятого пакета
*/
int  TProtocol_211::packet_proc(unsigned char* buf, int* len, BYTE addr)
{
		int idx = 0;
		BYTE packetBUF [1024];
		memset (packetBUF,0,1024);

//Проверяем разделитель пакета

		if (buf[0]!=PACKET_START)
						 return -1;//Нет разделителя пакета

//Проверяем тип протокола
		protocol_211_id pr_id = (protocol_211_id) buf[1];

		if (
				(pr_id!=MAIN_PR_211)&&
				(pr_id!=ADDN_PR_211)
		   )
	   return -2; //неправильный тип протокола

//Проверяем тип пакета

		main_packet_211_id main_pc_id = MEAS_VAL;

		if (pr_id == MAIN_PR_211)
        {
				main_pc_id = (main_packet_211_id)buf[2];

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
		   if (idx > 1000) return -4;//Нет разделителя пакета в конце
	   }

       idx++;
	   *len = idx;

	   addn_packet_211_id addn_pc_id = GET_BAUD;

		if (pr_id == ADDN_PR_211)
		{
				addn_pc_id = ((addn_packet_211_id)buf[2]);

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
memcpy(packetBUF,buf,*len);
int packetLEN = *len;

//Раскодируем ESCAPE последовательности в принятом пакете
this->EscapeBytesDecode(packetBUF,&packetLEN,1);

//кидаем в bufrx

clear_rx();
memcpy(bufrx, packetBUF, packetLEN);
rx_idx = packetLEN;

//Теперь можно проверить контрольную сумму

	   if (ChceckSumCheck()!=0)
       {
			return -6; //ошибка контрольной суммы
	   }

//Адрес измерителя лежит в байте 3
		int sensorADDR=packetBUF[3];
//датчика c таким адресом нет в списке
		if (sensorADDR != addr)
		{
			return -7;  //wrong addr
		}

//Обработка пакетов основного протокола
		if(pr_id == MAIN_PR_211)
		{

		   ind3_common_request_type request_TYPE = (ind3_common_request_type)(int)main_pc_id;

		   switch(main_pc_id)
           {

                case  MEAS_VAL:
				   AcceptSensorMeasVal();
                break;

                case  MEAS_VER:
				   //AcceptSensorMeasVer();
				break;

				case PACK_ERR:
				   //AcceptSensorLastErr();
                break;

           }
        }

//Обработка пакетов дополнительного протокола
        if(pr_id==ADDN_PR_211)
		{

		  ind3_common_request_type request_TYPE=(ind3_common_request_type)(int)(addn_pc_id|(0x10));

           switch(addn_pc_id)
           {
				case GET_BAUD:
				   //AcceptSensorBaudRate();
                break;

                case GET_NAME:
					//AcceptSensorName();
				break;

                case GET_ZOFF:
					//AcceptSensorZeroOffset();
				break;

				case GET_SVER:
					//AcceptSensorSoftVer();
                break;

				case GET_SERN:
					 //AcceptSensorSerialNumber();
				break;

				case GET_TACT:
					 //AcceptSensorAveragingTacts();
                break;

				case GET_PERD:
					  //AcceptSensorAveragingPeriod();
				break;
            }

        }

	   //Вызываем обработчик принятых данных в выхывающем объекте
	   //if (accept_data_func!=NULL) (*accept_data_func)();

	   return 0;
}

int TProtocol_211::ChceckSumCheck(void)
{
		if (ChceckSumCalculate() != bufrx[rx_idx - 2])
		{
			 return -1; //неправильная контрольная сумма
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

		for (int i=1; i<rx_idx - 2; i++)
        {
				 chsum=chsum^bufrx[i];
        }

		return chsum;
}

int TProtocol_211::AcceptSensorMeasVal(void)
{

//D5.7 знак угла по оси X
  unsigned char signX=(bufrx[DATAOFFSET+5])&(1<<7);

//D5.6 размерность углапо оси X
  unsigned char unitsX=(bufrx[DATAOFFSET+5])&(1<<6);

//D5.5 - D4.0 целая часть угла по X
  unsigned int intX=((unsigned int) (bufrx[DATAOFFSET+5])&0x3F)<<8;
			   intX+=(unsigned int) (bufrx[DATAOFFSET+4]);

//D3.7 - D3.0 дробная часть угла по X
  unsigned int flX=(unsigned int) (bufrx[DATAOFFSET+3]);

//D2.7 знак угла по оси Y
  unsigned char signY=(bufrx[DATAOFFSET+2])&(1<<7);

//D2.6 размерность угла по оси Y
  unsigned char unitsY=(bufrx[DATAOFFSET+2])&(1<<6);

//D2.5-D1.0 - целая часть угла Y
  unsigned int intY=((unsigned int) (bufrx[DATAOFFSET+2])&0x3F)<<8;
			   intY+=(unsigned int) (bufrx[DATAOFFSET+1]);

//D3.7 - D3.0 дробная часть угла по X
  unsigned int flY=(unsigned int) (bufrx[DATAOFFSET+0]);

//Устанавливаем датчику единицы по Х
//  if (unitsX==0)        sensor_IND3->SetUnitsX(ANGLE_SECONDS);
//  else                  sensor_IND3->SetUnitsX(ANGLE_MINUTES);

//Устанавливаем датчику единицы по Y
//  if (unitsY==0)        sensor_IND3->SetUnitsY(ANGLE_SECONDS);
//  else                  sensor_IND3->SetUnitsY(ANGLE_MINUTES);

//Вычисляем и устанавливаем значение угла по X
  double angle = (double)intX+((double)flX/(double)0x100);

//Приводимк 3знакам послезапятой
//  angle*=1000; angle=floor(angle); angle/=1000;

  if (signX) angle*=-1;
  //sensor_IND3->SetAngleX(angle);
  if (unitsX)
  {
	angle *= 360;
  }

  raw_X = angle;

//Вычисляем и устанавливаем значение угла пл Y
  angle = (double)intY+((double)flY/(double)0x100);

//Приводимк 3знакам послезапятой
//  angle*=1000; angle=floor(angle); angle/=1000;

  if (signY) angle*=-1;
  //sensor_IND3->SetAngleY(angle);
  if (unitsY)
  {
	angle *= 360;
  }

  raw_Y = angle;

  return 0;
}


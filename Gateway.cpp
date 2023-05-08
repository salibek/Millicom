#include "stdafx.h"
#include "consts.h"
#include "Gateway.h"
#include "FUini.h"
#include "SchedulerEventser.h"

using namespace std;

double Gateway::DelayFunc() // Функция вычисления задержки передачи данных
{
	return MkTranslTime;
}

void Gateway::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	if (MK<0 || !(MK < FUMkRange || MK >= FUMkGlobalRange && MK < FUMkGlobalRange + FUMkRange)) // Мк не передачи
	{
//		cout << FUName <<" " << MK << endl;
//		Modeling->EventModelingPrint();
		if (Modeling != nullptr && Modeling->ManualMode && Modeling->scheduler != nullptr && !Modeling->SchedulerFlag)
		{
			Modeling->SchedulerFlag = false;
			Modeling->qmk.push_back({ MK, Load, Sender });
			((Scheduler*)(Modeling->scheduler))->Scheduling(this, ProcessingTime);
			return;
		}

		//ProcessingTime
		MkCount++; // Посчитать количетво МК, переданных через шлюз
		DataCount += Load.DataSize(); // Подсчет объема переданных данных
		if(MK>=0)
			GatewayFriend->MkAwait(-MK, Load, Sender, DelayFunc()); // Передать сообщение на парный шлюз
		else
			Receiver->MkAwait(-MK, Load, Sender, DelayFunc()); // Передать сообщение на парный шлюз

		if (Modeling != nullptr)
		{
			MaxMKQueue = max(MaxMKQueue, (int)Modeling->qmk.size()); // Вычисление максимальной длины очереди
			MaxDataSize = max(MaxDataSize, DataSize); // Вычисление максимального объема данных в буфере
			DataSize -= Load.DataSize() + 4; // Обработка МК
		}
		if(Modeling!=nullptr)
			((Scheduler*)Modeling->scheduler)->CoreFree();

	}
	else
	switch (MK%FUMkRange)
	{
	case 0: // Reset 
		Receiver = nullptr; // 
		GatewayFriend = nullptr; //
		TransmTime = 0; //
		TransError = 0; // 
		GatewayProg = nullptr; //
		Mode = 0;
		break;
	case 1: // ResiverSet Установить ссылку на роутер
		Receiver = (FU*)Load.Point;
		break;
	case 2: // RoutingSimulat Симуляция передачи данных
		ProgFU(((IC_type)Load.Point)->begin()->atr, ((IC_type)Load.Point)->begin()->Load);
		break;
	case 5: // GatewayFriendSet Установить ссылку на парный шлюз (или ФУ-модель  другого сетевого оборудования)
		GatewayFriend = (FU*)Load.Point;
		break;
//	case 10: // TransmTimeSet Установить Врея передачи единицы данных
//		TransmTime = Load.toInt();
//		break;
	case 11: // TransErrorSet Установить вероятность ошибки передачи
		TransError = Load.toInt(); 
		break;
	case 12: // ModeSet Установить решим передачи данных: симплекс от шлюза, симплекс к шлюзу, полудуплекс, дуплекс
		Mode = Load.toInt(); 
		break;
	case 13: // ProcessingTimeSet Установить время обработки запроса передачу
		ProcessingTime = Load.toDouble();
		break;
	case 15: // GatewayProgSet Установить ссылку на программу описания протокола обмена данными и моделирования передачи данных
		GatewayProg = Load.Point;
		break;
	case 16: // MkOverflowProgSet Установить ссылку на подпрограмму обработки события превышения максимального количества МК в буфере 
		OverflowProg = Load.Point; // 
		break;
	case 17: // OverflowProgSet // Установить ссылку на подпрограмму обработки события переполнения шлюза
		MkOverflowProg = Load.Point;
		break;
	case 20: // DataCounterOut Выдать объём переданных данных
		Load.Write(DataCount);
		break;
	case 21: // DataCounterOutMk Выдать МК с объёмом переданных данных
		MkExec(Load, {Cint, &DataCount },this);
		break;

	case 22: //MkTranslTimeSet Установить время передачи МК по каналу связи 
		MkTranslTime=Load.toDouble();
		break;
	case 23: // DataTransmRateSet Установить скорость передачи данных (в бит/с)
		DataTransmRate = Load.toDouble();
		break;
	case 24: // HeaderVolumSet Установить длину заголовка передаваемого пакета данных
		HeaderVolum = Load.toInt();
		break;

		// Блок статистики
	case 30: //  MaxDataBufSet Установить максимальный объем буфера данных для передачи
		 MaxDataSize = Load.toInt();
		break;
	case 35: // MaxMkBufSet Установить максимальное количество МК, помещающиеся в буфере
		MaxMKQueue = Load.toInt();
		break;
	case 40: // StatReset Сброс блока статистики
		DataCount = 0;
		MkCount = 0;
		MaxMKQueue = 0;
		AverageMKQueue = 0;
		break;
	case 45: // DataCountOut Выдать объем переданных данных через шлюз (в обе стороны)
		Load.Write(DataCount);
		break;
	case 50: // DataCountMkOut Выдать МК с объемом переданных данных через шлюз (в обе стороны)
		MkExec(Load, { Cint, &DataCount });
		break;
	case 55: // DataCountMkOutMk Выдать объем переданных данных через шлюз
		Load.Write(DataCount);
		break;
	case 60: // Выдать МК с объемом переданных данных через шлюз
		MkExec(Load, { Cint, &DataCount }, this);		break;
	case 65: // DataCountOut Выдать объем принятых данных через шлюз
//		Load.Write();
		break;
	case 70: // DataCountOutMk Выдать МК с объемом принятых данных через шлюз
//		MkExec(Load, { Cint, & });
		break;
	//
	default:
		CommonMk(MK, Load);
		break;
	}

}
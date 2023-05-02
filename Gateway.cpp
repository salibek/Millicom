#include "stdafx.h"
#include "consts.h"
#include "Gateway.h"
#include "FUini.h"

using namespace std;

void Gateway::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	if (!(MK < FUMkRange || MK >= GatewayMkRange && MK < GatewayMkRange + FUMkRange))
	{
		MkCount++; // Посчитать количетво МК, переданных через шлюз
		DataCount += Load.DataSize(); // Подсчет объема переданных данных
		GatewayFriend->ProgFU(MK, Load, this); // Передать сообщение на парный шлюз
		if (Modeling != nullptr)
		{
			MaxMKQueue = max(MaxMKQueue, (int)Modeling->qmk.size()); // Вычисление максимальной длины очереди
			MaxDataSize = max(MaxDataSize, DataSize); // Вычисление максимального объема данных в буфере
			DataSize -= Load.DataSize() + 4; // Обработка МК
		}
	}
	else
	switch (MK%FUMkRange)
	{
	case 0: // Reset 
		Router = nullptr; // 
		GatewayFriend = nullptr; //
		TranslTime = 0; //
		TransError = 0; // 
		GatewayProg = nullptr; //
		Mode = 0;
		break;
	case 1: // RouterSet Установить ссылку на роутер
		Router = (FU*)Load.Point;
		break;
	case 5: // GatewayFriendSet Установить ссылку на парный шлюз (или ФУ-модель  другого сетевого оборудования)
		GatewayFriend = (FU*)Load.Point;
		break;
	case 10: // TranslTimeSet Время передачи единицы данных
		TranslTime = Load.toInt();
		break;
	case 11: // TransErrorSet Вероятность ошибки передачи
		TransError = Load.toInt(); 
		break;
	case 12: // ModeSet Установить решим передачи данных: симплекс от шлюза, симплекс к шлюзу, полудуплекс, дуплекс
		Mode = Load.toInt(); 
		break;
	case 15: // GatewayProgSet Установить ссылку на программу описания протокола обмена данными и моделирования передачи данных
		GatewayProg = Load.Point;
		break;
	case 16: // MkOverflowProgSet Установить ссылку на подпрограмму обработки события переполнения шлюза
		OverflowProg = nullptr; //
		break;
	case 17: // OverflowProgSet Установить ссылку на подпрограмму обработки события превышения максимального количества МК в буфере
		MkOverflowProg = nullptr;
		break;
	case 20: // DataCounterOut
		Load.Write(DataCount);
		break;

	// Блок статистики
	case 25: // TranslTimeSet Установить скорость передачи данных
		TranslTime = Load.toDouble();
		break;
	case 30: //  Установить максимальный объем буфера данных для передачи
		 MaxDataSize = Load.toInt();
		break;
	case 35: // Установить максимальное количество МК, помещающиеся в буфере
//		MaxDataSize = Load.toInt();
		break;
	case 40: // StatsReset Сброс блока статистики
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
	case 55: // Выдать объем переданных данных через шлюз
//		Load.Write();
		break;
	case 60: // Выдать МК с объемом переданных данных через шлюз
//		MkExec(Load, { Cint, & });
		break;
	case 65: // Выдать объем принятых данных через шлюз
//		Load.Write();
		break;
	case 70: // Выдать МК с объемом принятых данных через шлюз
//		MkExec(Load, { Cint, & });
		break;
	//
	default:
		CommonMk(MK, Load);
		break;
	}
}
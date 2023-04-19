#include "stdafx.h"
#include "consts.h"
#include "Gateway.h"
#include "FUini.h"

using namespace std;

void Gateway::ProgFU(int MK, LoadPoint Load)
{
	if (!(MK < FUMkRange || MK >= GatewayMkRange && MK < GatewayMkRange + FUMkRange))
	{
		MkCount++; // Посчитать количетво МК, переданных через шлюз
		GatewayFriend->ProgFU(MK, Load); // Передать сообщение на парный шлюз
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
	case 5: // Установить ссылку на парный шлюз (или ФУ-модель  другого сетевого оборудования)
		GatewayFriend = (FU*)Load.Point;
		break;
	case 10: // Время передачи единицы данных
		TranslTime = Load.toInt();
		break;
	case 11: // Вероятность ошибки передачи
		TransError = Load.toInt(); 
		break;
	case 12: // Установить решим передачи данных: симплекс от шлюза, симплекс к шлюзу, полудуплекс, дуплекс
		Mode = Load.toInt(); 
		break;
	case 15: // Установить ссылку на программу описания протокола обмена данными и моделирования передачи данных
		GatewayProg = Load.Point;
		break;
	case 16: // Установить ссылку на подпрограмму обработки события переполнения шлюза
		OverflowProg = nullptr; //
		break;
	case 17: // Установить ссылку на подпрограмму обработки события превышения максимального количества МК в буфере
		MkOverflowProg = nullptr;
		break;
	case 20: // DataCounterOut
		Load.Write(DataCount);
		break;

	// Блок статистики
	case 25: // Установить скорость передачи данных
		break;
	case 30: // Установить максимальный объем буфера данных для передачи
		break;
	case 35: // Установить максимальное количество МК, помещающиеся в буфере
		break;
	case 40: // Сброс блока статистики
		break;
	case 45: // Выдать объем переданных данных через канал (в обе стороны)
		break;
	case 50: // Выдать МК с объемом переданных данных через канал (в обе стороны)
		break;
	case 55: // Выдать объем переданных данных через канал
		break;
	case 60: // Выдать МК с объемом переданных данных через канал
		break;
	case 65: // Выдать объем принятых данных через канал
		break;
	case 70: // Выдать МК с объемом принятых данных через канал
		break;
	//

	default:
		CommonMk(MK, Load);
		break;
	}
}
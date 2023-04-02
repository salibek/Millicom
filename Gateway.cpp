#include "stdafx.h"
#include "consts.h"
#include "Gateway.h"
#include "FUini.h"

using namespace std;

void Gateway::ProgFU(int MK, LoadPoint Load)
{

	switch (MK)
	{
	case 0: // Reset Сброс
		Router = nullptr; // Ссылка на роутер
		GatewayFriend = nullptr; //Ссылка на парный шлюз
		TranslTime = 0; // Время передачи единицы данных
		TransError = 0; // Вероятность ошибки передачи
		RoutingProg = nullptr; //
		Mode = 0;
		break;
	case 1: // RouterSet Установить ссылку на роутер
		Router = (FU*)Load.Point;
		break;
	case 5:
		GatewayFriend = (FU*)Load.Point;
		break;
	case 10: //
		TranslTime = Load.toInt(); // Время передачи единицы данных
		break;
	case 11: //
		TransError = Load.toInt(); // Вероятность ошибки передачи
		break;
	case 12: //
		Mode = Load.toInt(); // 0 - дуплекс, 1 - симплекс от шлюза, 2 - симплекс к шлюзу, 3 - полудуплекс
		break;
	case 15: //
		RoutingProg = Load.Point; //
		break;
	case 16: //
		OverflowProg = nullptr; //
		break;
	case 17: //
		MkOverflowProg = nullptr; //
		break;

	case 20: // InDataCounterOut
		Load.Write(Indata);
		break;
	case 21: // InDataCounterOutMk
		MkExec(Load, { Cint, &Indata });
		break;

	default:
		CommonMk(MK, Load);
		break;
	}
}
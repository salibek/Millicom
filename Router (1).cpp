#include "stdafx.h"
#include "consts.h"
#include "Router.h"
#include "FUini.h"

using namespace std;

void Router::ProgFU(int MK, LoadPoint Load)
{

	switch (MK)
	{
	case 200: // Route Произвести марштуризацию
		if (Load.Point != nullptr)
		{
			bool findDiap = false;
			ip ipInfo;
			ipInfo.Load.Point = Load.Point;

			for (int i = 0; i < Channels.size(); i++)// поиск диапазона
				if ((ipInfo.atr > Channels[i].down) && (ipInfo.atr < Channels[i].Up))
					findDiap = true;
			//(ip*)Load.Point->atr - Милликоманда, по которой маршрутизируют...
			if (!findDiap)
				ProgExec(ErrRoutingProg);
		}
		break;
	case 0: // Сброс
		Channels.clear();
		break;
	case 1: // ChCreate Создать новый канал (на входе МК для приемника)
		Channels.push_back({0,0,nullptr,Load.ToInt(-1) });
		break;
	case 5: // DelayTimeSet установить время передачи МК
		DelayTime = Load.ToDouble();
		break;
	case 8: // ErrRoutingProgset Ссылка на программу обработки ошибки "Не найден маршрут"
		ErrRoutingProg = Load.Point;
		break;
	case 10: // ReceiverSet установить адрес ФУ-приемника
		Channels.back().Receiver = Load.Point;
		break;
	case 11: // ReceiverMkSet установить МК ФУ-приемника
		Channels.back().ReceiverMk = Load.ToInt(-1);
		break;
	case 15: // установить верхний диапазон адресов
		Channels.back().Up= Load.ToInt(-1);
		break;
	case 20: // установить нижний диапазон адресов
		Channels.back().down = Load.ToInt(-1);
		break;
	case 25: // выдать количество переданных МК
		Load.Write(int(MKCount));
		break;
	case 30: // выдать МК с количеством переданных МК
		Load.Write(MKCount);
		break;
	case 35: // выдать макс длину очереди МК
		Load.Write(MaxMKQueue);
		break;
	case 40: // выдать МК с макс длиной очереди МК
	{
		MkExec(Load, { Cint,&MKCount });
		break;
	}
	case 45: // выдать среднюю длину очереди МК
		Load.Write(double(AverageMKQueue));
		break;
	case 50:// выдать МК со средней длиной очереди МК
	{
		double t;
		if (CurrentTime > 0)
			t = AverageMKQueue / CurrentTime;
		else
			t = 0;
		MkExec(Load, { Cdouble,&t });

		break;
	}
	default:
		CommonMk(MK, Load);
		break;
	}
}
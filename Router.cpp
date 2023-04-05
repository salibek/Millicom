#include "stdafx.h"
#include "consts.h"
#include "Router.h"
#include "FUini.h"

using namespace std;

void Router::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case 0: // Сброс
		Channels.clear();
		MKCount = 0;
		break;
	case 1: // Создать новый канал (на входе МК нижняя граница диапазона)
	{
		Channels.push_back({ 0,Load.toInt(-1),Bus,-1 });
		if (Channels.back().Down >= 0)
			Channels.back().Up = Channels.back().Down + FUMkRange; // Установка верхней границы по умолчанию
		break;
	}
	case 3: // установить индекс текущего канала роутера
		break;
	case 5: // установить верхний диапазон МК для роутера (нижний диапазон подразумевается, как верхний плюс диапазон МК)
		break;
	case 6: // установить время маршрутизации
		RoutingTime = Load.toDouble();
		break;
	case 8: // установить индекс текущего канала связи
		break;
	case 9: // установить верхний диапазон адресов канала
		Channels.back().Up = Load.toInt(-1);
		break;
	case 10: // установить нижний диапазон адресов канала
	{
		Channels.back().Down = Load.toInt(-1);
		if (Channels.back().Up < 0)
			Channels.back().Up = Channels.back().Down + FUMkRange; // Автоматическая установка верхнего предела интервала равного мк-диапазону по умолчанию
		break;
	}
	case 11: // установить ссылку на шлюз для текущего канала связи
		Channels[Ind].Receiver = Load.Point;
		break;
	case 15: // выдать количество каналов на роутере
		Load.Write(Channels.size());
		break;
	case 20: // выдать МК с количеством каналов на роутере
		{
		int t = Channels.size();
		MkExec(Load, { Cint, &t });
		break;
		}
	case 25: // выдать указатель на шлюз для текущего канала
		Load.Write(Channels[Ind].Receiver);
		break;
	case 30: // выдать МК с указателем на шлюз для текущего канала
		MkExec(Load, { CFU, &Channels[Ind].Receiver });
		break;
	case 35: // выдать верхний диапазон адресов для текущего канала
		Load.Write(Channels[Ind].Up);
		break;
	case 40: // выдать МК с верхним диапазоном адресов для текущего канала
		MkExec(Load, { Cint, &Channels[Ind].Up });
	case 45: // выдать нижний диапазон для текущего канала
		break;
	case 50:// выдать МК с нижним диапазоном для текущего канала
		{
		break;
		}
	default:
		CommonMk(MK, Load);
		break;
	}
}
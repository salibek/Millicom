#include "stdafx.h"
#include "consts.h"
#include "Router.h"
#include "FUini.h"

using namespace std;

void Router::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case 200: // Routing Произвести марштуризацию MK
		if (Load.Point != nullptr)
		{
			// поиск диапазона
			auto i = Channels.begin();
			for (; i != Channels.end(); i++)
				if (Load.Type >> 1 == DIP)
				{
					if (((ip*)Load.Point)->atr >= i->Down && ((ip*)Load.Point)->atr < i->Up)
					{
						if(i->ReceiverMk<0)
							((FU*)i->Receiver)->ProgFU(((ip*)Load.Point)->atr, ((ip*)Load.Point)->Load); // Переслать приёмнику без специальной МК
						else
							((FU*)i->Receiver)->ProgFU(i->ReceiverMk, Load); // Переслать приёмнику
						MKCount++;
						break;						
					}
				}
				else if (Load.Type >> 1 == DIC)
					if (((IC_type)Load.Point)->begin()->atr >= i->Down && ((IC_type)Load.Point)->begin()->atr < i->Up)
						{
						if (i->ReceiverMk < 0)
							((FU*)i->Receiver)->ProgFU(((IC_type)Load.Point)->begin()->atr, ((IC_type)Load.Point)->begin()->Load); // Переслать приёмнику без специальной МК
						else
							((FU*)i->Receiver)->ProgFU(i->ReceiverMk, Load); // Переслать приёмнику
						MKCount++;
						break;
						}
			if (i == Channels.end())
				ProgExec(ErrRoutingProg); // Ошибка маршрутизации
		}
		break;
	case 0: // Сброс
		Channels.clear();
		MKCount = 0;
		break;
	case 1: // ChCreate Создать новый канал (на входе МК нижняя граница диапазона)
		Channels.push_back({0,Load.ToInt(-1),Bus,-1});
		if (Channels.back().Down >= 0)
			Channels.back().Up = Channels.back().Down + FUMkRange; // Установка верхней границы по умолчанию
		break;
	case 3: //ChClear Сбросить настройки каналов
		Channels.clear();
		break;
	case 5: // DelayTimeSet Установить время передачи МК
		DelayTime = Load.ToDouble();
		break;
	case 6: // MkCounterSet Установить значение счетчика МК
		MKCount = Load.ToInt();
		break;
	case 8: // ErrRoutingProgset Ссылка на программу обработки ошибки "Не найден маршрут"
		ErrRoutingProg = Load.Point;
		break;
	case 9: // ReceiverMkDefaultSet Установить значение МК для приемника по умолчанию
		ReceiverMkDefault = Load.ToInt(-1);
		break;
	case 10: // ReceiverSet Установить адрес ФУ-приемника
		Channels.back().Receiver = Load.Point;
		break;
	case 11: // ReceiverMkSet Установить МК ФУ-приемника
		Channels.back().ReceiverMk = Load.ToInt(-1);
		break;
	case 15: // RangeUpSet Установить верхний диапазон адресов
		Channels.back().Up= Load.ToInt(-1);
		break;
	case 20: // RangeDownSet Установить нижний диапазон адресов
		Channels.back().Down = Load.ToInt(-1);
		if (Channels.back().Up < 0)
			Channels.back().Up = Channels.back().Down + FUMkRange; // Автоматическая установка верхнего предела интервала равного мк-диапазону по умолчанию
		break;
	case 25: // MkCountOut Выдать количество переданных МК
		Load.Write(int(MKCount));
		break;
	case 30: // MkCountOutMk Выдать МК с количеством переданных МК
		Load.Write(MKCount);
		break;
	case 35: // MaxMkQeueOut Выдать макс длину очереди МК
		Load.Write(MaxMKQueue);
		break;
	case 40: // MaxMkQeueOutMk Выдать МК с макс длиной очереди МК
	{
		MkExec(Load, { Cint,&MKCount });
		break;
	}
	case 45: // EverageMkQeueOut Выдать среднюю длину очереди МК
		Load.Write(double(AverageMKQueue));
		break;
	case 50:// EverageMkQeueOutMk Выдать МК со средней длиной очереди МК
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
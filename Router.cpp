#include "stdafx.h"
#include "consts.h"
#include "Router.h"
#include "FUini.h"

using namespace std;

int Router::ChennelSearch(int MK) // Поиск канала по МК (Возврашает -1, если канал не найден)
{
	auto i = Channels.begin();
	for (; i != Channels.end(); i++)
		if (i->Up <= MK < i->Down) break;
	if (i == Channels.end()) // Канал не нейден
		return -1;
	return distance(Channels.begin(), i); // Вернуть номер канала
}

void Router::ProgFU(int MK, LoadPoint Load)
{
	if (!(MK < FUMkRange || MK >= FUMkGlobalRange && MK < FUMkGlobalRange + FUMkRange)) // Routing
	{
		// Маршрутизация
		// SendInd = ChennelSearch(MK); // Определить номер канала, по которому пришла МК для маршрутизации

		ProgExec(ReceiveProg); // Запустить программу по приходу данных
		MKCount++;
		LaslMkIp = {MK,Load}; // Запоминание последней пришедшей для маршрутизации МК
		if (RoutingProg != nullptr)
		{
			ProgExec(RoutingProg); // Запуск програмы ручного управления
			return;
		}
		// Стандартная программа машрутизации
		SendInd = -1;
		for (auto i = Channels.begin(); i != Channels.end(); i++)
			if (i->Up <= MK && MK < (i->Down)){
				SendInd = distance(Channels.begin(), i);
				break;
			}
		if (SendInd >= 0) // Стандартная маршрутизация
		{
			Channels[SendInd].MkCount++; // Посчитали МК
			Channels[SendInd].DataCount += Load.DataSize();
			((FU*) Channels[SendInd].Receiver)->ProgFU(MK,Load);
		}

	}
	else // МК для роутера
	switch (MK%FUMkRange)
	{
	case 0: // Сброс
		Channels.clear();
		MKCount = 0;
		break;
	// Настройки и управление маршрутизатором
	case 1: // ChCreate Создать новый канал (на входе МК верхняя граница диапазона)
	{
		//if (Load.Point == nullptr) break;
		Channels.push_back({ Load.toInt(-1),0 ,Bus,-1 });
		if (Channels.back().Up >= 0)
			Channels.back().Down = Channels.back().Up + FUMkRange; // Установка верхней границы по умолчанию
		Ind = Channels.size() - 1; // Настроить индекс на последний созданный канал
		break;
	}
	case 2: // RoutingSimulat Эмуляции прихода МК на маршрутизатор
		ProgFU(((IC_type)Load.Point)->begin()->atr, ((IC_type)Load.Point)->begin()->Load);
		break;
	case 3: // IndSet Установить индекс текущего канала роутера
		Ind = Load.toInt();
		break;
	case 5: // RangeSet Установить верхний диапазон МК для роутера (нижний диапазон подразумевается, как верхний плюс диапазон МК)
		FUMkGlobalRange = Load.toInt();
		break;
	case 6: // RouterTimeSet Установить время маршрутизации
		RoutingTime = Load.toDouble();
		break;
	case 9: // ChRangeUpSet Установить верхний диапазон адресов канала
		Channels[Ind].Up = Load.toInt(-1);
		break;
	case 10: // ChRangeDownSet Установить нижний диапазон адресов канала
	{
		Channels[Ind].Down = Load.toInt(-1);
		//if (Channels.back().Up < 0)
		//	Channels.back().Up = Channels.back().Down + FUMkRange; // Автоматическая установка верхнего предела интервала равного мк-диапазону по умолчанию
		break;
	}
	case 11: // ChGatewaySet Установить ссылку на шлюз для текущего канала связи
		Channels[Ind].Receiver = (FU*)Load.Point;
		break;
	case 15: // ChNumOut Выдать количество каналов на роутере
		Load.Write(Channels.size());
		break;
	case 20: // ChNumOutMk Выдать МК с количеством каналов на роутере
	{
		int t = Channels.size();
		MkExec(Load, { Cint, &t });
		break;	}
	case 25: // ChGatewayOut Выдать указатель на шлюз для текущего канала
		Load.Write(Channels[Ind].Receiver);
		break;
	case 30: // ChGatewayOutMk Выдать МК с указателем на шлюз для текущего канала
		MkExec(Load, { CFU, &Channels[Ind].Receiver });
		break;
	case 35: // ChRangeUpOut Выдать верхний диапазон адресов для текущего канала
		Load.Write(Channels[Ind].Up);
		break;
	case 40: // ChRangeUpOutMk Выдать МК с верхним диапазоном адресов для текущего канала
		MkExec(Load, { Cint, &Channels[Ind].Up });
	case 45: // ChRangeDownOut Выдать нижний диапазон для текущего канала
		Load.Write(Channels[Ind].Down);
		break;
	case 50:// ChRangeDownOutMk Выдать МК с нижним диапазоном для текущего канала
		MkExec(Load, { Cint, &Channels[Ind].Down });
		break;
	//
	// Подпрограммы
	case 55: // RoutingProgSet Установить ссылку на программу маршрутизации
		RoutingProg = Load.Point;
		break;
	case 60: // OverflowProgSet Установить ссылку на подпрограмму реакции на переполнение буфера для хранения данных
		OverflowProg = Load.Point;
		break;
	case 61: // MkOverflowProgSet Установить ссылку на подпрограмму реакции на переполнение буфера МК
		MkOverflowProg = Load.Point;
		break;
	case 62: // RoutindErrProgSet Установить ссылку на программу обработки события адрес МК для маршрутизации не найден в таблице маршрутизации
		RoutingErrProg = Load.Point;
		break;
		//
	// «Ручное» управление
	case 65: // RoutingAttributeOut Выдать атрибут пришедшей для маршрутизации МК
		Load.Write(LaslMkIp.atr);
		break;
	case 66: // RoutingMkOut
	//	Load.Write(LaslMkIp);
		break;
	case 70: // RoutingAttributeOutMk Выдать МК с атрибутом пришедшей для маршрутизации МК
		MkExec(Load, { Cint, &LaslMkIp.atr });
		break;
	case 71: // RoutingAttributeOutMk Выдать МК с атрибутом пришедшей для маршрутизации МК
		MkExec(Load, { CIP, &LaslMkIp });
		break;
		//	case 85: // ChRoutingMkOut Выдать индекс канала, по которому пришла последняя МК для маршрутизации
//		Load.Write(Ind);
//		break;
//	case 90: // ChRoutingMkOutMk Выдать МК с каналом, по которому пришла последняя МК для маршрутизации
//		MkExec(Load, { Cint, &Channels[Ind] });
//		break;
	case 105: // Переслать последнюю МК в канал с номером (номер канала в нагрузке МК для роутера)
		Channels[Load.toInt()].Receiver->ProgFU(LaslMkIp.atr, LaslMkIp.Load);
		break;
//	case 120: // ReceiveIndOut
//		Load.Write(SendInd);
//		break;
//	case 125: // ReceiveIndOutMk Выдать МК с индексом канала, на который пришел пакет для маршрутизации
//		MkExec(Load, { Cint, &SendInd });
//		break;
	//
	// Блок статистики
	case 320: // RouterStatsClear Сбросить блок статистики переданных данных через роутер
		MKCount = 0;
		MaxMKQueue = 0;
		AverageMKQueue = 0;
		for (auto i = Channels.begin(); i != Channels.end(); i++)
		{
			i->MkCount = 0;
			i->DataCount = 0;
		};
		break;
	case 325: // RouterDataCountOut Выдать объем переданных данных через роутер
	{
		int sum = 0;
		for (auto i = Channels.begin(); i != Channels.end(); i++)
			sum += i->DataCount;
		Load.Write(sum);
		break;
	}
	case 330: // RouterDataInCountOutMk Выдать МК с объемом переданных данных через роутер
	{
		int sum = 0;
		for (auto i = Channels.begin(); i != Channels.end(); i++)
			sum += i->DataCount;
		MkExec(Load, { Cint, &sum });
		break;
	}
	case 335: // ChDataInCountOut Выдать объем переданных данных через текущий канал (по индексу текущего канала)
		Load.Write(Channels[Ind].DataCount);
		break;
	case 340: // ChDataInCountOutMk Выдать МК с объемом переданных данных через канал
		MkExec(Load, { Cint, &Channels[Ind].DataCount });
		break;
	case 345: // RouterMkCountOut Выдать количество переданных МК через роутер
		Load.Write(MKCount);
		break;
	case 350: // RouterMkCountOutMk Выдать МК с количеством переданных МК через роутер
		MkExec(Load, { Cint, &MKCount });
		break;
	case 355: // ChMkInCountOut Выдать количество переданных МК через канал
		Load.Write(Channels[Ind].MkCount);
		break;
	case 360: // ChMkInCountOutMk Выдать МК с количеством переданных МК через канал
		MkExec(Load, { Cint, &Channels[Ind].MkCount });
		break;
	case 365: // RouterMaxMkQueueOut Выдать макс длину очереди МК на роутере
		Load.Write(MaxMKQueue);
		break;
	case 370: // RouterMaxMkQueueOutMk Выдать МК с макс длиной очереди МК на роутере
		MkExec(Load, { Cint, &MaxMKQueue });
		break;
	case 375: // RouterAverageMkQueueOut Выдать среднюю длину очереди МК на роутере
		Load.Write(AverageMKQueue);
		break;
	case 380: // RouterAverageMkQueueOuMk Выдать МК со средней длиной очереди МК на роутере
		MkExec(Load, { Cint, &AverageMKQueue });
		break;
	case 395: // ReceiveProgSet Установить ссылку на программу, активизирующуюся при приходе МК для маршрутизации (для реализации автоматического сбора статистики)
		ReceiveProg = Load.Point;
		break;
	//
	default:
		CommonMk(MK, Load);
		break;
	}
}
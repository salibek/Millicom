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
	if (!(MK < FUMkRange || MK >= RouterMkRange && MK < RouterMkRange + FUMkRange))
	{
		// Маршрутизация
		SendInd = ChennelSearch(MK); // Определить номер канала, по которому пришла МК для маршрутизации
		ProgExec(ReceiveProg); // Запустить программу по приходу данных
		MKCount++;

		if (RoutingProg != nullptr)
		{
			ProgExec(RoutingProg); // Запуск програмы ручного управления
			return;
		}

		if (SendInd >= 0) // Стандартная маршрутизация
		{
			Channels[SendInd].MkOutCount++; // Посчитали МК
			((FU*) Channels[SendInd].Receiver)->ProgFU(MK,Load);
		}
		// Стандартная программа машрутизации

	}
	else // МК для роутера
	switch (MK%FUMkRange)
	{
	case 0: // Сброс
		Channels.clear();
		MKCount = 0;
		break;
	// Настройки и управление маршрутизатором
	case 1: // Создать новый канал (на входе МК нижняя граница диапазона)
	{
		Channels.push_back({ 0,Load.toInt(-1),Bus,-1 });
		if (Channels.back().Down >= 0)
			Channels.back().Up = Channels.back().Down + FUMkRange; // Установка верхней границы по умолчанию
		break;
	}
	case 3: // установить индекс текущего канала роутера
		Ind = Load.toInt();
		break;
	case 5: // установить верхний диапазон МК для роутера (нижний диапазон подразумевается, как верхний плюс диапазон МК)
		RouterMkRange = Load.toInt();
		break;
	case 6: // установить время маршрутизации
		RoutingTime = Load.toDouble();
		break;
	case 9: // установить верхний диапазон адресов канала
		Channels[Ind].Up = Load.toInt(-1);
		break;
	case 10: // установить нижний диапазон адресов канала
	{
		Channels[Ind].Down = Load.toInt(-1);
		if (Channels.back().Up < 0)
			Channels.back().Up = Channels.back().Down + FUMkRange; // Автоматическая установка верхнего предела интервала равного мк-диапазону по умолчанию
		break;
	}
	case 11: // Установить ссылку на шлюз для текущего канала связи
		Channels[Ind].Receiver = Load.Point;
		break;
	case 15: // Выдать количество каналов на роутере
		Load.Write(Channels.size());
		break;
	case 20: // выдать МК с количеством каналов на роутере
	{
		int t = Channels.size();
		MkExec(Load, { Cint, &t });
		break;	}
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
		Load.Write(Channels[Ind].Down);
		break;
	case 50:// выдать МК с нижним диапазоном для текущего канала
		MkExec(Load, { Cint, &Channels[Ind].Down });
		break;
	//
	// Подпрограммы
	case 55: // установить ссылку на программу маршрутизации
		RoutingProg = Load.Point;
		break;
	case 60: // установить ссылку на подпрограмму реакции на переполнение буфера для хранения данных
		OverflowProg = Load.Point;
		break;
	case 61: // установить ссылку на подпрограмму реакции на переполнение буфера МК
		MkOverflowProg = Load.Point;
		break;
	//
	// «Ручное» управление
	case 65: // выдать атрибут пришедшей для маршрутизации МК
		break;
	case 70: // выдать МК с атрибутом пришедшей для маршрутизации МК
		break;
	case 85: // выдать канал, по которому пришла последняя МК для маршрутизации
		break;
	case 90: // выдать МК с каналом, по которому пришла последняя МК для маршрутизации
		break;
	case 105: // переслать последнюю МК в канал с номером (номер канала в нагрузке МК для роутера)
		break;
	case 110: // установить ссылку на программу, обрабатывающую событие «переполнение буфера памяти»
		break;
	case 115: // установить ссылку на программу обработки события адрес МК для маршрутизации не найден в таблице маршрутизации
		break;
	case 120: // ReceiveIndOut
		Load.Write(SendInd);
		break;
	case 125: // ReceiveIndOutMk Выдать МК с индексом канала, на который пришел пакет для маршрутизации
		MkExec(Load, { Cint, &SendInd });
		break;
	//
	// Блок статистики
	case 320: // сбросить блок статистики переданных данных через роутер
		  // Сброс всех счетчиков!!!
		break;
	case 325: // выдать объем переданных данных через роутер
		break;
	case 330: // выдать МК с объемом переданных данных через роутер
		break;
	case 335: // выдать объем переданных данных через текущий канал (по индексу текущего канала)
		break;
	case 340: // выдать МК с объемом переданных данных через канал
		break;
	case 345: // выдать количество переданных МК через роутер
		break;
	case 350: // выдать МК с количеством переданных МК через роутер
		break;
	case 355: // выдать количество переданных МК через канал
		Load.Write(MKCount);
		break;
	case 360: // выдать МК с количеством переданных МК через канал
		break;
	case 365: // выдать макс длину очереди МК на роутере
		Load.Write(MaxMKQueue);
		break;
	case 370: // выдать МК с макс длиной очереди МК на роутере
		break;
	case 175: // выдать среднюю длину очереди МК на роутере
		Load.Write(AverageMKQueue);
		break;
	case 380: // выдать МК со средней длиной очереди МК на роутере
		MkExec(Load, { Cint, &AverageMKQueue });
		break;
	case 395: // установить ссылку на программу, активизирующуюся при приходе МК для маршрутизации (для реализации автоматического сбора статистики)
		ReceiveProg = Load.Point;
		break;
	//
	default:
		CommonMk(MK, Load);
		break;
	}
}
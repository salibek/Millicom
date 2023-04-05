// ФУ Маршрутизатор
#pragma once

#include "FUini.h"

class Channel1 // Канал передачи данных
{
public:
	int Up = -1, Down = -1; // Верхний и нижний диапазоны
	void* Receiver = nullptr; // Приемник данных
	int ReceiverMk = -1; // МК для приемника данных
};

class Router : public FU
{
	int MkRangeStart = -1; // диапазоны адресов МК для каждого из каналов (если нижний диапазон не указан, то он считается равным верхний диапазон + диапазон МК для устройства)
//	vector<FU*> Gateways; // вектор ссылок на шлюзы (входные и выходные)
	vector<Channel1> Channels; // вектор ссылок на каналы связи

	int DelayTime = 0; // время задержки передачи данных
	vector<FU*> Queue; // очередь ожидания отправки (очередь МК)
	void* RoutingProg = nullptr; // ссылка на программу маршрутизации
	// ссылки на программы обработки исключительных ситуаций 
	int Ind = 0; // Индекс текущего канала связи (чтобы можно было проводить его настройку) При создании нового канала он автоматически устанавливается на номер последнего канала, чтобы можно было производить его настройку
	void* OverflowProg = nullptr; // нехватка оперативной памяти для пересылаемых данных
	void* MkOverflowProg = nullptr; //
	//
	//блок характеристик роутера
	int MaxMemorySize = 0; // макс. объем памяти
	double RoutingTime = 0; // время обработки
	// 
	// блок статистики передачи
	int MKCount = 0; // Количество переданных МК
	int MaxMKQueue = 0; // Максимальная длина очереди
	double AverageMKQueue = 0; // Средняя длина очереди
	//

public:
	void ProgFU(int MK, LoadPoint Load) override;
	Router(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 20;
		ProgFU(0, { 0, nullptr });
	};
};
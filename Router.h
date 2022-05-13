// ФУ Маршрутизатор
#pragma once

#include "FUini.h"

class Chanel // Канал передачи данных
{
public:
	int Up = -1, Down = -1; // Верхний и нижний диапазоны
	void* Receiver = nullptr; // Приемник данных
	int ReceiverMk = -1; // МК для приемника данных
};

class Router : public FU
{
	vector<Chanel> Channels; // Список каналов
	void* ErrRoutingProg = nullptr; // Ссылка на программу обработки ошибки "Не найден маршрут"
	int DelayTime = 0; // Время задержки
//	vector<FU*>Queue; // Очередь МК
	int MKCount = 0; // Количество переданных МК
	int MaxMKQueue = 0; // Максимальная длина очереди
	double AverageMKQueue = 0; // Средняя длина очереди
	double CurrentTime = 0; // Текущее время моделирования
	bool WorkMode = false; // Режим работы
	int ReceiverMkDefault = -1; // МК по умолчанию для приемника сообщений

public:
	void ProgFU(int MK, LoadPoint Load) override;
	Router(FU* BusContext, FU* Templ)
	{
		Bus = BusContext; ProgFU(0, { 0, nullptr });
	};

};
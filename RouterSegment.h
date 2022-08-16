#pragma once
#include "Consts.h"

class SegChannel : FU // Канал сегментного шлюза
{
public:
	void* Router; // Ссылка на 
	int Id = -1; // Идентифакатор (индекс) канала 
	//SegChannel(void* parent, FU* Templ = nullptr) { Bus = (FU*)parent; ProgFU(0, { 0, nullptr }); };
	void ProgFU(int MK, LoadPoint Load) override;
};

class RouterSegment : FU // Сегментный роутер
{
	vector<SegChannel> InChannels; // Вектор входных каналов
	vector<FU*> OutCannels; // Выходные каналы, содержащие ссылки на ФУ-приемники
	int DelayTime = 0; // Время задержки
	int MKCount = 0; // Количество переданных МК
	int MaxMKQueue = 0; // Максимальная длина очереди
	double AverageMKQueue = 0; // Средняя длина очереди
	double CurrentTime = 0; // Текущее время моделирования
public:
	RouterSegment(void* parent, FU* Templ = nullptr) { FUtype = 21; Bus = (FU*)parent; ProgFU(0, { 0, nullptr }); };
	void ProgFU(int MK, LoadPoint Load) override;
	void Routing(int MK, LoadPoint Load, int ChInd);
};
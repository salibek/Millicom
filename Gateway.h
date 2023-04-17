// ФУ Шлюз
#pragma once

#include "FUini.h"

class Gateway : public FU
{
	int GatewayMkRange = 0; // Диапазон МК для шлюза
	FU* Router = nullptr; // Ссылка на роутер
	FU* GatewayFriend = nullptr; //Ссылка на парный шлюз
	void* GatewayProg = nullptr; // Программа ручного управления для передачи данных
	void* ReceiveProg = nullptr; // Программа обработки события прихода данных для передачи (например обработка статистики)
	void* OverflowProg = nullptr; // Программа обработка события переполнения буфера данных
	void* MkOverflowProg = nullptr; // рограмма обработка события переполнения буфера МК
	int Mode = 0; // 0 - дуплекс, 1 - симплекс от шлюза, 2 - симплекс к шлюзу, 3 - полудуплекс
	// Блок характеристик роутера
	double TranslTime = 0; // Время передачи единицы данных
	double MkTranslTime = 0; // Время передачи МК (без учета объема данных в нагрузке МК)
	double TransError = 0; // Вероятность ошибки передачи
	int MaxMemorySize = 0; // макс. объем памяти
	double RoutingTime = 0; // время обработки
	// Блок статитики
	int DataCount = 0, MkCount=0; // Объем данных и количество МК переденных через шлюз
	int MaxMKQueue = 0; // Максимальная длина очереди
	double AverageMKQueue = 0; // Средняя длина очереди
public:
	void ProgFU(int MK, LoadPoint Load) override;
	Gateway(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 21;
		ProgFU(0, { 0, nullptr });
	};
};
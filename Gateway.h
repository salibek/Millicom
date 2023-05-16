// ФУ Шлюз
#pragma once

#include "FUini.h"

class Gateway : public FU
{
public:
	double DelayFunc(); // Функция вычисления задержки передачи данных
//	int GatewayMkRange = 0; // Диапазон МК для шлюза
	double DataTransmRate = 0; // Скорость передачи данных (в бит/с)
	int HeaderVolum = 0; // Длина заголовка передаваемого пакета данных
	FU* Receiver = nullptr; // Ссылка на роутер
	FU* GatewayFriend = nullptr; //Ссылка на парный шлюз
	void* GatewayProg = nullptr; // Программа ручного управления для передачи данных
	void* ReceiveProg = nullptr; // Программа обработки события прихода данных для передачи (например обработка статистики)
	void* OverflowProg = nullptr; // Программа обработка события переполнения буфера данных
	void* MkOverflowProg = nullptr; // Программа обработка события переполнения буфера МК
	int Mode = 0; // 0 - дуплекс, 1 - симплекс от шлюза, 2 - симплекс к шлюзу, 3 - полудуплекс
	// Блок характеристик роутера
	double TransmTime = 0; // Время передачи единицы данных
	double MkTranslTime = 0; // Время передачи МК (без учета объема данных в нагрузке МК)
	double TransError = 0; // Вероятность ошибки передачи
	int MaxMemorySize = 0; // макс. объем памяти
	double ProcessingTime = 0; // время обработки запроса на передачу
	// Блок статитики
	int DataCount = 0, MkCount=0; // Объем данных и количество МК переденных через шлюз
	int MaxMKQueue = 0; // Максимальная длина очереди
	double AverageMKQueue = 0; // Средняя длина очереди
	int DataSize = 0, MaxDataSize = 0; // Текущий объем занятого буфера и максимальный объем занятого буфера
	FU* Eventser = nullptr; // Ссылка на контроллер событий
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	Gateway(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 21;
		ProgFU(0, { 0, nullptr });
	};
	Gateway(){Gateway(nullptr, nullptr);}
};
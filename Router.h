// ФУ Маршрутизатор
#pragma once

#include "FUini.h"
#include "SchedulerEventser.h"


class channel // Канал передачи данных
{
public:
	long int Up = -1, Down = -1; // Верхний и нижний диапазоны
	FU* Receiver = nullptr; // Приемник данных
	long int ReceiverMk = -1; // МК для приемника данных

	long int MkOutCount = 0; // Счетчик отправленных МК их канала
	long int DataOutCount = 0; // Счетчик количества отправленных данных из канал
	long int MkInCount = 0; // Счетчик принятых МК в канал
	long int DataInCount = 0; // Счетчик количества принятых данных в канал
	double Delay = 0; // Задержка передачи данынх по каналу
};

class Router : public FU
{
public:
	double DelayGen(channel ch); //Генерация задержки передачи данных
	// Если начало и конец диапазонов равны нулю, то в данный канал пересылаются любые МК (т.е. это - канал для отправки МК по умолчанияю)

//	int MkRangeStart = -1; // Диапазоны адресов МК для каждого из каналов (если нижний диапазон не указан, то он считается равным верхний диапазон + диапазон МК для устройства)
//	vector<FU*> Gateways; // Вектор ссылок на шлюзы (входные и выходные)
	long int ChennelSearch(long int MK); // Поиск канала по МК (Возврашает -1, если канал не найден)
	vector<channel> Channels; // Вектор ссылок на каналы связи

	Eventser* eventser = nullptr; // Ссылка на контроллер событий
	double DelayTime = 0; // Время задержки передачи данных
	double RepeatTime = 0; // Время повторной передачи данных при ошибке передачи
	vector<FU*> Queue; // Очередь ожидания отправки (очередь МК)
	void* RoutingProg = nullptr; // Ссылка на программу маршрутизации
	// Ссылки на программы обработки исключительных ситуаций 
	long int Ind = 0; // Индекс текущего канала связи (чтобы можно было проводить его настройку) При создании нового канала он автоматически устанавливается на номер последнего канала, чтобы можно было производить его настройку
	long int SendInd = 0; // Индекс канала, на который требуется передать пришедшие для маршрутизациии данные
	ip LaslMkIp; // Последняя ИП с МК для маршрутизации
	void* OverflowProg = nullptr; // Нехватка оперативной памяти для пересылаемых данных
	void* MkOverflowProg = nullptr; //
	void* ReceiveProg = nullptr; // Программа реакции на приход данных для машрутизации (например, для обработки статистики)
	void* RoutingErrProg = nullptr; // Программа реакции на ошибку маршрутизации (атрибут МК не попадает ни в один из диапазонов маршрутизации)
	// Блок характеристик роутера
	long int MaxMemorySize = 0; // Макс. объем памяти
	double RoutingTime = 0; // Время обработки
	// Блок статистики передачи
	long int MKCount = 0; // Количество прошедших через роутер МК
	long int DataCount = 0; // Объем переданной информации через роутер
	long int MaxMKQueue = 0; // Максимальная длина очереди
	long int DataSize = 0, MaxDataSize = 0; // Текущий объем занятого буфера и максимальный объем занятого буфера

	vector <long int> SectorDim; // Размеры областей для моделирования распределенной ВС
	vector <long int> SectorCoordinate; // Координата сектора, обслуживаемого роутером
	vector <FU*> SectorReceivers; // Роутеры для обслуживания сектора
	void NetRouting(long int MK, LoadPoint Load, FU* Sender); // Маршрутизация в сети ФУ, разделенные на распределенные сетора
	//	double AverageMKQueue = 0; // Средняя длина очереди
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender=nullptr) override;
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст
	Router(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 20;
		ProgFU(0, { 0, nullptr });
	};
	Router() {
		FUtype = 20;
		ProgFU(0, { 0, nullptr });
	};
};
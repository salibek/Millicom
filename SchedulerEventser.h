#pragma once
#include "Consts.h"

class Eventser: public FU
{
	multimap<double, FU*> Events; // События
	bool work = false; // Флаг процесса моделирования
	bool start = false; // Флаг запуска процесса моделирования
	double CurrentTime = 0; // Текущее время моделирования
	void* FinProg = nullptr; // Программа, запускаемая по завершении моделирования
	int EventCount = 0; // Счетчик событий
public:
	void ProgFU(int MK, LoadPoint Load) override;
	void Eventsing(FU* Context, double tay);
	Eventser(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 13;
		ProgFU(0, { 0,nullptr });
	};
	Eventser() {};
};

class Scheduler : FU
{
	int NCores = 1, CoreCount = 0, CoreCountPrev = 0; // Количество ядер и счетчик занятых ядер
	int MkQueuePrev = 0; // Предыдущая длина очереди МК
	double SchedulingTime = 0, RunTime=0; // Время планирования вычислений и запуска на выполнение
	FU* eventser = nullptr; // Указатель на контроллер событий
	vector<FU*>Queue; // Очередь контекстов ФУ для выдачи задания для моделирования
	vector<double> MkTimeQueue; // Очередь времен выполнения МК, находящихся в очереди
	double* CurrentTime = 0; // Текущее время
	double PrevTime = 0, PrevCoreCount=0; // Модельное время предыдущего события и счетчик занятых ядер
	double ParallelFactor = 0; // коэффициент параллелизма
	double AverageMkQueue = 0; // Средняя и максимальная длины очереди МК
	int  MaxMkQueue = 0; // Максимальная длина очереди МК ко всем ФУ, подключенным к данному планировщику
public:
	void* SchedulingProg = nullptr;
	void ProgFU(int MK, LoadPoint Load) override;
	void Scheduling(FU*, double DTime, bool CoreContinue=false); // CoreContinue - флаг удержания вычислительного ядра
	void CoreFree(); // Освободить ядро
		Scheduler(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 12;
		ProgFU(0, { 0,nullptr });
	};
};
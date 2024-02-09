#include "stdafx.h"
#include "SchedulerEventser.h"

void Eventser::Eventsing(FU* Context, double tay, bool SchedulerFlag) // Запланиовать событие (tay - время задержки события)
{
	if (Events.size() == 0)
	{
//		Events.insert(pair<double, FU*>(CurrentTime+tay, Context));
		Events.insert(pair<double, Event>(CurrentTime + tay, { true, Context }));
		//return;
		if (work && !start)// ProgFU(1, { 0,nullptr });
			start = true;
			while (Events.size()!=0 && work) {
	//			cout << CurrentTime << endl;
				CurrentTime = Events.begin()->first;
				Events.begin()->second.Receiver->Scheduling(Events.begin()->second.SchedulerFlag);
				Events.erase(Events.begin());
			}
			ProgExec(FinProg);
			EventCount++;
	}
	else
	{
//		Events.insert(pair<double, FU*>(Events.begin()->first+tay, Context));
		Events.insert(pair<double, Event>(Events.begin()->first + tay, { true, Context }));
	}
}

void Eventser::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	switch (MK)
	{
	case 0: // Reset
		Events.clear();
		work = true;
		start = false;
		CurrentTime = 0;
		FinProg = nullptr;
		break;
	case 1: //Start Начать моделирование
		work = true;
		start = true;
		//Events.clear();
		while (Events.size() != 0 && work) {
			CurrentTime = Events.begin()->first;
//			EventsPrint();
			Events.begin()->second.Receiver->Scheduling(Events.begin()->second.SchedulerFlag);
//			cout << "----------------\n";
			if (Events.size() != 0) Events.erase(Events.begin());
		}
		ProgExec(FinProg);
		break;
	case 5: // WorkSet Установить флаг рабочего режима
		work = Load.toBool();
		break;
	case 10: // FUContextSet Установить ссылку на контекст ФУ для описания события
		FUContext = (FU*)Load.Point;
		break;
	case 11: // EventTimeSet Установить временя выполнения МК, инициированной планировщиком вычислительного процесса
		Delay = Load.toDouble();
		break;
	case 12: // EventMkInsert Установить МК для события
		Events.insert({ CurrentTime + Delay, {true, FUContext} });
		if (Load.isIC())
			FUContext->Modeling->qmk.push_back({ Load.IC()->at(0), nullptr });
		else if (Load.isIP())
			FUContext->Modeling->qmk.push_back({*((ip*)Load.Point), nullptr});
		break;
	case 15: // AwaitMkInsert Установить ожидаемую МК
		Events.insert({ CurrentTime + Delay, {false, FUContext} });
		if (Load.isIC())
			FUContext->Modeling->qAwaitMk.insert({ CurrentTime + Delay, {Load.IC()->at(0), Sender } });
		else if (Load.isIP())
			FUContext->Modeling->qAwaitMk.insert({ CurrentTime + Delay, {*((ip*)Load.Point), Sender }});
		break;
	case 45: //TimeSet Установить текущее модельное время
		CurrentTime = Load.toDouble();
		break;
	case 50: // TimeOut Выдать текущее модельное время
		if(Load.Type==Tdouble)
		break;
	case 51: // TimeOutMk Выдать МК с текущим модельным временем
	case 52: // TimeOutRefMk Выдать МК со ссылкой на переменную текущего модельного времени
		if (MK == 51)
			MkExec(Load, { Cdouble,&CurrentTime });
		else
			MkExec(Load, { Tdouble,&CurrentTime });
		break;
	case 55: // EventCountOut
		Load.Write(EventCount);
		break;
	case 56: // EventCountOutMk
		MkExec(Load, { Cint, &EventCount });
		break;
	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}

void Scheduler::CoreFree() // Освободить ядро
{
	int MkQueuePrev = Queue.size();
	if (Queue.size() == 0)
		CoreCount--;
	else
	{
		FU* t = Queue.back();
		MkQueuePrev--;
		double tMK = MkTimeQueue.back();
		Queue.pop_back(); MkTimeQueue.pop_back();
		((Eventser*)eventser)->Eventsing(t, tMK + SchedulingTime, true);
	}
	if (CurrentTime != nullptr)
	{
		ParallelFactor += CoreCountPrev * (*CurrentTime - PrevTime);
		AverageMkQueue+= MkQueuePrev * (*CurrentTime - PrevTime);
		PrevTime = *CurrentTime;
		CoreCountPrev = CoreCount;
	}
	ProgExec(SchedulingProg);
}

void Scheduler::Scheduling(FU* Context, double DTime, bool CoreContinue)
{
	if (CoreContinue)
		((Eventser*)eventser)->Eventsing(Context, DTime, true);
	else
		if (CoreCount < NCores)
		{
			CoreCount++;
			((Eventser*)eventser)->Eventsing(Context, DTime + SchedulingTime, true);
		}
		else
		{
			Queue.push_back(Context);
			MkTimeQueue.push_back(DTime);
			MkQueuePrev++;
			if (Queue.size() > MaxMkQueue)
				MaxMkQueue = Queue.size();
		}
	ProgExec(SchedulingProg);
}

void Scheduler::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	switch (MK)
	{
	case 0: // Reset
		eventser = nullptr;
		SchedulingTime = 0; RunTime = 0;
		CoreCountPrev = 0;
		MkQueuePrev = 0;
		CurrentTime = nullptr;
		NCores = 1; CoreCount = 0;
		MaxMkQueue = 0, AverageMkQueue = 0;
		PrevTime = 0; PrevCoreCount = 0;
		ParallelFactor = 0;
		SchedulingProg = nullptr;
		Queue.clear();
		break;
	case 1: // EventserSet
		eventser = (FU*)Load.Point;
		CurrentTime = &((Eventser*)Load.Point)->CurrentTime; // Ссылка на пеерменную с текущем временем моделирования
		break;
	case 2: // Clear Сбросить параметры моделирования
		CoreCountPrev = 0;
		ParallelFactor = 0;
		SchedulingTime = 0; RunTime = 0;
		MkQueuePrev = 0;
		if(CurrentTime != nullptr)
			*CurrentTime = 0;
		CoreCount = 0;
		MaxMkQueue = 0, AverageMkQueue = 0;
		PrevTime = 0; PrevCoreCount = 0;
		Queue.clear();
		break;
	case 5: //SchedulingTimeSet
		SchedulingTime = Load.toDouble();
		break;
	case 8: // NCoresSet
		NCores = Load.toInt();
		break;
	case 10: // TimeOut
		Load.Write(*CurrentTime);
		break;
	case 11: // TimeOutMk
		MkExec(Load, { Cdouble, CurrentTime });
		break;
	case 15: // SchedulingProgSet Установить ссылка на программу, запускаемую при планировании вычислений
		SchedulingProg = Load.Point;
		break;
	case 40: // CurrTimeRefSet Установить ссылку на переменную с текущим модельным временем 
		if(Load.Type==Tdouble)
			CurrentTime =(double*) Load.Point;
		break;
	case 50: // CoreCountOut Выдать число занятых ядер
		Load.Write(CoreCount);
		break;
	case 51: // CoreCountOutMk Выдать МК с числом занятых ядер
		MkExec(Load, { Cint,&CoreCount });
		break;
	case 55: // MkQueueOut Выдать количество МК в очереди на выполнение
		Load.Write(int(Queue.size()));
		break;
	case 56: // MkQueueOutMk Выдать МК с количеством МК в очереди на выполнение
	{
		int t = Queue.size();
		MkExec(Load, { Cint,&t });
		break;
	}
	case 60: // MkCountOut Выдать количество МК на выполнении и ожидании
		Load.Write(int(Queue.size()) + CoreCount);
		break;
	case 61: // MkCountOutMk Выдать МК с количеством МК на выполнении и ожидании
	{
		int t = Queue.size() + CoreCount;
		MkExec(Load, { Cint,&t });
		break;
	}

	case 65: // ParallelFactorOut Выдать коэффициент параллелизма
		Load.Write(ParallelFactor / *CurrentTime);
		break;
	case 66: //  ParallelFactorOutMk Выдать МК с коэффициентом параллелизма
//	if(Modeling!=nullptr)
	{
		double t;
		if (*CurrentTime > 0)
			t = ParallelFactor / *CurrentTime;
		else
			t = 0;
		MkExec(Load, { Cdouble,&t });
		break;
	}
	case 70: // AverageMkQueueOut Выдать среднюю длину очереди
//		if (Modeling != nullptr)
			Load.Write(AverageMkQueue);
		break;
	case 71: //  AverageMkQueueOutMk Выдать МК со средней длиной очереди
//		if (Modeling != nullptr)
		{
			double t;
		if (*CurrentTime > 0)
			t = AverageMkQueue / *CurrentTime;
		else
			t = 0;
		MkExec(Load, { Cdouble,&t });
		break;
	}
	case 75: // MaxMkQueueOut Выдать максимальную длину очереди
		if (Modeling != nullptr)
			Load.Write(ParallelFactor / *CurrentTime);
		break;
	case 76: //  MaxMkQueueOutMk Выдать МК с максимальной длиной очереди
	{
//		if (Modeling != nullptr)
		{
			int t = MaxMkQueue;
			MkExec(Load, { Cint,&t });
			break;
		}
	}

	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}
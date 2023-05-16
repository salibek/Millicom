#pragma once
#include "Consts.h"

class Eventser: public FU
{
	multimap<double, Event> Events; // �������
	bool work = false; // ���� �������� �������������
	bool start = false; // ���� ������� �������� �������������
	double CurrentTime = 0; // ������� ����� �������������
	void* FinProg = nullptr; // ���������, ����������� �� ���������� �������������
	int EventCount = 0; // ������� �������
	double Time = 0; // ����� ��� ��������� �������
	bool SchedulerF = true; // ���� ��� ��������� �������
	double Delay = 0; // �������� �� ��� ���������� �� ��� ��� �������� ��
FU* FUContext = nullptr; public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	void Eventsing(FU* Context, double tay, bool SchedulerFlag);
	Eventser(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 13;
		ProgFU(0, { 0,nullptr });
	};
	Eventser() {};
public:
	void EventsPrint() // ����������� ������� � ����������
	{
		for (auto i : Events)
			cout << i.first << " : ShFlag " << i.second.SchedulerFlag << " FU: " << i.second.Receiver->FUName << endl;
	};
};

class Scheduler : FU
{
	int NCores = 1, CoreCount = 0, CoreCountPrev = 0; // ���������� ���� � ������� ������� ����
	int MkQueuePrev = 0; // ���������� ����� ������� ��
	double SchedulingTime = 0, RunTime=0; // ����� ������������ ���������� � ������� �� ����������
	vector<FU*>Queue; // ������� ���������� �� ��� ������ ������� ��� �������������
	vector<double> MkTimeQueue; // ������� ������ ���������� ��, ����������� � �������
	double* CurrentTime = 0; // ������� �����
	double PrevTime = 0, PrevCoreCount=0; // ��������� ����� ����������� ������� � ������� ������� ����
	double ParallelFactor = 0; // ����������� ������������
	double AverageMkQueue = 0; // ������� � ������������ ����� ������� ��
	int  MaxMkQueue = 0; // ������������ ����� ������� �� �� ���� ��, ������������ � ������� ������������
public:
	FU* eventser = nullptr; // ��������� �� ���������� �������
	void* SchedulingProg = nullptr;
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	void Scheduling(FU*, double DTime, bool CoreContinue=false); // CoreContinue - ���� ��������� ��������������� ����
	void CoreFree(); // ���������� ����
	Scheduler(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 12;
		ProgFU(0, { 0,nullptr });
	};
	Scheduler() { Scheduler(nullptr, nullptr); };
};
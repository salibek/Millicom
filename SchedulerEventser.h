#pragma once
#include "Consts.h"
#include <limits>

class Eventser: public FU
{
	multimap<double, Event> Events; // �������
	bool work = false; // ���� �������� �������������
	bool start = false; // ���� ������� �������� �������������
	void* FinProg = nullptr; // ���������, ����������� �� ���������� �������������
	long int EventCount = 0; // ������� �������
	double Time = 0; // ����� ��� ��������� �������
	bool SchedulerF = true; // ���� ��� ��������� �������
	double Delay = 0; // �������� �� ��� ���������� �� ��� ��� �������� ��
	FU* FUContext = nullptr; public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������
	void Eventsing(FU* Context, double tay, bool SchedulerFlag);
	Eventser(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 13;
		ProgFU(0, { 0,nullptr });
	};
	Eventser() {};
public:
	double CurrentTime = 0; // ������� ����� �������������
	void EventsPrint() // ����������� ������� � ����������
	{
		for (auto &i : Events)
			cout << i.first << " : ShFlag " << i.second.SchedulerFlag << " FU: " << i.second.Receiver->FUName << endl;
	};
};

class Scheduler : FU
{
	long int NCores = 1, CoreCount = 0, CoreCountPrev = 0; // ���������� ���� � ������� ������� ����
	long int MkQueuePrev = 0; // ���������� ����� ������� ��
	double SchedulingTime = 0, RunTime=0; // ����� ������������ ���������� � ������� �� ����������
	long int SchedulingParallelFactor = 0; // ����������� ������������ ��� ������������ (������� �� ����� ������������ ���������)
	long int BusyCounter = numeric_limits<int>::min(); // ���� ��������� ������������ 
	vector<FU*>Queue; // ������� ���������� �� ��� ������ ������� ��� �������������
	vector<double> MkTimeQueue; // ������� ������ ���������� ��, ����������� � �������
	double* CurrentTime = 0; // ������� �����
	double PrevTime = 0, PrevCoreCount=0; // ��������� ����� ����������� ������� � ������� ������� ����
	double ParallelFactor = 0; // ����������� ������������
	double AverageMkQueue = 0; // ������� � ������������ ����� ������� ��
	long int  MaxMkQueue = 0; // ������������ ����� ������� �� �� ���� ��, ������������ � ������� ������������
public:
	FU* eventser = nullptr; // ��������� �� ���������� �������
	void* SchedulingProg = nullptr;
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������	void Scheduling(FU*, double DTime, bool CoreContinue=false); // CoreContinue - ���� ��������� ��������������� ����
	void Scheduling(FU*, double DTime, bool CoreContinue = false); // CoreContinue - ���� ��������� ��������������� ����
	void CoreFree(); // ���������� ����
	Scheduler(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 12;
		ProgFU(0, { 0,nullptr });
	};
	Scheduler() { Scheduler(nullptr, nullptr); };
};
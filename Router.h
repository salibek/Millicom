// �� �������������
#pragma once

#include "FUini.h"
#include "SchedulerEventser.h"


class channel // ����� �������� ������
{
public:
	long int Up = -1, Down = -1; // ������� � ������ ���������
	FU* Receiver = nullptr; // �������� ������
	long int ReceiverMk = -1; // �� ��� ��������� ������

	long int MkOutCount = 0; // ������� ������������ �� �� ������
	long int DataOutCount = 0; // ������� ���������� ������������ ������ �� �����
	long int MkInCount = 0; // ������� �������� �� � �����
	long int DataInCount = 0; // ������� ���������� �������� ������ � �����
	double Delay = 0; // �������� �������� ������ �� ������
};

class Router : public FU
{
public:
	double DelayGen(channel ch); //��������� �������� �������� ������
	// ���� ������ � ����� ���������� ����� ����, �� � ������ ����� ������������ ����� �� (�.�. ��� - ����� ��� �������� �� �� ����������)

//	int MkRangeStart = -1; // ��������� ������� �� ��� ������� �� ������� (���� ������ �������� �� ������, �� �� ��������� ������ ������� �������� + �������� �� ��� ����������)
//	vector<FU*> Gateways; // ������ ������ �� ����� (������� � ��������)
	long int ChennelSearch(long int MK); // ����� ������ �� �� (���������� -1, ���� ����� �� ������)
	vector<channel> Channels; // ������ ������ �� ������ �����

	Eventser* eventser = nullptr; // ������ �� ���������� �������
	double DelayTime = 0; // ����� �������� �������� ������
	double RepeatTime = 0; // ����� ��������� �������� ������ ��� ������ ��������
	vector<FU*> Queue; // ������� �������� �������� (������� ��)
	void* RoutingProg = nullptr; // ������ �� ��������� �������������
	// ������ �� ��������� ��������� �������������� �������� 
	long int Ind = 0; // ������ �������� ������ ����� (����� ����� ���� ��������� ��� ���������) ��� �������� ������ ������ �� ������������� ��������������� �� ����� ���������� ������, ����� ����� ���� ����������� ��� ���������
	long int SendInd = 0; // ������ ������, �� ������� ��������� �������� ��������� ��� �������������� ������
	ip LaslMkIp; // ��������� �� � �� ��� �������������
	void* OverflowProg = nullptr; // �������� ����������� ������ ��� ������������ ������
	void* MkOverflowProg = nullptr; //
	void* ReceiveProg = nullptr; // ��������� ������� �� ������ ������ ��� ������������ (��������, ��� ��������� ����������)
	void* RoutingErrProg = nullptr; // ��������� ������� �� ������ ������������� (������� �� �� �������� �� � ���� �� ���������� �������������)
	// ���� ������������� �������
	long int MaxMemorySize = 0; // ����. ����� ������
	double RoutingTime = 0; // ����� ���������
	// ���� ���������� ��������
	long int MKCount = 0; // ���������� ��������� ����� ������ ��
	long int DataCount = 0; // ����� ���������� ���������� ����� ������
	long int MaxMKQueue = 0; // ������������ ����� �������
	long int DataSize = 0, MaxDataSize = 0; // ������� ����� �������� ������ � ������������ ����� �������� ������

	vector <long int> SectorDim; // ������� �������� ��� ������������� �������������� ��
	vector <long int> SectorCoordinate; // ���������� �������, �������������� ��������
	vector <FU*> SectorReceivers; // ������� ��� ������������ �������
	void NetRouting(long int MK, LoadPoint Load, FU* Sender); // ������������� � ���� ��, ����������� �� �������������� ������
	//	double AverageMKQueue = 0; // ������� ����� �������
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender=nullptr) override;
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������
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
// �� �������������
#pragma once

#include "FUini.h"

class Channel1 // ����� �������� ������
{
public:
	int Up = -1, Down = -1; // ������� � ������ ���������
	void* Receiver = nullptr; // �������� ������
	int ReceiverMk = -1; // �� ��� ��������� ������
};

class Router : public FU
{
	int MkRangeStart = -1; // ��������� ������� �� ��� ������� �� ������� (���� ������ �������� �� ������, �� �� ��������� ������ ������� �������� + �������� �� ��� ����������)
//	vector<FU*> Gateways; // ������ ������ �� ����� (������� � ��������)
	vector<Channel1> Channels; // ������ ������ �� ������ �����

	int DelayTime = 0; // ����� �������� �������� ������
	vector<FU*> Queue; // ������� �������� �������� (������� ��)
	void* RoutingProg = nullptr; // ������ �� ��������� �������������
	// ������ �� ��������� ��������� �������������� �������� 
	int Ind = 0; // ������ �������� ������ ����� (����� ����� ���� ��������� ��� ���������) ��� �������� ������ ������ �� ������������� ��������������� �� ����� ���������� ������, ����� ����� ���� ����������� ��� ���������
	void* OverflowProg = nullptr; // �������� ����������� ������ ��� ������������ ������
	void* MkOverflowProg = nullptr; //
	//
	//���� ������������� �������
	int MaxMemorySize = 0; // ����. ����� ������
	double RoutingTime = 0; // ����� ���������
	// 
	// ���� ���������� ��������
	int MKCount = 0; // ���������� ���������� ��
	int MaxMKQueue = 0; // ������������ ����� �������
	double AverageMKQueue = 0; // ������� ����� �������
	//

public:
	void ProgFU(int MK, LoadPoint Load) override;
	Router(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 20;
		ProgFU(0, { 0, nullptr });
	};
};
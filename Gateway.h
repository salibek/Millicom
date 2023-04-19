// �� ����
#pragma once

#include "FUini.h"

class Gateway : public FU
{
	int GatewayMkRange = 0; // �������� �� ��� �����
	FU* Router = nullptr; // ������ �� ������
	FU* GatewayFriend = nullptr; //������ �� ������ ����
	void* GatewayProg = nullptr; // ��������� ������� ���������� ��� �������� ������
	void* ReceiveProg = nullptr; // ��������� ��������� ������� ������� ������ ��� �������� (�������� ��������� ����������)
	void* OverflowProg = nullptr; // ��������� ��������� ������� ������������ ������ ������
	void* MkOverflowProg = nullptr; // �������� ��������� ������� ������������ ������ ��
	int Mode = 0; // 0 - �������, 1 - �������� �� �����, 2 - �������� � �����, 3 - �����������
	// ���� ������������� �������
	double TranslTime = 0; // ����� �������� ������� ������
	double MkTranslTime = 0; // ����� �������� �� (��� ����� ������ ������ � �������� ��)
	double TransError = 0; // ����������� ������ ��������
	int MaxMemorySize = 0; // ����. ����� ������
	double RoutingTime = 0; // ����� ���������
	// ���� ���������
	int DataCount = 0, MkCount=0; // ����� ������ � ���������� �� ���������� ����� ����
	int MaxMKQueue = 0; // ������������ ����� �������
	double AverageMKQueue = 0; // ������� ����� �������
public:
	void ProgFU(int MK, LoadPoint Load) override;
	Gateway(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 21;
		ProgFU(0, { 0, nullptr });
	};
};
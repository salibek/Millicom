// �� ����
#pragma once

#include "FUini.h"

class Gateway : public FU
{
public:
	double DelayFunc(); // ������� ���������� �������� �������� ������
//	int GatewayMkRange = 0; // �������� �� ��� �����
	double DataTransmRate = 0; // �������� �������� ������ (� ���/�)
	long int HeaderVolum = 0; // ����� ��������� ������������� ������ ������
	FU* Receiver = nullptr; // ������ �� ������
	FU* GatewayFriend = nullptr; //������ �� ������ ����
	void* GatewayProg = nullptr; // ��������� ������� ���������� ��� �������� ������
	void* ReceiveProg = nullptr; // ��������� ��������� ������� ������� ������ ��� �������� (�������� ��������� ����������)
	void* OverflowProg = nullptr; // ��������� ��������� ������� ������������ ������ ������
	void* MkOverflowProg = nullptr; // ��������� ��������� ������� ������������ ������ ��
	long int Mode = 0; // 0 - �������, 1 - �������� �� �����, 2 - �������� � �����, 3 - �����������
	// ���� ������������� �������
	double TransmTime = 0; // ����� �������� ������� ������
	double MkTranslTime = 0; // ����� �������� �� (��� ����� ������ ������ � �������� ��)
	double TransError = 0; // ����������� ������ ��������
	long int MaxMemorySize = 0; // ����. ����� ������
	double ProcessingTime = 0; // ����� ��������� ������� �� ��������
	// ���� ���������
	long int DataCount = 0, MkCount=0; // ����� ������ � ���������� �� ���������� ����� ����
	long int MaxMKQueue = 0; // ������������ ����� �������
	double AverageMKQueue = 0; // ������� ����� �������
	long int DataSize = 0, MaxDataSize = 0; // ������� ����� �������� ������ � ������������ ����� �������� ������
	FU* Eventser = nullptr; // ������ �� ���������� �������
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������
	Gateway(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 21;
		ProgFU(0, { 0, nullptr });
	};
	Gateway(){Gateway(nullptr, nullptr);}
};
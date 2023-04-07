// �� ����
#pragma once

#include "FUini.h"

class Gateway : public FU
{
	FU* Router = nullptr; // ������ �� ������
	FU* GatewayFriend = nullptr; //������ �� ������ ����
	double TranslTime = 0; // ����� �������� ������� ������
	double TransError = 0; // ����������� ������ ��������
	void* RoutingProg = nullptr; //
	void* OverflowProg = nullptr; //
	void* MkOverflowProg = nullptr; //
	int Mode = 0; // 0 - �������, 1 - �������� �� �����, 2 - �������� � �����, 3 - �����������

	int Indata=0, OutData = 0; // ���������� ���������� ������ � �����, � �� �����

public:
	void ProgFU(int MK, LoadPoint Load) override;
	Gateway(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
	};
};
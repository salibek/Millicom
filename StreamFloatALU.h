#pragma once
#include "Consts.h"

class StreamFloatALU : public FU
{
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	vector<double>RezStack; // ���� �����������
	double Rez = 0; // ������� ����������
	int OperandsCounter = 0; // ������� ���������
	bool Redy = false; // ���� ���������� ����������
	vector<int>ReseiverMk; // ������ �� ��� ��������� ����������
	vector<FU*>ReseiverContext; // ������ �� ��������� ��������� ����������
	int AngleMode = 0; // ����� ����
	StreamFloatALU(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 12;
		ProgFU(0, { 0,nullptr });
	};
	StreamFloatALU() { StreamFloatALU(nullptr, nullptr); };
};
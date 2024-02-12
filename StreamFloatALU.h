#pragma once
#include "Consts.h"

class StreamFloatALU : public FU
{
private:
	vector<double>RezStack; // ���� �����������
	double Rez = 0; // ������� ����������
	bool Ready = false; // ���� ���������� ����������
	bool OutBlock = false; // ���� ������� ������ ���������� ����������
	int OperandsCounter = 0; // ������� ���������
	vector<double>Operands; // ������ ���������
	vector<double>OperandsFlags; // ������ ������ ���������
	bool Redy = false; // ���� ���������� ����������
	vector<int>ReseiverMk; // ������ �� ��� ��������� ����������
	vector<FU*>ReseiverContext; // ������ �� ��������� ��������� ����������
	int AngleMode = 0; // ����� ����
	void* ZProg = nullptr, *NZProg = nullptr;
	void* ErrProg = nullptr;

public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	StreamFloatALU(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 12;
		ProgFU(0, { 0,nullptr });
	};
	StreamFloatALU() { StreamFloatALU(nullptr, nullptr); };
};
#pragma once
#include "Consts.h"
#include <fstream>
#include <vector>

class GaussPoint : public FU {
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	GaussPoint(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; };
	GaussPoint() : FU() { Bus = nullptr; };
	double a = 0;
	bool FreeF = false; //���� ���������� �����
	bool KeyF = false; // ���� �������� �� ������� ���������
	GaussPoint* Neighbours[4] = {nullptr, nullptr, nullptr, nullptr};

	double CalcTime = 0; // ����� ����������
	double SendTime = 0; // ����� ��������� ������

private:
	double UpBuf = 0;
	double S = 0;
};

class Gauss : public FU {
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
//	FU* Copy() override; // ��������� ����������� ��
//	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������)
	Gauss(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; FUtype = 16; };
	Gauss() : FU() { Bus = nullptr; };
	long int Size = 10; // ����������� �������
	double Max = 10, Min = 10;// ������������ � ����������� �������� ������������� � ��������� ������
private:
		vector< vector<GaussPoint> > Field;
};


#pragma once
#include <omp.h>
#include "Consts.h"

class TemperatEx : public FU  // �������������� ���������� ������� ��������� ����
{
public:
	~TemperatEx()
	{
		for (auto i : T_Neighbours)
			i.clear();
		T_Neighbours.clear();
		T.clear();
	};
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������
	TemperatEx(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; ProgFU(0, { 0,nullptr }); };
	TemperatEx() { TemperatEx(nullptr, nullptr); };
	vector<double> T; // ������� �����������
	vector<vector<double>> T_Neighbours; // ����������� �������
	double a = 1; // ����������� � ���������
	vector<TemperatEx*> Neighbours; // ������ ������
	long int X=-1, Y=-1; // ���������� ����
	double h2 = 0, tay = 0;
	bool FConst = false; // ���� ����������� �����������
	double ReadTime = 0, CalcTime = 0; // ����� ������ ��������� �������� � ���������� ������ ��������
	void* FiringProg = nullptr; // ���������, ����������� ��� ��������� ����
};

class NetManager: public FU // �������� ������� ��������
{
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	NetManager(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; FUtype = 11; ProgFU(0, { 0,nullptr }); };
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������
	NetManager() : FU() { Bus = nullptr; };
private:
	double ManageTime = 0; // ����� ������ ��������� �������������� �����
	FU* NetScheduler = nullptr;
	vector<vector<TemperatEx*> > Items;
	long int ConstTCounter = 0; // ������� ��� ConstTSet
	long int X = 0, Y = 0, XC=0, YC=0;
	double h = 0, tay = 0;
	double a = 1; // ����������� � ���������
	long int Ntay = 0, NtayCounter = 0; // ���������� ������ �������������
	double ReadTime = 0, CalcTime = 0; // ����� ������ ��������� �������� � ���������� ������ ��������
};
#pragma once
#include "Consts.h"
#include <vector>

double LinF(double S, vector<double> Args); // ���������� �����������

double Porog(double S, vector<double> Args); // 1. x, ��� ������� ������� �� ����� ������� �������, 2. �������� �� ��������, 3. ����� ��������

class Neuro : public FU
{
private:
	vector<double> W;
	vector<double> X;
	vector<bool>XF;
	vector<long int> MK_Out;
	double S = 0; // �������� ������
	double eps = 0; // �����������
	long int FType = 0; // ������ ������� ���������
	//... ������ ������ �� ������� ���������
	vector<double> F_Arg;
	long int Counter = 0;
	long int WInd = 0, XInd = 0; // �������
	long int WIndStep = 1, XIndStep = 1; // ��� ���������� �������� ���� � �������� �������

	double (*Activation[2])(double, vector<double>) = { LinF,Porog }; // ������ ������� ���������
	double delta = 0, Z=0, Y=0;
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender=nullptr) override;
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������
	Neuro(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; FUtype = 10; ProgFU(0, { 0,nullptr }); };
	Neuro(){ Neuro(nullptr, nullptr); };
private:
};
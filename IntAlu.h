#pragma once
#include "Consts.h"

class IntContext {
	public:
	int* AccumulatUk; // ��������� �� �����������
	int Accumulat; // �����������
	int AutoInc; // ��� ������������� �������������
	int Fin; // �������� �������� �����
};

class IntAlu : public FU {
private:
	int * AccumulatUk= &Accumulat; // ��������� �� �����������
	vector <IntContext> Stack; // ���� �������������
	int Compare = 0; // ������������� ��������
	int RandFrom = 0, RandTo = 0; // �������� ������������ ��������� ��������
	int* CompareUk = &Compare; // ��������� �� ������������ ��������
	void* EmptyStackErrProg = nullptr; // ��������� ��������� ������ pop �� ������� �����
	void* BiggerProg=nullptr, * LessProg = nullptr, * EQProg = nullptr, * NEQProg = nullptr, * BiggerEQProg = nullptr, * LessEQProg = nullptr;
	void* NoCorrectTypeErrProg = nullptr; // ��������� ��� ������ ������ ������������� (�������� ����)
	void* NoIntTypeErrProg = nullptr; // ��������� ��� ������ ������ �������� ����
	void* NoBoolIntTypeErrProg = nullptr; // ��������� ��� ������ ������ �������� ���� ��� boolean 
public:
	int Accumulat = 0; // �����������
	int AutoInc = 0; // ��� ������������� �������������
	int Fin = 0; // �������� �������� �����
	void ProgFU(int MK, LoadPoint Load, FU* Sender=nullptr);
	IntAlu(FU* BusContext, FU* Templ=nullptr) : FU(BusContext) { ProgFU(0, { 0, nullptr }); Bus = BusContext; FUtype = 7; };
	IntAlu(){ IntAlu(nullptr, nullptr); };
};
#pragma once
#include "Consts.h"

class IntAlu : public FU {
private:
	int Accum = 0; // �����������
	int *AccumUk = &Accum; // ��������� �� �����������
	vector <int> Stack; // ���� �������������
	int AutoInc = 0; // ��� ������������� �������������
	int Compare = 0; // ������������� ��������
	int* CompareUk = &Compare; // ��������� �� ������������ ��������
	void* EmptyStackErrProg = nullptr; // ��������� ��������� ������ pop �� ������� �����
	void* BiggerProg=nullptr, * LessProg = nullptr, * EQProg = nullptr, * NEQProg = nullptr, * BiggerEQProg = nullptr, * LessEQProg = nullptr;
	void* NoCorrectTypeErrProg = nullptr; // ��������� ��� ������ ������ ������������� (�������� ����)
	void* NoIntTypeErrProg = nullptr; // ��������� ��� ������ ������ �������� ����
	void* NoBoolIntTypeErrProg = nullptr; // ��������� ��� ������ ������ �������� ���� ��� boolean 
public:
	void ProgFU(int MK, LoadPoint Load);
	IntAlu(FU* BusContext, FU* Templ) : FU(BusContext) { ProgFU(0, { 0, nullptr }); Bus = BusContext; FUtype = 7; };
	IntAlu() : FU() { IntAlu(nullptr, nullptr); };
};
#pragma once
#include "Consts.h"
#include <cmath>

class StreamFloatALU : public FU
{
	bool WrongFormatCheck(LoadPoint Load); // �������� ������� ������� ������ (���������� true, ���� ������������ ������)
	void OperandsClear(int MK); // ����� ��������� ��� ������ ���������� ����� ��������

public:
	vector<bool> FOperands; // ����� ����������� ���������  
	int Ready = 0; // ��� ���������� ���������� 0 - �� �����, 1 - �����, 2 - ������
	bool OutRezBlock = false; // ���� ������������ ������ ����������  
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override; // ��������� ���������� ��
	void RezExec(); // ���������� ����������� ��� ��������� ����������
	vector<double>RezStack; //  ���� ��� �������� ����������� � ������������� ������
	vector<double>RezExtStack; // ���� ����������� ����������� (��������, ������� ��� �������� �������������� �������)
	double Rez = 0; //  ��������� ��������
	int OperandsCounter = 0; // ������� ���������� ���������� ���������
	int OpInd = 0; // ������ ��������
	vector<int>ReseiverMk; // �� ��� ���������� ����������
	vector<FU*>ReseiverContext; // �������� ���������� ����������
	int AngleMode = 0; // ����� ��������� ���� (0 - �������, 1 -�������)
	int Noperands = 2; // ���������� ��������� ��� ��������
	void* ZProg = nullptr, * NZProg = nullptr, * BProg = nullptr, * BZProg = nullptr, * LProg = nullptr, * LZProg = nullptr; //      
	void* ErrProg = nullptr, * WrongFormatErrProg = nullptr, * OveflowErrProg = nullptr, *DivZeroErrProg=nullptr; //  
	void* MatErrProg = nullptr; // ��������� ��������� ������ �������������� ��������
	void* NoOperandErrProg = nullptr;// ������ ��� ��������
	void* OpIndErrProg = nullptr;// ������ ������� ��������
	void* OperationErrProg = nullptr; // ������ ��������
	void* RezStackIsEmpyProg = nullptr; // ������ ��� ������� ���������� �� ������� ����� �����������    
	void* RezExtStackIsEmpyProg = nullptr; // ������ ��� ������� ���������� �� ������� ����� ������������ �����������
	void* OperetionProg = nullptr;// ��������� ��� ���������� ����������� ��������
	void* RezProg = nullptr; // ���������, ����������� ����� ���������� ����������
	void* PreRezProg = nullptr; // ���������, ����������� ����� ���������� ����������    
	vector<double> Operands;// ���� ���������
	int OpCode = 0; // ��� ��������, ��� ������� ������������� ��������

	StreamFloatALU(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 12;
		ProgFU(0, { 0,nullptr });
	};
	StreamFloatALU() { StreamFloatALU(nullptr, nullptr); };

};

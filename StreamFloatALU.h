#pragma once
#include "Consts.h"
#include <cmath>

class StreamFloatALU : public FU
{
	bool WrongFormatCheck(LoadPoint Load); // �������� ������� ������� ������ (���������� true, ���� ������������ ������)
	void OperandsClear(long int MK); // ����� ��������� ��� ������ ���������� ����� ��������
	void* RoutProg = nullptr; // ��������� ��� ������������ ������ �� � ��� ����������� �������
	void* FUSelfAdrProg = nullptr; // ��������� ��� ���������� ������ �� � ��� ����������� �������
	vector<void*> AdrBuf; // ����� ������� �� ��� ���������
	ip IpForMkAdrOut = { 0,{0,0} }; // ����������� �� ��� �����������, ��� �� ��� ��������� ��������� ��

public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override; // ��������� ���������� ��
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������
	vector<bool> FOperands; // ����� ����������� ���������  
	long int Ready = 0; // ��� ���������� ���������� 0 - �� �����, 1 - �����, 2 - ������
	bool OutRezBlock = false; // ���� ������������ ������ ����������  
	void RezExec(); // ���������� ����������� ��� ��������� ����������
	vector<double>RezStack; //  ���� ��� �������� ����������� � ������������� ������
	vector<double>RezExtStack; // ���� ����������� ����������� (��������, ������� ��� �������� �������������� �������)
	double Rez = 0; //  ��������� ��������
	long int OperandsCounter = 0; // ������� ���������� ���������� ���������
	long int OpInd = 0; // ������ ��������
	vector<long int>ReceiverMk; // �� ��� ���������� ����������
	vector<FU*>ReceiverContexts; // �������� ���������� ����������
	vector<LoadPoint>OutVars; // ������ �� ���������� ��� ���������� ����������
	long int AngleMode = 0; // ����� ��������� ���� (0 - �������, 1 -�������)
	long int Noperands = 2; // ���������� ��������� ��� ��������
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
	long int OpCode = 0; // ��� ��������, ��� ������� ������������� ��������

	StreamFloatALU(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 12;
		ProgFU(0, { 0,nullptr });
		StreamFloatALU((void *)Templ);

	};
	StreamFloatALU() { StreamFloatALU(nullptr, nullptr); };
	StreamFloatALU(void *); // ���������� �����������
};

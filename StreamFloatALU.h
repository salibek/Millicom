#pragma once
#include "Consts.h"
#include <cmath>

class StreamFloatALU : public FU
{
public:
	vector<bool> Foperands; //   
	int Ready = 0; // ��� ���������� ���������� 0 - �� �����, 1 - �����, 2 - ������
	bool OutRezBlock = false; //    
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override; //
	void RezExec(); // ���������� ����������� ��� ��������� ����������
	vector<double>RezStack; //  
	double Rez = 0; //  
	int OperandsCounter = 0; //  
	int OpInd = 0; // ������ �������� 
	bool ErrFlag = false; // ���� ������
	vector<int>ReseiverMk; //
	vector<FU*>ReseiverContext; //     
	int AngleMode = 0; //  
	int Noperands = 2; //  
	void* ZProg = nullptr, * NZProg = nullptr, * BProg = nullptr, * BZProg = nullptr, * LProg = nullptr, * LZProg = nullptr; //      
	void* ErrProg = nullptr, * WrongFormatErrProg = nullptr, * OveflowErrProg = nullptr, *DivZeroErrProg=nullptr; //  
	void* NoOperandErrProg = nullptr;// ������ ��� ��������
	void* OpIndErrProg = nullptr;// ������ ������� ��������
	void* OperationErrProg = nullptr; // ������ ��������
	void* RezStackIsEmpyProg = nullptr; //     
	void* OperetionProg = nullptr;//  
	void* RezProg = nullptr; // ,    
	vector<double> Operands;//
	int OpCode = 0; // ��� ��������, ��� ������� ������������� ��������

	StreamFloatALU(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 12;
		ProgFU(0, { 0,nullptr });
	};
	StreamFloatALU() { StreamFloatALU(nullptr, nullptr); };
};

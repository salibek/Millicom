#pragma once
#include "Consts.h"
#include "StreamFloatALUManager.h"

using namespace std;

class StreamIntALUManager : public FU
{
private:
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	StreamIntALUManager(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 24;
		ProgFU(0, { 0,nullptr });
	};
	StreamIntALUManager() { StreamIntALUManager(nullptr, nullptr); };
};

class StreamIntALU : public FU
{
private:
	vector<bool> FOperands; // ����� ����������� ���������  
	int Ready = 0; // ��� ���������� ���������� 0 - �� �����, 1 - �����, 2 - ������
	bool OutRezBlock = false; // ���� ������������ ������ ����������  
	void RezExec(); // ���������� ����������� ��� ��������� ����������
	vector<int>RezStack; //  ���� ��� �������� ����������� � ������������� ������
	vector<int>RezExtStack; // ���� ����������� ����������� (��������, ������� ��� �������� �������������� �������)
	int Rez = 0; //  ��������� ��������
	int OperandsCounter = 0; // ������� ���������� ���������� ���������
	int OpInd = 0; // ������ ��������
	vector<int> Operands;// ���� ���������
	vector<int>ReseiverMk; // �� ��� ���������� ����������
	vector<FU*>ReseiverContext; // �������� ���������� ����������
	int Noperands = 2; // ���������� ��������� ��� ��������
	void* ZProg = nullptr, * NZProg = nullptr, * BProg = nullptr, * BZProg = nullptr, * LProg = nullptr, * LZProg = nullptr; //      
	void* ErrProg = nullptr, * WrongFormatErrProg = nullptr, * OveflowErrProg = nullptr, * DivZeroErrProg = nullptr; //  
	void* MatErrProg = nullptr; // ��������� ��������� ������ �������������� ��������
	void* NoOperandErrProg = nullptr;// ������ ��� ��������
	void* OpIndErrProg = nullptr;// ������ ������� ��������
	void* OperationErrProg = nullptr; // ������ ��������
	void* RezStackIsEmpyProg = nullptr; // ������ ��� ������� ���������� �� ������� ����� �����������    
	void* RezExtStackIsEmpyProg = nullptr; // ������ ��� ������� ���������� �� ������� ����� ������������ �����������
	void* OperetionProg = nullptr;// ��������� ��� ���������� ����������� ��������
	void* RezProg = nullptr; // ���������, ����������� ����� ���������� ����������
	void* PreRezProg = nullptr; // ���������, ����������� ����� ���������� ����������    
	int OpCode = 0; // ��� ��������, ��� ������� ������������� ��������
	bool WrongFormatCheck(LoadPoint Load); // �������� ������� ������� ������ (���������� true, ���� ������������ ������)
	void OperandsClear(int MK); // ����� ��������� ��� ������ ���������� ����� ��������

public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	StreamIntALU(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 25;
		ProgFU(0, { 0,nullptr });
	};
	StreamIntALU() { StreamIntALU(nullptr, nullptr); };
};

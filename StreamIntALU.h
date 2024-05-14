#pragma once
#include "Consts.h"
#include "StreamManager.h"

using namespace std;

class StreamIntALU : public FU
{
private:
	vector<bool> FOperands; // ����� ����������� ���������  
	bool OutRezBlock = false; // ���� ������������ ������ ����������  
	void RezExec(long int* tRez, bool RezExec = false); // ���������� ����������� ��� ��������� ����������
	vector<long int>RezStack; //  ���� ��� �������� ����������� � ������������� ������
	vector<long int>RezExtStack; // ���� ����������� ����������� (��������, ������� ��� �������� �������������� �������)
	long int OperandsCounter = 0; // ������� ���������� ���������� ���������
	long int OpInd = 0; // ������ ��������
	vector<long int> Operands;// ���� ���������
	vector<long int>ReceiverMk; // �� ��� ���������� ����������
	vector<FU*>ReceiverContexts; // �������� ���������� ����������
	vector<LoadPoint>OutVars; // ������ �� ���������� ��� ���������� ����������
	long int Noperands = 2; // ���������� ��������� ��� ��������
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
	long int OpCode = 0; // ��� ��������, ��� ������� ������������� ��������
	bool WrongFormatCheck(LoadPoint Load); // �������� ������� ������� ������ (���������� true, ���� ������������ ������)
	void OperandsClear(long int MK); // ����� ��������� ��� ������ ���������� ����� ��������
	void* RoutProg = nullptr; // ��������� ��� ������������ ������ �� � ��� ����������� �������
	void* SelfAdrProg = nullptr; // ��������� ��� ���������� ������ �� � ��� ����������� �������
	vector<void*> AdrBuf; // ����� ������� �� ��� ���������
	ip IpForMkAdrOut = { 0,{0,0} }; // ����������� �� ��� �����������, ��� �� ��� ��������� ��������� ��
	bool MkAbort = false; // ���� ���������� ����� ���������� ���������������� �������
	bool EarlyCalculi = false; // ���� ������ ���������� (��� ��������������� �������� ��������� �������� ����������� ��� �� ������� ������ (�������� ����������)
	bool AccumMode = false;// �������������� ����� ������
public:
	long RezBuf = 0; // ����� ����������
	long int Rez = 0; //  ��������� ��������
	long int Ready = 0; // ��� ���������� ���������� 0 - �� �����, 1 - �����, 2 - ������
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������

	StreamIntALU(FU* BusContext, FU* Templ=nullptr)
	{
		Bus = BusContext;
		FUtype = 25;
		ProgFU(0, { 0,nullptr });
	};
	StreamIntALU() { StreamIntALU(nullptr, nullptr); };
	StreamIntALU(void* Dev); // ���������� �����������
};

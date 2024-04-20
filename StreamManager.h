#pragma once
#include "Consts.h"
#include "FUIni.h"
#include "StreamFloatALU.h"
#include "StreamIntALU.h"

using namespace std;

class StreamManager : public FU
{
private:
	vector<vector<FU*> > Field; // ���� ���
	long int Ind = 0, IndGroup = 0; // ������� �� � ������
	long int Ind2 = 0, IndGroup2 = 0; // ������ ������� �� � ������
	long int Mk1 = -1, Mk2=-1; // ������������ ��� ���������� �� �� �� ������� � ������� ��������
	long int Counter = 1; // ������� �������� ��� �������� �� (������� ��� ���������� ��������� ����������)
	long int DeviseCounter = 0;
	void* DevNotExistErrProg = nullptr; // ������������ ������ '��� ���������� ����������'
	vector<StreamFloatALU> Group; // ������ ������ �� ��� �����������
	FuFabric MakeFU;
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������
	StreamManager(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 23;
		ProgFU(0, { 0,nullptr });
	};
	StreamManager() { StreamManager(nullptr, nullptr); };
	~StreamManager();
};

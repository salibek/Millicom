#pragma once
#include "Consts.h"
#include "StreamFloatALU.h"

using namespace std;

class StreamFloatALUManager : public FU
{
private:
	vector<vector<StreamFloatALU*> > Field; // ���� ���
	int Ind = 0, IndGroup=0;
	int DeviseCounter = 0;
	void* DevNotExistErrProg = nullptr; // ������������ ������ '��� ���������� ����������'
	vector<StreamFloatALU> Group; // ������ ������ �� ��� �����������
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	StreamFloatALUManager(FU* BusContext, FU* Templ) 
	{
		Bus = BusContext;
		FUtype = 23;
		ProgFU(0, { 0,nullptr },nullptr);
	};
	StreamFloatALUManager() { StreamFloatALUManager(nullptr, nullptr); };
	~StreamFloatALUManager();
};

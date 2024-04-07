#pragma once
#include "Consts.h"
#include "StreamFloatALU.h"
#include "StreamIntALU.h"

using namespace std;

class StreamManager : public FU
{
private:
	vector<vector<FU*> > Field; // ���� ���
	int Ind = 0, IndGroup=0;
	int Mode = 0; //  ����� ������ �� (0 - ������� ���, 1 - ������������� ���)
	int DeviseCounter = 0;
	void* DevNotExistErrProg = nullptr; // ������������ ������ '��� ���������� ����������'
	vector<StreamFloatALU> Group; // ������ ������ �� ��� �����������
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	StreamManager(FU* BusContext, FU* Templ) 
	{
		Bus = BusContext;
		FUtype = 23;
		ProgFU(0, { 0,nullptr },nullptr);
	};
	StreamManager() { StreamManager(nullptr, nullptr); };
	~StreamManager();
};

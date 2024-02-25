// �� ����
#pragma once
#include "FUini.h"
#include <vector>

const int NFUTypes = 26;

typedef FU*(*FUiniArr[NFUTypes])(FU*, FU*);

class BusFU : public FU {
public:
	BusFU(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; FUTypesIni(); FUtype = 0; };
	BusFU() : FU() { Bus = this; FUTypesIni(); };
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* (*FUTypes[NFUTypes])(FU*, FU*) = {BusIni, ConsIni, StrGenIni, LexIni, FindIni, 
		ListIni, GraphTrasfIni, IntAluIni, InOutIni, AutomatManagerIni, NeuroIni,
		NetManagerIni,SchedulerIni,EventserIni, MeanShiftIni, StreamFloatALUIni, GaussIni, ALUIni,
		CellularAutomatIni, CellularAutomatManagerIni, RouterIni, GatewayIni, StreamFloatALUManagerIni, StreamIntALUIni, StreamIntALUManagerIni };
	vector <FU *> FUs; // ������ ���������� �� ��������� ��
	FU * FUTempl = nullptr; // ��������� �� �������� ������� ��
	int FUMkRange = 1000; // �������� �� ��� ������� ��
	int FUTypeCorrect; // ��������� ������ ���� �� (��� ������������ �� ������ ��-������)
	bool InterpretatorMode = false; // ����� �������������� (���� �����������, �� ����������� �� InterpretatorExec
private:
	void FUTypesIni();
};
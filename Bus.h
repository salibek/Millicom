// ФУ Шина
#pragma once
#include "FUini.h"
#include <vector>

const int NFUTypes = 22;

typedef FU*(*FUiniArr[NFUTypes])(FU*, FU*);

class BusFU : public FU {
public:
	BusFU(FU *BusContext, FU *Templ) : FU(BusContext) { Bus = BusContext; FUTypesIni();};
	BusFU() : FU() { Bus = this; FUTypesIni(); };
	void ProgFU(int MK, LoadPoint Load) override;
	FU* (*FUTypes[NFUTypes])(FU*, FU*) = {BusIni, ConsIni, StrGenIni, LexIni, FindIni, 
		ListIni, GraphTrasfIni, CounterIni, InOutIni, AutomatManagerIni, NeuroIni,
		NetManagerIni,SchedulerIni,EventserIni, MeanShiftIni, BagIni, GaussIni, ALUIni,
		CellularAutomatIni, CellularAutomatManagerIni, RouterIni};
	vector <FU *> FUs; // Вектор указателей на контексты ФУ
	FU * FUTempl = nullptr; // Указатель на контекст шаблона ФУ
	int FUMkRange = 1000; // Диапазон МК для каждого ФУ
	int FUTypeCorrect; // Коррекция номера типы ФУ (для согласования со старой ОА-средой)
private:
	void FUTypesIni();
};
// ФУ Шина
#pragma once
#include "FUini.h"
#include <vector>

//const int NFUTypes = 27;

typedef FU*(*FUiniArr[NFUTypes])(FU*, FU*);

class BusFU : public FU {
public:
	BusFU(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; FUTypesIni(); FUtype = 0; };
	BusFU() : FU() { Bus = this; FUTypesIni(); };
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст
	FuFabric FUTypes;
//	FU* (*FUTypes[NFUTypes])(FU*, FU*) = {BusIni, ConsIni, StrGenIni, LexIni, FindIni,
//		ListIni, GraphTrasfIni, IntAluIni, InOutIni, AutomatManagerIni, NeuroIni,
//		NetManagerIni,SchedulerIni,EventserIni, MeanShiftIni, StreamFloatALUIni, GaussIni, ALUIni,
//		CellularAutomatIni, CellularAutomatManagerIni, RouterIni, GatewayIni, StreamFloatALUManagerIni, 
//		StreamIntALUIni, StreamIntALUManagerIni, MatPlotIni };
	vector <FU *> FUs; // Вектор указателей на контексты ФУ
	FU * FUTempl = nullptr; // Указатель на контекст шаблона ФУ
	long int FUMkRange = 1000; // Диапазон МК для каждого ФУ
	long int FUTypeCorrect= fuTypeCorrect; // Коррекция номера типы ФУ (для согласования со старой ОА-средой)
	bool InterpretatorMode = false; // Режим интерпретатора (если установлено, то выполняется МК InterpretatorExec
	long int Ind = 0, Ind2 = 0; // Индексы ФУ, управляемых Шиной
private:
	void FUTypesIni();
};
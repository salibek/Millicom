// Подпрограммы инициализации ФУ
#pragma once
#include "Consts.h"

int SizeOfFUType(unsigned int Type); // Объём данных, занимаемый ФУ определенного типа

FU *BusIni(FU *BusContext, FU *TEmpl);
FU *ConsIni(FU *BusContext, FU *TEmpl);
FU *LexIni(FU *BusContext, FU *TEmpl);
FU *StrGenIni(FU *BusContext, FU *TEmpl);
FU *LexIni(FU *BusContext, FU *TEmpl);
FU *FindIni(FU *BusContext, FU *TEmpl);
FU* ListIni(FU* BusContext, FU* TEmpl);
FU* GraphTrasfIni(FU* BusContext, FU* TEmpl);
FU* ThreaderIni(FU* BusContext, FU* TEmpl);
FU* ALUIni(FU* BusContext, FU* TEmpl);
FU* InOutIni(FU* BusContext, FU* TEmpl);
FU* AutomatManagerIni(FU* BusContext, FU* TEmpl);
FU* NeuroIni(FU* BusContext, FU* TEmpl);
FU* NetManagerIni(FU* BusContext, FU* TEmpl);
FU* SchedulerIni(FU* BusContext, FU* TEmpl);
FU* EventserIni(FU* BusContext, FU* TEmpl);
FU* MeanShiftIni(FU* BusContext, FU* TEmpl);
FU* GaussIni(FU* BusContext, FU* TEmpl);
//FU* BagIni(FU* BusContext, FU* TEmpl);
FU* CellularAutomatIni(FU* BusContext, FU* TEmpl);
FU* CellularAutomatManagerIni(FU* BusContext, FU* TEmpl);
FU* IntAluIni(FU* BusContext, FU* TEmpl);
FU* RouterIni(FU* BusContext, FU* TEmpl);
FU* GatewayIni(FU* BusContext, FU* TEmpl);
FU* StreamFloatALUIni(FU* BusContext, FU* TEmpl);
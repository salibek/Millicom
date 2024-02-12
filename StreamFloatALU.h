#pragma once
#include "Consts.h"

class StreamFloatALU : public FU
{
private:
	vector<double>RezStack; // Стек результатов
	double Rez = 0; // Регистр результата
	bool Ready = false; // Флаг готовности результата
	bool OutBlock = false; // Флаг запрета выдачи результата вычисления
	int OperandsCounter = 0; // Счетчик операндов
	vector<double>Operands; // Вектор операндов
	vector<double>OperandsFlags; // Вектор флагов операндов
	bool Redy = false; // Флаг готовности результата
	vector<int>ReseiverMk; // Вектор МК для адресатов результата
	vector<FU*>ReseiverContext; // Ссылки на контексты адресатов результата
	int AngleMode = 0; // Режим угла
	void* ZProg = nullptr, *NZProg = nullptr;
	void* ErrProg = nullptr;

public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	StreamFloatALU(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 12;
		ProgFU(0, { 0,nullptr });
	};
	StreamFloatALU() { StreamFloatALU(nullptr, nullptr); };
};
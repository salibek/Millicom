#pragma once
#include "Consts.h"

class StreamFloatALU : public FU
{
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	vector<double>RezStack; // Стек результатов
	double Rez = 0; // Регистр результата
	int OperandsCounter = 0; // Счетчик операндов
	bool Redy = false; // Флаг готовности результата
	vector<int>ReseiverMk; // Вектор МК для адресатов результата
	vector<FU*>ReseiverContext; // Ссылки на контексты адресатов результата
	int AngleMode = 0; // Режим угла
	StreamFloatALU(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 12;
		ProgFU(0, { 0,nullptr });
	};
	StreamFloatALU() { StreamFloatALU(nullptr, nullptr); };
};
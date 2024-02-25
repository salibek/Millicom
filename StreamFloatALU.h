#pragma once
#include "Consts.h"
#include <cmath>

class StreamFloatALU : public FU
{
public:
	vector<bool> Foperands;
	bool Ready = false; // флаг готовности результата
	bool OutRezBlock = false; // флаг блокирующий выдачу результата
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override; // Программа обработки приходящего токена
	vector<double>RezStack; // Стек результатов
	double Rez = 0; // Регистр результата
	int OperandsCounter = 0; // Счетчик операндов
	bool Redy = false; // Флаг готовности результата
	vector<int>ReseiverMk; // Вектор МК для адресатов результата
	vector<FU*>ReseiverContext; // Ссылки на контексты адресатов результата
	int AngleMode = 0; // Режим угла
	int Noperands = 0;
	void* ZProg = nullptr, *NZProg=nullptr, *BProg=nullptr, *BZProg=nullptr, *LProg=nullptr, *LZProg=nullptr; // Указатель не подпрограмму по нулевому результату
	void* ErrProg = nullptr, * WrongFormatErrProg = nullptr, * OveflowProg = nullptr; // Программы по обработке ошибок
	void* RezStackIsEmpyProg = nullptr;
	void* OperetionProg = nullptr;
	void* RezProg = nullptr; // Подпрограмма, запускаемая при получении результата
	vector<double> Operands;// вектор операндов

	StreamFloatALU(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 12;
		ProgFU(0, { 0,nullptr });
	};
	StreamFloatALU() { StreamFloatALU(nullptr, nullptr); };
};
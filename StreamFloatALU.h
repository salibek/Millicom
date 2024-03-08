#pragma once
#include "Consts.h"
#include <cmath>

class StreamFloatALU : public FU
{
public:
	vector<bool> Foperands; //   
	int Ready = 0; // Код готовности результата 0 - не готов, 1 - готов, 2 - ошибка
	bool OutRezBlock = false; //    
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override; //
	void RezExec(); // Выполнение подпрограмм при получении результата
	vector<double>RezStack; //  Стек для хранения результатов и промежуточных данных
	vector<bool>FRezStack; // Флаги расширенного результата операции
	double Rez = 0; //  
	int OperandsCounter = 0; //  
	int OpInd = 0; // Индекс операнда
	vector<int>ReseiverMk; //
	vector<FU*>ReseiverContext; //     
	int AngleMode = 0; //  
	int Noperands = 2; //  
	void* ZProg = nullptr, * NZProg = nullptr, * BProg = nullptr, * BZProg = nullptr, * LProg = nullptr, * LZProg = nullptr; //      
	void* ErrProg = nullptr, * WrongFormatErrProg = nullptr, * OveflowErrProg = nullptr, *DivZeroErrProg=nullptr; //  
	void* MatErrProg = nullptr; // Программа обработки ошибки математической операции
	void* NoOperandErrProg = nullptr;// Ошибка нет операнда
	void* OpIndErrProg = nullptr;// Ошибка индекса операнда
	void* OperationErrProg = nullptr; // Ошибка операции
	void* RezStackIsEmpyProg = nullptr; //     
	void* OperetionProg = nullptr;//  
	void* RezProg = nullptr; // Программа, запускаемая перед получением результата
	void* PreRezProg = nullptr; // Программа, запускаемая перед получением результата    
	vector<double> Operands;// 
	int OpCode = 0; // Код операции, для которой накапливаются операнды

	StreamFloatALU(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 12;
		ProgFU(0, { 0,nullptr });
	};
	StreamFloatALU() { StreamFloatALU(nullptr, nullptr); };
};

#pragma once
#include "Consts.h"
#include <cmath>

class StreamFloatALU : public FU
{
	bool WrongFormatCheck(LoadPoint Load); // Проверка формата входных данных (возвращает true, если неправильный формат)
	void OperandsClear(int MK); // Сброс операндов при начале обоработки новой операции

public:
	vector<bool> FOperands; // Флаги поступления операндов  
	int Ready = 0; // Код готовности результата 0 - не готов, 1 - готов, 2 - ошибка
	bool OutRezBlock = false; // Флаг блокирования выдачи результата  
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override; // Программа обоработка МК
	void RezExec(); // Выполнение подпрограмм при получении результата
	vector<double>RezStack; //  Стек для хранения результатов и промежуточных данных
	vector<double>RezExtStack; // Стек расширеного резульатата (например, остаток при операции целочисленного деления)
	double Rez = 0; //  Результат операции
	int OperandsCounter = 0; // Счетчик количества полученных операндов
	int OpInd = 0; // Индекс операнда
	vector<int>ReseiverMk; // МК для получателя результата
	vector<FU*>ReseiverContext; // Контекст получателя результата
	int AngleMode = 0; // Режим измерения угла (0 - радианы, 1 -градусы)
	int Noperands = 2; // Количество операндов для операции
	void* ZProg = nullptr, * NZProg = nullptr, * BProg = nullptr, * BZProg = nullptr, * LProg = nullptr, * LZProg = nullptr; //      
	void* ErrProg = nullptr, * WrongFormatErrProg = nullptr, * OveflowErrProg = nullptr, *DivZeroErrProg=nullptr; //  
	void* MatErrProg = nullptr; // Программа обработки ошибки математической операции
	void* NoOperandErrProg = nullptr;// Ошибка нет операнда
	void* OpIndErrProg = nullptr;// Ошибка индекса операнда
	void* OperationErrProg = nullptr; // Ошибка операции
	void* RezStackIsEmpyProg = nullptr; // Ошибка при попытке извлечения из пуского стека результатов    
	void* RezExtStackIsEmpyProg = nullptr; // Ошибка при попытке извлечения из пуского стека расширенного результатов
	void* OperetionProg = nullptr;// Программа для выполнения специальной операции
	void* RezProg = nullptr; // Программа, запускаемая перед получением результата
	void* PreRezProg = nullptr; // Программа, запускаемая перед получением результата    
	vector<double> Operands;// Стек операндов
	int OpCode = 0; // Код операции, для которой накапливаются операнды

	StreamFloatALU(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 12;
		ProgFU(0, { 0,nullptr });
	};
	StreamFloatALU() { StreamFloatALU(nullptr, nullptr); };

};

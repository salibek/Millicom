#pragma once
#include "Consts.h"
#include "StreamManager.h"

using namespace std;

class StreamIntALU : public FU
{
private:
	vector<bool> FOperands; // Флаги поступления операндов  
	int Ready = 0; // Код готовности результата 0 - не готов, 1 - готов, 2 - ошибка
	bool OutRezBlock = false; // Флаг блокирования выдачи результата  
	void RezExec(); // Выполнение подпрограмм при получении результата
	vector<int>RezStack; //  Стек для хранения результатов и промежуточных данных
	vector<int>RezExtStack; // Стек расширеного резульатата (например, остаток при операции целочисленного деления)
	int Rez = 0; //  Результат операции
	int OperandsCounter = 0; // Счетчик количества полученных операндов
	int OpInd = 0; // Индекс операнда
	vector<int> Operands;// Стек операндов
	vector<int>ReseiverMk; // МК для получателя результата
	vector<FU*>ReseiverContexts; // Контекст получателя результата
	int Noperands = 2; // Количество операндов для операции
	void* ZProg = nullptr, * NZProg = nullptr, * BProg = nullptr, * BZProg = nullptr, * LProg = nullptr, * LZProg = nullptr; //      
	void* ErrProg = nullptr, * WrongFormatErrProg = nullptr, * OveflowErrProg = nullptr, * DivZeroErrProg = nullptr; //  
	void* MatErrProg = nullptr; // Программа обработки ошибки математической операции
	void* NoOperandErrProg = nullptr;// Ошибка нет операнда
	void* OpIndErrProg = nullptr;// Ошибка индекса операнда
	void* OperationErrProg = nullptr; // Ошибка операции
	void* RezStackIsEmpyProg = nullptr; // Ошибка при попытке извлечения из пуского стека результатов    
	void* RezExtStackIsEmpyProg = nullptr; // Ошибка при попытке извлечения из пуского стека расширенного результатов
	void* OperetionProg = nullptr;// Программа для выполнения специальной операции
	void* RezProg = nullptr; // Программа, запускаемая перед получением результата
	void* PreRezProg = nullptr; // Программа, запускаемая перед получением результата    
	int OpCode = 0; // Код операции, для которой накапливаются операнды
	bool WrongFormatCheck(LoadPoint Load); // Проверка формата входных данных (возвращает true, если неправильный формат)
	void OperandsClear(int MK); // Сброс операндов при начале обоработки новой операции

public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст

	StreamIntALU(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 25;
		ProgFU(0, { 0,nullptr });
	};
	StreamIntALU() { StreamIntALU(nullptr, nullptr); };
	StreamIntALU(void* Dev); // Копирующий конструктор
};

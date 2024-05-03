#pragma once
#include "Consts.h"
#include <cmath>

class StreamFloatALU : public FU
{
	bool WrongFormatCheck(LoadPoint Load); // Проверка формата входных данных (возвращает true, если неправильный формат)
	void OperandsClear(long int MK); // Сброс операндов при начале обоработки новой операции
	void* RoutProg = nullptr; // Программа при несовпадении адреса ФУ с его собственным адресом
	void* SelfAdrProg = nullptr; // Программа при совпадении адреса ФУ с его собственным адресом
	vector<void*> AdrBuf; // Буфер адресов ФУ для пересылок
	ip IpForMkAdrOut = { 0,{0,0} }; // Сохраненная МК при обнаружении, что МК вне адресного диапазона ФУ

	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override; // Программа обоработка МК
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст
	vector<bool> FOperands; // Флаги поступления операндов  
	bool OutRezBlock = false; // Флаг блокирования выдачи результата  
	void RezExec(bool RezExec = false); // Выполнение подпрограмм при получении результата
	vector<double>RezStack; //  Стек для хранения результатов и промежуточных данных
	vector<double>RezExtStack; // Стек расширеного резульатата (например, остаток при операции целочисленного деления)
	long int OperandsCounter = 0; // Счетчик количества полученных операндов
	long int OpInd = 0; // Индекс операнда
	vector<LoadPoint>OutVars; // Ссылки на переменные для результата вычислений
	long int AngleMode = 0; // Режим измерения угла (0 - радианы, 1 -градусы)
	long int Noperands = 2; // Количество операндов для операции
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
	vector<double> Operands;// Стек операндов
	long int OpCode = 0; // Код операции, для которой накапливаются операнды
	bool MkAbort = false; // Флаг прерывания после обоработки марштуризируемой команды
	bool EarlyCalculi = false; // Флаг ранних вычислений (при многооперандных командах результат начинает вычисляться уже по приходе данных (ускоряет вычисления)
public:
	double Rez = 0; //  Результат операции
	long int Ready = 0; // Код готовности результата 0 - не готов, 1 - готов, 2 - ошибка
	vector<long int>ReceiverMk; // МК для получателя результата
	vector<FU*>ReceiverContexts; // Контекст получателя результата

	StreamFloatALU(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 12;
		ProgFU(0, { 0,nullptr });
		StreamFloatALU((void *)Templ);

	};
	StreamFloatALU() { StreamFloatALU(nullptr, nullptr); };
	StreamFloatALU(void *); // Копирующий конструктор
};

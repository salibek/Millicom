#pragma once
#include "Consts.h"
#include "StreamManager.h"

using namespace std;

class StreamIntALU : public FU
{
private:
	vector<bool> FOperands; // Флаги поступления операндов  
	bool OutRezBlock = false; // Флаг блокирования выдачи результата  
	void RezExec(long int* tRez, bool RezExec = false); // Выполнение подпрограмм при получении результата
	vector<long int>RezStack; //  Стек для хранения результатов и промежуточных данных
	vector<long int>RezExtStack; // Стек расширеного резульатата (например, остаток при операции целочисленного деления)
	long int OperandsCounter = 0; // Счетчик количества полученных операндов
	long int OpInd = 0; // Индекс операнда
	vector<long int> Operands;// Стек операндов
	vector<long int>ReceiverMk; // МК для получателя результата
	vector<FU*>ReceiverContexts; // Контекст получателя результата
	vector<LoadPoint>OutVars; // Ссылки на переменные для результата вычислений
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
	long int OpCode = 0; // Код операции, для которой накапливаются операнды
	bool WrongFormatCheck(LoadPoint Load); // Проверка формата входных данных (возвращает true, если неправильный формат)
	void OperandsClear(long int MK); // Сброс операндов при начале обоработки новой операции
	void* RoutProg = nullptr; // Программа при несовпадении адреса ФУ с его собственным адресом
	void* SelfAdrProg = nullptr; // Программа при совпадении адреса ФУ с его собственным адресом
	vector<void*> AdrBuf; // Буфер адресов ФУ для пересылок
	ip IpForMkAdrOut = { 0,{0,0} }; // Сохраненная МК при обнаружении, что МК вне адресного диапазона ФУ
	bool MkAbort = false; // Флаг прерывания после обоработки марштуризируемой команды
	bool EarlyCalculi = false; // Флаг ранних вычислений (при многооперандных командах результат начинает вычисляться уже по приходе данных (ускоряет вычисления)
	bool AccumMode = false;// Аккумуляторных режим работы
public:
	long RezBuf = 0; // Буфер результата
	long int Rez = 0; //  Результат операции
	long int Ready = 0; // Код готовности результата 0 - не готов, 1 - готов, 2 - ошибка
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст

	StreamIntALU(FU* BusContext, FU* Templ=nullptr)
	{
		Bus = BusContext;
		FUtype = 25;
		ProgFU(0, { 0,nullptr });
	};
	StreamIntALU() { StreamIntALU(nullptr, nullptr); };
	StreamIntALU(void* Dev); // Копирующий конструктор
};

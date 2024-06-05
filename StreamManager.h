#pragma once
#include "Consts.h"
#include "FUIni.h"
#include "StreamFloatALU.h"
#include "StreamIntALU.h"

using namespace std;

class StreamManager : public FU
{
private:
	map<string, vector<pair<long, FU*> > > VarFuMap; // Словарь соответствия имени переменной и FU c МК
	string VarName; // Имя текущей переменной (для ее засылки в на ФУ)
	string AddedVarName; // Имя последней добавленной переменной (для ее засылки в на ФУ)

	vector<vector<FU*> > Field; // Поле АЛУ
	long int Ind = 0, IndGroup = 0; // Индексы ФУ и группы
	long int Ind2 = 0, IndGroup2 = 0; // Вторые индексы ФУ и группы
	long int Mk1 = -1, Mk2=-1, MkLast=-1; // Милликоманды для выполнения на ФУ по первому и второму индексам
//	bool DevBusSubstitution = false; // Флаг разрешения подмены шины для устройств при выполнении МК MkExec Mk2Exec 
	FU* DevCurrent = nullptr; // Ссылка на текущее ФУ
	long DevCurrentMk = -1; // МК для текущего ФУ
	long int Counter = 1; // Счетчик действий при создании ФУ (сколько раз необходимо создавать устройства)
	long int DeviseCounter = 0;
	void* DevNotExistErrProg = nullptr; // Подпрограмма ошибки 'Нет существует устройства'
	vector<StreamFloatALU> Group; // Вектор группы ФУ для копирования
	FuFabric MakeFU;
	vector<int> ExecCounter = {1}; // Количество итераций при Exec
	bool ExecFlag = false; // Флаг выполнения подпрограммы
	void* ValNotFaund = nullptr; // Программа обработки ошибки 'Не найдена переменная по имети'
	void ProgExec(void* Uk, unsigned int CycleMode = 0, FU* Bus = nullptr, vector<ip>::iterator* Start = nullptr) override; // Исполнение программы из ИК
	void ProgExec(LoadPoint Uk, unsigned int CycleMode = 0, FU* Bus = nullptr, vector<ip>::iterator* Start = nullptr) override; // Исполнение программы из ИК
	int FUTypeCorrect = fuTypeCorrect; // Корректировка индекса тип
	long DevMkRange = 1000; // Диапазон МК для ФУ поля

public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст
	StreamManager(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 23;
		ProgFU(0, { 0,nullptr });
	};
	StreamManager() { StreamManager(nullptr, nullptr); };
	~StreamManager();
};

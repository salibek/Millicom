#pragma once
#include "Consts.h"
#include <vector>
#include "Accumulator.h"

struct ThreadContext // структура контекста
{
	vector <Accumulator> AluStack; // стек аккумуляторов
	ip* ThreadPC = nullptr;
	bool CycleFlag = false; // Флаг для циклов
	bool PostFlag = false;
	vector<int> MkOut; // вектор МК для резульатов
	vector<LoadPoint> Out; // вектор адресов для записи результатов
	int RangeStop = 0, RangeStep = 1; // Цикл по range
	int MkStage = 0; // Стадия выполнения МК
	int CycleLimit = 0; // Лимит итераций цикла (чтобы не было зацикливания)
	void* MaxProg = nullptr; // Программа, запускаемая при достижении максимума
	void* MinProg = nullptr; // Программа, запускаемая при достижении минимума
public:
//	void *Parent = nullptr;
//	ThreadContext(void* parent){Parent = parent;}
	~ThreadContext() {
		AluStack.clear(); // Метод для освобождения стека
	}

};

class ALUGeneral : public FU
{
private:
	int breakCounter = 0;
	LoadPoint NewVar = { 0,nullptr }; // Ссылка на созданную новую переменную 
public:
	vector <ThreadContext> ThreadStack; // Стек контекстов
	ip* DivisinByZeroProg = nullptr; // Программа, вызываемая при делении на 0

	void ProgFU(int MK, LoadPoint Load) override;
	ALUGeneral(FU* BusContext, FU* Templ) : FU(BusContext) {
		Bus = BusContext;
		ProgFU(0, { 0,nullptr });
	};
	ALUGeneral() : FU() {
		Bus = nullptr;
	};
	// Подрограммы сообщений об ошибках
	void* NoOperandErrProg = nullptr;
	void* IncompatibleTypesErrProg = nullptr;

};
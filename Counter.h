#pragma once
#include "Consts.h"

class Counter : public FU {
private:
	int Accum = 0; // Аккумулятор
	int *AccumUk = &Accum; // Указатель на аккумулятор
	vector <int> Stack; // Стек аккумуляторов
	int AutoInc = 0; // Шаг автоматичекой инкрементация
	int Compare = 0; // Сравинаваемое значение
	int* CompareUk = &Compare; // Указатель на стравнваемое значение
	void* EmptyStackErrProg = nullptr; // Программа обработки ошибки pop из пустого стека
	void* BiggerProg=nullptr, * LessProg = nullptr, * EQProg = nullptr, * NEQProg = nullptr, * BiggerEQProg = nullptr, * LessEQProg = nullptr;
public:
	void ProgFU(int MK, LoadPoint Load);
	Counter(FU* BusContext, FU* Templ) : FU(BusContext) { ProgFU(0, { 0, nullptr }); Bus = BusContext; };
	Counter() : FU() { Counter(nullptr, nullptr); };
};
#pragma once
#include "Consts.h"

class IntAlu : public FU {
private:
	int Accum = 0; // Аккумулятор
	int *AccumUk = &Accum; // Указатель на аккумулятор
	vector <int> Stack; // Стек аккумуляторов
	int AutoInc = 0; // Шаг автоматичекой инкрементация
	int Compare = 0; // Сравинаваемое значение
	int* CompareUk = &Compare; // Указатель на стравнваемое значение
	void* EmptyStackErrProg = nullptr; // Программа обработки ошибки pop из пустого стека
	void* BiggerProg=nullptr, * LessProg = nullptr, * EQProg = nullptr, * NEQProg = nullptr, * BiggerEQProg = nullptr, * LessEQProg = nullptr;
	void* NoCorrectTypeErrProg = nullptr; // Программа для ошибки записи некорректного (нецелого типа)
	void* NoIntTypeErrProg = nullptr; // Программа для ошибки записи нецелого типа
	void* NoBoolIntTypeErrProg = nullptr; // Программа для ошибки записи нецелого типа или boolean 
public:
	void ProgFU(int MK, LoadPoint Load);
	IntAlu(FU* BusContext, FU* Templ) : FU(BusContext) { ProgFU(0, { 0, nullptr }); Bus = BusContext; FUtype = 7; };
	IntAlu() : FU() { IntAlu(nullptr, nullptr); };
};
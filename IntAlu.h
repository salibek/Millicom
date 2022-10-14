#pragma once
#include "Consts.h"

class IntContext {
	public:
	int* AccumUk; // Указатель на аккумулятор
	int Accum; // Аккумулятор
	int AutoInc; // Шаг автоматичекой инкрементация
	int Fin; // Конечное значение цикла
};

class IntAlu : public FU {
private:
	int *AccumUk= &Accum; // Указатель на аккумулятор
	vector <IntContext> Stack; // Стек аккумуляторов
	int Compare = 0; // Сравинаваемое значение
	int* CompareUk = &Compare; // Указатель на стравнваемое значение
	void* EmptyStackErrProg = nullptr; // Программа обработки ошибки pop из пустого стека
	void* BiggerProg=nullptr, * LessProg = nullptr, * EQProg = nullptr, * NEQProg = nullptr, * BiggerEQProg = nullptr, * LessEQProg = nullptr;
	void* NoCorrectTypeErrProg = nullptr; // Программа для ошибки записи некорректного (нецелого типа)
	void* NoIntTypeErrProg = nullptr; // Программа для ошибки записи нецелого типа
	void* NoBoolIntTypeErrProg = nullptr; // Программа для ошибки записи нецелого типа или boolean 
public:
	int Accum = 0; // Аккумулятор
	int AutoInc = 0; // Шаг автоматичекой инкрементация
	int Fin = 0; // Конечное значение цикла
	void ProgFU(int MK, LoadPoint Load);
	IntAlu(FU* BusContext, FU* Templ=nullptr) : FU(BusContext) { ProgFU(0, { 0, nullptr }); Bus = BusContext; FUtype = 7; };
	IntAlu() : FU() { IntAlu(nullptr, nullptr); };
};
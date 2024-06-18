#pragma once
#include "Consts.h"

class IntContext {
public:
	long int* AccumulatUk; // Указатель на аккумулятор
	long int Accumulat; // Аккумулятор
	long int AutoInc; // Шаг автоматичекой инкрементация
	long int Fin; // Конечное значение цикла
};

class IntAlu : public FU {
private:
	void ProgExec(void* Uk, unsigned int CycleMode = 0, FU* Bus = nullptr, vector<ip>::iterator* Start = nullptr) override; // Исполнение программы из ИК
	void ProgExec(LoadPoint Uk, unsigned int CycleMode = 0, FU* Bus = nullptr, vector<ip>::iterator* Start = nullptr) override; // Исполнение программы из ИК

	vector<int> ExecCounter = { 1 }; // Количество итераций при Exec

	FU* Receiver = nullptr; // Приемщик данных
	long int * AccumulatUk= &Accumulat; // Указатель на аккумулятор
	vector <IntContext> Stack; // Стек аккумуляторов
	long int Compare = 0; // Сравинаваемое значение
	long int RandFrom = 0, RandTo = 0; // Диапазон генерируемых случайных значений
	long int* CompareUk = &Compare; // Указатель на стравнваемое значение
	void* EmptyStackErrProg = nullptr; // Программа обработки ошибки pop из пустого стека
	void* BiggerProg=nullptr, * LessProg = nullptr, * EQProg = nullptr, * NEQProg = nullptr, * BiggerEQProg = nullptr, * LessEQProg = nullptr;
	void* NoCorrectTypeErrProg = nullptr; // Программа для ошибки записи некорректного (нецелого типа)
	void* NoIntTypeErrProg = nullptr; // Программа для ошибки записи нецелого типа
	void* NoBoolIntTypeErrProg = nullptr; // Программа для ошибки записи нецелого типа или boolean 
public:
	long int Accumulat = 0; // Аккумулятор
	long int AutoInc = 0; // Шаг автоматичекой инкрементация
	long int Fin = 0; // Конечное значение цикла
	void ProgFU(long int MK, LoadPoint Load, FU* Sender=nullptr);
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст
	IntAlu(FU* BusContext, FU* Templ=nullptr) : FU(BusContext) { ProgFU(0, { 0, nullptr }); Bus = BusContext; FUtype = 7; };
	IntAlu(){ IntAlu(nullptr, nullptr); };
};
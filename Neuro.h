#pragma once
#include "Consts.h"
#include <vector>

double LinF(double S, vector<double> Args); // Передается коэффициент

double Porog(double S, vector<double> Args); // 1. x, при котором переход на более высокий уровень, 2. Значение до перехода, 3. После перехода

class Neuro : public FU
{
private:
	vector<double> W;
	vector<double> X;
	vector<bool>XF;
	vector<long int> MK_Out;
	double S = 0; // Выходной сигнал
	double eps = 0; // Погрешность
	long int FType = 0; // Индекс функции активации
	//... вектор ссылок на функции активации
	vector<double> F_Arg;
	long int Counter = 0;
	long int WInd = 0, XInd = 0; // Индексы
	long int WIndStep = 1, XIndStep = 1; // Шаг увеличения индексов веса и значения сигнала

	double (*Activation[2])(double, vector<double>) = { LinF,Porog }; // вектор функций активации
	double delta = 0, Z=0, Y=0;
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender=nullptr) override;
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст
	Neuro(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; FUtype = 10; ProgFU(0, { 0,nullptr }); };
	Neuro(){ Neuro(nullptr, nullptr); };
private:
};
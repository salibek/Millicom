#pragma once
#include <omp.h>
#include "Consts.h"

class TemperatEx : public FU  // Исполнительное устройство расчета теплового поля
{
public:
	~TemperatEx()
	{
		for (auto i : T_Neighbours)
			i.clear();
		T_Neighbours.clear();
		T.clear();
	};
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	TemperatEx(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; ProgFU(0, { 0,nullptr }); };
	TemperatEx() : FU() { Bus = nullptr; };
	vector<double> T; // Текущая температура
	vector<vector<double>> T_Neighbours; // Температура соседей
	double a = 1; // Коэффициент в уравнении
	vector<TemperatEx*> Neighbours; // Соседи ссылки
	int X=-1, Y=-1; // координаты узла
	double h2 = 0, tay = 0;
	bool FConst = false; // Флаг константной температуры
	double ReadTime = 0, CalcTime = 0; // Время записи соседнего значения и вычисления нового значения
	void* FiringProg = nullptr; // Программа, запускаемая при активации узла
};

class NetManager: public FU // Менеджер сетевых расчетов
{
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	NetManager(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; FUtype = 11; ProgFU(0, { 0,nullptr }); };
	NetManager() : FU() { Bus = nullptr; };
private:
	double ManageTime = 0; // Время работы менеджера вычислительной сетки
	FU* NetScheduler = nullptr;
	vector<vector<TemperatEx*> > Items;
	int ConstTCounter = 0; // Счетчик для ConstTSet
	int X = 0, Y = 0, XC, YC;
	double h = 0, tay = 0;
	double a = 1; // Коэффициент в уравнении
	int Ntay = 0, NtayCounter = 0; // Количество тактов моделирования
	double ReadTime = 0, CalcTime = 0; // Время записи соседнего значения и вычисления нового значения
};
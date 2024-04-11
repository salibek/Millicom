#pragma once
#include "Consts.h"
#include <fstream>
#include <vector>

class GaussPoint : public FU {
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	GaussPoint(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; };
	GaussPoint() : FU() { Bus = nullptr; };
	double a = 0;
	bool FreeF = false; //Флаг свободного члена
	bool KeyF = false; // Флаг элемента на главной диагонали
	GaussPoint* Neighbours[4] = {nullptr, nullptr, nullptr, nullptr};

	double CalcTime = 0; // Время вычисления
	double SendTime = 0; // Время пересылки данных

private:
	double UpBuf = 0;
	double S = 0;
};

class Gauss : public FU {
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
//	FU* Copy() override; // Программа копирования ФУ
//	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст)
	Gauss(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; FUtype = 16; };
	Gauss() : FU() { Bus = nullptr; };
	int Size = 10; // Размерность матрицы
	double Max = 10, Min = 10;// Максимальная и минимальная значения коээфициентов и свободных членов
private:
		vector< vector<GaussPoint> > Field;
};


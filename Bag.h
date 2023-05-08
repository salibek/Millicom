#pragma once
#include "Consts.h"
#include <fstream>
#include <vector>

class BagPoint : public FU {
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	BagPoint(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; };
	BagPoint() : FU() { Bus = nullptr; };
	double p = 0; // = max (p_Up, p_Diagonal)
	double p_Up = -1;
	double p_Diag = -1;

	double w_i = 0;   // текущий объем ( = номеру столбца матрицы)
	double w_a = 0;   // объем предмета w[i]

	// Ссылки на нижнего соседа и соседа по диагонали в нижнем ряду. Обоим передаем значение p
	BagPoint* Down = nullptr;
	BagPoint* Diagonal = nullptr;
	BagPoint* Next = nullptr;

	double CalcTime = 0; // Время вычисления
	double SendTime = 0; // Время пересылки данных
};

class Bag : public FU {
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	Bag(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; FUtype = 15; };
	Bag() : FU() { Bus = nullptr; };
private:
	vector< vector<BagPoint> > Field;
	int WMax = 0;			// Размер рюкзака
	int NItem = 10; // Количество элементов для укладки в рюкзак
	double  PMaxOfItem = 10; // Максимальный размер одной вещи и ее максимальная цена
	int WMaxOfItem = 10; //  Максимальная цена вещи
	vector<double> P;		// Вектор стоимостей предметов
	vector<int> W;			// Вектор объемов предметов

	double CalcTime = 0;	// Время вычисления
	double SendTime = 0;	// Время пересылки данных
};


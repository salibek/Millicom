#pragma once
#include "Consts.h"
#include <fstream>
#include <vector>

class SimplexPoint : public FU {
	double Val = 0;
	vector < SimplexPoint* > Neighbours = {nullptr, nullptr, nullptr, nullptr};
};

class Simplex : public FU {
public:
	void ProgFU(int MK, LoadPoint Load) override;
	Simplex(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; ProgFU(0, { 0,nullptr }); };
	Simplex() : FU() { Bus = nullptr; };
private:
	vector< vector<SimplexPoint> > Field;
	int WMax = 0;
	vector<double> P; // Вектор стоимостей предметов
	vector<int> W; // Вектор объемов предметов
};


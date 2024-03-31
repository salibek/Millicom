#pragma once
#include "Consts.h"
#include "Consts.h"

class MatPlot : public FU
{
private:
	vector<double> X,Y; // Исходные данные для 2-мерного графика
	vector< vector<double> > Mesh; //  Сетка для рисования
	vector< vector<double> > Z; //  2-мерный график
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	MatPlot(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 26;
		ProgFU(0, { 0,nullptr });
	};
	MatPlot() { MatPlot(nullptr, nullptr); };
};
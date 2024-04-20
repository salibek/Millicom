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
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст
	MatPlot(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 26;
		ProgFU(0, { 0,nullptr });
	};
	MatPlot() { MatPlot(nullptr, nullptr); };
};
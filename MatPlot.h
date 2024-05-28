#pragma once
#include "Consts.h"
#include "Consts.h"

class MatPlot : public FU
{
private:
	vector<double> X,Y; // Исходные данные для 2-мерного графика
	vector< vector<double> > Mesh; //  Сетка для рисования
	vector< vector<double> > Z; //  2-мерный график
	double XStart = 0, XEnd = 0; // Начило и конец интервала по оси X
	double Xh = 1; // Шаг генерации точек по X
	long XN = 1; // Количество точек по оси Z
	long N=0, M = 0; // Количество строк и количество элементов в строке матрицы сетки
	
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
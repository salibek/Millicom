#pragma once
#include "Consts.h"

class MatPlot : public FU
{
private:
	vector<double> X,Y; // Исходные данные для 2-мерного графика
	vector< vector<double> > MeshY, MeshX; //  Сетка для рисования
	vector< vector<double> > Z; //  2-мерный график
	double Start = 0, End = 0; // Начило и конец интервала по оси X
	double XStart = 0, XEnd = 0, YStart = 0, YEnd = 0; // Начило и конец интервала по оси X и Y
	double h = 1; // Шаг генерации точек по X
	long N = 1; // Количество точек по оси Z
	long Nx=0, Ny = 0; // Количество строк и количество элементов в строке матрицы сетки
	double Dx = 1, Dy = 1; // Шаг сетки для 3-мерного графика по x и y
	
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
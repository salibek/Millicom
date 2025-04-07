#pragma once
#include "Consts.h"
#  if defined(_DEBUG)
#    define PYBIND11_DEBUG_MARKER
#    undef _DEBUG
#  endif
#include "matplotlibcpp.h"

#include <fstream>      // Для проверки существования файла
#include <sstream>      // Для обработки строк
#include <stdexcept>    // Для исключений
class MatPlot : public FU
{
private:

	std::vector<std::vector<double>> excelData; // Для хранения числовых данных из Excel
	std::vector<std::string> excelHeaders;     // Для хранения заголовков столбцов (если они есть)
	bool FileDataOrientation = false; // Орииентация данных в файле горизонтальная/вертикальная
	vector<double> X, Y,Z; // Исходные данные для 2-мерных графиков
	vector< vector<double> > MeshY, MeshX; //  Сетка для рисования
	vector< vector<double> > X0,Y0, Z0; //  3-мерный график

	void WriteToCSV(const std::string& fileName);
	void WriteToExcel(const std::string& fileName);
	void ReadFromExcel(const std::string& fileName);
	void ReadFromCSV(const std::string& fileName);

	//vector<double> kk = { 0,1,2,3,6 };
	double Start = 0, End = 10; // Начало и конец интервала по оси X
	double XStart = 0, XEnd = 10, YStart = 0, YEnd = 10, ZStart = 7, ZEnd = 12; // Начило и конец интервала по оси X, Z и Y

	double h = 0.001, hZ = 0.01; // Шаг генерации точек по X, Z
	double hY = 0.25; // Шаг генерации точек по Y
	long N = 1; // Количество точек по оси X
	long Nx = 0, Ny = 0, Nz = 0; // Количество строк и количество элементов в строке матрицы сетки
	double Dx = 1, Dy = 1; // Шаг сетки для 3-мерного графика по x и y
	
	int ZRow = 2, ZCol = 5;

	int PlotWidth = 1; // Толщина графика
	double xAxe = 0;
	double yAxe = 0;
	string PlotFormat = ""; //Тип графика
	float PlotWigth = 1; //Толщина графика
	bool grid_draw = false; //Сетка

	string plotAxis = "equal"; //Масштаб графика
	string plotTitle = "";
	string xTitle = "";
	string yTitle = "";
	string zTitle = "";
	string legendText = "";
	string plotText = "";
	string xCat = "";
	vector<string> xCategories = {}; //Имена столбцов столбчатой диаграммы
	vector<string> categories = { }; //Названия частей круговой диаграммы
	vector<double> values = { 20.1, 15.7, 30.3, 25.2 }; //Значения для цветовой карты диаграммы рассеивания
	//int types = 1;
	int rows = 1; //количество строк в окне
	int cols = 1; //количество столбцов в окне
	int rowId = 0; //строка графика в окне
	int colId = 0; //столбец графика в окне
	double lineWidth3 = 1.0; //Толщина 3-мерного линейного графика
	double barmWidth = 1.0; //Толщина границы графтка
	double barWidth = 1.0; //
	double scatterSize = 1; //Размер маркера диграммы рассеивания
	PyObject* ax; //Оси 3-мерного графика
	int numCols = 3; //Количество колонок гистограммы
	double histAlpha = 1; //Яркость гистограммы
	bool histSort = false; //Сортировать значения гистограммы


	vector<string > Format0 = { "Blue","*","1","1","Red","Black" }; // Color, Marker, MarkerSize, LineWidth
	vector<string > Format1 = { "White","White","Black","false","Black","1" }; // Color, Marker, MarkerSize, LineWidth

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
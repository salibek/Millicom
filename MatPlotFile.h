#pragma once
#include "Consts.h"

#include <vector>
#include <string>

#  if defined(_DEBUG)
#    define PYBIND11_DEBUG_MARKER
#    undef _DEBUG
#  endif
#include "matplotlibcpp.h"

class MatPlot : public FU
{
private:
	// Поля для хранения данных из Excel
	std::vector<std::vector<double>> excelData; // Для хранения числовых данных из Excel
	std::vector<std::string> excelHeaders;     // Для хранения заголовков столбцов (если они есть)

	vector<double> X, Y; // Данные для 2D-графика
	vector< vector<double> > Z; // 2D матрица для хранения данных
	vector< vector<double> > MeshY, MeshX; // Сетки для отображения графика
	vector< vector<double> > X0,Y0,Z0; // 2D данные для отображения
	bool FileDataOrietation = false; // Ориентация данных в файле: false - по строкам, true - по столбцам

	vector<double> kk = { 0,1,2,3,6 }; // Пример значений для графика
	double Start = 0, End = 10; // Диапазоны данных по оси X
	double XStart = 0, XEnd = 0, YStart = 0, YEnd = 0, ZStart = 7, ZEnd = 12; // Диапазоны для осей X, Y и Z

	double h = 0.5; // Шаг сетки по оси X
	double hZ = 0.25; // Шаг сетки по оси Z
	long N = 1; // Количество данных по оси Z
	long Nx = 0, Ny = 0; // Размерности по осям X и Y
	double Dx = 1, Dy = 1; // Шаг сетки для 3D-графика по осям X и Y
	
	int ZRow = 2, ZCol = 5; // Размерность матрицы данных Z

	int PlotWidth = 1; // Толщина линии графика
	double xAxe = 0; // Позиция оси X
	double yAxe = 0; // Позиция оси Y
	string PlotFormat = ""; // Формат отображения графика
	float PlotWigth = 1; // Толщина линии для графика
	bool grid_draw = false; // Включение/выключение отображения сетки

	string plotAxis = "equal"; // Тип отображения осей
	string plotTitle = ""; // Заголовок графика
	string xTitle = ""; // Заголовок оси X
	string yTitle = ""; // Заголовок оси Y
	string zTitle = ""; // Заголовок оси Z
	string legendText = ""; 
	string plotText = ""; // Текст на графике
	string xCat = ""; // Категории для оси X
	vector<string> xCategories = {}; // Перечень категорий по оси X
	vector<string> categories = { "Category A", "Category B", "Category C", "Category D" }; // Пример категорий для графика
	vector<double> values = { 20.1, 15.7, 30.3, 25.2 }; // Пример значений для категорий
	int types = 10; // Тип графика
	int rows = 1; // Количество строк в подграфиках
	int cols = 1; // Количество столбцов в подграфиках
	int rowId = 0; // Идентификатор строки
	int colId = 0; // Идентификатор столбца
	double lineWidth3 = 1.0; // Толщина линии для 3D графиков
	double barmWidth = 1.0; // Ширина столбца для гистограммы
	double barWidth = 1.0;
	double scatterSize = 1; // Размер точек для графиков рассеяния
	PyObject* ax;
	int numCols = 3; // Количество столбцов для подграфиков
	double histAlpha = 1; // Прозрачность для гистограммы
	bool histSort = false; // Сортировка данных для гистограммы


	vector<string > Format0 = { "Blue","*","1","1","Red","Black" }; // Color, Marker, MarkerSize, LineWidth
	vector<string > Format1 = { "White","White","Black","false","Black","1" }; // Color, Marker, MarkerSize, LineWidth

public:
	void WriteToCSV(const std::string& fileName); // Вывод данных в CSV-файл

	void WriteToExcel(const std::string& fileName); // Вывод данных в Excel-файл

	// Метод для чтения данных из Excel-файла
	void ReadFromExcel(const std::string& fileName);

	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // Метод для создания копии объекта
	FU* TypeCopy() override; // Метод для создания копии объекта с типом данных
	MatPlot(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 26;
		ProgFU(0, { 0,nullptr });
	};
	MatPlot() { MatPlot(nullptr, nullptr); };
};
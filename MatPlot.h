#pragma once
#include "Consts.h"
#  if defined(_DEBUG)
#    define PYBIND11_DEBUG_MARKER
#    undef _DEBUG
#  endif

#if MatPlotInclude
#include "matplotlibcpp.h"
#endif
class MatPlot : public FU
{
private:
	vector<double> X, Y,Z; // �������� ������ ��� 2-������� �������
	vector< vector<double> > MeshY, MeshX; //  ����� ��� ���������
	vector< vector<double> > X0,Y0,Z0; //  2-������ ������

	vector<double> kk = { 0,1,2,3,6 };
	double Start = 0, End = 10; // ������ � ����� ��������� �� ��� X
	double XStart = 0, XEnd = 10, YStart = 0, YEnd = 10, ZStart = 7, ZEnd = 12; // ������ � ����� ��������� �� ��� X � Y

	double h = 0.001; // ��� ��������� ����� �� X
	double hZ = 0.25;
	long N = 1; // ���������� ����� �� ��� Z
	long Nx = 0, Ny = 0; // ���������� ����� � ���������� ��������� � ������ ������� �����
	double Dx = 1, Dy = 1; // ��� ����� ��� 3-������� ������� �� x � y
	
	int ZRow = 2, ZCol = 5;

	int PlotWidth = 1; // ������� �������
	double xAxe = 0;
	double yAxe = 0;
	string PlotFormat = "";
	float PlotWigth = 1;
	bool grid_draw = false;

	string plotAxis = "equal";
	string plotTitle = "";
	string xTitle = "";
	string yTitle = "";
	string zTitle = "";
	string legendText = "";
	string plotText = "";
	string xCat = "";
	vector<string> xCategories = {};
	vector<string> categories = { "Category A", "Category B", "Category C", "Category D" };
	vector<double> values = { 20.1, 15.7, 30.3, 25.2 };
	int types = 1;
	int rows = 1;
	int cols = 1;
	int rowId = 0;
	int colId = 0;
	double lineWidth3 = 1.0;
	double barmWidth = 1.0;
	double barWidth = 1.0;
	double scatterSize = 1;
	//PyObject* ax=nullptr;
	int numCols = 3;
	double histAlpha = 1;
	bool histSort = false;
	string filename; // Имя файла


	vector<string > Format0 = { "Blue","*","1","1","Red","Black" }; // Color, Marker, MarkerSize, LineWidth
	vector<string > Format1 = { "White","White","Black","false","Black","1" }; // Color, Marker, MarkerSize, LineWidth

public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������
	MatPlot(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 26;
		ProgFU(0, { 0,nullptr });
	};
	MatPlot() { MatPlot(nullptr, nullptr); };
};
#include "MatPlot.h"
#if MatPlotInclude
namespace plt = matplotlibcpp;
#endif // MatPlotInclude

void MatPlot::ProgFU(long int MK, LoadPoint Load, FU* Sender) {
	MK %= FUMkRange;
	switch (MK)
	{
	case 0: //Reset
		X.clear(); Y.clear();
		MeshX.clear();
		MeshY.clear();
		Z.clear();
		break;
#if MatPlotInclude

	case 1: // Plot Нарисовать график (На входе тип графика)
	{
		plt::rcparams({
	{"figure.facecolor", Format1[0]},  // Цвет фона всего рисунка
	{"axes.facecolor", Format1[1]},   // Цвет фона области графика
	{"axes.edgecolor", Format1[2]},        // Цвет рамки области графика
	{"axes.grid", Format1[3]},              // Включить сетку
	{"grid.color", Format1[4]},             // Цвет сетки
	{"grid.alpha", Format1[5]}               // Прозрачность сетки
			});
		plt::subplot2grid(rows, cols, rowId, colId);
		numCols = Y.size() - 1;
		switch (Load.toInt()) {
		case 0: // Обычный график
		{
			plt::plot(X, Y, { {"color", Format0[0]},              // Set line color 
			{"marker", Format0[1]},               // Set marker type (circle in this case) 
			{"markersize",Format0[2]},
			{"linewidth",Format0[3]},
			{"markerfacecolor",Format0[4]},
			{"markeredgecolor",Format0[5]}
				});
		}
		break;
		case 1:
		{
			
			plt::bar(X, Y, Format0[0],  "-", barmWidth, barWidth,
				{
					{"color", Format0[4]}
				});

		}
			break;
		case 2:
			plt::barh(X, Y, Format0[0], "-", barmWidth, barWidth,
				{
					{"color", Format0[4]}
				});
			break;
		case 3:
			plt::hist(Y, numCols, Format0[0], histAlpha, histSort);
			break;
		case 4:
			plt::pie(X);
			break;
		case 5:
		{
			Z.push_back(X);
			Z.push_back(Y);
			plt::boxplot(Z);
		}
			break;
		case 6:
			plt::scatter(X, Y,scatterSize,{ {"marker",Format0[1]},{"edgecolors",Format0[0]},{"cmap","magma"}});
			break;
		}
		//plt::yticks(values, categories);
		plt::show();
	}
			break;
		case 5: // XAdd Добавить координату по X
			X.push_back(Load.toDouble());
			break;
		case 6: // StartSet Установить начальное заначение отрезка по X
			Start = Load.toDouble();
			break;
		case 7:	// EndSet Установить конечное заначение отрезка по X
			End = Load.toDouble();
			break;
		case 8:	// hSet Установить шаг точек отрезка по X
			X.clear();
			h = Load.toDouble();
			N = 0;
			for (double x = Start; x <= End; x += h) // Генерация точек по оси X
			{
				N++;
				X.push_back(x);
			}
			break;
		case 9:	// NSet Установть количестов точек отрезка по X
			X.clear();
			N = Load.toDouble();
			h = (End - Start) / (N - 1);
			for (double x = Start; x <= End; x += h) // Генерация точек по оси X
			{
				N++;
				X.push_back(x);
			}

			break;
		case 10: // XVectSet // Установить вектор по координате X
			if (!Load.isVect()) break;
			X.resize(Load.toVect()->size());
			{
				auto L = Load.toVect()->begin();
				for (auto x = X.begin(); x != X.end(); x++, L++)
					*x = L->toDouble();
			}
			break;
		case 11: // YVectSet // Установить вектор по координате Y
			if (!Load.isVect()) break;
		
			Y.resize(Load.toVect()->size());
			{
				auto L = Load.toVect()->begin();
				for (auto y = Y.begin(); y != Y.end(); y++, L++)
					*y = L->toDouble();
			}
			break;
		case 12: // MeshSet // Установить сетку координат для 3-мерного графика
			if (!Load.isVect()) break;
			//...
			break;
		case 13: // ZSet // Установить значения для 3-мерного графика
			if (!Load.isVect()) break;
			//...
			break;
		case 14: //Show_plot//Вывести график
			plt::show(false);
			break;
		case 15:

		case 20: // XVectOut Выдать вектор значений по оси X
			Load.Write(X);
			break;
		case 21: // XVectOutMk Выдать МК с вектором значений по оси X
		{
			MkExec(Load, { CintArray, &X });
			break;
		}
		case 22: // XLoadVectOut Выдать вектор значений по оси X
		case 23: // XLoadVectOutMk Выдать МК с вектором значений по оси X
		{
			vector<LoadPoint> t;
			t.resize(X.size());
			auto tuk = t.begin();
			for (auto xuk = X.begin(); xuk != X.end(); xuk++)
			{
				*tuk = { Cint,new double };
				*(double*)(tuk->Point) = *xuk;
			}
			if (MK == 23)MkExec(Load, { CintArray, &X });
			else Load.Write(t);
			for (auto tuk = t.begin(); tuk != t.end(); tuk++)
				delete (double*)tuk->Point;
		}
			break;
		case 25: // YVectOut Выдать вектор значений по оси X
			Load.Write(Y);
			break;
		case 26: // YVectOutMk Выдать вектор значений по оси X
		{
			MkExec(Load, { CintArray, &Y });
			break;
		}
		case 27: // XLoadVectOut Выдать вектор значений по оси X
		case 28: // XLoadVectOutMk Выдать МК с вектором значений по оси X
		{
			vector<LoadPoint> t;
			t.resize(Y.size());
			auto tuk = t.begin();
			for (auto xuk = Y.begin(); xuk != Y.end(); xuk++)
			{
				*tuk = { Cint,new double };
				*(double*)(tuk->Point) = *xuk;
			}
			if (MK == 23)MkExec(Load, { CintArray, &X });
			else Load.Write(t);
			for (auto tuk = t.begin(); tuk != t.end(); tuk++)
				delete (double*)tuk->Point;
		}
		break;

		case 35: // DxSet
			Dx = Load.toDouble();
			break;
		case 36: // DySet
			Dy = Load.toDouble();
			break;
		case 37: // XStartSet
			XStart = Load.toDouble();
			break;
		case 38: // XEndSet
			XEnd = Load.toDouble();
			break;
		case 39: // YStartSet
			YStart = Load.toDouble();
			break;
		case 40: // YEndSet
			YEnd = Load.toDouble();
			break;
		case 45: // NxSet
			Nx = Load.toDouble();
			N = Load.toInt();
			break;
		case 46: // NySet
			Ny = Load.toInt();
			break;
		case 50: //MeshN Сгенерировать сетку для отображения 3-мерного графика по количеству точек
			MeshX.clear();
			MeshY.clear();
			Dy = (YEnd - YStart) / (Ny - 1);
			Dx = (XEnd - XStart) / (Nx - 1);
			MeshY.resize(Ny);
			for (int i = 0; i < Ny; i++)
				MeshY[i].resize(Nx);
			for (int i = 0; i < Ny; i++)
				for (int j = 0; j < Nx; j++)
					MeshY[i][j] = YStart + i * Dy;
			for (int i = 0; i < Ny; i++)
				for (int j = 0; j < Nx; j++)
					MeshX[i][j] = XStart + j * Dy;
					
			break;
		case 51: //MeshD Сгенерировать сетку для отображения 3-мерного графика по длине шага
			Nx = Ny = 0;
			for (double y = YStart; y <= YEnd; y += Dy)
			{
				MeshY.push_back({});
				Ny++;
				for (double x = XStart; x <= XEnd; x += Dx);

			}
			break;

		case 55: // YAdd Добавить значение функции
			Y.push_back(Load.toDouble());
			break;
		case 56: // Y3DAdd Добавить значение функции для 3-мерного графика
			Y.push_back(Load.toDouble());
			break;

		case 60: // XSort Упорядочить данные по оси X по возрастанию
			for(int k=0;k<X.size()-1;k++)
				for(int i=0; i<X.size()-1-k;i++)
					if (X[i] > X[i + 1])
					{
						swap(X[i], X[i + 1]);
						swap(Y[i], Y[i + 1]);
					}
			break;

		case 61: // PlotTitleSet Добавить заголовок
		{
			plotTitle = Load.toStr();
			plt::title(plotTitle);
		}
			break;
		case 62: // xTitleSet Подписать ось X
		{
			xTitle = Load.toStr();
			plt::xlabel(xTitle);
		}
			break;
		case 63: // yTitleSet Подписать ось Y
		{
			yTitle = Load.toStr();
			plt::ylabel(yTitle);
		}
			break;
		case 64: //LegendSet Добавить легенду
		{
			plt::named_plot(legendText, X, Y);
			plt::legend();
		}
		break;
		case 65: //LegendSet Добавить текст легенды
			legendText = Load.toStr();
			break;
		case 66:
			plt::text(xAxe, yAxe, plotText);
			break;
		case 67: //xTextSet Добавить координату X поля с текстом
			xAxe = Load.toDouble();
			break;
		case 68: //yTextSet Добавить координату X поля с текстом
			yAxe = Load.toDouble();
			break;
		case 69: //TextSet Добавить текст для поля
			plotText = Load.toStr();
			break;
		case 70:
			Format0[3] = Load.toStr();
			break;
		case 71:
			Format0[4] = Load.toStr();
			break;
		case 72:
			Format0[5] = Load.toStr();
			break;
		case 73: //RowsSet Установить количество столбцов фигуры
			rows = Load.toInt();
			break;
		case 74: //ColsSet Установить количество столбцов фигуры
			cols = Load.toInt();
			break;
		case 75: //RowIdSet Установить номер строки графика
			rowId = Load.toInt();
			break;
		case 76: //ColIdSet Установить номер столбца графика
			colId = Load.toInt();
			break;
		case 77: // BarmWidthSet Установить цвет фона всего рисунка
			barmWidth = Load.toDouble();
			break;
		case 78: // BarWidthSet Установить ширину столбцов
			barWidth = Load.toDouble();
			break;
		case 79: // HistAlphaSet Установить прозрачность гистограммы
			histAlpha = Load.toDouble();
			break;
		case 80: // NumColsSet Установить количество частей гистограммы
			numCols = Load.toInt();
			break;
		case 81: // HistSortSet Сортировать значения гистограммы
			histSort = Load.toBool();
			break;
		case 82: // ScatterSizeSet Установить размер маркера
			scatterSize = Load.toDouble();
			break;
		case 94: // FigureColorSet Установить цвет фона всего рисунка
			Format1[0] = Load.toStr();
			break;
		case 95: // AxesColorSet Установить цвет фона области графика
			Format1[1] = Load.toStr();
			break;
		case 96: // EdgeColorSet Установить цвет рамки области графика
			Format1[2] = Load.toStr();
			break;
		case 97: // GridAlphaSet Включить сетку
			Format1[3] = Load.toStr();
			break;
		case 98: // GridColorSet Установить цвет сетки
			Format1[4] = Load.toStr();
			break;
		case 99: // PlotFormatSet Установить прозрачность сетки
			Format1[5] = Load.toStr();
			break;
		case 100: // PlotFormatSet Установить формат графика
			PlotFormat = Load.toStr();
			break;
		case 101: // MarkSet Установить маркер
			Format0[1] = Load.toStr();
			break;
		case 102: // MarkWidthSet Установить размер маркера
			Format0[2] = Load.toStr();
			break;
		case 103: // LineColorSet Установить цвет
			Format0[0] = Load.toStr();
			break;
		case 104: // LineWidthSet Установить цвет
			Format0[3] = Load.toStr();
			break;
#endif // MatPlotInclude

		default:
			CommonMk(MK, Load, Sender);
			break;
		}
}

FU* MatPlot::Copy() // Программа копирования ФУ
{
	return new MatPlot(Bus, this);
}

FU* MatPlot::TypeCopy() // Создать ФУ такого же типа (не копируя контекст
{
	return new MatPlot(Bus, nullptr);
}

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

	case 1: // Plot ���������� ������ (�� ����� ��� �������)
	{
		plt::rcparams({
	{"figure.facecolor", Format1[0]},  // ���� ���� ����� �������
	{"axes.facecolor", Format1[1]},   // ���� ���� ������� �������
	{"axes.edgecolor", Format1[2]},        // ���� ����� ������� �������
	{"axes.grid", Format1[3]},              // �������� �����
	{"grid.color", Format1[4]},             // ���� �����
	{"grid.alpha", Format1[5]}               // ������������ �����
			});
		plt::subplot2grid(rows, cols, rowId, colId);
		numCols = Y.size() - 1;
		switch (Load.toInt()) {
		case 0: // ������� ������
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
		case 5: // XAdd �������� ���������� �� X
			X.push_back(Load.toDouble());
			break;
		case 6: // StartSet ���������� ��������� ��������� ������� �� X
			Start = Load.toDouble();
			break;
		case 7:	// EndSet ���������� �������� ��������� ������� �� X
			End = Load.toDouble();
			break;
		case 8:	// hSet ���������� ��� ����� ������� �� X
			X.clear();
			h = Load.toDouble();
			N = 0;
			for (double x = Start; x <= End; x += h) // ��������� ����� �� ��� X
			{
				N++;
				X.push_back(x);
			}
			break;
		case 9:	// NSet ��������� ���������� ����� ������� �� X
			X.clear();
			N = Load.toDouble();
			h = (End - Start) / (N - 1);
			for (double x = Start; x <= End; x += h) // ��������� ����� �� ��� X
			{
				N++;
				X.push_back(x);
			}

			break;
		case 10: // XVectSet // ���������� ������ �� ���������� X
			if (!Load.isVect()) break;
			X.resize(Load.toVect()->size());
			{
				auto L = Load.toVect()->begin();
				for (auto x = X.begin(); x != X.end(); x++, L++)
					*x = L->toDouble();
			}
			break;
		case 11: // YVectSet // ���������� ������ �� ���������� Y
			if (!Load.isVect()) break;
		
			Y.resize(Load.toVect()->size());
			{
				auto L = Load.toVect()->begin();
				for (auto y = Y.begin(); y != Y.end(); y++, L++)
					*y = L->toDouble();
			}
			break;
		case 12: // MeshSet // ���������� ����� ��������� ��� 3-������� �������
			if (!Load.isVect()) break;
			//...
			break;
		case 13: // ZSet // ���������� �������� ��� 3-������� �������
			if (!Load.isVect()) break;
			//...
			break;
		case 14: //Show_plot//������� ������
			plt::show(false);
			break;
		case 15:

		case 20: // XVectOut ������ ������ �������� �� ��� X
			Load.Write(X);
			break;
		case 21: // XVectOutMk ������ �� � �������� �������� �� ��� X
		{
			MkExec(Load, { CintArray, &X });
			break;
		}
		case 22: // XLoadVectOut ������ ������ �������� �� ��� X
		case 23: // XLoadVectOutMk ������ �� � �������� �������� �� ��� X
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
		case 25: // YVectOut ������ ������ �������� �� ��� X
			Load.Write(Y);
			break;
		case 26: // YVectOutMk ������ ������ �������� �� ��� X
		{
			MkExec(Load, { CintArray, &Y });
			break;
		}
		case 27: // XLoadVectOut ������ ������ �������� �� ��� X
		case 28: // XLoadVectOutMk ������ �� � �������� �������� �� ��� X
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
		case 50: //MeshN ������������� ����� ��� ����������� 3-������� ������� �� ���������� �����
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
		case 51: //MeshD ������������� ����� ��� ����������� 3-������� ������� �� ����� ����
			Nx = Ny = 0;
			for (double y = YStart; y <= YEnd; y += Dy)
			{
				MeshY.push_back({});
				Ny++;
				for (double x = XStart; x <= XEnd; x += Dx);

			}
			break;

		case 55: // YAdd �������� �������� �������
			Y.push_back(Load.toDouble());
			break;
		case 56: // Y3DAdd �������� �������� ������� ��� 3-������� �������
			Y.push_back(Load.toDouble());
			break;

		case 60: // XSort ����������� ������ �� ��� X �� �����������
			for(int k=0;k<X.size()-1;k++)
				for(int i=0; i<X.size()-1-k;i++)
					if (X[i] > X[i + 1])
					{
						swap(X[i], X[i + 1]);
						swap(Y[i], Y[i + 1]);
					}
			break;

		case 61: // PlotTitleSet �������� ���������
		{
			plotTitle = Load.toStr();
			plt::title(plotTitle);
		}
			break;
		case 62: // xTitleSet ��������� ��� X
		{
			xTitle = Load.toStr();
			plt::xlabel(xTitle);
		}
			break;
		case 63: // yTitleSet ��������� ��� Y
		{
			yTitle = Load.toStr();
			plt::ylabel(yTitle);
		}
			break;
		case 64: //LegendSet �������� �������
		{
			plt::named_plot(legendText, X, Y);
			plt::legend();
		}
		break;
		case 65: //LegendSet �������� ����� �������
			legendText = Load.toStr();
			break;
		case 66:
			plt::text(xAxe, yAxe, plotText);
			break;
		case 67: //xTextSet �������� ���������� X ���� � �������
			xAxe = Load.toDouble();
			break;
		case 68: //yTextSet �������� ���������� X ���� � �������
			yAxe = Load.toDouble();
			break;
		case 69: //TextSet �������� ����� ��� ����
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
		case 73: //RowsSet ���������� ���������� �������� ������
			rows = Load.toInt();
			break;
		case 74: //ColsSet ���������� ���������� �������� ������
			cols = Load.toInt();
			break;
		case 75: //RowIdSet ���������� ����� ������ �������
			rowId = Load.toInt();
			break;
		case 76: //ColIdSet ���������� ����� ������� �������
			colId = Load.toInt();
			break;
		case 77: // BarmWidthSet ���������� ���� ���� ����� �������
			barmWidth = Load.toDouble();
			break;
		case 78: // BarWidthSet ���������� ������ ��������
			barWidth = Load.toDouble();
			break;
		case 79: // HistAlphaSet ���������� ������������ �����������
			histAlpha = Load.toDouble();
			break;
		case 80: // NumColsSet ���������� ���������� ������ �����������
			numCols = Load.toInt();
			break;
		case 81: // HistSortSet ����������� �������� �����������
			histSort = Load.toBool();
			break;
		case 82: // ScatterSizeSet ���������� ������ �������
			scatterSize = Load.toDouble();
			break;
		case 94: // FigureColorSet ���������� ���� ���� ����� �������
			Format1[0] = Load.toStr();
			break;
		case 95: // AxesColorSet ���������� ���� ���� ������� �������
			Format1[1] = Load.toStr();
			break;
		case 96: // EdgeColorSet ���������� ���� ����� ������� �������
			Format1[2] = Load.toStr();
			break;
		case 97: // GridAlphaSet �������� �����
			Format1[3] = Load.toStr();
			break;
		case 98: // GridColorSet ���������� ���� �����
			Format1[4] = Load.toStr();
			break;
		case 99: // PlotFormatSet ���������� ������������ �����
			Format1[5] = Load.toStr();
			break;
		case 100: // PlotFormatSet ���������� ������ �������
			PlotFormat = Load.toStr();
			break;
		case 101: // MarkSet ���������� ������
			Format0[1] = Load.toStr();
			break;
		case 102: // MarkWidthSet ���������� ������ �������
			Format0[2] = Load.toStr();
			break;
		case 103: // LineColorSet ���������� ����
			Format0[0] = Load.toStr();
			break;
		case 104: // LineWidthSet ���������� ����
			Format0[3] = Load.toStr();
			break;
#endif // MatPlotInclude

		default:
			CommonMk(MK, Load, Sender);
			break;
		}
}

FU* MatPlot::Copy() // ��������� ����������� ��
{
	return new MatPlot(Bus, this);
}

FU* MatPlot::TypeCopy() // ������� �� ������ �� ���� (�� ������� ��������
{
	return new MatPlot(Bus, nullptr);
}

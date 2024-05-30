#include "MatPlot.h"

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
		case 1: // Plot Нарисовать график (На входе тип графика)
			// .....Load.toInt()
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
		case 13: // ZMatrSet // Установить значения для 3-мерного графика
			if (!Load.isVect()) break;
			//...
			break;
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

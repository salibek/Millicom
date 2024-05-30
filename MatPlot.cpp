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
		case 1: // Plot ���������� ������ (�� ����� ��� �������)
			// .....Load.toInt()
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
		case 13: // ZMatrSet // ���������� �������� ��� 3-������� �������
			if (!Load.isVect()) break;
			//...
			break;
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

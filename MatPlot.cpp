#include "MatPlot.h"

void MatPlot::ProgFU(long int MK, LoadPoint Load, FU* Sender) {
	MK %= FUMkRange;
	switch (MK)
		{
		case 0: //Reset
			X.clear(); Y.clear();
			Mesh.clear();
			Z.clear();
			break;
		case 1: // Plot ���������� ������ (�� ����� ��� �������)
			// .....Load.toInt()
			break;
		case 5: // XAdd �������� ���������� �� X
			X.push_back(Load.toDouble());
			break;
		case 6: // XStartSet ���������� ��������� ��������� ������� �� X
			XStart = Load.toDouble();
			break;
		case 7:	// XEndSet ���������� �������� ��������� ������� �� X
			XEnd = Load.toDouble();
			break;
		case 8:	// XhSet ���������� ��� ����� ������� �� X
			X.clear();
			Xh = Load.toDouble();
			XN = 0;
			for (double x = XStart; x <= XEnd; x += Xh) // ��������� ����� �� ��� X
			{
				XN++;
				X.push_back(x);
			}
			break;
		case 9:	// XNSet ��������� ���������� ����� ������� �� X
			X.clear();
			XN = Load.toDouble();
			Xh = (XEnd - XStart) / (XN - 1);
			for (double x = XStart; x <= XEnd; x += Xh) // ��������� ����� �� ��� X
			{
				XN++;
				X.push_back(x);
			}
			break;

		case 20: // NSet
			N = Load.toInt();
			break;
		case 21: // MSet
			M = Load.toInt();
			break;
		case 30: //Mesh ������������� ����� ��� ����������� 3-������� �������
			// ...
			break;
		case 35: // YAdd �������� ���������� �� X
			Y.push_back(Load.toDouble());
			break;
		
		

		case 40: // XSort ����������� ������ �� ��� X �� �����������
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

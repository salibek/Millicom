#pragma once
#include "Consts.h"

class MatPlot : public FU
{
private:
	vector<double> X,Y; // �������� ������ ��� 2-������� �������
	vector< vector<double> > MeshY, MeshX; //  ����� ��� ���������
	vector< vector<double> > Z; //  2-������ ������
	double Start = 0, End = 0; // ������ � ����� ��������� �� ��� X
	double XStart = 0, XEnd = 0, YStart = 0, YEnd = 0; // ������ � ����� ��������� �� ��� X � Y
	double h = 1; // ��� ��������� ����� �� X
	long N = 1; // ���������� ����� �� ��� Z
	long Nx=0, Ny = 0; // ���������� ����� � ���������� ��������� � ������ ������� �����
	double Dx = 1, Dy = 1; // ��� ����� ��� 3-������� ������� �� x � y
	
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
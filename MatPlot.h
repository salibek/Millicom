#pragma once
#include "Consts.h"
#include "Consts.h"

class MatPlot : public FU
{
private:
	vector<double> X,Y; // �������� ������ ��� 2-������� �������
	vector< vector<double> > Mesh; //  ����� ��� ���������
	vector< vector<double> > Z; //  2-������ ������
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
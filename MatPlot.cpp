#include "MatPlot.h"

void MatPlot::ProgFU(long int MK, LoadPoint Load, FU* Sender) {
	MK %= FUMkRange;
	switch (MK)
		switch (MK)
		{
		case 0: //Reset
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

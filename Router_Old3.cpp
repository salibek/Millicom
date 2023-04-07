#include "stdafx.h"
#include "consts.h"
#include "Router.h"
#include "FUini.h"

using namespace std;

void Router::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case 0: // �����
		Channels.clear();
		MKCount = 0;
		break;
	case 1: // ������� ����� ����� (�� ����� �� ������ ������� ���������)
	{
		Channels.push_back({ 0,Load.toInt(-1),Bus,-1 });
		if (Channels.back().Down >= 0)
			Channels.back().Up = Channels.back().Down + FUMkRange; // ��������� ������� ������� �� ���������
		break;
	}
	case 3: // ���������� ������ �������� ������ �������
		break;
	case 5: // ���������� ������� �������� �� ��� ������� (������ �������� ���������������, ��� ������� ���� �������� ��)
		break;
	case 6: // ���������� ����� �������������
		RoutingTime = Load.toDouble();
		break;
	case 8: // ���������� ������ �������� ������ �����
		break;
	case 9: // ���������� ������� �������� ������� ������
		Channels.back().Up = Load.toInt(-1);
		break;
	case 10: // ���������� ������ �������� ������� ������
	{
		Channels.back().Down = Load.toInt(-1);
		if (Channels.back().Up < 0)
			Channels.back().Up = Channels.back().Down + FUMkRange; // �������������� ��������� �������� ������� ��������� ������� ��-��������� �� ���������
		break;
	}
	case 11: // ���������� ������ �� ���� ��� �������� ������ �����
		Channels[Ind].Receiver = Load.Point;
		break;
	case 15: // ������ ���������� ������� �� �������
		Load.Write(Channels.size());
		break;
	case 20: // ������ �� � ����������� ������� �� �������
		{
		int t = Channels.size();
		MkExec(Load, { Cint, &t });
		break;
		}
	case 25: // ������ ��������� �� ���� ��� �������� ������
		Load.Write(Channels[Ind].Receiver);
		break;
	case 30: // ������ �� � ���������� �� ���� ��� �������� ������
		MkExec(Load, { CFU, &Channels[Ind].Receiver });
		break;
	case 35: // ������ ������� �������� ������� ��� �������� ������
		Load.Write(Channels[Ind].Up);
		break;
	case 40: // ������ �� � ������� ���������� ������� ��� �������� ������
		MkExec(Load, { Cint, &Channels[Ind].Up });
	case 45: // ������ ������ �������� ��� �������� ������
		break;
	case 50:// ������ �� � ������ ���������� ��� �������� ������
		{
		break;
		}
	default:
		CommonMk(MK, Load);
		break;
	}
}
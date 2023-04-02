#include "stdafx.h"
#include "consts.h"
#include "Gateway.h"
#include "FUini.h"

using namespace std;

void Gateway::ProgFU(int MK, LoadPoint Load)
{

	switch (MK)
	{
	case 0: // Reset �����
		Router = nullptr; // ������ �� ������
		GatewayFriend = nullptr; //������ �� ������ ����
		TranslTime = 0; // ����� �������� ������� ������
		TransError = 0; // ����������� ������ ��������
		RoutingProg = nullptr; //
		Mode = 0;
		break;
	case 1: // RouterSet ���������� ������ �� ������
		Router = (FU*)Load.Point;
		break;
	case 5:
		GatewayFriend = (FU*)Load.Point;
		break;
	case 10: //
		TranslTime = Load.toInt(); // ����� �������� ������� ������
		break;
	case 11: //
		TransError = Load.toInt(); // ����������� ������ ��������
		break;
	case 12: //
		Mode = Load.toInt(); // 0 - �������, 1 - �������� �� �����, 2 - �������� � �����, 3 - �����������
		break;
	case 15: //
		RoutingProg = Load.Point; //
		break;
	case 16: //
		OverflowProg = nullptr; //
		break;
	case 17: //
		MkOverflowProg = nullptr; //
		break;

	case 20: // InDataCounterOut
		Load.Write(Indata);
		break;
	case 21: // InDataCounterOutMk
		MkExec(Load, { Cint, &Indata });
		break;

	default:
		CommonMk(MK, Load);
		break;
	}
}
#include "stdafx.h"
#include "consts.h"
#include "Gateway.h"
#include "FUini.h"

using namespace std;

void Gateway::ProgFU(int MK, LoadPoint Load)
{
	if (!(MK < FUMkRange || MK >= GatewayMkRange && MK < GatewayMkRange + FUMkRange))
	{
		MkCount++; // ��������� ��������� ��, ���������� ����� ����
		GatewayFriend->ProgFU(MK, Load); // �������� ��������� �� ������ ����
	}
	else

	switch (MK%FUMkRange)
	{
	case 0: // Reset 
		Router = nullptr; // 
		GatewayFriend = nullptr; //
		TranslTime = 0; //
		TransError = 0; // 
		GatewayProg = nullptr; //
		Mode = 0;
		break;
	case 1: // RouterSet ���������� ������ �� ������
		Router = (FU*)Load.Point;
		break;
	case 5: // ���������� ������ �� ������ ���� (��� ��-������  ������� �������� ������������)
		GatewayFriend = (FU*)Load.Point;
		break;
	case 10: // ����� �������� ������� ������
		TranslTime = Load.toInt();
		break;
	case 11: // ����������� ������ ��������
		TransError = Load.toInt(); 
		break;
	case 12: // ���������� ����� �������� ������: �������� �� �����, �������� � �����, �����������, �������
		Mode = Load.toInt(); 
		break;
	case 15: // ���������� ������ �� ��������� �������� ��������� ������ ������� � ������������� �������� ������
		GatewayProg = Load.Point;
		break;
	case 16: // ���������� ������ �� ������������ ��������� ������� ������������ �����
		OverflowProg = nullptr; //
		break;
	case 17: // ���������� ������ �� ������������ ��������� ������� ���������� ������������� ���������� �� � ������
		MkOverflowProg = nullptr;
		break;
	case 20: // DataCounterOut
		Load.Write(DataCount);
		break;

	// ���� ����������
	case 25: // ���������� �������� �������� ������
		break;
	case 30: // ���������� ������������ ����� ������ ������ ��� ��������
		break;
	case 35: // ���������� ������������ ���������� ��, ������������ � ������
		break;
	case 40: // ����� ����� ����������
		break;
	case 45: // ������ ����� ���������� ������ ����� ����� (� ��� �������)
		break;
	case 50: // ������ �� � ������� ���������� ������ ����� ����� (� ��� �������)
		break;
	case 55: // ������ ����� ���������� ������ ����� �����
		break;
	case 60: // ������ �� � ������� ���������� ������ ����� �����
		break;
	case 65: // ������ ����� �������� ������ ����� �����
		break;
	case 70: // ������ �� � ������� �������� ������ ����� �����
		break;
	//

	default:
		CommonMk(MK, Load);
		break;
	}
}
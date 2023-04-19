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
	case 5: // GatewayFriendSet ���������� ������ �� ������ ���� (��� ��-������  ������� �������� ������������)
		GatewayFriend = (FU*)Load.Point;
		break;
	case 10: // TranslTimeSet ����� �������� ������� ������
		TranslTime = Load.toInt();
		break;
	case 11: // TransErrorSet ����������� ������ ��������
		TransError = Load.toInt(); 
		break;
	case 12: // ModeSet ���������� ����� �������� ������: �������� �� �����, �������� � �����, �����������, �������
		Mode = Load.toInt(); 
		break;
	case 15: // GatewayProgSet ���������� ������ �� ��������� �������� ��������� ������ ������� � ������������� �������� ������
		GatewayProg = Load.Point;
		break;
	case 16: // MkOverflowProgSet ���������� ������ �� ������������ ��������� ������� ������������ �����
		OverflowProg = nullptr; //
		break;
	case 17: // OverflowProgSet ���������� ������ �� ������������ ��������� ������� ���������� ������������� ���������� �� � ������
		MkOverflowProg = nullptr;
		break;
	case 20: // DataCounterOut
		Load.Write(DataCount);
		break;

	// ���� ����������
	case 25: // TranslTimeSet ���������� �������� �������� ������
//		TranslTime = Load.toInt();
		break;
	case 30: //  ���������� ������������ ����� ������ ������ ��� ��������
//		 = Load.toInt();
		break;
	case 35: // ���������� ������������ ���������� ��, ������������ � ������
//		 = Load.toInt();
		break;
	case 40: // StatsReset ����� ����� ����������
		DataCount = 0;
		MkCount = 0;
		MaxMKQueue = 0;
		AverageMKQueue = 0;
		break;
	case 45: // DataCountOut ������ ����� ���������� ������ ����� ����� (� ��� �������)
		Load.Write(DataCount);
		break;
	case 50: // DataCountMkOut ������ �� � ������� ���������� ������ ����� ����� (� ��� �������)
		MkExec(Load, { Cint, &DataCount });
		break;
	case 55: // ������ ����� ���������� ������ ����� �����
//		Load.Write();
		break;
	case 60: // ������ �� � ������� ���������� ������ ����� �����
//		MkExec(Load, { Cint, & });
		break;
	case 65: // ������ ����� �������� ������ ����� �����
//		Load.Write();
		break;
	case 70: // ������ �� � ������� �������� ������ ����� �����
//		MkExec(Load, { Cint, & });
		break;
	//
	default:
		CommonMk(MK, Load);
		break;
	}
}
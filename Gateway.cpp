#include "stdafx.h"
#include "consts.h"
#include "Gateway.h"
#include "FUini.h"

using namespace std;

void Gateway::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	if (!(MK < FUMkRange || MK >= GatewayMkRange && MK < GatewayMkRange + FUMkRange))
	{
		MkCount++; // ��������� ��������� ��, ���������� ����� ����
		DataCount += Load.DataSize(); // ������� ������ ���������� ������
		GatewayFriend->ProgFU(MK, Load, this); // �������� ��������� �� ������ ����
		if (Modeling != nullptr)
		{
			MaxMKQueue = max(MaxMKQueue, (int)Modeling->qmk.size()); // ���������� ������������ ����� �������
			MaxDataSize = max(MaxDataSize, DataSize); // ���������� ������������� ������ ������ � ������
			DataSize -= Load.DataSize() + 4; // ��������� ��
		}
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
		TranslTime = Load.toDouble();
		break;
	case 30: //  ���������� ������������ ����� ������ ������ ��� ��������
		 MaxDataSize = Load.toInt();
		break;
	case 35: // ���������� ������������ ���������� ��, ������������ � ������
//		MaxDataSize = Load.toInt();
		break;
	case 40: // StatsReset ����� ����� ����������
		DataCount = 0;
		MkCount = 0;
		MaxMKQueue = 0;
		AverageMKQueue = 0;
		break;
	case 45: // DataCountOut ������ ����� ���������� ������ ����� ���� (� ��� �������)
		Load.Write(DataCount);
		break;
	case 50: // DataCountMkOut ������ �� � ������� ���������� ������ ����� ���� (� ��� �������)
		MkExec(Load, { Cint, &DataCount });
		break;
	case 55: // ������ ����� ���������� ������ ����� ����
//		Load.Write();
		break;
	case 60: // ������ �� � ������� ���������� ������ ����� ����
//		MkExec(Load, { Cint, & });
		break;
	case 65: // ������ ����� �������� ������ ����� ����
//		Load.Write();
		break;
	case 70: // ������ �� � ������� �������� ������ ����� ����
//		MkExec(Load, { Cint, & });
		break;
	//
	default:
		CommonMk(MK, Load);
		break;
	}
}
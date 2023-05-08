#include "stdafx.h"
#include "consts.h"
#include "Gateway.h"
#include "FUini.h"
#include "SchedulerEventser.h"

using namespace std;

double Gateway::DelayFunc() // ������� ���������� �������� �������� ������
{
	return MkTranslTime;
}

void Gateway::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	if (MK<0 || !(MK < FUMkRange || MK >= FUMkGlobalRange && MK < FUMkGlobalRange + FUMkRange)) // �� �� ��������
	{
//		cout << FUName <<" " << MK << endl;
//		Modeling->EventModelingPrint();
		if (Modeling != nullptr && Modeling->ManualMode && Modeling->scheduler != nullptr && !Modeling->SchedulerFlag)
		{
			Modeling->SchedulerFlag = false;
			Modeling->qmk.push_back({ MK, Load, Sender });
			((Scheduler*)(Modeling->scheduler))->Scheduling(this, ProcessingTime);
			return;
		}

		//ProcessingTime
		MkCount++; // ��������� ��������� ��, ���������� ����� ����
		DataCount += Load.DataSize(); // ������� ������ ���������� ������
		if(MK>=0)
			GatewayFriend->MkAwait(-MK, Load, Sender, DelayFunc()); // �������� ��������� �� ������ ����
		else
			Receiver->MkAwait(-MK, Load, Sender, DelayFunc()); // �������� ��������� �� ������ ����

		if (Modeling != nullptr)
		{
			MaxMKQueue = max(MaxMKQueue, (int)Modeling->qmk.size()); // ���������� ������������ ����� �������
			MaxDataSize = max(MaxDataSize, DataSize); // ���������� ������������� ������ ������ � ������
			DataSize -= Load.DataSize() + 4; // ��������� ��
		}
		if(Modeling!=nullptr)
			((Scheduler*)Modeling->scheduler)->CoreFree();

	}
	else
	switch (MK%FUMkRange)
	{
	case 0: // Reset 
		Receiver = nullptr; // 
		GatewayFriend = nullptr; //
		TransmTime = 0; //
		TransError = 0; // 
		GatewayProg = nullptr; //
		Mode = 0;
		break;
	case 1: // ResiverSet ���������� ������ �� ������
		Receiver = (FU*)Load.Point;
		break;
	case 2: // RoutingSimulat ��������� �������� ������
		ProgFU(((IC_type)Load.Point)->begin()->atr, ((IC_type)Load.Point)->begin()->Load);
		break;
	case 5: // GatewayFriendSet ���������� ������ �� ������ ���� (��� ��-������  ������� �������� ������������)
		GatewayFriend = (FU*)Load.Point;
		break;
//	case 10: // TransmTimeSet ���������� ���� �������� ������� ������
//		TransmTime = Load.toInt();
//		break;
	case 11: // TransErrorSet ���������� ����������� ������ ��������
		TransError = Load.toInt(); 
		break;
	case 12: // ModeSet ���������� ����� �������� ������: �������� �� �����, �������� � �����, �����������, �������
		Mode = Load.toInt(); 
		break;
	case 13: // ProcessingTimeSet ���������� ����� ��������� ������� ��������
		ProcessingTime = Load.toDouble();
		break;
	case 15: // GatewayProgSet ���������� ������ �� ��������� �������� ��������� ������ ������� � ������������� �������� ������
		GatewayProg = Load.Point;
		break;
	case 16: // MkOverflowProgSet ���������� ������ �� ������������ ��������� ������� ���������� ������������� ���������� �� � ������ 
		OverflowProg = Load.Point; // 
		break;
	case 17: // OverflowProgSet // ���������� ������ �� ������������ ��������� ������� ������������ �����
		MkOverflowProg = Load.Point;
		break;
	case 20: // DataCounterOut ������ ����� ���������� ������
		Load.Write(DataCount);
		break;
	case 21: // DataCounterOutMk ������ �� � ������� ���������� ������
		MkExec(Load, {Cint, &DataCount },this);
		break;

	case 22: //MkTranslTimeSet ���������� ����� �������� �� �� ������ ����� 
		MkTranslTime=Load.toDouble();
		break;
	case 23: // DataTransmRateSet ���������� �������� �������� ������ (� ���/�)
		DataTransmRate = Load.toDouble();
		break;
	case 24: // HeaderVolumSet ���������� ����� ��������� ������������� ������ ������
		HeaderVolum = Load.toInt();
		break;

		// ���� ����������
	case 30: //  MaxDataBufSet ���������� ������������ ����� ������ ������ ��� ��������
		 MaxDataSize = Load.toInt();
		break;
	case 35: // MaxMkBufSet ���������� ������������ ���������� ��, ������������ � ������
		MaxMKQueue = Load.toInt();
		break;
	case 40: // StatReset ����� ����� ����������
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
	case 55: // DataCountMkOutMk ������ ����� ���������� ������ ����� ����
		Load.Write(DataCount);
		break;
	case 60: // ������ �� � ������� ���������� ������ ����� ����
		MkExec(Load, { Cint, &DataCount }, this);		break;
	case 65: // DataCountOut ������ ����� �������� ������ ����� ����
//		Load.Write();
		break;
	case 70: // DataCountOutMk ������ �� � ������� �������� ������ ����� ����
//		MkExec(Load, { Cint, & });
		break;
	//
	default:
		CommonMk(MK, Load);
		break;
	}

}
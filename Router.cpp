#include "stdafx.h"
#include "consts.h"
#include "Router.h"
#include "FUini.h"

using namespace std;

int Router::ChennelSearch(int MK) // ����� ������ �� �� (���������� -1, ���� ����� �� ������)
{
	auto i = Channels.begin();
	for (; i != Channels.end(); i++)
		if (i->Up <= MK < i->Down) break;
	if (i == Channels.end()) // ����� �� ������
		return -1;
	return distance(Channels.begin(), i); // ������� ����� ������
}

void Router::ProgFU(int MK, LoadPoint Load)
{
	if (!(MK < FUMkRange || MK >= FUMkGlobalRange && MK < FUMkGlobalRange + FUMkRange)) // Routing
	{
		// �������������
		// SendInd = ChennelSearch(MK); // ���������� ����� ������, �� �������� ������ �� ��� �������������

		ProgExec(ReceiveProg); // ��������� ��������� �� ������� ������
		MKCount++;
		LaslMkIp = {MK,Load}; // ����������� ��������� ��������� ��� ������������� ��
		if (RoutingProg != nullptr)
		{
			ProgExec(RoutingProg); // ������ �������� ������� ����������
			return;
		}
		// ����������� ��������� ������������
		SendInd = -1;
		for (auto i = Channels.begin(); i != Channels.end(); i++)
			if (i->Up <= MK && MK < (i->Down)){
				SendInd = distance(Channels.begin(), i);
				break;
			}
		if (SendInd >= 0) // ����������� �������������
		{
			Channels[SendInd].MkCount++; // ��������� ��
			Channels[SendInd].DataCount += Load.DataSize();
			((FU*) Channels[SendInd].Receiver)->ProgFU(MK,Load);
		}

	}
	else // �� ��� �������
	switch (MK%FUMkRange)
	{
	case 0: // �����
		Channels.clear();
		MKCount = 0;
		break;
	// ��������� � ���������� ���������������
	case 1: // ChCreate ������� ����� ����� (�� ����� �� ������� ������� ���������)
	{
		//if (Load.Point == nullptr) break;
		Channels.push_back({ Load.toInt(-1),0 ,Bus,-1 });
		if (Channels.back().Up >= 0)
			Channels.back().Down = Channels.back().Up + FUMkRange; // ��������� ������� ������� �� ���������
		Ind = Channels.size() - 1; // ��������� ������ �� ��������� ��������� �����
		break;
	}
	case 2: // RoutingSimulat �������� ������� �� �� �������������
		ProgFU(((IC_type)Load.Point)->begin()->atr, ((IC_type)Load.Point)->begin()->Load);
		break;
	case 3: // IndSet ���������� ������ �������� ������ �������
		Ind = Load.toInt();
		break;
	case 5: // RangeSet ���������� ������� �������� �� ��� ������� (������ �������� ���������������, ��� ������� ���� �������� ��)
		FUMkGlobalRange = Load.toInt();
		break;
	case 6: // RouterTimeSet ���������� ����� �������������
		RoutingTime = Load.toDouble();
		break;
	case 9: // ChRangeUpSet ���������� ������� �������� ������� ������
		Channels[Ind].Up = Load.toInt(-1);
		break;
	case 10: // ChRangeDownSet ���������� ������ �������� ������� ������
	{
		Channels[Ind].Down = Load.toInt(-1);
		//if (Channels.back().Up < 0)
		//	Channels.back().Up = Channels.back().Down + FUMkRange; // �������������� ��������� �������� ������� ��������� ������� ��-��������� �� ���������
		break;
	}
	case 11: // ChGatewaySet ���������� ������ �� ���� ��� �������� ������ �����
		Channels[Ind].Receiver = (FU*)Load.Point;
		break;
	case 15: // ChNumOut ������ ���������� ������� �� �������
		Load.Write(Channels.size());
		break;
	case 20: // ChNumOutMk ������ �� � ����������� ������� �� �������
	{
		int t = Channels.size();
		MkExec(Load, { Cint, &t });
		break;	}
	case 25: // ChGatewayOut ������ ��������� �� ���� ��� �������� ������
		Load.Write(Channels[Ind].Receiver);
		break;
	case 30: // ChGatewayOutMk ������ �� � ���������� �� ���� ��� �������� ������
		MkExec(Load, { CFU, &Channels[Ind].Receiver });
		break;
	case 35: // ChRangeUpOut ������ ������� �������� ������� ��� �������� ������
		Load.Write(Channels[Ind].Up);
		break;
	case 40: // ChRangeUpOutMk ������ �� � ������� ���������� ������� ��� �������� ������
		MkExec(Load, { Cint, &Channels[Ind].Up });
	case 45: // ChRangeDownOut ������ ������ �������� ��� �������� ������
		Load.Write(Channels[Ind].Down);
		break;
	case 50:// ChRangeDownOutMk ������ �� � ������ ���������� ��� �������� ������
		MkExec(Load, { Cint, &Channels[Ind].Down });
		break;
	//
	// ������������
	case 55: // RoutingProgSet ���������� ������ �� ��������� �������������
		RoutingProg = Load.Point;
		break;
	case 60: // OverflowProgSet ���������� ������ �� ������������ ������� �� ������������ ������ ��� �������� ������
		OverflowProg = Load.Point;
		break;
	case 61: // MkOverflowProgSet ���������� ������ �� ������������ ������� �� ������������ ������ ��
		MkOverflowProg = Load.Point;
		break;
	case 62: // RoutindErrProgSet ���������� ������ �� ��������� ��������� ������� ����� �� ��� ������������� �� ������ � ������� �������������
		RoutingErrProg = Load.Point;
		break;
		//
	// ������� ����������
	case 65: // RoutingAttributeOut ������ ������� ��������� ��� ������������� ��
		Load.Write(LaslMkIp.atr);
		break;
	case 66: // RoutingMkOut
	//	Load.Write(LaslMkIp);
		break;
	case 70: // RoutingAttributeOutMk ������ �� � ��������� ��������� ��� ������������� ��
		MkExec(Load, { Cint, &LaslMkIp.atr });
		break;
	case 71: // RoutingAttributeOutMk ������ �� � ��������� ��������� ��� ������������� ��
		MkExec(Load, { CIP, &LaslMkIp });
		break;
		//	case 85: // ChRoutingMkOut ������ ������ ������, �� �������� ������ ��������� �� ��� �������������
//		Load.Write(Ind);
//		break;
//	case 90: // ChRoutingMkOutMk ������ �� � �������, �� �������� ������ ��������� �� ��� �������������
//		MkExec(Load, { Cint, &Channels[Ind] });
//		break;
	case 105: // ��������� ��������� �� � ����� � ������� (����� ������ � �������� �� ��� �������)
		Channels[Load.toInt()].Receiver->ProgFU(LaslMkIp.atr, LaslMkIp.Load);
		break;
//	case 120: // ReceiveIndOut
//		Load.Write(SendInd);
//		break;
//	case 125: // ReceiveIndOutMk ������ �� � �������� ������, �� ������� ������ ����� ��� �������������
//		MkExec(Load, { Cint, &SendInd });
//		break;
	//
	// ���� ����������
	case 320: // RouterStatsClear �������� ���� ���������� ���������� ������ ����� ������
		MKCount = 0;
		MaxMKQueue = 0;
		AverageMKQueue = 0;
		for (auto i = Channels.begin(); i != Channels.end(); i++)
		{
			i->MkCount = 0;
			i->DataCount = 0;
		};
		break;
	case 325: // RouterDataCountOut ������ ����� ���������� ������ ����� ������
	{
		int sum = 0;
		for (auto i = Channels.begin(); i != Channels.end(); i++)
			sum += i->DataCount;
		Load.Write(sum);
		break;
	}
	case 330: // RouterDataInCountOutMk ������ �� � ������� ���������� ������ ����� ������
	{
		int sum = 0;
		for (auto i = Channels.begin(); i != Channels.end(); i++)
			sum += i->DataCount;
		MkExec(Load, { Cint, &sum });
		break;
	}
	case 335: // ChDataInCountOut ������ ����� ���������� ������ ����� ������� ����� (�� ������� �������� ������)
		Load.Write(Channels[Ind].DataCount);
		break;
	case 340: // ChDataInCountOutMk ������ �� � ������� ���������� ������ ����� �����
		MkExec(Load, { Cint, &Channels[Ind].DataCount });
		break;
	case 345: // RouterMkCountOut ������ ���������� ���������� �� ����� ������
		Load.Write(MKCount);
		break;
	case 350: // RouterMkCountOutMk ������ �� � ����������� ���������� �� ����� ������
		MkExec(Load, { Cint, &MKCount });
		break;
	case 355: // ChMkInCountOut ������ ���������� ���������� �� ����� �����
		Load.Write(Channels[Ind].MkCount);
		break;
	case 360: // ChMkInCountOutMk ������ �� � ����������� ���������� �� ����� �����
		MkExec(Load, { Cint, &Channels[Ind].MkCount });
		break;
	case 365: // RouterMaxMkQueueOut ������ ���� ����� ������� �� �� �������
		Load.Write(MaxMKQueue);
		break;
	case 370: // RouterMaxMkQueueOutMk ������ �� � ���� ������ ������� �� �� �������
		MkExec(Load, { Cint, &MaxMKQueue });
		break;
	case 375: // RouterAverageMkQueueOut ������ ������� ����� ������� �� �� �������
		Load.Write(AverageMKQueue);
		break;
	case 380: // RouterAverageMkQueueOuMk ������ �� �� ������� ������ ������� �� �� �������
		MkExec(Load, { Cint, &AverageMKQueue });
		break;
	case 395: // ReceiveProgSet ���������� ������ �� ���������, ���������������� ��� ������� �� ��� ������������� (��� ���������� ��������������� ����� ����������)
		ReceiveProg = Load.Point;
		break;
	//
	default:
		CommonMk(MK, Load);
		break;
	}
}
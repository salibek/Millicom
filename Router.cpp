#include "stdafx.h"
#include "consts.h"
#include "Router.h"
#include "FUini.h"

using namespace std;

long int Router::ChennelSearch(long int MK) // ����� ������ �� �� (���������� -1, ���� ����� �� ������)
{
	auto i = Channels.begin();
	for (; i != Channels.end(); i++)
		if (i->Up <= MK < i->Down) break;
		else if (i->Up == i->Down && i->Up==0) break; // ����� �� ���������
	if (i == Channels.end()) // ����� �� ������
		return -1;
	return distance(Channels.begin(), i); // ������� ����� ������
}

double Router::DelayGen(channel ch) //��������� �������� �������� ������
{
	return ch.Delay; // ���������� ��������
}

void Router::NetRouting(long int MK, LoadPoint Load, FU* Sender) // ������������� � ���� ��, ����������� �� �������������� ������
{
	MK -= FUMkRange; // ������� ������ ������
	vector<int> D; // ������������ ��� �������� �� �����
	// ���������� ������������� �� ������ ����������
	D.push_back(((MK / FUMkRange) % ((SectorDim[2] / SectorDim[1]) * SectorDim[0]) / SectorDim[0]) - SectorCoordinate[0]);
	D.push_back((MK / FUMkRange) / SectorDim[2] - SectorCoordinate[1]);
	//	for(int i=1; i<SectorDim.size();i++)
//		D.push_back(MK/FUMkRange - SectorCoordinate[i]);
	int c = 0;
	for (auto i : D)
		if (i != 0) c++;
	c = rand() % c; // ��������������� ��������� ������� �� ���� �� ���������
	for(int i=0; i<SectorDim.size(); i++)
		if (D[i] != 0)
		{
			c--;
			if (!c)
			{
				SectorReceivers[D[i]<0 ? 0: 1]->ProgFU(MK, Load, Sender);
			}
		}
}

void Router::ProgFU(long int MK, LoadPoint Load, FU* Sender)
{
	if (!(MK < FUMkRange || MK >= FUMkGlobalAdr && MK < FUMkGlobalAdr + FUMkRange)) // Routing
	{
//		cout << FUName << " " << MK << endl;
//		Modeling->EventModelingPrint();
		// �������������
		// SendInd = ChennelSearch(MK); // ���������� ����� ������, �� �������� ������ �� ��� �������������

		if (Modeling != nullptr && Modeling->ManualMode && Modeling->scheduler != nullptr && !Modeling->SchedulerFlag)
		{
			Modeling->SchedulerFlag = false;
			Modeling->qmk.push_back({ MK, Load });
			((Scheduler*)(Modeling->scheduler))->Scheduling(this, RoutingTime);
			return;
		}

		ProgExec(ReceiveProg); // ��������� ��������� �� ������� ������
		MKCount++;
		DataCount += Load.DataSize() + 4;
		if (Modeling != nullptr)
		{
			MaxMKQueue = max(int(MaxMKQueue), (int)Modeling->qmk.size()); // ���������� ������������ ����� �������
			MaxDataSize =max(int(MaxDataSize), int(DataSize)); // ���������� ������������� ������ ������ � ������
			DataSize -= Load.DataSize() + 4; // ��������� ��
		}
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
			Channels[SendInd].MkOutCount++; // ��������� ��
			Channels[SendInd].DataOutCount += Load.DataSize();
			//if(Modeling==nullptr)

			((FU*) Channels[SendInd].Receiver)->MkAwait(MK,Load, Sender, DelayGen(Channels[SendInd]));
		}
		else if (SectorDim.size() > 0) // ����������� ����� ���������, ������������� �������� �������������
		{
			
		}

		// ����� �������������
		if (Modeling!=0)
			((Scheduler*)Modeling->scheduler)->CoreFree();
		// -------------------
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
		FUMkGlobalAdr = Load.toInt();
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
		long int t = Channels.size();
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
	case 51: // ChDelaySet ����������� ����������� �������� �������� ������ �� ������
		Channels[Ind].Delay = Load.toDouble();
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
	case 63: //EventserSet ���������� ��������� �� ���������� �������
		eventser = (Eventser *) Load.Point;
		break;
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
		Channels[Load.toInt()].Receiver->ProgFU(LaslMkIp.atr, LaslMkIp.Load, this);
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
		//AverageMKQueue = 0;
		for (auto i = Channels.begin(); i != Channels.end(); i++)
		{
			i->MkOutCount = 0;
			i->DataOutCount = 0;
		};
		break;
	case 325: // RouterDataCountOut ������ ����� ���������� ������ ����� ������
	{
		int sum = 0;
		for (auto i = Channels.begin(); i != Channels.end(); i++)
			sum += i->DataOutCount;
		Load.Write(sum);
		break;
	}
	case 330: // RouterDataCountOutMk ������ �� � ������� ���������� ������ ����� ������
	{
		int sum = 0;
		for (auto i = Channels.begin(); i != Channels.end(); i++)
			sum += i->DataOutCount;
		MkExec(Load, { Cint, &sum });
		break;
	}
	case 335: // ChDataOutCountOut ������ ����� ���������� ������ �� �������� ������ (�� ������� �������� ������)
		Load.Write(Channels[Ind].DataOutCount);
		break;
	case 336: // ChDataInCountOut ������ ����� ��������� ������ � ������� ����� (�� ������� �������� ������)
		Load.Write(Channels[Ind].DataInCount);
		break;
	case 340: // ChDataOutCountOutMk ������ �� � ������� ���������� ������ ���������� �� ������
		MkExec(Load, { Cint, &Channels[Ind].DataOutCount });
		break;
	case 341: // ChDataInCountOutMk ������ �� � ������� ��������� ������ � �����
		MkExec(Load, { Cint, &Channels[Ind].DataInCount });
		break;
	case 345: // RouterMkCountOut ������ ���������� ���������� �� ����� ������
		Load.Write(MKCount);
		break;
	case 350: // RouterMkCountOutMk ������ �� � ����������� ���������� �� ����� ������
		MkExec(Load, { Cint, &MKCount });
		break;
	case 355: // ChMkInCountOut ������ ���������� ���������� �� ����� �����
		Load.Write(Channels[Ind].MkOutCount);
		break;
	case 360: // ChMkInCountOutMk ������ �� � ����������� ���������� �� ����� �����
		MkExec(Load, { Cint, &Channels[Ind].MkOutCount });
		break;
	case 365: // RouterMaxMkQueueOut ������ ���� ����� ������� �� �� �������
		Load.Write(MaxMKQueue);
		break;
	case 370: // RouterMaxMkQueueOutMk ������ �� � ���� ������ ������� �� �� �������
		MkExec(Load, { Cint, &MaxMKQueue });
		break;
	case 375: // RouterAverageMkQueueOut ������ ������� ����� ������� �� �� �������
		Load.Write((double) DataCount / MKCount);
		break;
	case 380: // RouterAverageMkQueueOuMk ������ �� �� ������� ������ ������� �� �� �������
	{
		double t = (double)DataCount / MKCount;
		MkExec(Load, { Cdouble, &t });
	}
		break;
	case 395: // ReceiveProgSet ���������� ������ �� ���������, ���������������� ��� ������� �� ��� ������������� (��� ���������� ��������������� ����� ����������)
		ReceiveProg = Load.Point;
		break;

	case 450: // SectorClear �������� ����������� �������������� �������������� ����� � �������
		SectorDim.clear();
		SectorCoordinate.clear();
		SectorReceivers.clear();
		break;
	case 451: // SectorDimAdd // �������� ����������� ���������
		SectorDim.push_back(Load.toInt());
		break;
	case 452: // SectorCoordinateAdd �������� ���������� �������
		SectorCoordinate.push_back(Load.toInt());
		break;
	case 453: // SectorRouterAdd �������� ������������ ������
		SectorReceivers.push_back((Router*)Load.Point);
	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}

FU* Router::Copy() // ��������� ����������� ��
{
	return new Router(Bus, this);
}

FU* Router::TypeCopy() // ������� �� ������ �� ���� (�� ������� ��������
{
	return new Router(Bus, nullptr);
}

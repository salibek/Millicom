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
	if (!(MK < FUMkRange || MK >= RouterMkRange && MK < RouterMkRange + FUMkRange))
	{
		// �������������
		SendInd = ChennelSearch(MK); // ���������� ����� ������, �� �������� ������ �� ��� �������������
		ProgExec(ReceiveProg); // ��������� ��������� �� ������� ������
		MKCount++;

		if (RoutingProg != nullptr)
		{
			ProgExec(RoutingProg); // ������ �������� ������� ����������
			return;
		}

		if (SendInd >= 0) // ����������� �������������
		{
			Channels[SendInd].MkOutCount++; // ��������� ��
			((FU*) Channels[SendInd].Receiver)->ProgFU(MK,Load);
		}
		// ����������� ��������� ������������

	}
	else // �� ��� �������
	switch (MK%FUMkRange)
	{
	case 0: // �����
		Channels.clear();
		MKCount = 0;
		break;
	// ��������� � ���������� ���������������
	case 1: // ������� ����� ����� (�� ����� �� ������ ������� ���������)
	{
		Channels.push_back({ 0,Load.toInt(-1),Bus,-1 });
		if (Channels.back().Down >= 0)
			Channels.back().Up = Channels.back().Down + FUMkRange; // ��������� ������� ������� �� ���������
		break;
	}
	case 3: // ���������� ������ �������� ������ �������
		Ind = Load.toInt();
		break;
	case 5: // ���������� ������� �������� �� ��� ������� (������ �������� ���������������, ��� ������� ���� �������� ��)
		RouterMkRange = Load.toInt();
		break;
	case 6: // ���������� ����� �������������
		RoutingTime = Load.toDouble();
		break;
	case 9: // ���������� ������� �������� ������� ������
		Channels[Ind].Up = Load.toInt(-1);
		break;
	case 10: // ���������� ������ �������� ������� ������
	{
		Channels[Ind].Down = Load.toInt(-1);
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
		break;	}
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
		Load.Write(Channels[Ind].Down);
		break;
	case 50:// ������ �� � ������ ���������� ��� �������� ������
		MkExec(Load, { Cint, &Channels[Ind].Down });
		break;
	//
	// ������������
	case 55: // ���������� ������ �� ��������� �������������
		RoutingProg = Load.Point;
		break;
	case 60: // ���������� ������ �� ������������ ������� �� ������������ ������ ��� �������� ������
		OverflowProg = Load.Point;
		break;
	case 61: // ���������� ������ �� ������������ ������� �� ������������ ������ ��
		MkOverflowProg = Load.Point;
		break;
	//
	// ������� ����������
	case 65: // ������ ������� ��������� ��� ������������� ��
		break;
	case 70: // ������ �� � ��������� ��������� ��� ������������� ��
		break;
	case 85: // ������ �����, �� �������� ������ ��������� �� ��� �������������
		break;
	case 90: // ������ �� � �������, �� �������� ������ ��������� �� ��� �������������
		break;
	case 105: // ��������� ��������� �� � ����� � ������� (����� ������ � �������� �� ��� �������)
		break;
	case 110: // ���������� ������ �� ���������, �������������� ������� ������������� ������ ������
		break;
	case 115: // ���������� ������ �� ��������� ��������� ������� ����� �� ��� ������������� �� ������ � ������� �������������
		break;
	case 120: // ReceiveIndOut
		Load.Write(SendInd);
		break;
	case 125: // ReceiveIndOutMk ������ �� � �������� ������, �� ������� ������ ����� ��� �������������
		MkExec(Load, { Cint, &SendInd });
		break;
	//
	// ���� ����������
	case 320: // �������� ���� ���������� ���������� ������ ����� ������
		  // ����� ���� ���������!!!
		break;
	case 325: // ������ ����� ���������� ������ ����� ������
		break;
	case 330: // ������ �� � ������� ���������� ������ ����� ������
		break;
	case 335: // ������ ����� ���������� ������ ����� ������� ����� (�� ������� �������� ������)
		break;
	case 340: // ������ �� � ������� ���������� ������ ����� �����
		break;
	case 345: // ������ ���������� ���������� �� ����� ������
		break;
	case 350: // ������ �� � ����������� ���������� �� ����� ������
		break;
	case 355: // ������ ���������� ���������� �� ����� �����
		Load.Write(MKCount);
		break;
	case 360: // ������ �� � ����������� ���������� �� ����� �����
		break;
	case 365: // ������ ���� ����� ������� �� �� �������
		Load.Write(MaxMKQueue);
		break;
	case 370: // ������ �� � ���� ������ ������� �� �� �������
		break;
	case 175: // ������ ������� ����� ������� �� �� �������
		Load.Write(AverageMKQueue);
		break;
	case 380: // ������ �� �� ������� ������ ������� �� �� �������
		MkExec(Load, { Cint, &AverageMKQueue });
		break;
	case 395: // ���������� ������ �� ���������, ���������������� ��� ������� �� ��� ������������� (��� ���������� ��������������� ����� ����������)
		ReceiveProg = Load.Point;
		break;
	//
	default:
		CommonMk(MK, Load);
		break;
	}
}
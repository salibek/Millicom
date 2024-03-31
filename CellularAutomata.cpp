#include "CellularAutomata.h"
#include "SchedulerEventser.h"

// ���������� ��������� ������� (���������� ��� ���������� �������� �������)
void CellularAutomat::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	MK %= FUMkRange;
	// ����� �������������
	if (Modeling != nullptr && Modeling->ManualMode && Modeling->scheduler != nullptr && !Modeling->SchedulerFlag)
	{
		Modeling->SchedulerFlag = false;
		Modeling->qmk.push_back({ MK, Load });
//		if (Load.Type % 2 == 1) Load.Point = Load.VarClone(); // ����������� ���������
		if(MK<100 || MK>565) // �� ������� ���������
			((Scheduler*)(Modeling->scheduler))->Scheduling(this, OtherMkTime);
		else
		{
			switch((MK-100)/50)
				{
			case 0: // In_0_Set
				((Scheduler*)(Modeling->scheduler))->Scheduling(this, ReceiveTime);
				break;
			case 1: // In_0_Out
				((Scheduler*)(Modeling->scheduler))->Scheduling(this, SendTime);
				break;
			case 2: // In_0_OutMk
				((Scheduler*)(Modeling->scheduler))->Scheduling(this, SendTime);
				break;
			case 3: // NeighbourMk_0_Set
				((Scheduler*)(Modeling->scheduler))->Scheduling(this, OtherMkTime);
				break;
			case 4: // Neighbour_0_Set
				((Scheduler*)(Modeling->scheduler))->Scheduling(this, OtherMkTime);
				break;
			case 5: // Parametr_0_Set
				((Scheduler*)(Modeling->scheduler))->Scheduling(this, OtherMkTime);
				break;
			case 6: // Parametr_0_Out
				((Scheduler*)(Modeling->scheduler))->Scheduling(this, OtherMkTime);
				break;
			case 7: // Parametr_0_OutMk
				((Scheduler*)(Modeling->scheduler))->Scheduling(this, SendTime);
				break;
			case 8: // MkAdd_0_Send
				((Scheduler*)(Modeling->scheduler))->Scheduling(this, SendTime);
				break;
			case 9: // In_0_Set
				((Scheduler*)(Modeling->scheduler))->Scheduling(this, ReceiveTime);
				break;
			default:
				CommonMk(MK, Load);
				break;

			}
		}
		return;
	}
	bool SchedulerFlag = false;
	if (Modeling != nullptr && Modeling->ManualMode && Modeling->SchedulerFlag)
	{
		Modeling->SchedulerFlag = false;
		SchedulerFlag = true;
	}
	// -------------------
	
	// �������� ����� �� � ��������� ???
	if (ReceiveProg != nullptr && Mode == 0) // ������ ��������� ����������� ������, �.�. ������ ������� �������������� ������������ ���������� ��
	{
		int t = Mode; 
		Mode = 0; // ������� ����� ������ �� �������
		ProgExec(ReceiveProg);
		if(!Mode)
			Mode = t; // ������������ ������� ����� ������ ��
	}
	if (MK == ContextOutMk || MK == ContextOutMkMk)
		N_In++; // �������������� ������� ����������� ��������� ������ (������� ��� ������� ������� ���� ��������, ������� ��� � ������ ������� ������
	if (MK == ProgExecMk)
		Received = { MK, Load, Sender };
	switch (MK)
	{
	case 0: // Reset
		PlyInd = 0;
		if (AccumCreating)
		{
			delete Alu;
			Alu = nullptr;
			AccumCreating = false;
		}
		Neighbours.resize(0);
		NeighboursMk.resize(0);
		RezReady.resize(1);
		InCounter.resize(1);
		Rez.resize(0);
		Plys.resize(1);
		Plys[0].resize(0);
		PlyCurrent = 0; ParameterInd = -1;
		FiringProg = nullptr; ReceiveProg = nullptr; IndFuOffcet = 0; FUInd = 0;
		break;
	case 20: // NeighbourAdd �������� ������ �� �������������� ����������
		Neighbours.push_back((FU*)Load.Point);
		break;
	case 21: // NeighbourMkAdd �������� �� ��� ������ (������� ��������� ������, ����� ��; ���� ����������� ������ ��, �� � �������� ������ ������������� ����������� Bus)
		if (Neighbours.size() == NeighboursMk.size())
			Neighbours.push_back(Bus);
		NeighboursMk.push_back(Load.toInt());
		break;
	case 22: // NeighbourClear �������� ������ ������ �� ������� � �� ��� ���
		Neighbours.clear();
		NeighboursMk.clear();
		break;
	case 30: // StateSet ���������� ������ ��������� ��������
		State = Load.toInt();
		break;
	case 31: // StateOut ������ ������ ��������� ��������
		Load.Write(State);
		break;
	case 32: // StateOutMk ������ �� � �������� ��������� ��������
		MkExec(Load, { Cint,&State });
		break;
	case 35: // IndSet ���������� ������
		Ind = Load.toInt();
		break;
	case 36: // IndAdd ��������� � �������
		Ind += Load.toInt();
		break;
	case 37: // IndOut ������ ������
		Load.Write(Ind);
		break;
	case 38: // IndOutMk ������ �� � ��������
		MkExec(Load, {Cint, &Ind});
		break;

	case 50: // SendTimeSet ���������� ����� �������� ��������� ��� ������������ �������������
		SendTime = Load.toDouble();
		break;
	case 51: // ReceiveTimeSet ���������� ����� ������ ��������� ��� ������������ �������������
		SendTime = Load.toDouble();
		break;
	case 52: // OtherMkTimeSet ���������� ����� ����� ���������� ������ �������� ��� ������������ �������������
		SendTime = Load.toDouble();
		break;
	case 53: // TransferTimeSet ���������� ����� �������� ������ ����� ����������
		TransferTime = Load.toDouble();
		break;
	case 700: // IndSet ���������� ������
		Ind = Load.toInt();
		break;
	case 701: //IndAdd ��������� ������ �� �������� �� �������� �������� ��
		Ind += Load.toInt();
		break;
	case 799: // N_InAdd �������� ����� ������� ���������� (�� ��������� +1)
		N_In += Load.toInt(1);
		Plys[PlyCurrent].resize(N_In);
		Plys[PlyCurrent][Plys[PlyCurrent].size() - 1] = { 0, nullptr };
		ReceiveProgs.resize(N_In);
		ReceiveProgs[N_In-1]=nullptr;
		//InCounter.resize(N_In);
		//InCounter[N_In - 1] = 0;
		break;
	case 800: // N_InSet ���������� ���������� ������� ������ ��� ������������
		N_In = Load.toInt();
		Plys[PlyCurrent].resize(N_In);
		Plys[PlyCurrent][N_In - 1] = { 0, nullptr };
		ReceiveProgs.resize(N_In);
		for (auto& i : ReceiveProgs)
			i = nullptr;
		//InCounter.resize(N_In);
//		for (auto& i : InCounter)
//			i = 0;
//		cout << FUInd << endl;
		break;
	case 801: // N_PlySet ���������� ���������� ����� ��� �������
		PlyN = Load.toInt();
		Plys.resize(PlyN);
		RezReady.resize(PlyN);
		InComplectF.resize(PlyN);
		Rez.resize(PlyN);
		InCounter.resize(PlyN);
		if (!Neighbours.size())
			for (auto& i : Plys)
				i.resize(Neighbours.size());
		//InCounter[PlyCurrent] = Load.toInt();
		break;
	case 802: // N_InOut ������ ���������� ��������� ������� ������
		Load.Write(N_In);
		break;
	case 803: // N_InOutMk ������ �� � ����������� ��������� ������� ������
		MkExec(Load, {Cint, &N_In});
		break;

	case 805: //RezSet  ���������� ��������� ���������� (���� PlyInd<0,  ��������������� �������� �������� ����)
		Rez[PlyInd < 0 ? Rez.size() - 1 : PlyInd] = Load.toDouble();
		break;
	case 806: //RezOut ������ ��������� ���������� (���� PlyInd<0, �������� �������� ��������� ������� �������� ����)
		Load.Write(Rez[PlyInd < 0 ? Rez.size() - 1 : PlyInd]);
		break;
	case 807: //RezOutMk ������ �� � ����������� ���������� (���� PlyInd<0, �������� �������� ��������� ������� �������� ����)
	{
//		double t = Rez[PlyInd < 0 ? Rez.size() - 1 : 0];
		double t = Rez[PlyCurrent];
		MkExec(Load, {Cdouble, &t });
		break;
	}
	case 808: // PlyIndSet ���������� ������ ���� ��� �������
		PlyInd = Load.toInt(0);
		break;
	case 810: //�	IndFuOffcetSet �������� ������� �� (������ ����������� �� ��� �������� � � ���� ������������ ������ ����)
		IndFuOffcet = Load.toInt();
		break;
	case 811: // �	ModeSet ����� ������ �������� (0 - ���������, 1 - ������� �����)
		Mode = Load.toInt();
	case 815: //CollectorSet ���������� ��������� ��� ����� �����������
		Collector = Load.Point;
		break;
	case 820: // �FiringProgSet ���������� ������ �� ��������� ���������� ���������� ��� ���������� ������ ������
		FiringProg = Load.Point;
		break;
	case 821: // FiringProgOut ���������� ������ �� ��������� ���������� ���������� ��� ���������� ������ ������
		Load.Write(FiringProg);
		break;
	case 822: // FiringProgOutMk ���������� ������ �� ��������� ���������� ���������� ��� ���������� ������ ������
		MkExec(Load, {CIC, FiringProg });
		break;
	case 824: // ReceiveProgSet ���������� ���������, ����������� ��� ������ ������ �� ������ Ind
		ReceiveProgs[Ind] = Load.Point;
		break;
	case 825: // ReceiveAllProgSet ���������� ���������, ����������� ��� ������ ������
		ReceiveProg = Load.Point;
		break;
	case 826: // ReceiveProgOut ������ ������ �� ��������� ���������� ���������� ��� ���������� ������ ������
		Load.Write(ReceiveProg);
		break;
	case 827: // �ReceiveProgOutMk ������ �� �� ������� �� ��������� ���������� ���������� ��� ���������� ������ ������
		MkExec(Load, { CIC, ReceiveProg });
		break;
	case 830: // �AutoSendSet ���������� ���� �������������� ��������� ����������� ����������
		AutoSend = Load.toBool();
		break;
	case 834: // RezReadySet
		RezReady[PlyCurrent] = Load.toBool();
	case 835: // �	RezReadyOut 
		Load.Write(RezReady[PlyCurrent]);
		break;
	case 836: // �	RezReadyOutMk
	{
		bool t = RezReady[PlyCurrent];
		MkExec(Load, { Cbool, &t });
		break;
	}
	case 840: // VarAdd �������� ��������� ����������
		Vars.push_back(Load.toDouble());
		break;
	case 841: // VarClear �������� ������ ��������� ����������
		Vars.clear();
		break;
	case 842: // VarIndexSet ���������� ������ ��������� (���� -1, �� ���������� ��������� ����������� ��������)
	case 843: // VarOut ������ ��������� ����������
		if (VarInd < 0)
			Load.Write(Vars.back());
		else
			Load.Write(Vars[VarInd]);
		break;
	case 844: // VarOutMk ������ �� � ��������� ����������
		if (VarInd < 0)
			MkExec(Load, { Cdouble,&Vars.back() });
		else
			MkExec(Load, { Cdouble,&Vars[VarInd] });
		break;
	case 848: // VarSet ���������� �������� � ������ ���������� (���� VarInd=-1, �� ������������� ��������� ��������)
		if (VarInd < 0)
			Vars[Vars.size() - 1] = Load.toDouble();
		else
			Vars[VarInd] = Load.toDouble();
		break;
	case 845: // �ParameterIndSet ������ �������� ��������� (��� -1 �������� ��������� ��������)
		ParameterInd = Load.toInt();
		break;
	case 846: // �ParameterClear �������� ������ ����������
		ParameterInd = -1;
		parameters.clear();
		break;
	case 850: // ParameterAdd ���������� �������� ��� �������� (��� -1 ����������� ����� ��������)
		if (ParameterInd < 0)
			parameters.push_back(Load);
		else
			if(ParameterInd< parameters.size())
				parameters[ParameterInd]=Load;
		break;
	case 851: // ParametrOut
		if (ParameterInd < 0)
			Load.WriteFromLoad(parameters.back());
		else
			if(ParameterInd< parameters.size())
				parameters.back().WriteFromLoad(Load);
		break;
	case 852: // ParametrOutMk
		if (ParameterInd < 0)
			MkExec(Load, parameters.back());
		else
			if (ParameterInd < parameters.size())
				MkExec(Load, parameters[ParameterInd]);
		break;
	case 853: // ParameterAdd �������� ����� ��������
		parameters.push_back(Load.Clone());
		break;
	case 854: // ParametersClear �������� ������ ����������
		parameters.clear();
		break;
	case 855: // �InComplectFSet
		Load.Write(InComplectF[PlyCurrent]);
		break;
	case 856: // �InComplectFOut
		Load.Write(InComplectF[PlyCurrent]);
		break;
	case 857: // �InComplectFOutMk
		MkExec(Load, { Cbool,&InComplectF });
		break;
	case 860: //ParametersCountOut
		Load.Write(parameters.size());
		break;
	case 861: //ParametersCountOutMk
	{
		int t = parameters.size();
		MkExec(Load, { Cint,&t });
		break;
	}
	case 863: // FUIndSet ���������� ������ ��-��������
		FUInd = Load.toInt();
		break;
	case 865: // �	ManagerSet ���������� ������ �� ��������� ��������
		Manager = Load.Point;
		break;
	case 870: // ���������/������� �����
		Mode = Load.toInt();
		break;
	case 875: // ReceivedOut ������ ���������� ������
		Load.Write(Received.Load);
		break;
	case 876: // ReceivedOutMk ������ �� � ����������� �������
		MkExec(Load,Received.Load, Received.Sender);
		break;
	case 877: // FiringProgExec ��������� ���������, ���� � ������� ������ �������� �������� ������ 
		if (N_In == InCounter[PlyCurrent])
			ProgExec(Load);
		break;
	case 878: // PlyNext ������� � ���������� ���� (� �������� ���������� �������� ���������� �� ����� ����)
		PlyCurrent++;
		PlyCurrent %= PlyN;
		InCounter[PlyCurrent] = 0;
		RezReady[PlyCurrent] = false;
		InComplectF[PlyCurrent] = false;
		Rez[PlyCurrent] = Load.toDouble();
		break;
	case 879: // PriorityMkSet ���������� ������������ �� ��� �������� ������� (��� -1 ���������, ��� ������������ �� ���
		PriorityMk = Load.toInt();
		break;
	case 880: // SendToAll ������ ��������� ���������� �������
		if (OutBlock) break;
		for (int i = 0; i < Neighbours.size(); i++)
			if(Neighbours[i]!=nullptr)
				Neighbours[i]->ProgFU(NeighboursMk[i], {Cdouble,&Rez[PlyCurrent]},this);
		break;
	case 882: // InCounterSet ���������� ������� ������� ������ (���� PlyInd<0, �� ��������������� ��� �������� ������)
		if(PlyInd<0)
			InCounter[PlyCurrent] = Load.toInt();
		else
			InCounter[PlyInd] = Load.toInt();
		break;
	case 883: // InCounterOut ������ ������� �������� ������� ������
		Load.Write(InCounter);
		break;
	case 884: // InCounterOutMk ������ �� �� ��������� �������� ������� ������
		MkExec(Load, { Cint,&InCounter });
		break;
	case 885: //PlyNSet ���������� ���������� � ����� ��� �������
		PlyN = Load.toInt();
		break;
	case 887: //  PlyCurrentOut ������ ������ �������� ���� �������
		Load.Write(PlyCurrent);
		break;
	case 888: //  PlyCurrentOutMk ������ �� � �������� �������� ���� �������
		MkExec(Load, { Cint, &PlyCurrent });
		break;
	case 895: // SendToManager ��������� �� ��� ��������� �� ���������
		if(Load.Type>>1==DIC)
			for(auto i: *(IC_type)Load.Point)
				MkExec(i.atr,i.Load,(FU*)Manager);
		else if(Load.Type >> 1 == DIP)
			MkExec(((ip*)Load.Point)->atr, ((ip*)Load.Point)->Load, (FU*)Manager);
		break;
	case 896: // SendToCollector ��������� �� ��� ��������� �� ����������
		if (Load.isIC())
			for (auto i : *(IC_type)Load.Point)
				MkExec(i.atr, i.Load, (FU*)Collector);
		else if (Load.Type >> 1 == DIP)
			MkExec(((ip*)Load.Point)->atr, ((ip*)Load.Point)->Load, (FU*)Collector);
		break;

	case 897: //Synk ������������� ���������� (�� ����� ������� �� ������ ��������� ���������� �������, ���� �� ���������� AutoSend)
		if (RezReady[PlyCurrent])
		{
			for (int i=0;i< Neighbours.size(); i++)
				MkExec(NeighboursMk[i]+IndFuOffcet, {Cdouble,&Rez[PlyCurrent]}, Neighbours[i]);
			PlyCurrent = (PlyCurrent + 1) % Neighbours.size();
		}
		break;
	case 898: // NeighboursPrint ������� ������� ��
		cout << FUInd << " :";
		for (int i = 0; i < Neighbours.size(); i++)
			if (Neighbours[i] == nullptr)
				cout << " nil |";
			else
				cout << " " << Neighbours[i]->FUInd << " Mk: " << NeighboursMk[i] << " | ";
		cout << endl;
		break;
	case 899: // OutBlockSet ���������� ���������� ������ ������ ��� ��
		OutBlock = Load.toBool();
		break;

	case 3: // InOut ������ ���������� �� ������ �������� �� �������
		Load.Write(Plys[PlyInd][Ind]);
		break;
	case 4: // InOutMk ������ ������ �� �� ��������� �� ������ �� �������
		MkExec(Load, Plys[PlyInd][Ind]);
		break;
	case 5: // NeighbourMkSet ���������� �� ��� ������ �� �������
		NeighboursMk[Ind] = Load.toInt(); // ���������� �� ��� ������
		break;
	case 6: // NeighbourSet ���������� ������ �� ������ �� �������
		Neighbours[Ind] = (FU*)Load.Point;
		break;
	case 7: // ParametrSet ���������� �������� �� �������
		parameters[Ind].WriteFromLoad(Load);
		break;
	case 8: // ParametrOut ������ �� � ���������� �� �������
		if (OutBlock) break;
		MkExec(Load, parameters[Ind]);
		break;
	case 9: // ParametrOutMk ���������� ������ �� ������ �� �������
		if (OutBlock) break;
		Neighbours[Ind] = (FU*)Load.Point;
		break;
	case 10: // SendTo ������ �� �� ��������� ��� ������ �� �������
		if (OutBlock) break;
		if (Ind > Plys[PlyInd].size()) break;
		if(PriorityMk>=0)
			Neighbours[Ind]->MkAwait(PriorityMk, Load, this, TransferTime);
		else
		    Neighbours[Ind]->MkAwait(NeighboursMk[Ind], Load, this, TransferTime);
		//Neighbours[PlyInd][Ind].ProgFU(NeighboursMk[Ind], Load, this);
		break;
	case 11: // MkAdd ��������� �������� �� ��� ������
		if (Ind > Plys[PlyInd].size()) break;
		NeighboursMk[Ind] += Load.toInt();
		break;
	case 12: // In_In ������� �������� �� ������ �� �������
	{
		if (Plys[PlyCurrent][Ind].Point == nullptr)
			InCounter[PlyCurrent]++;
		Plys[PlyCurrent][Ind] = Load.Clone();
		InCounter[PlyCurrent]++;
		if (InCounter[PlyCurrent] == Neighbours.size()) // �������� ������ ��������
		{
			bool ModelingFlag = false;
			InComplectF[PlyCurrent] = true;

			if(Modeling!=nullptr){
				ModelingFlag = Modeling->ManualMode; // �������� ����� �������������
				Modeling->ManualMode = false;}
			ProgExec(FiringProg);
			if (Modeling != nullptr)
				Modeling->ManualMode= ModelingFlag; // ������������ ����� �������������

			RezReady[PlyCurrent] = true;
			if (AutoSend) ProgFU(880, { 0, nullptr });
			if (FiringProg == nullptr && !AutoSend) {
				PlyCurrent++;
				PlyCurrent %= PlyN;
				InCounter[PlyCurrent] = 0;
				RezReady[PlyCurrent] = false;
				InComplectF[PlyCurrent] = false;
			}
//			for (int i = 0; i < Neighbours.size(); i++)
//				InComplectF[PlyCurrent][i] = false;
		}
		if (Ind > 0 && Ind < ReceiveProgs.size())
			ProgExec(ReceiveProgs[MK % 50]);
		ProgExec(ReceiveProg); // ������ ��������� �� ������ ������ �� ������
	}
	break;
	case 40: // N_In_Out ������ ��������� ���������� ���������
		Load.Write(N_In);
		break;
	case 41: // N_In_OutMk ������ �� � ��������� ����������� ���������
		MkExec(Load, { Cint, &N_In });
		break;
	case 42: // N_Neighbour_Out ������ ���������� ����������� ����������
		Load.Write(Neighbours.size());
		break;
	case 43: // N_Neighbour_OutMk ������ �� � ����������� ����������� ����������
	{
		int t;
		t = Neighbours.size();
		MkExec(Load, { Cint, &t });
	}
	break;

	case 100: //In_0_Set ������� �������� �� 0-�� ������
	case 150: //In_0_Out ������ ��������, ���������� �� 0-�� ������
	case 200: //In_0_OutMk ������ �� �� ���������, ���������� �� 0-�� ������
	case 250: //NeighbourMk_0_Set ���������� �� ��� 0-�� ������
	case 300: //Neighbour_0_Set ���������� ������ �� 0-�� ������
	case 350: //Parametr_0_Set ���������� 0-� ��������
	case 400: //Parametr_0_Out ������ 0-� ��������
	case 450: //Parametr_0_OutMk ������  �� � 0-� ����������
	case 500: //SendTo_0 ������ ��� ������ 0
	case 550: //MkAdd_0_Out ������ �������� ������������ ������ 0

	case 101:   //In_1_Set ������� �������� �� 1-�� ������
	case 151: //In_1_Out ������ ��������, ���������� �� 1-�� ������
	case 201: //In_1_OutMk ������ �� ��� ���������, ���������� �� 1-�� ������
	case 251: //NeighbourMk_1_Set ���������� �� ��� 1-�� ������
	case 301: //Neighbour_1_Set ���������� ������ �� 1-�� ������
	case 351: //Parametr_1_Set ���������� 1-� ��������
	case 401: //Parametr_1_Out ������ 1-� ��������
	case 451: //Parametr_1_OutMk ������  �� � 1-� ����������
	case 501: //SendTo_1 ������ ��� ������ 1
	case 551: //MkAdd_1_Out ������ �������� ������������ ������ 1

	case 102:   //In_2_Set ������� �������� �� 2-�� ������
	case 152: //In_2_Out ������ ��������, ���������� �� 2-�� ������
	case 202: //In_2_OutMk ������ �� �� ���������, ���������� �� 2-�� ������
	case 252: //NeighbourMk_2_Set ���������� �� ��� 2-�� ������
	case 302: //Neighbour_2_Set ���������� ������ �� 2-�� ������
	case 352: //Parametr_2_Set ���������� 2-� ��������
	case 402: //Parametr_2_Out ������ 2-� ����������
	case 452: //Parametr_2_OutMk ������  �� � 1-� ��������
	case 502: //SendTo_1 ������ ��� ������ 2
	case 552: //MkAdd_2_Out ������ �������� ������������ ������ 2

	case 103:   //In_3_Set ������� �������� �� 3-�� ������
	case 153: //In_3_Out ������ ��������, ���������� �� 3-�� ������
	case 203: //In_3_OutMk ������ �� ��� ���������, ���������� �� 3-�� ������
	case 253: //NeighbourMk_3_Set ���������� �� ��� 3-�� ������
	case 303: //Neighbour_3_Set ���������� ������ �� 3-�� ������
	case 353: //Parametr_3_Set ���������� 3-� ��������
	case 403: //Parametr_3_Out ������ 3-� ��������
	case 453: //Parametr_3_OutMk ������ �� � 3-� ����������
	case 503: //SendTo_3 ������ ��� ������ 3
	case 553: //MkAdd_3_Out ������ �������� ������������ ������ 3

	case 104:   //In4_Set ������� �������� �� 4-�� ������
	case 154: //In4_Out ������ ��������, ���������� �� 4-�� ������
	case 204: //In4_OutMk ������ �� ��� ���������, ���������� �� 4-�� ������
	case 254: //NeighbourMk_4_Set ���������� �� ��� 4-�� ������
	case 304: //Neighbour_4_Set ���������� ������ �� 4-�� ������
	case 354: //Parametr_4_Set ���������� 4-� ��������
	case 404: //Parametr_4_Out ������ 4-� ��������
	case 454: //Parametr_4_OutMk ������  �� � 4-� ����������
	case 504: //ToNeighbour_4_Send ������ ��� ������ 4
	case 554: //MkAdd_4_Out ������ �������� ������������ ������ 4

	case 105:   //In5_Set ������� �������� �� 5-�� ������
	case 155: //In5_Out ������ ��������, ���������� �� 5-�� ������
	case 205: //In5_OutMk ������ �� ��� ���������, ���������� �� 5-�� ������
	case 255: //NeighbourMk_5_Set ���������� �� ��� 5-�� ������
	case 305: //Neighbour_5_Set ���������� ������ �� 5-�� ������
	case 355: //Parametr_5_Set ���������� 5-� ��������
	case 405: //Parametr_5_Out ������ 5-� ��������
	case 455: //Parametr_5_OutMk ������  �� � 5-� ��������
	case 505: //SendTo_5 ������ ��� ������ 5
	case 555: //MkAdd_5_Out ������ �������� ������������ ������ 5

	case 106:   //In6_Set ������� �������� �� 6-�� ������
	case 156: //In6_Out ������ ��������, ���������� �� 6-�� ������
	case 206: //In6_OutMk ������ �� ��� ���������, ���������� �� 6-�� ������
	case 256: //NeighbourMk_6_Set ���������� �� ��� 6-�� ������
	case 306: //Neighbour_6_Set ���������� ������ �� 6-�� ������
	case 356: //Parametr_6_Set ���������� 6-� ��������
	case 406: //Parametr_6_Out ������ 6-� ��������
	case 456: //Parametr_6_OutMk ������  �� � 6-� ��������
	case 506: //SendTo_6 ������ ��� ������ 6
	case 556: //MkAdd_6_Out ������ �������� ������������ ������ 6

	case 107:   //In7_Set ������� �������� �� 7-�� ������
	case 157: //In7_Out ������ ��������, ���������� �� 7-�� ������
	case 207: //In7_OutMk ������ �� ��� ���������, ���������� �� 7-�� ������
	case 257: //NeighbourMk_7_Set ���������� �� ��� 7-�� ������
	case 307: //Neighbour_7_Set ���������� ������ �� 7-�� ������
	case 357: //Parametr_7_Set ���������� 7-� ��������
	case 407: //Parametr_7_Out ������ 7-� ��������
	case 457: //Parametr_7_OutMk ������  �� � 7-� ��������
	case 507: //SendTo_7 ������ ��� ������ 7
	case 557: //MkAdd_7_Out ������ �������� ������������ ������ 7

	case 108:   //In8_Set ������� �������� �� 8-�� ������
	case 158: //In8_Out ������ ��������, ���������� �� 8-�� ������
	case 208: //In8_OutMk ������ �� ��� ���������, ���������� �� 8-�� ������
	case 258: //NeighbourMk_8_Set ���������� �� ��� 8-�� ������
	case 308: //Neighbour_8_Set ���������� ������ �� 8-�� ������
	case 358: //Parametr_8_Set ���������� 8-� ��������
	case 408: //Parametr_8_Out ������ 8-� ��������
	case 458: //Parametr_8_OutMk ������  �� � 8-� ��������
	case 508: //SendTo_8 ������ ��� ������ 8
	case 558: //MkAdd_8_Out ������ �������� ������������ ������ 8

	case 109:   //In9_Set ������� �������� �� 9-�� ������
	case 159: //In9_Out ������ ��������, ���������� �� 9-�� ������
	case 209: //In9_OutMk ������ �� ��� ���������, ���������� �� 9-�� ������
	case 259: //NeighbourMk_9_Set ���������� �� ��� 9-�� ������
	case 309: //Neighbour_9_Set ���������� ������ �� 9-�� ������
	case 359: //Parametr_9_Set ���������� 9-� ��������
	case 409: //Parametr_9_Out ������ 9-� ��������
	case 459: //Parametr_9_OutMk ������  �� � 9-� ��������
	case 509: //SendTo_9 ������ ��� ������ 9
	case 559: //MkAdd_9_Out ������ �������� ������������ ������ 9

	case 110:  //In10_Set ������� �������� �� 10-�� ������
	case 160: //In10_Out ������ ��������, ���������� �� 10-�� ������
	case 210: //In10_OutMk ������ �� ��� ���������, ���������� �� 10-�� ������
	case 260: //NeighbourMk_10_Set ���������� �� ��� 10-�� ������
	case 310: //Neighbour_10_Set ���������� ������ �� 10-�� ������
	case 360: //Parametr_10_Set ���������� 10-� ��������
	case 410: //Parametr_10_Out ������ 10-� ��������
	case 460: //Parametr_10_OutMk ������   �� � 10-� ��������
	case 510: //SendTo_10 ������ ��� ������ 10
	case 560: //MkAdd_10_Out ������ �������� ������������ ������ 10

	case 111:  //In11_Set ������� �������� �� 11-�� ������
	case 161: //In11_Out ������ ��������, ���������� �� 11-�� ������
	case 211: //In11_OutMk ������ �� ��� ���������, ���������� �� 11-�� ������
	case 261: //NeighbourMk_11_Set ���������� �� ��� 11-�� ������
	case 311: //Neighbour_11_Set ���������� ������ �� 11-�� ������
	case 361: //Parametr_11_Set ���������� 11-� ��������
	case 411: //Parametr_11_Out ������ 11-� ��������
	case 461: //Parametr_11_OutMk ������  �� 11-� ��������
	case 511: //SendTo_11 ������ ��� ������ 11
	case 561: //MkAdd_11_Out ������ �������� ������������ ������ 11

	case 112:  //In12_Set ������� �������� �� 12-�� ������
	case 162: //In12_Out ������ ��������, ���������� �� 12-�� ������
	case 212: //In12_OutMk ������ �� ��� ���������, ���������� �� 12-�� ������
	case 262: //Neighbour_12_Set ���������� ������ �� 12-�� ������
	case 312: //NeighbourMk_12_Set ���������� �� ��� 12-�� ������
	case 362: //Parametr_12_Set ���������� 12-� ��������
	case 412: //Parametr_12_Out ������ 12-� ��������
	case 462: //Parametr_12_OutMk ������  �� 12-� ��������
	case 512: //TSendTo_12 ������ ��� ������ 12
	case 562: //MkAdd_12_Out ������ �������� ������������ ������ 12

	case 113:  //In13_Set ������� �������� �� 13-�� ������
	case 163: //In13_Out ������ ��������, ���������� �� 13-�� ������
	case 213: //In13_OutMk ������ �� ��� ���������, ���������� �� 13-�� ������
	case 263: //Neighbour_13_Set ���������� �� ��� 13-�� ������
	case 313: //NeighbourMk_13_Set ���������� ������ �� 13-�� ������
	case 363: //Parametr_13_Set ���������� 13-� ��������
	case 413: //Parametr_13_Out ������ 13-� ��������
	case 463: //Parametr_13_OutMk ������  �� 13-� ��������
	case 513: //SendTo_13 ������ ��� ������ 13
	case 563: //MkAdd_13_Out ������ �������� ������������ ������ 13

	case 114:  //In14_Set ������� �������� �� 14-�� ������
	case 164: //In14_Out ������ ��������, ���������� �� 14-�� ������
	case 214: //In14_OutMk ������ �� ��� ���������, ���������� �� 14-�� ������
	case 264: //NeighbourMk_14_Set ���������� �� ��� 14-�� ������
	case 314: //Neighbour_14_Set ���������� ������ �� 14-�� ������
	case 364: //Parametr_14_Set ���������� 14-� ��������
	case 414: //Parametr_14_Out ������ 14-� ��������
	case 464: //Parametr_14_OutMk ������ �� 14-� ��������
	case 514: //SendTo_14 ������ ��� ������ 14
	case 564: //MkAdd_14_Out ������ �������� ������������ ������ 14

	case 115:  //In15_Set ������� �������� �� 15-�� ������
	case 165: //In15_Out ������ ��������, ���������� �� 15-�� ������
	case 215: //In15_OutMk ������ �� ��� ���������, ���������� �� 15-�� ������
	case 265: //Neighbour_15_Set ���������� �� ��� 15-�� ������
	case 315: //NeighbourMk_15_Set ���������� ������ �� 15-�� ������
	case 365: //Parametr_15_Set ���������� 15-� ��������
	case 415: //Parametr_15_Out ������ 15-� ��������
	case 465: //Parametr_15_OutMk ������ �� 15-� ��������
	case 515: //SendTo_15 ������ ��� ������ 15
	case 565: //MkAdd_15_Out ������ �������� ������������ ������ 15

		MK-=100;
		switch (MK / 50) {
		case 1: // ������ ���������� �� ������ ��������
			if (MK % 50 >= Plys[PlyInd].size()) break;
			Load.Write(Plys[PlyInd][MK % 50]);
			break;
		case 2: // ������ �� �� ��������� �� ������
			if (MK % 50 >= Plys[PlyInd].size()) break;
			MkExec(Load, Plys[PlyInd][MK % 50]);
			break;
		case 3: // ���������� �� ��� ������
			if (MK % 50 >= Neighbours.size()) {
				Neighbours.resize(MK % 50 + 1);
				NeighboursMk.resize(MK % 50 + 1);
			}
			NeighboursMk[MK % 50] = Load.toInt(); // ���������� �� ��� ������
			break;
		case 4: // ���������� ������ �� ������
			if (MK % 50 >= Neighbours.size()) {
				Neighbours.resize(MK % 50 + 1);
				NeighboursMk.resize(MK % 50 + 1);
			}
			Neighbours[MK % 50] = (FU*)Load.Point;
			break;
		case 5: // ���������� ��������
			if (MK % 50 >= parameters.size())
				parameters.resize(MK % 50 + 1);
			parameters[MK % 50].WriteFromLoad(Load);
			break;
		case 6: // ������ ��������
			if (OutBlock) break;
			if (MK % 50 >= parameters.size()) break;
			Load.Write(parameters[MK % 50]);
			break;
		case 7: // ������ �� � ����������
			if (OutBlock) break;
			if (MK % 50 >= parameters.size()) break;
			MkExec(Load, parameters[MK % 50]);
			break;
		case 8: // Send_N ��������� �������� ��� ������ � ��������
			if (OutBlock) break;
			if (MK % 50 >= Neighbours.size()) break;
			if (Neighbours[MK % 50] == nullptr) break;
			//Neighbours[MK % 50]->ProgFU(NeighboursMk[MK % 50], Load, this);
			if (Neighbours[MK % 50] == nullptr) break;
			if (PriorityMk >= 0)
				Neighbours[MK % 50]->MkAwait(PriorityMk, Load, this, TransferTime);
			else
				Neighbours[MK % 50]->MkAwait(NeighboursMk[MK % 50], Load, this, TransferTime);
			//cout << Load.toDouble() << endl;
			break;
		case 9: //MkAdd_N ��������� �������� � ��
			if (MK % 50 >= Neighbours.size()) break;
			NeighboursMk[MK % 50]+=Load.toInt();
			break;
		case 0: // In_N_Set ������ �������� �� ������
		{
			//cout << FUInd << endl;
			if (MK % 50 >= Plys[PlyCurrent].size())
				Plys[PlyCurrent].resize(MK % 50 + 1);
			if (Plys[PlyCurrent][MK % 50].Point != nullptr)
				Plys[PlyCurrent][MK % 50].Clear();
			InCounter[PlyCurrent]++;
			Plys[PlyCurrent][MK % 50] = Load.Clone();

//			cout << Plys[PlyCurrent][MK % 50].toDouble() << endl;
			//InCounter[PlyCurrent]++;
			if (MK % 50 >= 0 && MK % 50 < ReceiveProgs.size())
				ProgExec(ReceiveProgs[MK%50]);
			ProgExec(ReceiveProg); // ������ ��������� �� ������ ������ �� ������
//			if (InCounter[PlyCurrent] == Plys[PlyCurrent].size()) // �������� ������ ��������
			if (InCounter[PlyCurrent] == N_In) // �������� ������ ��������
			{
				bool ModelingFlag = false;
//				InComplectF[PlyCurrent]= true;
				if (Modeling != nullptr) {
					ModelingFlag = Modeling->ManualMode; // �������� ����� �������������
					Modeling->ManualMode = false;
				}
				ProgExec(FiringProg);
				if (Modeling != nullptr)
					Modeling->ManualMode = ModelingFlag; // ������������ ����� �������������

				RezReady[PlyCurrent] = true;
				if (AutoSend) ProgFU(880, { 0, nullptr });
				if (FiringProg == nullptr && !AutoSend) {
					PlyCurrent++;
					PlyCurrent %= PlyN;
					InCounter[PlyCurrent] = 0;
					RezReady[PlyCurrent] = false;
				}
//				for (int i = 0; i < Neighbours.size(); i++)
//					InComplectF[PlyCurrent][i] = false;
//				InComplectF[PlyCurrent] = false;
			}
		}
		}
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
	// ����� �������������
	if (SchedulerFlag)
		((Scheduler*)Modeling->scheduler)->CoreFree();
	// -------------------
}

void CellularAutomatManager::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	MK %= FUMkRange;
	// �������� ����� �� � ���������
	switch (MK)
	{
	case 0: // Reset
		NetType = 0;
		break;
	case 1:// NetTypeSet ���������� ��� �����
		NetType = Load.toInt();
		break;
	case 3: // DimSet �������� ���������
		Dim = Load.toInt();
		break;
	case 10: // IniAutmataProgSet ���������� ��������� ������������� ��������
		iniAutmataProg = Load.Point;
		break;
	case 11: // CollectorSet ��������� ������ �� ���������
		Collector = Load.Point;
		break;
	case 15: // FiringProgSet ���������� ��������� ���������� ����������
//		FiringProg = Load.Point;
		break;
	case 16: // ReceiveProgSet ���������� ���������, ����������� ��� ������� ������ �������� �� ������
//		ReceiveProg = Load.Point;
		break;
	case 17: // InCounterSet ���������� ������� ������� ������
//		InCounter = Load.toInt();
		break;
	case 24: // Ind1Out ������ ������ ������
		Load.Write(Ind1);
		break;
	case 25: // Ind1OutMk ������ �� � ������ ��������
		MkExec(Load, {Cint,&Ind1});
		break;
	case 26: // Ind2Out ������ ������ ������
		Load.Write(Ind2);
		break;
	case 27: // Ind2Out ������ �� �������� ��������
		MkExec(Load, { Cint,&Ind2 });
		break;
	case 28: // Ind1Add ��������� �������� ������� ��������
		Ind1 += Load.toInt(1);
		break;
	case 22: // Ind1Sub ������� �� �������� ������� ��������
		Ind1 -= Load.toInt(1);
		break;
	case 29: //Ind2Add ��������� �������� ������� ��������
		Ind2 += Load.toInt(1);
		break;
	case 23: // Ind1Sub ������� �� �������� ������� ��������
		Ind2 -= Load.toInt(1);
		break;
	case 30: // Ind1Set ���������� ������ ������ ��-��������
		Ind1 = Load.toInt();
		break;
	case 31: // Ind2Set ���������� ������ ������ ��-��������
		Ind2 = Load.toInt();
		break;
	case 32: // Step1Set ���������� ��� �������������� ������������� ��� 1-�� �������
		Step1 = Load.toInt();
		break;
	case 33: // Step2Set ���������� ��� �������������� ������������� ��� 2-�� �������
		Step2 = Load.toInt();
		break;
	case 35: // Context1Out ������ �������� ������� ��-��������
		if (Ind1 >= Net.size() || Ind1 < 0) break;
		Load.Write((FU*)&Net[Ind1]);
		break;
	case 36: // Context2OutMk ������ �� � ���������� ������� ��-��������
		if (Ind1 >= Net.size() || Ind1 < 0) break;
		MkExec(Load, { TFU, &Net[Ind1] });
		break;
	case 37: // Context2Out ������ �������� ������� ��-��������
		if (Ind2 >= Net.size() || Ind2 < 0) break;
		Load.Write((FU*)&Net[Ind2]);
		break;
	case 38: // Context2OutMk ������ �� � ���������� ������� ��-��������
		if (Ind2 >= Net.size() || Ind2 < 0) break;
		MkExec(Load, { TFU, &Net[Ind2] });
		break;
	case 40: // ContextAddMkSet ���������� �� ��� ���������� ��������� ������ ��������������� ��
		ContextAddMk = Load.toInt();
		break;
	case 41: // MkAppendMkSet ���������� �� ��� ���������� �� ��� ������ ��������������� ��
		MkAddMk = Load.toInt();
		break;
	case 50: // Ind1Exec ��������� ��������� ��� ������� ��-��������
		if (Ind1 >= Net.size() || Ind1 < 0) break;
		Net[Ind1].ProgExec(Load);
		Ind1 += Step1;
		break;
	case 51: // Ind2Exec ��������� ��������� ��� ������� ��-��������
		if (Ind2 >= Net.size() || Ind2 < 0) break;
		Net[Ind2].ProgExec(Load);
		Ind2 += Step2;
		break;
	case 52: // AllExec ��������� ��������� ��� ���� �������������� ��
		for (auto& i : Net)
			if (Load.isProg())
				i.ProgExec(Load.Point);
			else
				i.ProgExec(Prog);				
		break;
	case 58: //  Neitborder1To2Append �������� ������ � �������� 2 ��� �� � �������� 1
		if (Ind1 >= Net.size() || Ind1 < 0) break;
		if (Ind2 >= Net.size() || Ind2 < 0)
			Net[Ind1].Neighbours.push_back(Bus);
		else
		{
			Net[Ind1].Neighbours.push_back(&Net[Ind2]);
			Net[Ind2].N_In++;
			Net[Ind2].Plys[Net[Ind2].PlyCurrent].push_back({ 0, nullptr });
			Net[Ind2].ReceiveProgs.push_back(nullptr);
			//Ind1 += Step1;
			Ind2 += Step2;
		}
		break;
	case 59: //  Neitborder2To1Append �������� ������ � �������� 1 ��� �� � �������� 2
		if (Ind2 >= Net.size() || Ind2 < 0) break;
		if (Ind1 >= Net.size() || Ind1 < 0)
			Net[Ind2].Neighbours.push_back(nullptr);
		else
		{
			Net[Ind2].Neighbours.push_back(&Net[Ind1]);
			Net[Ind1].N_In++;
			Net[Ind1].Plys[Net[Ind1].PlyCurrent].push_back({ 0, nullptr });
			Net[Ind1].ReceiveProgs.push_back(nullptr);

			Ind1 += Step1;
			//Ind2 += Step2;
		}
		break;
	case 60: // Neitborder1Append �������� ������ ��� �� � �������� 1 (�� ����� ������ �� ������, ���� nil, �� ������� �������� �� � Ind2)
		if (Ind1 >= Net.size() || Ind1<0) break;
		Net[Ind1].Neighbours.push_back((FU*)Load.Point);
		//Ind1 += Step1;
		break;
	case 61: // Neitborder2Append �������� ������ ��� �� � �������� 2 (�� ����� ������ �� ������, ���� nil, �� ������� �������� �� � Ind1)
		if (Ind2 >= Net.size() || Ind2 < 0) break;
		Net[Ind2].Neighbours.push_back((FU*)Load.Point);
		//Ind2 += Step2;
		break;
	case 62: // Neitborder1MkAppend �������� �� ��� ������ ��� �� � �������� 1

		if (Ind1 >= Net.size() || Ind1 < 0) break;
		Net[Ind1].NeighboursMk.push_back(Load.toInt());
		if (Net[Ind1].Neighbours.size() < Net[Ind1].NeighboursMk.size())
			Net[Ind1].Neighbours.push_back(Bus);
		Ind1 += Step1;
		break;
	case 63: // Neitborder2MkAppend �������� �� ��� ������ ��� �� � �������� 2
		if (Ind2 >= Net.size() || Ind2 < 0) break;
		Net[Ind2].NeighboursMk.push_back(Load.toInt());
		if (Net[Ind2].Neighbours.size() < Net[Ind2].NeighboursMk.size())
			Net[Ind2].Neighbours.push_back(Bus);
		Ind2 += Step2;
		break;
	case 65: // Neitborder1Out ������ ������ �� �������� �� � �������� 1
		Load.Write((FU*)&Net[Ind1]);
		Ind1 += Step1;
		break;
	case 66: // Neitborder2Out  ������ ������ �� �������� �� � �������� 2
		Load.Write((FU*)&Net[Ind2]);
		Ind2 += Step2;
		break; 
	case 67: // Neitborder1OutMk  ������ �� �� ������� �� �������� �� � �������� 1
		MkExec(Load, {TFU,&Net[Ind1] });
		Ind1 += Step1;
		break;
	case 68: // Neitborder2OutMk ������ �� �� ������� �� �������� �� � �������� 2
		MkExec(Load, { TFU,&Net[Ind2] });
		Ind2 += Step2;
		break;
	case 70: // Mk1Set ���������� �� ��� �������� � �������� 1
		AutomataMk1 = Load.toInt();
		break;
	case 71: // Mk2Set ���������� �� ��� �������� � �������� 2
		AutomataMk2 = Load.toInt();
		break;
	case 72: // Mk1Exec  ��������� �� ��� ������� � �������� 1
		Net[Ind1].ProgFU(AutomataMk1, Load);
		break;
	case 73: // Mk2Exec ��������� �� ��� ������� � �������� 2
		Net[Ind2].ProgFU(AutomataMk2, Load);
		break;
	case 74: // Mk1AllExec  ��������� �� ��� ������� � �������� 1 ��� ���� ���������

		for(auto &i: Net)

			i.ProgFU(AutomataMk1, Load);
		break;
	case 75: // Mk1toInd2Exec ��������� ������ �� ��� ���� �� �� ������� 1 �� ������� 2 ��������������
	case 76: // Mk1toInd2IncludeExec ��������� ������ �� ��� ���� �� �� ������� 1 �� ������� 2 ������������
		if (Ind1 >= Ind2) break;
		for(int i=Ind1; i<Ind2;i++)
			Net[i].ProgFU(AutomataMk1, Load);
		if(MK==76)
			Net[Ind2].ProgFU(AutomataMk1, Load);
		break;
	case 80: // Ind1N_inOutMk ������� �� � ����������� ��������� ������� ������
	{
		int t = Net[Ind1].Plys[Net[Ind1].PlyCurrent].size();
		MkExec(Load, { Cint, &t });
		break;
	}
	case 81: // Ind1InCounterOutMk ������ �� � ���������� ��������� ������� ������
		MkExec(Load, { Cint, &Net[Ind2].InCounter[Net[Ind1].PlyCurrent]});
		break;
	case 85: // ForExec ��������� ���� (�� ����� ��������� ��� ��������� ����� ��� ���������� �������� (����� ����������� ��������� �� ��������� Prog �� ���������)
	{
		int N_it = -1; // ���������� �������� 
		int  step1 = Step1, step2 = Step2;
		Step1 = 0; Step2 = 0; // ��������� �����, ����� �� ����������� ������ �� ������ �������

		void* Pr = Prog;
		if (Load.isProg())
			Pr = Load.Point;
		else if (Load.isDigitBool())
			N_it = Load.toInt();

		for (int i = 0; (i < N_it || N_it < 0) && Ind1 >= 0 && Ind1 < Net.size() && Ind2 >= 0 && Ind2 < Net.size(); Ind1 += step1, Ind2 += step2,i++)
			ProgExec(Pr);
		Step1 = step1, Step2 = step2; // ������������ ���� � ��������� ��
	}
	break;

	case 100: // SectorDimClear �������� ��������� ��������������� �������������
		SectorDim.clear();
		break;
	case 101: // SectorDimAdd �������� ��������� ��� ������������� �������������� �������������� �������
		SectorDim.push_back(Load.toInt());
		break;
	case 105: // SectorGenearte ����������� �������������� ����� ��� ������������� �������������� ��
		DistrebuteModelGenerate();
		break;

	case 110: // RouterExec ��������� ��������� ��� �������� �������
		Routers[Ind1].ProgExec(Load);
		break;
	case 111: // RouterAllExec ��������� ��������� ��� ���� ��������
		for (auto& i : Routers)
			i.ProgExec(Load);
		break;
	case 112: // GatewayExec ��������� ��������� ��� �������� �����
		Gateways[Ind1].ProgExec(Load);
		break;
	case 113: // GatewayAllExec ��������� ��������� ��� ���� �����
		for (auto& i : Gateways)
			i.ProgExec(Load);
		break;
	case 115: // RouterMkSet ���������� �� ��� �������
		RouterMk = Load.toInt();
		break;
	case 116: // RouterMkExec ��������� �� ��� �������� �������
		Routers[Ind1].ProgFU(RouterMk, Load, Sender);
		break;
	case 117: // RouterAllMkExec ��������� �� ��� ���� ��������
		for (auto& i : Routers)
			i.ProgFU(RouterMk, Load, Sender);
		break;
	case 118: // GatewayMkSet ���������� �� ��� �����
		GatewayMk = Load.toInt();
		break;
	case 119: // GatewayMkExec ��������� �� ��� �������� �����
		Gateways[Ind1].ProgFU(GatewayMk, Load, Sender);
		break;
	case 120: // GatewayAllMkExec ��������� �� ��� ���� ������
		for (auto& i : Gateways)
			i.ProgFU(GatewayMk, Load, Sender);
		break;
	case 125: // NetEvenserSet ���������� ��������� �� ���������� ������� ��� �������������� ����
		NetEventser = (Eventser*)Load.Point;
		break;


	case 200: //NetGenerate ��������� ����� (�� ���� ����� ���������� ���������� �� � �����)
	{
		if (Load.Point != nullptr)
			Dim = Load.toInt();
		Net.resize(Dim); // ������� ���� ���������
		for (auto& i : Net)
			i.Bus = Bus;
		int k = 0;
		for (auto& i : Net)
		{
			i.Manager = this; // ���������� ������ �� ���������
			i.Collector = Collector; // ��������� ������ �� ���������
			i.ProgFU(0, { 0,nullptr });
			i.ProgFU(FUIndSetMk, { Cint,&k }); // ���������� ������ ��
			i.ProgExec(iniAutmataProg); // ������ ��������� ������������� ������� ��������
			k++;
		}
	}
		break;
	case 201: // NetClear �������� �����
		Net.clear();
		break;
	case 205: // NetDimAppend ������� ����������� ����������� �����
		NetDim.push_back(Load.toInt());
		break;
	case 206: // NetDimClear �������� ����������� ����������� �����
		NetDim.clear();
		break;
	case 207: // NetDimGenerate ������������� ������������� �����
	{
		int P = 1;
		for (auto i : NetDim)
			P *= i;
		if (Net.size()!=0 &&  P != Net.size()) break; // ����������� ����� �� ������������ ���������� ��������� � ���
		if (Net.size() != 0)
			ProgFU(200, {Cint, &P}, this); // ������������ ��-�������� ��������������� ����
		// ����������� ������ ����� ����������
		switch(NetDim.size()){
		case 1:
			for (int i=1; i < NetDim[0]; i++)
				Net[i].Neighbours.push_back(&Net[i - 1]);
			for (int i=0; i < NetDim[0] - 1; i++)
				Net[i].Neighbours.push_back(&Net[i + 1]);
			break;
		case 2:
			break;
		case 3:
			break;
		}
	}
	default:
		CommonMk(MK, Load);
		break;
	}
}


void CellularAutomatManager::DistrebuteModelGenerate() // ��������� ������ �������������� ��
{
	for (auto& i : Net) // ����������� �������� �� ��������� ��� �������������
	{
		i.FUMkGlobalRange = (i.FUInd + 1) * i.FUMkRange; // ��������� ����������� ��������� �� ��� ��
		auto k = i.NeighboursMk.begin();
		if (i.Neighbours.size())
			for (auto j = i.Neighbours.begin(); j != i.Neighbours.end(); j++, k++)
				if (*j != nullptr)
					*k = *k + (FUMkRange + 1) * ((*j)->FUInd);
	}
	// ����������� ����������� �����
	int M = SectorDim[2] / SectorDim[1]; // ���������� �������� � ������
	int N = Net.size() / SectorDim[1]; // ����� ���������� ��������
	int K = SectorDim[0] * (SectorDim[2] / SectorDim[1]); // ���������� ����� � ������

	Routers.resize(N << 2);
	Gateways.resize(N << 2);
	Schedullers.resize(N << 2);

	for (int i = 0; i < N; i++) // ���� �� ��������
	{
		int x = i % M;
		int y = i / M;

		Gateways[i << 2].Receiver = &Routers[i << 2];
		Gateways[(i << 2) + 1].Receiver = &Routers[(i << 2) + 1];
		Gateways[(i << 2) + 2].Receiver = &Routers[(i << 2) + 2];
		Gateways[(i << 2) + 3].Receiver = &Routers[(i << 2) + 3];

		if (x > 0) Gateways[i << 2].GatewayFriend = &Gateways[((x - 1 + y * M) << 2) + 1];
		if (x != M - 1) Gateways[(i << 2) + 1].GatewayFriend = &Gateways[(x + 1 + y * M) << 2];
		if (y > 0) Gateways[(i << 2) + 2].GatewayFriend = &Gateways[((x + (y - 1) * M) << 2) + 3];
		if (y != N / M - 1) Gateways[(i << 2) + 3].GatewayFriend = &Gateways[((x + (y + 1) * M) << 2) + 2];

		// ������������� ������ ����� ��-����������
		for (int j = 0; j < 4; j++) // ��������� ������������ ��������� �������
			for (int k = 0; k < 4; k++)
				Routers[(i << 2) + j].SectorReceivers.push_back(&Gateways[(i << 2) + k]);

		// ����������� ������ � ������� �� ��-�������� ������ �������
		for (int k = 0; i < SectorDim[1] / SectorDim[0]; i++)
			for (int j = 0; j < SectorDim[0]; j++)
			{
				Routers[i << 2].Channels.push_back({});
				Routers[(i << 2) + 1].Channels.push_back({});
				Routers[(i << 2) + 2].Channels.push_back({});
				Routers[(i << 2) + 3].Channels.push_back({});

				Routers[i << 2].Channels.back().Up = (i + j + K * M) + 1 * FUMkRange; // + 1 ��� ����, ����� �������� �� 0-�� �������� �� ��������� � 0
				Routers[(i << 2) + 1].Channels.back().Up = (i + j + K * M + 1) * FUMkRange;
				Routers[(i << 2) + 2].Channels.back().Up = (i + j + K * M + 1) * FUMkRange;
				Routers[(i << 2) + 3].Channels.back().Up = (i + j + K * M + 1) * FUMkRange;

				Routers[i << 2].Channels.back().Down = (i + j + K * M + 1 + 1) * FUMkRange;
				Routers[(i << 2) + 1].Channels.back().Down = (i + j + K * M + 1 + 1) * FUMkRange;
				Routers[(i << 2) + 2].Channels.back().Down = (i + j + K * M + 1 + 1) * FUMkRange;
				Routers[(i << 2) + 3].Channels.back().Down = (i + j + K * M + 1 + 1) * FUMkRange;

				Routers[i << 2].Channels.back().Receiver = &Net[i + j + K * M];
				Routers[(i << 2) + 1].Channels.back().Receiver = &Net[i + j + K * M];
				Routers[(i << 2) + 2].Channels.back().Receiver = &Net[i + j + K * M];
				Routers[(i << 2) + 3].Channels.back().Receiver = &Net[i + j + K * M];

				if (NetEventser != nullptr) // ��������� �������������
				{
					Schedullers[i].eventser = NetEventser;
					if (Net[i + j + K * M].Modeling == nullptr)
						Net[i + j + K * M].Modeling = new FUModeling;
					Net[i + j + K * M].Modeling->scheduler = &Schedullers[i];
					Net[i + j + K * M].Modeling->eventser = (FU*)&NetEventser;
				}
			}
	}


	// ����������� ������ ����� �� ��� ������������ ������ �������������� ��
	int cc = 0;
	for (auto& i : Net)
	{
		//		cout << cc << endl; cc++;
		//		if (cc == 40)
		//			cout << "!!!!!\n";
		vector<int> El; // ������������ ��� �������� �� �����
		El.push_back(((i.FUInd) % ((SectorDim[2] / SectorDim[1]) * SectorDim[0]) / SectorDim[0])); // ���������� �� x
		El.push_back((i.FUInd / i.FUMkRange) / SectorDim[1]); // ���������� �� y

		auto Nieb = i.Neighbours.begin();
		for (auto Mk = i.NeighboursMk.begin(); Mk != i.NeighboursMk.end(); Mk++, Nieb++) //������ �� ������� ��������
		{
			int Index = (*Mk - FUMkRange) / FUMkRange;
			vector<int> D; // ������������ ��� �������� �� �����
			D.push_back((Index % ((SectorDim[2] / SectorDim[1]) * SectorDim[0]) / SectorDim[0]) - El[0]); // ���������� �� x
			D.push_back((Index / SectorDim[2]) - El[1]); // ���������� �� y
			if (D[0] == 0 && D[1] == 0) continue; // ������ �� ��� �� ������
			int c = 0;
			if (D[0])
			{
				D[0] = D[0] / abs(D[0]);
				c++;
			}
			if (D[1])
			{
				D[1] = D[1] / abs(D[1]);
				c++;
			}
			if (c == 2) // ���� ��� ��������� �����������, �������� ������ ���� ��������� �������
				D[rand() % 2] = 0;
			if (D[0] < 0)
				*Nieb = (FU*)&Gateways[(El[1] * M + El[0] - 1) << 2];
			else if (D[0] > 0)
				*Nieb = (FU*)&Gateways[(El[1] * M + El[0] + 1) << 2 + 1];

			if (D[1] < 0)
				*Nieb = (FU*)&Gateways[((El[1] * M - M + El[0] + 2) << 2) + 2];
			else if (D[1] > 0)
				*Nieb = (FU*)&Gateways[((El[1] * M + M + El[0]) << 2) + 3];
		}

	}
	//	cout << "End Sectoring\n";
}
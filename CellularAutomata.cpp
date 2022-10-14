#include "CellularAutomat.h"

// ���������� ��������� ������� (���������� ��� ���������� �������� �������)
void CellularAutomat::ProgFU(int MK, LoadPoint Load)
{
	// �������� ����� �� � ���������
	if (ReceiveProg != nullptr && Mode == 0) // ������ ��������� ����������� ������, �.�. ������ ������� �������������� ������������ ���������� ��
	{
		int t = Mode; 
		Mode = 0; // ������� ����� ������ �� �������
		ProgExec(ReceiveProg);
		if(!Mode)
			Mode = t; // ������������ ������� ����� ������ ��
	}
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
		RezReady.resize(0);
		InCounter.resize(0);
		Rez.resize(0);
		Plys.resize(1);
		InComplectF.resize(0);
		PlyCurrent = 0; ParameterInd = -1;
		FiringProg = nullptr; ReceiveProg = nullptr; IndFuOffcet = 0; FUInd = 0;
		break;
	case 700: // IndSet ���������� ������
		Ind = Load.toInt();
		break;
	case 701: //IndAdd ��������� ������ �� �������� �� �������� �������� ��
		Ind += Load.toInt();
		break;
	case 800: // N_NeighbourSet ���������� ���������� �������
		Neighbours.resize(Load.toInt());
		NeighboursMk.resize(Load.toInt());
		if (!Plys.size())
		{
			for (auto& i : Plys)
				i.resize(Load.toInt());
		}
		break;
	case 801: // N_PlySet ���������� ���������� ����� ��� �������
		Plys.resize(Load.toInt());
		Rez.resize(Load.toInt());
		if (!Neighbours.size())
			for (auto& i : Plys)
				i.resize(Neighbours.size());
		InCounter[PlyCurrent] = Load.toInt();
		break;
	case 805: //RezSet  ���������� ��������� ���������� (���� PlyInd<0,  ��������������� �������� �������� ����)
		Rez[PlyInd < 0 ? Rez.size() - 1 : PlyInd] = Load.toDouble();
		break;
	case 806: //RezOut ������ ��������� ���������� (���� PlyInd<0, �������� �������� ��������� ������� �������� ����)
		Load.Write(Rez[PlyInd < 0 ? Rez.size() - 1 : PlyInd]);
		break;
	case 807: //RezOutMk ������ �� � ����������� ���������� (���� PlyInd<0, �������� �������� ��������� ������� �������� ����)
	{
		double t = Rez[PlyInd < 0 ? Rez.size() - 1 : 0];
		MkExec(Load, {Cdouble, &t });
		break;
	}
	case 808: // PlyIndSet ���������� ������ ���� ��� �������
		PlyInd = Load.toInt(-1);
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
	case 825: // ReceiveProgSet ���������� ���������, ����������� ��� ������ ������
		ReceiveProg = Load.Point;
		break;
	case 826: // ReceiveProgOut ���������� ������ �� ��������� ���������� ���������� ��� ���������� ������ ������
		Load.Write(ReceiveProg);
		break;
	case 827: // �ReceiveProgOutMk ���������� ������ �� ��������� ���������� ���������� ��� ���������� ������ ������
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
	case 850: // ParameterSet ���������� �������� (��� -1 ����������� ����� ��������)
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
	case 880: // RezSendToAll ������ ��������� ���������� �������
		for (int i = 0; i < Neighbours.size(); i++)
			Neighbours[i]->ProgFU(NeighboursMk[i], {Cdouble,&Rez[PlyCurrent]});
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
	case 885: //PlyNSet ���������� ���������� �
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
		Neighbours[Ind] = (CellularAutomat*)Load.Point;
		break;
	case 7: // ParametrSet ���������� �������� �� �������
		parameters[Ind].WriteFromLoad(Load);
		break;
	case 8: // ParametrOut ������ �� � ���������� �� �������
		MkExec(Load, parameters[Ind]);
		break;
	case 9: // ParametrOutMk ���������� ������ �� ������ �� �������
		Neighbours[Ind] = (FU*)Load.Point;
		break;
	case 10: // ToNeighbourOutMk ������ �������� ��� ������ � �������� �� �������
		Neighbours[PlyInd][Ind].ProgFU(NeighboursMk[MK % 50], Load);
		break;

	
	case 100: //In_0_Set ������� �������� �� 0-�� ������
	case 150: //In_0_Out ������ ��������, ���������� �� 0-�� ������
	case 200: //In_1_OutMk ������ �� ��� ���������, ���������� �� 0-�� ������
	case 250: //NeighbourMk_0_Set ���������� �� ��� 0-�� ������
	case 300: //Neighbour_0_Set ���������� ������ �� 0-�� ������
	case 350: //Parametr_0_Set ���������� 0-� ��������
	case 400: //Parametr_0_Out ������ 0-� ��������
	case 450: //Parametr_0_OutMk ������  �� � 0-� ����������
	case 500: //ToNeighbour_1_OutMk ������ �� ��� ������

	case 101:   //In_1_Set ������� �������� �� 1-�� ������
	case 151: //In_1_Out ������ ��������, ���������� �� 1-�� ������
	case 201: //In_1_OutMk ������ �� ��� ���������, ���������� �� 1-�� ������
	case 251: //NeighbourMk_1_Set ���������� �� ��� 1-�� ������
	case 301: //Neighbour_1_Set ���������� ������ �� 1-�� ������
	case 351: //Parametr_1_Set ���������� 1-� ��������
	case 401: //Parametr_1_Out ������ 1-� ��������
	case 451: //Parametr_1_OutMk ������  �� � 1-� ����������
	case 501: //ToNeighbour_1_OutMk ������ �� ��� ������

	case 102:   //In_2_Set ������� �������� �� 2-�� ������
	case 152: //In_2_Out ������ ��������, ���������� �� 2-�� ������
	case 202: //In_2_OutMk ������ �� ��� ���������, ���������� �� 2-�� ������
	case 252: //NeighbourMk_2_Set ���������� �� ��� 2-�� ������
	case 302: //Neighbour_2_Set ���������� ������ �� 2-�� ������
	case 352: //Parametr_2_Set ���������� 2-� ��������
	case 402: //Parametr_2_Out ������ 2-� ����������
	case 452: //Parametr_2_OutMk ������  �� � 1-� ��������
	case 502: //ToNeighbour_1_OutMk ������ �� ��� ������

	case 103:   //In_3_Set ������� �������� �� 3-�� ������
	case 153: //In_3_Out ������ ��������, ���������� �� 3-�� ������
	case 203: //In_3_OutMk ������ �� ��� ���������, ���������� �� 3-�� ������
	case 253: //NeighbourMk_3_Set ���������� �� ��� 3-�� ������
	case 303: //Neighbour_3_Set ���������� ������ �� 3-�� ������
	case 353: //Parametr_3_Set ���������� 3-� ��������
	case 403: //Parametr_3_Out ������ 3-� ��������
	case 453: //Parametr_3_OutMk ������ �� � 3-� ����������
	case 503: //ToNeighbour_3_OutMk ������ �� ��� ������

	case 104:   //In4_Set ������� �������� �� 4-�� ������
	case 154: //In4_Out ������ ��������, ���������� �� 4-�� ������
	case 204: //In4_OutMk ������ �� ��� ���������, ���������� �� 4-�� ������
	case 254: //NeighbourMk_4_Set ���������� �� ��� 4-�� ������
	case 304: //Neighbour_4_Set ���������� ������ �� 4-�� ������
	case 354: //Parametr_4_Set ���������� 4-� ��������
	case 404: //Parametr_4_Out ������ 4-� ��������
	case 454: //Parametr_4_OutMk ������  �� � 4-� ����������
	case 504: //ToNeighbour_4_OutMk ������ �� ��� ������

	case 105:   //In5_Set ������� �������� �� 5-�� ������
	case 155: //In5_Out ������ ��������, ���������� �� 5-�� ������
	case 205: //In5_OutMk ������ �� ��� ���������, ���������� �� 5-�� ������
	case 255: //NeighbourMk_5_Set ���������� �� ��� 5-�� ������
	case 305: //Neighbour_5_Set ���������� ������ �� 5-�� ������
	case 355: //Parametr_5_Set ���������� 5-� ��������
	case 405: //Parametr_5_Out ������ 5-� ��������
	case 455: //Parametr_5_OutMk ������  �� � 5-� ��������
	case 505: //ToNeighbour_5_OutMk ������ �� ��� ������

	case 106:   //In6_Set ������� �������� �� 6-�� ������
	case 156: //In6_Out ������ ��������, ���������� �� 6-�� ������
	case 206: //In6_OutMk ������ �� ��� ���������, ���������� �� 6-�� ������
	case 256: //NeighbourMk_6_Set ���������� �� ��� 6-�� ������
	case 306: //Neighbour_6_Set ���������� ������ �� 6-�� ������
	case 356: //Parametr_6_Set ���������� 6-� ��������
	case 406: //Parametr_6_Out ������ 6-� ��������
	case 456: //Parametr_6_OutMk ������  �� � 6-� ��������
	case 506: //ToNeighbour_6_OutMk ������ �� ��� ������

	case 107:   //In7_Set ������� �������� �� 7-�� ������
	case 157: //In7_Out ������ ��������, ���������� �� 7-�� ������
	case 207: //In7_OutMk ������ �� ��� ���������, ���������� �� 7-�� ������
	case 257: //NeighbourMk_7_Set ���������� �� ��� 7-�� ������
	case 307: //Neighbour_7_Set ���������� ������ �� 7-�� ������
	case 357: //Parametr_7_Set ���������� 7-� ��������
	case 407: //Parametr_7_Out ������ 7-� ��������
	case 457: //Parametr_7_OutMk ������  �� � 7-� ��������
	case 507: //ToNeighbour_7_OutMk ������ �� ��� ������

	case 108:   //In8_Set ������� �������� �� 8-�� ������
	case 158: //In8_Out ������ ��������, ���������� �� 8-�� ������
	case 208: //In8_OutMk ������ �� ��� ���������, ���������� �� 8-�� ������
	case 258: //NeighbourMk_8_Set ���������� �� ��� 8-�� ������
	case 308: //Neighbour_8_Set ���������� ������ �� 8-�� ������
	case 358: //Parametr_8_Set ���������� 8-� ��������
	case 408: //Parametr_8_Out ������ 8-� ��������
	case 458: //Parametr_8_OutMk ������  �� � 8-� ��������
	case 508: //ToNeighbour_8_OutMk ������ �� ��� ������

	case 109:   //In9_Set ������� �������� �� 9-�� ������
	case 159: //In9_Out ������ ��������, ���������� �� 9-�� ������
	case 209: //In9_OutMk ������ �� ��� ���������, ���������� �� 9-�� ������
	case 259: //NeighbourMk_9_Set ���������� �� ��� 9-�� ������
	case 309: //Neighbour_9_Set ���������� ������ �� 9-�� ������
	case 359: //Parametr_9_Set ���������� 9-� ��������
	case 409: //Parametr_9_Out ������ 9-� ��������
	case 459: //Parametr_9_OutMk ������  �� � 9-� ��������
	case 509: //ToNeighbour_9_OutMk ������ �� ��� ������

	case 110:  //In10_Set ������� �������� �� 10-�� ������
	case 160: //In10_Out ������ ��������, ���������� �� 10-�� ������
	case 210: //In10_OutMk ������ �� ��� ���������, ���������� �� 10-�� ������
	case 260: //NeighbourMk_10_Set ���������� �� ��� 10-�� ������
	case 310: //Neighbour_10_Set ���������� ������ �� 10-�� ������
	case 360: //Parametr_10_Set ���������� 10-� ��������
	case 410: //Parametr_10_Out ������ 10-� ��������
	case 460: //Parametr_10_OutMk ������   �� � 10-� ��������
	case 510: //ToNeighbour_10_OutMk ������ �� ��� ������

	case 111:  //In11_Set ������� �������� �� 11-�� ������
	case 161: //In11_Out ������ ��������, ���������� �� 11-�� ������
	case 211: //In11_OutMk ������ �� ��� ���������, ���������� �� 11-�� ������
	case 261: //NeighbourMk_11_Set ���������� �� ��� 11-�� ������
	case 311: //Neighbour_11_Set ���������� ������ �� 11-�� ������
	case 361: //Parametr_11_Set ���������� 11-� ��������
	case 411: //Parametr_11_Out ������ 11-� ��������
	case 461: //Parametr_11_OutMk ������  �� 11-� ��������
	case 511: //ToNeighbour_11_OutMk ������ �� ��� ������

	case 112:  //In12_Set ������� �������� �� 12-�� ������
	case 162: //In12_Out ������ ��������, ���������� �� 12-�� ������
	case 212: //In12_OutMk ������ �� ��� ���������, ���������� �� 12-�� ������
	case 262: //Neighbour_12_Set ���������� ������ �� 12-�� ������
	case 312: //NeighbourMk_12_Set ���������� �� ��� 12-�� ������
	case 362: //Parametr_12_Set ���������� 12-� ��������
	case 412: //Parametr_12_Out ������ 12-� ��������
	case 462: //Parametr_12_OutMk ������  �� 12-� ��������
	case 512: //ToNeighbour_12_OutMk ������ �� ��� ������

	case 113:  //In13_Set ������� �������� �� 13-�� ������
	case 163: //In13_Out ������ ��������, ���������� �� 13-�� ������
	case 213: //In13_OutMk ������ �� ��� ���������, ���������� �� 13-�� ������
	case 263: //Neighbour_13_Set ���������� �� ��� 13-�� ������
	case 313: //NeighbourMk_13_Set ���������� ������ �� 13-�� ������
	case 363: //Parametr_13_Set ���������� 13-� ��������
	case 413: //Parametr_13_Out ������ 13-� ��������
	case 463: //Parametr_13_OutMk ������  �� 13-� ��������
	case 513: //ToNeighbour_13_OutMk ������ �� ��� ������

	case 114:  //In14_Set ������� �������� �� 14-�� ������
	case 164: //In14_Out ������ ��������, ���������� �� 14-�� ������
	case 214: //In14_OutMk ������ �� ��� ���������, ���������� �� 14-�� ������
	case 264: //NeighbourMk_14_Set ���������� �� ��� 14-�� ������
	case 314: //Neighbour_14_Set ���������� ������ �� 14-�� ������
	case 364: //Parametr_14_Set ���������� 14-� ��������
	case 414: //Parametr_14_Out ������ 14-� ��������
	case 464: //Parametr_14_OutMk ������ �� 14-� ��������
	case 514: //ToNeighbour_14_OutMk ������ �� ��� ������

	case 115:  //In15_Set ������� �������� �� 15-�� ������
	case 165: //In15_Out ������ ��������, ���������� �� 15-�� ������
	case 215: //In15_OutMk ������ �� ��� ���������, ���������� �� 15-�� ������
	case 265: //Neighbour_15_Set ���������� �� ��� 15-�� ������
	case 315: //NeighbourMk_15_Set ���������� ������ �� 15-�� ������
	case 365: //Parametr_15_Set ���������� 15-� ��������
	case 415: //Parametr_15_Out ������ 15-� ��������
	case 465: //Parametr_15_OutMk ������ �� 15-� ��������
	case 515: //ToNeighbour_15_OutMk ������ �� ��� ������

		switch (MK / 50) {
		case 3: // ������ ���������� �� ������ ��������
			Load.Write(Plys[PlyInd][MK % 50]);
			break;
		case 4: // ������ ������ �� �� ��������� �� ������
			MkExec(Load, Plys[PlyInd][MK % 50]);
			break;
		case 5: // ���������� �� ��� ������
			NeighboursMk[MK % 50] = Load.toInt(); // ���������� �� ��� ������
			break;
		case 6: // ���������� ������ �� ������
			Neighbours[MK % 50] = (CellularAutomat*)Load.Point;
			break;
		case 7: // ���������� ��������
			parameters[MK % 50].WriteFromLoad(Load);
			break;
		case 8: // ������ �� � ����������
			MkExec(Load, parameters[MK % 50]);
			break;
		case 9: // ���������� ������ �� ������
			Neighbours[MK % 50] = (FU*)Load.Point;
			break;
		case 10: // ToNeighbour_N_OutMk ������ �������� ��� ������ � ��������
			Neighbours[PlyInd][MK % 50].ProgFU(NeighboursMk[MK % 50], Load);
			break;
		case 2: // ������ �������� �� ������
		{
			if (Plys[PlyCurrent][MK % 50].Point == nullptr)
				InCounter[PlyCurrent]++;
			Plys[PlyCurrent][MK % 50] = Load.Clone();
			InCounter[PlyCurrent]++;
			ProgExec(ReceiveProg); // ������ ��������� �� ������ ������ �� ������
			if (InCounter[PlyCurrent] == Neighbours.size()) // �������� ������ ��������
			{
				InComplectF[PlyCurrent][MK % 50] = true;
				ProgExec(FiringProg);
				RezReady[PlyCurrent] = true;
				if (AutoSend) ProgFU(890, { 0, nullptr });
				PlyCurrent++;
				InCounter[PlyCurrent] = 0;
				RezReady[PlyCurrent] = false;
				for (int i = 0; i < Neighbours.size(); i++)
					InComplectF[PlyCurrent][i] = false;
			}
		}
		}
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
}

void CellularAutomatManager::ProgFU(int MK, LoadPoint Load)
{
	// �������� ����� �� � ���������
	switch (MK)
	{
	case 0: // Reset
		NetType = 0;
		break;
	case 1:// NetTypeSet ���������� ��� �����
		NetType = Load.toInt();
		break;
	case 3: // DimAdd �������� ���������
		Dim.push_back(Load.toInt());
		break;
	case 5: // DimClear �������� ������� ������������
		Dim.clear();
		break;
	case 10: // IniAutmataProgSet ���������� ��������� ������������� ��������
		iniAutmataProg = Load.Point;
		break;
	case 11: // CollectorSet ��������� ������ �� ���������
		Collector = Load.Point;
		break;
	case 15: // FiringProgSet ���������� ��������� ���������� ����������
		FiringProg = Load.Point;
		break;
	case 16: // ReceiveProgSet ���������� ��������� ���������� ����������
		ReceiveProg = Load.Point;
		break;
	case 17: // InCounterSet ���������� ������� ������� ������
		InCounter = Load.toInt();
		break;
	case 30: // Ind1Set ���������� ������ ������ ��-��������
		Ind1 = Load.toInt();
		break;
	case 31: // Ind2Set ���������� ������ ������ ��-��������
		Ind2 = Load.toInt();
		break;
	case 35: // Ind1ContextOut ������ �������� ������� ��-��������
		Load.Write((FU*)&Net[Ind1]);
		break;
	case 36: // Ind1ContextOutMk ������ �� � ���������� ������� ��-��������
		MkExec(Load, { TFU, &Net[Ind1] });
		break;
	case 37: // Ind2ContextOut ������ �������� ������� ��-��������
		Load.Write((FU*)&Net[Ind2]);
		break;
	case 38: // Ind2ContextOutMk ������ �� � ���������� ������� ��-��������
		MkExec(Load, { TFU, &Net[Ind2] });
		break;
	case 50: // Ind1ProgExec ��������� ��������� ��� ������� ��-��������
		Net[Ind1].ProgExec(Load);
	case 51: // Ind2ProgExec ��������� ��������� ��� ������� ��-��������
		Net[Ind1].ProgExec(Load);
	case 60: // Neitborder1Set ���������� ������ ��� �� � �������� 1
		Net[Ind1].Neighbours.push_back((FU*)Load.Point);
	case 61: // Neitborder2Set ���������� ������ ��� �� � �������� 2
		Net[Ind2].Neighbours.push_back((FU*)Load.Point);
	case 65: // Neitborder1Out ������ ������ �� �������� �� � �������� 1
		Load.Write((FU*)&Net[Ind1]);
		break;
	case 66: // Neitborder2Out  ������ ������ �� �������� �� � �������� 2
		Load.Write((FU*)&Net[Ind2]);
		break; 
	case 67: // Neitborder1OutMk  ������ �� �� ������� �� �������� �� � �������� 1
		MkExec(Load, {TFU,&Net[Ind1] });
		break;
	case 68: // Neitborder2OutMk ������ �� �� ������� �� �������� �� � �������� 2
		MkExec(Load, { TFU,&Net[Ind2] });
		break;
	case 70: //PlyNSet ���������� ���������� �����
		PlyN = Load.toInt(1);

	case 100: //NetGenerate ��������� �����
	{
		int N = 0;
		for (auto i : Dim)
			N += i;
		Net.resize(N); // ������� ���� ���������
		int k = 0;
		for (auto& i : Net)
		{
			i.Manager = this; // ���������� ������ �� ���������
			i.Collector = Collector; // ��������� ������ �� ���������
			i.ProgExec(iniAutmataProg); // ������ ��������� ������������� ������� ��������
			i.FUInd = k; // ���������� ������ ��������
			i.FiringProg = FiringProg; // ���������� ��������� ���������� ����������
			i.InCounter[0] = InCounter;
			i.Plys[0].resize(InCounter);
			k++;
		}
	}
		break;
	case 101: // NetClear �������� �����
		Net.clear();
		Dim.clear();
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
}
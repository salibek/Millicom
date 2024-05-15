#include "StreamManager.h"
#include "FUIni.h"

using namespace std;

void StreamManager::ProgFU(long int MK, LoadPoint Load, FU* Sender)
{
	int MKinitial = MK;
	MK %= FUMkRange;
	switch (MK % FUMkRange)
	{
	case 0: //Reset
		for (auto& i : Field)
		{
			for (auto& j : i)
				free(j);
			i.clear();
		}
		Field.clear();
		DeviseCounter = 0;
		break;
	case 1: // FieldCreate ��������� ���� ��� (�� ����� ����� ���� ��� ��������� �� ������ ��)
	{
		Field.push_back({});
		for (long int i = 0; i < Counter; i++)
		{
			if (Load.isEmpty()) continue;
			if (Load.isInt())
				Field.back().push_back(MakeFU.MakeFu(Load.toInt() + FUTypeCorrect, Bus));
			else if (Load.isFU()) // ���� ��������� �� ��
				Field.back().push_back(((FU*)Load.Point)->Copy());

			Field.back().back()->FUInd = i; // ������ ��
			Field.back().back()->FUInd2 = Field.size() - 1; // ������ ������
		}
		break;
	}
	case 2: // GroupCreateTempl ��������� ������ �� ������ ������� �� ����� ����� ������, �� ��������� �� ������� GroupInd)
	{
		if (Load.isEmpty() && (IndGroup >= Field.size() || IndGroup < 0))
		{
			FU::ProgExec(DevNotExistErrProg);
			break;
		}
		long int t;
		if (Load.isEmpty())

			for (int i = 1; i <= Counter; i++)
			{
				Field.push_back({});
				if (Load.isInt())
				{
					Field.back().push_back(MakeFU.MakeFu(Load.toInt()+ FUTypeCorrect, Bus));
					break;
				}
				else if (Load.isFU()) // ���� ��������� �� ��
				{
					Field.back().push_back(MakeFU.MakeFu(Load.Point));
					break;
				}
			}
	}
	break;
	case 3: // FieldClear �������� ���� ���
		for (int i = 0; i < Field.size(); i++) // ��������� ��� ��, ����� �������
		{
			for (int j = 0; j < Field[i].size(); j++)
				free(Field[i][j]);
			Field[i].clear();
		}
		Field.clear();
		break;
	case 5: // IndGroupSet ���������� ������ ������ ���
		IndGroup = Load.toInt();
		break;
	case 6:	//IndSet ���������� ������ ��� � ������
		Ind = Load.toInt();
		break;
	case 4: //Ind2Set ���������� ������ ������ ��� � ������
		Ind2 = Load.toInt();
		break;
	case 7: // IndSwap �������� ������� ��������� �������
		swap(Ind, Ind2);
		//swap(Mk1, Mk2);
		break;
	case 8: // GroupIndSwap �������� ������� ����� �������
		swap(IndGroup, IndGroup2);
		break;
	case 9: //Ind2GroupSet ���������� ������ ������ ������ ���
		IndGroup2 = Load.toInt();
		break;
	case 10: // GoupCreate ������� ������ ���
		Field.push_back({});
		break;
	case 45: // DevCopy ����������� �� � ������
	case 11: // DevCreate ������� ��� � ������ (�� ����� ������ ��)
		if (IndGroup >= Field.size() && IndGroup >= 0)
		{
			FU::ProgExec(DevNotExistErrProg);
			break;
		}
		if (Load.isEmpty()) break;
		for (int i = Counter; i < 0; i++)
			if (Load.isInt())
			{
				Field[IndGroup].push_back(MakeFU.MakeFu(Load.toInt() + FUTypeCorrect, Bus));
				break;
			}
			else if (Load.isFU()) // ���� ��������� �� ��
			{
				if(MK==11)
					Field[IndGroup].push_back(MakeFU.MakeFu(Load.Point));
				break;
			}
		break;
	case 12: // DevCreateLast ������� ��� � ��������� ������
		if (Field.size() == 0) // ���� ������ ������, �� �������� ���� ������
		{
			Field.push_back({});
			IndGroup = 0;
		}
		if (Load.isEmpty()) break;
		for (int i = Counter; i < 0; i++)
			if (Load.isInt())
			{
				Field.back().push_back(MakeFU.MakeFu(Load.toInt() + FUTypeCorrect, Bus));
				break;
			}
			else if (Load.isFU()) // ���� ��������� �� ��
			{
				Field.back().push_back(MakeFU.MakeFu(Load.Point));
				break;
			}
		break;
	case 13: // DevAdd �������� ���������� � ������� ������
		//if (!Load.isFU()) break;
		if (Field.size() == 0) // ���� ������ ������, �� �������� ���� ������
		{
			Field.push_back({});
			IndGroup = 0;
		}
		if (Load.isInt())
			Field[IndGroup].push_back(MakeFU.MakeFu(Load.toInt() + FUTypeCorrect, Bus));
		else if (!Load.isFU())
			Field[IndGroup].push_back((FU*)Load.Point);
		break;
	case 14: // DevLastAdd �������� ��������� � ��������� ������
		//if (!Load.isFU()) break;
		if (Field.size() == 0) // ���� ������ ������, �� �������� ���� ������
		{
			Field.push_back({});
			IndGroup = 0;
		}
		if (Load.isInt())
			Field.back().push_back(MakeFU.MakeFu(Load.toInt() + FUTypeCorrect, Bus));
		else if (!Load.isFU())
			Field.back().push_back((FU*)Load.Point);
		Field.back().back()->FUInd = Field.back().size() - 1;
		Field.back().back()->FUInd2 = Field.size() - 1;
		break;
	case 15: // GroupRefCreate C������ ������ �� �� ��� ������ ��
	{
		for (auto& i : Field[IndGroup])
			switch (i->GetFuType())
			{
			case FUStreamFloatALU:
				for (int j = 0; j < ((StreamFloatALU*)i)->ReceiverMk.size(); j++)
				{
					if (((StreamFloatALU*)i)->ReceiverContexts[j] == Bus || ((StreamFloatALU*)i)->ReceiverContexts[j] == nullptr)
					{
						FU* t = nullptr;
						for (auto& k : Field[IndGroup])
							if (k->FUMkGlobalAdr <= 10);
					}
				}
				break;
			case FUStreamIntALU:
				break;
			}
	}
	break;
	case 16: // IndOut ������ ������ ������
		Load.Write(Ind);
		break;
	case 17: // IndOutMk ������ �� � ������ ��������
		MkExec(Load, { Cint,&Ind });
		break;
	case 18: // Ind2Out ������ ������ ������
		Load.Write(Ind2);
		break;
	case 19: // Ind2OutMk ������ �� �� ������ ��������
		MkExec(Load, { Cint,&Ind2 });
		break;
	case 151: // IndLastOut ������ ������ ���������� �� (��� ������� �������� ������������ � ������� ������)
		if (!Field.size()) break;
		if (Load.isEmpty())
		{
			Ind = Field[IndGroup].size() - 1;
			break;
		}
		Load.Write(Field[IndGroup].size() - 1);
		break;
	case 152: // IndLastOutMk ������ �� � �������� ���������� �� (��� ������� �������� ������������ � ������� ������)
	{
		if (!Field.size()) break;
		if (Load.isEmpty())
		{
			Ind = Field[IndGroup].size() - 1;
			break;
		}
		long t = Field[IndGroup].size() - 1;
		MkExec(Load, {Cint,&t});
		}
		break;
	case 20: // IndAdd ��������� � ������� ������ (� �������� ��������, ������� ������������ � �������
		Ind += Load.toInt();
		break;
	case 21: // IndGroupAdd   ��������� � ������� ������ (� �������� ��������, ������� ������������ � �������, 1 �� ���������) 
		IndGroup += Load.toInt();
		break;
	case 22: // Ind2Add ��������� �� ������� ������� ���������� (� �������� ��������, ������� ������������ � �������)
		Ind2 += Load.toInt();
		break;
	case 23: // Ind2GroupAdd   ��������� �� ������� ������� ������ (� �������� ��������, ������� ������������ � �������, 1 �� ���������) 
		IndGroup2 += Load.toInt();
		break;
	case 24: // IndSub ������� �� ������� ���������
		Ind -= Load.toInt();
		break;
	case 25: // IndMul �������� ������ ����������
		Ind *= Load.toInt();
		break;
	case 26: // IndMod ������� �� ������� ������� ����������
		Ind %= Load.toInt();
		break;

	case 28: // MkSet ���������� �� ��� ������� �������
		Mk1 = Load.toInt();
		break;
	case 29: // Mk2Set ���������� �� ��� ������� �������
		Mk2 = Load.toInt();
		break;
	case 27: // MkLastSet ���������� �� ��� ���������� ��
		MkLast = Load.toInt();
		break;
	case 30: // ExecAll ��������� ��������� ��� ���� �� ����
		for (auto& i : Field)
			for (auto& j : i)
				j->ProgExec(Load);
		break;
	case 31: // ExecGroup ��������� ��������� ��� ���� �� ������
		if (IndGroup >= Field.size())
		{
			FU::ProgExec(DevNotExistErrProg);
			break;
		}
		for (auto& i : Field[IndGroup])
			i->ProgExec(Load);
		break;
	case 32: // ExecDev ��������� ��������� ��� ����������� �� � �������� Ind
		if (IndGroup >= Field.size() || Ind >= Field[IndGroup].size())
		{
			FU::ProgExec(DevNotExistErrProg);
			break;
		}
		Field[IndGroup][Ind]->ProgExec(Load);
		break;
	case 43: // ExecLastDev ��������� ��������� ��� ���������� ��
		if (!Field.size() || !Field.back().size()) break;
		Field.back().back()->ProgExec(Load);
		break;
	case 33: // Exec2Group ��������� ��������� ��� ���� �� ������
		if (IndGroup >= Field.size())
		{
			FU::ProgExec(DevNotExistErrProg);
			break;
		}
		for (auto& i : Field[IndGroup2])
			i->ProgExec(Load);
		break;
	case 34: // Exec2Dev ��������� ��������� ��� ����������� ��
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{
			FU::ProgExec(DevNotExistErrProg);
			break;
		}
		Field[IndGroup2][Ind]->ProgExec(Load);
		break;
	case 35: // MkExec ��������� �� ��� �� ����
	{
		FU* t = nullptr;
		if (Load.toInt() == 36) // ���� ���������� ���������� �� ��� ���� �� ����
		{
			t = Field[IndGroup][Ind]->Bus; // ��������� ��������� �� ���� ��� ������� ��
			Field[IndGroup][Ind]->Bus = Field[IndGroup][Ind2];
			Field[IndGroup][Ind]->MkExec(Mk1, { Cmk, &Mk2 });
			Field[IndGroup][Ind]->Bus = t; // ������������ ��������� �� ����
			break;
		}
		Field[IndGroup][Ind]->MkExec(Mk1, Load);
	}
	break;
	case 42: // MkLastExec // ��������� �� ��� ���������� ��
		if (!Field.size() || !Field.back().size()) break;
		Field.back().back()->MkExec(MkLast, Load);
		break;
	case 36: // Mk2Exec ��������� �� ��� �� ���� �� ������� �������
	{
		FU* t = nullptr;
		if (Load.toInt() == 35) // ���� ���������� ���������� �� ��� ���� �� ����
		{
			t = Field[IndGroup][Ind2]->Bus; // ��������� ��������� �� ���� ��� ������� ��
			Field[IndGroup][Ind2]->Bus = Field[IndGroup][Ind];
			Field[IndGroup2][Ind2]->MkExec(Mk2, { Cmk, &Mk1 });
			Field[IndGroup][Ind2]->Bus = t; // ������������ ��������� �� ����
			break;
		}
		Field[IndGroup2][Ind2]->MkExec(Mk2, Load);
		break;
	}
	case 37: // MkBackExec ��������� �� ��� ���������� �� ��������� ������ ����
		Field.back().back()->MkExec(Mk1, Load);
		break;
	case 38: // Mk2BackExec ��������� �� ��� �� ���� �� ������� �������
		Field.back().back()->MkExec(Mk2, Load);
		break;
	case 39: // MkAllExec ��������� �� ��� ���� �� ����
		for (auto& i : Field)
			for (auto& j : i)
				j->MkExec(Mk1, Load);
		break;
	case 40: // MkAllGroupExec ��������� �� ��� ���� �� ������� ������
		for (auto& j : Field[IndGroup])
			j->MkExec(Mk1, Load);
		break;
	case 41: // MkAllLastGroupExec ��������� �� ��� ���� �� ��������� ������
		for (auto& j : Field.back())
			j->MkExec(Mk1, Load);
		break;

	case 50: //  DevCountOut ������ ���������� ��� � ����
		Load.Write(DeviseCounter);
		break;
	case 51: //  DevCountOutMk ������ �� � ����������� ��� � ����
		MkExec(Load, { Cint,&DeviseCounter });
		break;
	case 52: //  DevGroupCountOut ������ ���������� ��� � ������� ������
		if (IndGroup >= Field.size())
		{
			FU::ProgExec(DevNotExistErrProg);
			break;
		}
		Load.Write(Field[IndGroup].size());
		break;
	case 53: //  DevGroupCountOutMk ������ ���������� ��� � ������� ������
	{
		if (IndGroup >= Field.size())
		{
			FU::ProgExec(DevNotExistErrProg);
			break;
		}
		long int t = Field[IndGroup].size();
		MkExec(Load, { Cint,&t });
		break;
	}
	case 55: //  GroupCountOut ������ ���������� �����
		Load.Write(Field.size());
		break;
	case 56: //  GroupCountOutMk ������ �� � ����������� �����
	{
		long int t = Field.size();
		MkExec(Load, { Cint,&t });
		break;
	}
	case 57: //  GroupLastIndOutMk ������ �� � ��������� �������� ������
	{
		long int t = Field.size()-1;
		MkExec(Load, { Cint,&t });
		break;
	}
	case 60: // CounterSet ���������� ������� ��� ���������� ��������� ����������
		Counter = Load.toInt(1);
		break;
	case 61: //CounterAdd ��������� � �������� (�� ��������� 1)
		Counter += Load.toInt(1);
		break;
	case 62: // CounterMul �������� ������� (�� ��������� 2)
		Counter *= Load.toInt(2);
		break;
	case 63: // CounterSub ������� �� �������� (�� ��������� 1)
		Counter -= Load.toInt(1);
		break;
	case 64: // ExecCounterSet ���������� ������� �������� ���������� ������������
		if (!ExecFlag)
			ExecCounter.back() = Load.toInt(1);
		else
			ExecCounter.push_back(Load.toInt(1));
		break;
	case 65: // ExecCounterAdd ��������� � �������� ��������
		ExecCounter.back() += Load.toInt();
		break;
	case 66: //ExecCounterSub ������� �� �������� ��������
		ExecCounter.back() -= Load.toInt();
		break;
	case 67: // ExecCounterMul �������� ������� ��������
		ExecCounter.back() *= Load.toInt();
		break;
	case 68: // ExecCounterDiv ������������ ��������� ������� ��������
		ExecCounter.back() /= Load.toInt();
		break;

	case 70: //  DevOut ������ �������� �������� ���
	case 71: //  DevOutMk ������ �������� �������� ���
	case 72: //  Dev2Out ������ �������� �������� ���
	case 73: //  Dev2OutMk ������ �������� �������� ���
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{
			FU::ProgExec(DevNotExistErrProg);
			break;
		}
		switch (MK % FUMkRange) {
		case 70:
			Load.Write((FU*)Field[IndGroup][Ind]);
			break;
		case 71:
			MkExec(Load, { CFU,Field[IndGroup][Ind] });
			break;
		case 72:
			Load.Write((FU*)Field[IndGroup2][Ind2]);
			break;
		case 73:
			MkExec(Load, { CFU,Field[IndGroup2][Ind2] });
			break;
		}
		break;
	case 80: //  DevLastOut ������ �������� ���������� ���������� ���
		if (!Field.size())
		{
			FU::ProgExec(DevNotExistErrProg);
			break;
		}
		Load.Write((FU*)&Field.back().back());
		break;
	case 81: //  DevLastOutMk ������ �� � ���������� ���������� ���������� ���
		if (!Field.size())
		{
			FU::ProgExec(DevNotExistErrProg);
			break;
		}
		MkExec(Load, { CFU,(FU*)&Field.back().back() });
		break;
	case 85: // DevCurrentSet ���������� ��������� �� ������� ��
		DevCurrent = (FU*)Load.Point;
		break;
	case 86: // DevCurrentExec ��������� ��������� ��� �������� ��
		DevCurrent->ProgExec(Load);
		break;
	case 87: // DevCurrentMkSet ���������� �� ��� �������� ��
		DevCurrentMk = Load.toInt();
		break;
	case 88: // DevCurrentMkExec ��������� �� ��� �������� ��
		DevCurrent->MkExec(DevCurrentMk,Load);
		break;
	case 89: // DevCurrentOutMk ������ �� � ������� ��
		MkExec(Load, { CFU,DevCurrent });
		break;
	case 84: // DevCurrentOut ������ ������� ��
		Load.Write(DevCurrent);
		break;
	case 150: // DevLastExec ��������� ��������� ��� ���������� ��
		if (!Field.size() || !Field.back().size()) break;
		Field.back().back()->ProgExec(Load);
		break;
	case 90: //  GroupLastIndOut ������ ������ ��������� ��������� ������ ��� (-1, ���� ���� ������)
		Load.Write(Field.size() - 1);
		break;
	case 91: //  GroupLastIndOutMk ������ �� � �������� ��������� ��������� ������ ��� (-1, ���� ���� ������)
	{
		long int t = Field.size() - 1;
		MkExec(Load, { CFU,&t });
	}
	break;
	case 100: //  DevLastIndOut ������ ������ ���������� ���������� ��� (-1, ���� ���� ������)
		if (!Field.size() || !Field.back().size())
			Load.Write((long int)-1);
		else
			Load.Write(Field.back().size() - 1);
		break;
	case 101: //  DevLastIndOutMk ������ �� � �������� ���������� ���������� ��� (-1, ���� ���� ������)
	{
		long int t = -1;
		if (Field.size() && Field.back().size())
			t = Field.back().size() - 1;
		MkExec(Load, { CFU,&t });
	}
	break;

	case 108: //RezBufVectOut ������ ������ �������� ������ ���������� ���� �� ����
	case 109: //RezBufVectOutMk ������ �� � �������� �������� ������ ���������� ���� �� ����
	case 110: //RezVectOut ������ ������ ����������� ���� �� ����
	case 111: //RezVectOutMk ������ �� � �������� ����������� ���� �� ����
	case 112: //ReadyVectOut ������ ������ ���������� ����������� ���� �� ����
	case 113: //ReadyVectOutMk  ������ �� � �������� ���������� ����������� ���� �� ����
	{
		if (IndGroup >= Field.size() || IndGroup < 0)
		{
			FU::ProgExec(DevNotExistErrProg);
			break;
		}
		LoadVect_type t = new vector<LoadPoint>;
		for (auto& i : Field)
			for (auto& j : i)
				switch (j->FUtype)
				{
				case FUStreamFloatALU:
					switch (MK)
					{
					case 108:
					case 109: t->push_back({ Cdouble, &((StreamFloatALU*)j)->RezBuf }); break;
					case 111:
					case 110: t->push_back({ Cdouble, &((StreamFloatALU*)j)->Rez }); break;
					default:
						t->push_back({ Cint, &((StreamFloatALU*)j)->Ready });
						break;
					}
					break;
				case FUStreamIntALU:
					switch (MK) {
					case 108:
					case 109: t->push_back({ Cint, &((StreamIntALU*)j)->RezBuf }); break;
					case 111:
					case 110: t->push_back({ Cint, &((StreamIntALU*)j)->Rez }); break;
					default:
						t->push_back({ Cint, &((StreamIntALU*)j)->Ready });
						break;
					}
				}
		if (MK % 2 == 0)
			Load.Write(t);
		else
			MkExec(Load, { CLoadVect, t });
		break;
	}

	case 115: //RezGroupVectOut ������ ������ ����������� ���� �� ������
	case 116: //RezGroupVectPutMk ������ �� � �������� ����������� ���� �� ������
	{
		if (IndGroup >= Field.size() || IndGroup < 0)
		{
			FU::ProgExec(DevNotExistErrProg);
			break;
		}
		LoadVect_type t = new vector<LoadPoint>;
		for (auto& j : Field[IndGroup])
			switch (j->FUtype)
			{
			case FUStreamFloatALU:
				t->push_back({ Cdouble, &((StreamFloatALU*)j)->Ready });
				break;
			case FUStreamIntALU:
				t->push_back({ Cdouble, &((StreamIntALU*)j)->Ready });
				break;
			}
		if (MK == 110)
			Load.Write(t);
		else
			MkExec(Load, { CLoadArray, &t });
		break;
	}
	case 117: //ReadyGroupVectOut ������ ������ ���������� ����������� ���� �� ������
	case 118: //ReadyGroupVectOutMk ������ �� � �������� ���������� ����������� ���� �� ������
	{
		if (IndGroup >= Field.size() || IndGroup < 0)
		{
			FU::ProgExec(DevNotExistErrProg);
			break;
		}
		LoadVect_type t = new vector<LoadPoint>;
		for (auto& j : Field[IndGroup])
			switch (j->FUtype)
			{
			case FUStreamFloatALU:
				t->push_back({ Cdouble, &((StreamFloatALU*)j)->Ready });
				break;
			case FUStreamIntALU:
				t->push_back({ Cdouble, &((StreamIntALU*)j)->Ready });
				break;
			}
		if (MK == 110)
			Load.Write(t);
		else
			MkExec(Load, { CLoadArray, &t });
		break;
	}
	// ������ � ����������, ����������� � ���� ��
	case 129: // VarClear ������� ������ ���������� ��� ��������� �� ����
		VarFuMap.clear();
		break;
	case 130: // VarNameAdd �������� ��� ����������
		VarFuMap.insert({ Load.toStr(), {} });
		AddedVarName = Load.toStr();
		break;
	case 131: // VarMkAdd �������� �� ��� ����������
		VarFuMap[AddedVarName].push_back({ Load.toInt(), nullptr });
		break;
	case 132: // VarFuAdd �������� �� ��� ����������
		VarFuMap[AddedVarName].back().second=(FU*)Load.Point;
		break;
	case 140: //VarNameSet ���������� ��� ����������, ������� ����� ����� ��������� �� �� ����
		VarName = Load.toStr();
		break;
	case 145: // ValByNameSend ��������� �������� ���������� �� �� �����
		if (!VarFuMap.count(VarName))
			ProgExec(ValNotFaund);
		else
			for (auto& i : VarFuMap[VarName])
				MkExec(i.first, Load, i.second);
		break;
	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}

 void StreamManager::ProgExec(void* Uk, unsigned int CycleMode, FU* Bus, vector<ip>::iterator* Start) // ���������� ��������� �� ��
{
	 ExecFlag = true;
	 for (int i = 0; i < ExecCounter.back(); i++)
		 FU::ProgExec(Uk, CycleMode, Bus, Start);
	 if (ExecCounter.size() == 1) ExecFlag = false;
	 if(ExecCounter.size()>1) ExecCounter.pop_back();
 }
void StreamManager::ProgExec(LoadPoint Uk, unsigned int CycleMode, FU* Bus, vector<ip>::iterator* Start) // ���������� ��������� �� ��
{
	ExecFlag = true;
	for (int i = 0; i < ExecCounter.back(); i++)
		FU::ProgExec(Uk, CycleMode, Bus, Start);
	if (ExecCounter.size() == 1) ExecFlag = false;
	if (ExecCounter.size() > 1) ExecCounter.pop_back();
}

StreamManager::~StreamManager() // ����������
{
	ProgFU(0, {0,nullptr},nullptr);
	Field.clear();
}

FU* StreamManager::Copy() // ��������� ����������� ��
{
	return new StreamManager(Bus, this);
}

FU* StreamManager::TypeCopy() // ������� �� ������ �� ���� (�� ������� ��������
{
	return new StreamManager(Bus, nullptr);
}
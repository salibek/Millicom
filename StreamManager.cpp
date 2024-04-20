#include "StreamManager.h"
#include "FUIni.h"

using namespace std;

void StreamManager::ProgFU(long int MK, LoadPoint Load, FU* Sender)
{
	switch (MK%FUMkRange)
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
		for (int i = 1; i <= Counter; i++)
		{
			Field.push_back({});
			if (Load.isEmpty()) continue;
			if (Load.isInt())
			{
				Field.back().push_back(MakeFU.MakeFu(Load.toInt(), Bus));
				break;
			}
			else if (Load.isFU()) // ���� ��������� �� ��
			{
				Field.back().push_back(((FU*)Load.Point)->Copy());
				break;
			}
		}
		break;
	case 2: // GroupCreateTempl ��������� ������ �� ������ ������� �� ����� ����� ������, �� ��������� �� ������� GroupInd)
	{
		if (Load.isEmpty() && (IndGroup >= Field.size() && IndGroup >= 0))
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		long int t;
		if (Load.isEmpty())

			for (int i = 1; i <= Counter; i++)
			{
				Field.push_back({});
				if (Load.isInt())
				{
					Field.back().push_back(MakeFU.MakeFu(Load.toInt(), Bus));
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
	case 7: // CounterSet ���������� ������� ��� ���������� ��������� ����������
		Counter = Load.toInt(1);
		break;
	case 4: //Ind2Set ���������� ������ ������ ��� � ������
		Ind2 = Load.toInt();
		break;
	case 9: //Ind2GroupSet ���������� ������ ������ ������ ���
		IndGroup2 = Load.toInt();
		break;
	case 10: // CreateGoup ������� ������ ���
		Field.push_back({});
		break;
	case 11: // DevCreate ������� ��� � ������ (�� ����� ������ ��)
		if (IndGroup >= Field.size() && IndGroup >= 0)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		if (Load.isEmpty()) break;
		for (int i = Counter; i < 0; i++)
			if (Load.isInt())
			{
				Field[IndGroup].push_back(MakeFU.MakeFu(Load.toInt(), Bus));
				break;
			}
			else if (Load.isFU()) // ���� ��������� �� ��
			{
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
				Field.back().push_back(MakeFU.MakeFu(Load.toInt(), Bus));
				break;
			}
			else if (Load.isFU()) // ���� ��������� �� ��
			{
				Field.back().push_back(MakeFU.MakeFu(Load.Point));
				break;
			}
		break;
	case 13: // DevAdd �������� ���������� � ������� ������
		if (!Load.isFU()) break;
		if (Field.size() == 0) // ���� ������ ������, �� �������� ���� ������
		{
			Field.push_back({});
			IndGroup = 0;
		}
		Field[IndGroup].push_back((FU*)Load.Point);
		break;
	case 14: // DevLastAdd �������� ��������� � ��������� ������
		if (!Load.isFU()) break;
		if (Field.size() == 0) // ���� ������ ������, �� �������� ���� ������
		{
			Field.push_back({});
			IndGroup = 0;
		}
		Field.back().push_back((FU*)Load.Point);
		break;
	case 19: // GroupRefCreate C������ ������ �� �� ��� ������ ��
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
	case 24: // MkSet ���������� �� ��� �������� �������
		Mk1 = Load.toInt();
		break;
	case 25: // Mk2Set ���������� �� ��� ������� �������
		Mk2 = Load.toInt();
		break;
	case 30: // ExecAll ��������� ��������� ��� ���� �� ����
		for (auto& i : Field)
			for (auto& j : i)
				j->ProgExec(Load);
		break;
	case 31: // ExecGroup ��������� ��������� ��� ���� �� ������
		if (IndGroup >= Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		for (auto& i : Field[IndGroup])
			i->ProgExec(Load);
		break;
	case 32: // ExecDev ��������� ��������� ��� ����������� ��
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Field[IndGroup][Ind]->ProgExec(Load);
		break;
	case 33: // Exec2Group ��������� ��������� ��� ���� �� ������
		if (IndGroup >= Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		for (auto& i : Field[IndGroup2])
			i->ProgExec(Load);
		break;
	case 34: // Exec2Dev ��������� ��������� ��� ����������� ��
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Field[IndGroup2][Ind]->ProgExec(Load);
		break;
	case 35: // MkExec ��������� �� ��� �� ����
		Field[IndGroup][Ind]->MkExec(Mk1, Load);
		break;
	case 36: // Mk2Exec ��������� �� ��� �� ���� �� ������� �������
		Field[IndGroup2][Ind2]->MkExec(Mk1, Load);
		break;
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
			ProgExec(DevNotExistErrProg);
			break;
		}
		Load.Write(Field[IndGroup].size());
		break;
	case 53: //  DevGroupCountOutMk ������ ���������� ��� � ������� ������
	{
		if (IndGroup >= Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		long int t = Field[IndGroup].size();
		MkExec(Load, {Cint,&t});
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
	case 70: //  DevOut ������ �������� �������� ���
	case 71: //  DevOutMk ������ �������� �������� ���
	case 72: //  Dev2Out ������ �������� �������� ���
	case 73: //  Dev2OutMk ������ �������� �������� ���
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		switch (MK%FUMkRange) {
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
	case 80: //  LastDevOut ������ �������� ���������� ���������� ���
		if (!Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Load.Write((FU*) & Field.back().back());
		break;
	case 81: //  LastDevOutMk ������ �� � ���������� ���������� ���������� ���
		if (!Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
			MkExec(Load, { CFU,(FU*)&Field.back().back()});
		break;
	case 90: //  LastGroupIndOut ������ ������ ��������� ��������� ������ ��� (-1, ���� ���� ������)
		Load.Write(Field.size()-1);
		break;
	case 91: //  LastGroupIndOutMk ������ �� � �������� ��������� ��������� ������ ��� (-1, ���� ���� ������)
		{
		long int t = Field.size() - 1;
		MkExec(Load, { CFU,&t});
		}
		break;
	case 100: //  LastDevIndOut ������ ������ ���������� ���������� ��� (-1, ���� ���� ������)
		if (!Field.size() || !Field.back().size())
			Load.Write((long int)-1);
		else
			Load.Write(Field.back().size()-1);
		break;
	case 101: //  LastDevIndOutMk ������ �� � �������� ���������� ���������� ��� (-1, ���� ���� ������)
	{
		long int t = -1;
		if (Field.size() && Field.back().size())
			t = Field.back().size() - 1;
		MkExec(Load, { CFU,&t });
	}
		break;

	default:
		CommonMk(MK, Load, Sender);
		break;
	}
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
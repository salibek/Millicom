#include "StreamManager.h"

using namespace std;

void StreamManager::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	switch (MK)
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
	case 1: // FieldCreate ��������� ���� ��� (�� ����� �����������, �� ��������� 1)
		Field.resize(Load.toInt(1));			
		break;
	case 2: // FieldCreateTempl ��������� ���� ��� �� ������ ������� (�� ����� �����������, ������ ��������� � ������� ���� ��� ��� �������� 0, � ������� ������)
		if (!Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
	//	for (int i = 1; i < Field.size(); i++) // ��������� ��� ��, ����� �������
	//	{
	//		for(int j=0; j< Field[i].size();j++)
	//			free(Field[i][j]);
	//		Field[i].clear();
	//	}
		{
			int N = Load.toInt();
			for (int i = 1; i <= N; i++)
			{
				Field.push_back({});
				for (auto& j : Field.back())
					switch (Mode)
					{
					case 0: Field.back().push_back(new StreamFloatALU((FU*)j)); break;
					case 1: Field.back().push_back(new StreamIntALU((FU*)j)); break;
					}
			}
			// ��������� ������� ���
			// ...
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
	case 4: // ModeSet ���������� ����� ������ �� (0 - ������� ���, 1 - ������������� ���)
		Mode = Load.toInt();
		break;
	case 5: // IndGroupSet ���������� ������ ������ ���
		IndGroup = Load.toInt();
		break;
	case 6:	//IndDevSet ���������� ������ ��� � ������
		Ind = Load.toInt();
		break;
	case 10: // CreateGoup ������� ������ ���
		Field.push_back({});
		break;
	case 11: // CreateDev ������� ��� � ������ (�� ����� ������ ��)
		if (IndGroup >= Field.size() && IndGroup>=0)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		if (!Load.isEmpty())
			;//Field[IndGroup].push_back();
		else
			switch (Mode)
			{
			case 0: Field[IndGroup].push_back(new StreamFloatALU(this, (FU*)Load.Point)); break;
			case 1: Field[IndGroup].push_back(new StreamIntALU(this, (FU*)Load.Point)); break;
			}
		break;
	case 12: // CreateLastDev ������� ��� � ��������� ������
		if (Field.size() == 0) // ���� ������ ������, �� �������� ���� ������
			Field.push_back({});
		switch (Mode)
		{
		case 0: Field.back().push_back(new StreamFloatALU((FU*)this, (FU*)Load.Point)); break;
		case 1: Field.back().push_back(new StreamIntALU((FU*)this, (FU*)Load.Point)); break;
		}
		break;

	case 20: // IndGroupAdd ��������� � ������� ������ (� �������� ��������, ������� ������������ � �������
		Ind += Load.toInt();
		break;
	case 21: // IndGroupAdd   ��������� � ������� ������ (� �������� ��������, ������� ������������ � �������, 1 �� ���������) 
		IndGroup += Load.toInt();
		break;
	case 22: // IndDevAdd ��������� � ������� �� � ������ ������ (� �������� ��������, ������� ������������ � �������, 1 �� ���������)
		Ind += Load.toInt();
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
		if (IndGroup >= Field.size() || Field[IndGroup].size()>=Ind)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Field[IndGroup][Ind]->ProgExec(Load);
		break;

	case 50: //  DevCountOut ������ ���������� ��� � ����
		Load.Write(DeviseCounter);
		break;
	case 51: //  DevCountOutMk ������ �� � ����������� ��� � ����
		MkExec(Load, {Cint,&DeviseCounter});
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
		int t = Field[IndGroup].size();
		break;
	}
	case 55: //  GroupCountOut ������ ���������� �����
		Load.Write(Field.size());
		break;
	case 56: //  GroupCountOutMk ������ �� � ����������� �����
	{
		int t = Field.size();
		break;
	}
	case 70: //  DevOut ������ �������� �������� ���
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Load.Write((FU*)Field[IndGroup][Ind]);
		break;
	case 71: //  DevOutMk ������ �������� �������� ���
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{ProgExec(DevNotExistErrProg); 
		break;
		}
		MkExec(Load,{ CFU,Field[IndGroup][Ind] });
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
		int t = Field.size() - 1;
		MkExec(Load, { CFU,&t});
		}
		break;
	case 100: //  LastDeviseIndOut ������ ������ ���������� ���������� ��� (-1, ���� ���� ������)
		if (!Field.size() || !Field.back().size())
			Load.Write((int)-1);
		else
			Load.Write(Field.back().size()-1);
		break;
	case 101: //  LastDevIndOutMk ������ �� � �������� ���������� ���������� ��� (-1, ���� ���� ������)
	{
		int t = -1;
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


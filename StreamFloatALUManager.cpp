#include "StreamFloatALUManager.h"

using namespace std;

void StreamFloatALUManager::ProgFU(int MK, LoadPoint Load, FU* Sender)
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
	case 1: // FieldCreate ��������� ���� ��� (�� ����� ����������� �� ��������� 1)
		Field.resize(Load.toInt(1));			
		break;
	case 2: // FieldCreateProg ��������� ���� ��� �� ������ ������� (�� ����� �����������, ������ ��������� � ������� ���� ��� ��� �������� 0, � ������� ������)
		if (!Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		for (int i = 0; i < Field.size(); i++) // ��������� ��� ��, ����� �������
		{
			for(int j=0; j< Field[i].size();j++)
				free(Field[i][j]);
			Field[i].clear();
		}
		Field.clear();
		break;
	case 3: // FieldClear �������� ���� ���
		for (int i = 1; i < Field.size(); i++) // ��������� ��� ��, ����� �������
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
	case 6:	//IndDeviceSet ���������� ������ ��� � ������
		Ind = Load.toInt();
		break;
	case 10: // CreateGoup ������� ������ ���
		Field.push_back({});
		break;
	case 11: // CreateDevise ������� ��� � ������
		if (IndGroup >= Field.size() || IndGroup<0)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		if(Load.isEmpty())
			Field[IndGroup].push_back(new StreamFloatALU(this,nullptr));
		else
			Field[IndGroup].push_back(new StreamFloatALU((FU*)this, (FU*)Load.Point));
		break;
	case 12: // CreateLastDevise �������� ��� � ��������� ������
		if (Load.isEmpty())
			Field.back().push_back(new StreamFloatALU(this, nullptr));
		else
			Field.back().push_back(new StreamFloatALU((FU*)this, (FU*)Load.Point));
		break;
	case 15: // AddDevise �������� ���������� � ������� ������
		if (IndGroup >= Field.size() || IndGroup < 0)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Field[IndGroup].push_back((StreamFloatALU*)Load.Point);
		break;
	case 16: // AddLasrDevise �������� ���������� � ��������� ������
		if (!Field.size())
			Field.push_back({});
		Field.back().push_back((StreamFloatALU*)Load.Point);
		break;
	case 20: // IndGroupAdd ��������� � ������� ������ (� �������� ��������, ������� ������������ � �������
		Ind += Load.toInt();
		break;
	case 21: // IndDeviceAdd ��������� � ������� �� � ������ ������ (� �������� ��������, ������� ������������ � �������, 1 �� ���������)
		IndGroup += Load.toInt();
		break;
	case 22: // IndGroupAdd ��������� � ������� ������ (� �������� ��������, ������� ������������ � �������, 1 �� ���������)
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
	case 32: // ExecDevise ��������� ��������� ��� ����������� ��
		if (IndGroup >= Field.size() || Field[IndGroup].size()>=Ind)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Field[IndGroup][Ind]->ProgExec(Load);
		break;

	case 40: // CreateGoup ������� ������ ���
		Field.push_back({});
		IndGroup = Field.size() - 1; // ���������� ��������� ������� ������ �� ��������� ������
		break;
	case 41: // CreateDevise ������� ��� � ������
		if (IndGroup >= Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		if (Load.isEmpty())
			Field[IndGroup].push_back({});
		else // �������� �� �������
			Field[IndGroup].push_back(new StreamFloatALU(this, (StreamFloatALU*)Load.Point));
		DeviseCounter++;
		break;
	case 42: // CreateLastDevise ������� ��� � ��������� ������
		if (!Field.size())
			Field.push_back({});
		if (Load.isEmpty())
			Field.back().push_back({});
		else // �������� �� �������
			Field.back().push_back(new StreamFloatALU(this, (StreamFloatALU*)Load.Point));
		DeviseCounter++;
		break;

	case 50: //  DeviseCountOut ������ ���������� ��� � ����
		Load.Write(DeviseCounter);
		break;
	case 51: //  DeviseCountOutMk ������ ���������� ��� � ����
		MkExec(Load, {Cint,&DeviseCounter});
		break;
	case 52: //  DeviseGroupCountOut ������ ���������� ��� � ������� ������
		if (IndGroup >= Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Load.Write(Field[IndGroup].size());
		break;
	case 53: //  DeviseGroupCountOutMk ������ ���������� ��� � ������� ������
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
	case 56: //  GroupCountOutMk ������ ���������� �����
	{
		int t = Field.size();
		break;
	}
	case 70: //  DeviseOut ������ �������� �������� ���
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Load.Write((FU*)Field[IndGroup][Ind]);
		break;
	case 71: //  DeviseOutMk ������ �������� �������� ���
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{ProgExec(DevNotExistErrProg); 
		break;
		}
		MkExec(Load,{ CFU,Field[IndGroup][Ind] });
		break;
	case 80: //  LastDeviseOut ������ �������� ���������� ���������� ���
		if (!Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Load.Write((FU*) & Field.back().back());
		break;
	case 81: //  LastDeviseOutMk ������ �������� ���������� ���������� ���
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
	case 91: //  LastGroupIndOutMk ������ ������ ��������� ��������� ������ ���
		{
		int t = Field.size() - 1;
		MkExec(Load, { CFU,&t});
		}
		break;
	case 100: //  LastDeviseIndOut ������ ������ ���������� ���������� ���
		if (!Field.size() || !Field.back().size())
			Load.Write((int)-1);
		else
			Load.Write(Field.back().size()-1);
		break;
	case 101: //  LastDeviseIndOutMk ������ ������ ���������� ���������� ���
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

StreamFloatALUManager::~StreamFloatALUManager() // ����������
{
	ProgFU(0, {0,nullptr},nullptr);
	Field.clear();
}

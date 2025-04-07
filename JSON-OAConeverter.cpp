// ��������� �� JSON � OA � �������

#include "JSON-OAConeverter.h"

void JSON_OAConeverter::Recurs(LoadPoint Load, ofstream& F, string Tab)
{
	F << "\n" << Tab << "[" << Tab;
	bool FComma = false; // ���� �������
	for (auto i : *Load.IC())
	{
		if (!FComma)
			FComma = true;
		else
			F << ",";
		F << "\n" << Tab;
		F << Tab << "{";
		//ip* IPTable = AtrFind(MnemoCaps, i.atr);
		ip* IPCaps = AtrFind(MnemoCaps, i.atr);
		ip* IPTable = nullptr;
		if(IPCaps ==nullptr && MnemoTable==nullptr)
			F << "\"" << i.atr << "\":";
		else
		{
			ip* IPCaps = AtrFind(MnemoCaps, i.atr);
			if (IPCaps != nullptr)
				F << "\"" << IPCaps->Load.toStr() << "\":";
		}

		if (i.Load.Point == nullptr)
			F << "nil ";
		else if (!i.Load.isScalar())
			Recurs(i.Load, F, Tab + "    ");
		else
			if (i.Load.isStr())
				F << "\"" << i.Load.toStr() << "\"";
			else
				F << i.Load.toStr();
		F << "}";
	}
	F << "\n" << Tab << "]\n";
}
/*
void JSON_OAConeverter::Recurs(LoadPoint Load, ofstream &F, string Tab)
{
	F << "\n" << Tab << "[\n" << Tab;
	bool FComma = false; // ���� �������
	for (auto i : *Load.IC())
	{
		if (!FComma)
			FComma = true;
		else {
			F << ",\n" << Tab;
			F << "{" << " \"";
			ip* IP = AtrFind(MnemoCaps, i.atr);
			if (MnemoCaps == nullptr || IP==nullptr)
				F << i.atr;
			else
			{
					F << "\"" << IP->Load.toStr() << "\"";
			}				
			F << "\":";
		}
		if (i.Load.Point == nullptr)
			F << "nil ";
		else if (i.Load.isScalar())
		{
			if (i.Load.isStr())
				F << "\"";
			F << i.Load.toStr();
			if (i.Load.isStr())
				F << "\"";
			F << "}";
		}
		else
		{
			Recurs(i.Load, F, Tab + "    ");
			F << "}";
		}
	}
	F <<"\n" << Tab << "]\n";
}
*/

void JSON_OAConeverter::ProgFU(long int MK, LoadPoint Load, FU* Sender)
{
	MK %= FUMkRange;
	switch (MK)
	{
	case 1: //MnemoCapsSet ���������� ��������� �� �� � �����������
		if (Load.isIC())
			MnemoCaps = (IC_type)Load.Point;
		break;
	case 2: //MnemoTableSet ���������� ��������� �� ������� ����������
		if (Load.isIC())
			MnemoTable = (IC_type)Load.Point;
		break;
	case 3: //GraphSet ���������� ��������� �� ��-���� ��� �����������
		if (Load.isIC())
			Graph = Load;
		break;
	case 4: //FOutNameSet ���������� ���� ��� ������ json-������
		FOutName = Load.toStr();
		break;
	case 5: //FInNameSet ���������� ���� ��� ����� json-�����
		FInName = Load.toStr();
		break;
	case 20: //FOpenErrProgSet ���������� ��������� �� ��������� ��������� ������ �������� �����
		FOpenErrProg = (IC_type) Load.Point;
		break;
	case 10: // ToJson �������������� � JSON �� ����� ��� �����
	{
		ofstream FOut; // ���� ��� ������ ����������� ���������
		if (Load.isEmpty())
			FOut.open(FOutName);
		else
			FOut.open(Load.toStr());
		
		if (!FOut.is_open())
		{
			ProgExec(FOpenErrProg);
			break;
		}
		FOut << "{\n";
		Recurs(Graph, FOut, "");
		FOut.close();
		FOut << "\n}";
		break;
	}
	case 11: // ToOA �������������� � �� �� JSON
	{
		ifstream FIn; // ���� ��� ������ ����������� ���������
		if (Load.isEmpty())
			FIn.open(FInName);
		else
			FIn.open(Load.toStr());
		FIn.close();
		break;
	}
	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}

void JSON_OAConeverter::ToJson(LoadPoint Graph, string FileName) // �������������� �� �� � JSON
{
	ofstream FOut(FileName);
	Recurs(Graph, FOut, "");
	FOut.close();
}

LoadPoint JSON_OAConeverter::FromJSON(string FileName) // �������������� �� JSPN � ��
{
	ifstream FIn(FileName);
	FIn.close();
	return {0, nullptr};
}
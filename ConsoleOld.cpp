// �� �������
#include "stdafx.h"
#include "Console.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>

void Console::ProgFU(int MK, LoadPoint Load)
{
	MK %= FUMkRange; // �������� ������ ���� ��
	switch (MK)
	{
	case 0: // Reset
		AtrMnemo.clear();
		break;
	case 1: // Out �����
	case 2: // OutLn ����� � ������� ������
	case 3: // LnOut ������� ������ � �����
	case 4: // LnOutLn ������� ������, ����� � ����� ������� ������
		cout << prefix;
		if (MK == 3 || MK == 4) cout << endl;
		if (Load.Point != nullptr)
			Load.print(AtrMnemo,"",Sep,End,ArrayBracketStart,ArrayBracketFin);
		if (MK == 2 || MK == 4) cout << endl;
		break;
	case 10: // Ln ������� ������
		cout << endl;
		break;
	case 15: //SepSet ���������� ������-�����������
		Sep = Load.toStr();
		break;
	case 16: //EndSet ���������� ������ � ����� ������
		End = Load.toStr();
		break;
	case 17: // ArrayBracketStartSet ���������� ������, ������������ ������������� ������ ��� ����� �������
		ArrayBracketStart = Load.toStr();
		break;
	case 18: // ArrayBracketStartSet ���������� ������, ������������ ������������� ������ ��� ����� �������
		ArrayBracketFin = Load.toStr();
		break;
	case 20: // PrefixSet ���������� ������� ����� �������
		if ((Load.Type) >> 1 == Dstring)
			prefix = *(string*)Load.Point;
		break;
	case 25: // FileNameSet
		filename = Load.toStr();
		break;
	case 30: // OutFileSet ���������� ���� ��� ������
		if(Load.Point==nullptr)
			ostream& out = cout;
		else
			freopen_s(&streamOut, Load.toStr().c_str(), "w", stdout);
		break;

	case 31: // StdOutFileAppend ���������� ���� ��� ����������
		if (Load.Point == nullptr)
			ostream& out = cout;
		else
			freopen_s(&streamOut, Load.toStr().c_str(), "a", stdout);
		break;

	case 35: // StdInFileSet ���������� ���� ��� �����
		if (Load.Point == nullptr)
//			std::istream& in == cin;
			;
		else
			freopen_s(&streamIn, Load.toStr().c_str(), "r", stdin);
		break;
	case 40: // AtrMnemoAdd �������� ��������� ���������
		if (Load.Type >> 1 == DIP)
			AtrMnemo[((ip*)Load.Point)->atr] = ((ip*)Load.Point)->Load.toStr();
		else if (Load.isIC())
			for (auto& i : *(IC_type)Load.Point)
				AtrMnemo[i.atr] = i.Load.toStr();
		break;
	case 41: // AtrMnemoClear �������� ��������� ���������
		AtrMnemo.clear();
		break;

	case 44: // TrueValSet
		TrueVar.clear();
		if(Load.Point!=nullptr)
			TrueVar.push_back(Load.toStr());
		break;
	case 45: // TrueValAdd
		if (Load.Point != nullptr)
			TrueVar.push_back(Load.toStr());
		break;
	case 46: // TrueValSet
		FalseVar.clear();
		if (Load.Point != nullptr)
			FalseVar.push_back(Load.toStr());
		break;
	case 47: // TrueValAdd
		if (Load.Point != nullptr)
			FalseVar.push_back(Load.toStr());
		break;

	case 50: //VectIn ���� ������� 
		break;
	case 55: //MatrIn ���� �������	
		break;
	case 60: //VarClear �������� ����� ������� ��� ���������� �����
		VarOutBuf.clear();
		break;
	case 61: //VarSet �������� ����� ���������� ��� ������ ���������� �����
		VarOutBuf.clear();
		VarOutBuf.push_back({ -1, Load });
		break;
	case 62: //VarAdd �������� ����� ���������� ��� ������ ���������� �����
		VarOutBuf.push_back({-1, Load});
		break;
	case 80: //VarOut � ������ ����� ����������
		Load.Write(Var);
		break;
	case 85: // VarOutMk
		MkExec(Load, Var);
		break;
	case 86: // InStrOut ������ ��������� ��������� ������
		Load.Write(inStr);
		break;
	case 87: // InStrOutMk  ������ �� � ��������� ��������� �������
		MkExec(Load, {Cstring, &inStr});
		break;
	case 90: // Input ���� ������
//	case 91: // Input ���� ������ � ������ �� � ����������
	case 92: // InputMk ���� ������ � ������ �� � ����
	{
		getline(cin, inStr);
		Var.Clear(); // �������� ���������� ��������
		if (MK == 92 && Load.Point != nullptr) // ������ ��
		{
			// ����������� ��� ��������� ������
			// ����������� � �������������� ��� � ���������� � Var
			// Var={���, new �������������� ��� ������} ���� ������ Cbool, Cint, Cchar, Cdouble, Cstring, CLoadVect
			  // ��������� - ���������� ��� Var={Cbool, new bool(true)}
			MkExec(Load, Var);
			break;
		}
		// ---

		if (MK == 90 && Load.Point!=nullptr)
		{
			if (Load.isBool()) // ����� ���
			{
				// �������� �������
				// ���������� �� ������
				// �������������� ������ � ����������
				Load.WriteFromLoad(Var); // ������ ����������� ����������� ��������� ������
			}
			else if (Load.isInt()) // ����� ���
			{

			}
			else if (Load.isFloatDouble()) // ������� ���
			{
			}
			else if (Load.isChar())   //
			{
			}
			else if (Load.isStr())    //
			{
			}
			else if (Load.isVector()) //
			{
			}
			else
			{
				ProgExec(WrongFormatProg); // ��������� � ������������ ������� ������
				break;
			}
			break;
		}
		// ������ �� ������ ������� ������������� ������� � �� � VarOutBuf
		for (auto i : VarOutBuf)
		{
			// ������ ���������� ����� �� VarOutBuf
		}
		break;
	}
	case 95: // TrueFalseClear �������� ����� ������������ true � false
		False.clear();
		break;
	case 96: // TrueAdd
		True.insert(Load.toStr());
		break;
	case 97: // FalseAdd
		False.insert(Load.toStr());
		break;

	case 200: // NoVarToOutProgSet ���������� ����������� ������� �� ������ "��� ���������� ��� �����"
		NoVarToOutProg = Load.Point;
		break;
	case 201: // WrongFormatProgSet ���������� ����������� ������� �� ������ "������������ ������ ������� ������"
		WrongFormatProg = Load.Point;
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
}
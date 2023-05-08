// �� �������
#include "stdafx.h"
#include "Console.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <regex>


void Console::ProgFU(int MK, LoadPoint Load, FU* Sender)
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
			Load.print(AtrMnemo,"",Sep,End, quote, ArrayBracketStart,ArrayBracketFin);
		if (MK == 2 || MK == 4) cout << endl;
		break;
	case 5: // LoadInfoOut ������� �������� � ��������
		cout << "LoadInfo : Type " << Load.Type << " Ind " << Load.Ind;
		if (Load.Point == nullptr) cout << " Point=null";
		else cout << Load.Type % 2 ? " Const" : "Var";
		cout << endl;
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
	case 19: // QuoteSet ���������� ������ ������� ��� ������ ������ ��� �������
		quote = Load.toStr();
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
	case 90: // TrueFalseClear �������� ����� ������������ true � false
		False.clear();
		break;
	case 91: // TrueAdd
		True.insert(Load.toStr());
		break;
	case 92: // FalseAdd
		False.insert(Load.toStr());
		break;
	case 94: // InputErrProgSet ���������� ��������� ��������� ������ ������������� ������� ������� ������
		InputFormatErrProg = Load.Point;
	case 100: // Input ���� ������
	case 101: // InputMk ���� ������ � ������ �� � ����
	{
		std::cmatch results;
		regex regular_str("[\\D\\s]+");
		regex regular_int("([\\d]+)");
		regex regular_float("\\d+\\.\\d+");
		regex regular_true("[Tt][Rr][Uu][Ee]");
		regex regular_false("[Ff][Aa][Ll][Ss][Ee]");
		regex regular_vector("^\\[(\\d+,?\\s?)+\\]$"); //������� ������
		regex regular_matrix("^\\[(\\d+[,;]?\\s?)+\\]$"); //������� �������
		regex regular_char("\\w{1}");





		getline(cin, inStr);
		Var.Clear(); // �������� ���������� ��������
	//	if (MK == 101 && Load.Point != nullptr) // ������ ��
		{
			if (std::regex_match(inStr.c_str(), regular_float)) {


				//cout << "its float";
				double res = stof(inStr);
				Var = { Cdouble,new double(res) };
			}
			else  if (std::regex_match(inStr.c_str(), regular_int)) {

				//cout << "its int";
				int res = stoi(inStr);
				Var = { Cint,new int(res) };
			}
			else if (std::regex_match(inStr.c_str(), regular_true)) {

				Var = { Cbool, new bool(true) };

			}
			else if (std::regex_match(inStr.c_str(), regular_false)) {

				Var = { Cbool, new bool(false) };

			}
			else if (std::regex_match(inStr.c_str(), regular_char)) {

				//cout << "its char";
				char res;
				res = inStr[0];
				Var = { Cchar,new char(res) };
			}
			else if (std::regex_match(inStr.c_str(), regular_str)) {

				///cout << "its string";
				Var = { Cstring,new string(inStr) };
			}
			else if (std::regex_match(inStr.c_str(), regular_vector)) {

				int res;
				int counter = -1;
				int* intbufArray = new int[256];
				vector<LoadPoint> Ar;
				int i = 0;
				for (sregex_iterator it = sregex_iterator(inStr.begin(), inStr.end(), regular_int);
					it != sregex_iterator(); it++) {
					smatch match = *it;
					counter++;
					res = stoi(match.str(0));
					intbufArray[i] = res;
					i++;
				}
				int* intArray = new int[counter];

				for (int i = 0; i < counter; i++) {
					intArray[i] = intbufArray[i];
				}
				delete[] intbufArray;
				//delete[] intArray;

				Var = { CLoadVect, new int* (intArray) };
			}
			// ����������� �������
			/*
			else if (std::regex_match(inStr.c_str(), regular_vector)) {
				int res1;
				float res2;
				std::vector <LoadPoint> Ar;
				for (sregex_iterator it = sregex_iterator(inStr.begin(), inStr.end(), regular_number);
					it != sregex_iterator(); it++) {
					smatch match = *it;
					if (std::regex_match(match.str(0).c_str(), regular_float)) {
						res2 = stof(match.str(0));
						Ar.push_back(res2);
					}
					else if (std::regex_match(match.str(0).c_str(), regular_int)) {
						res1 = stoi(match.str(0));
						Ar.push_back(res1);
					}
				}

			}
			*/
			else if (std::regex_match(inStr.c_str(), regular_matrix)) {

				cout << "its matrix";

			}

			// ����������� ��� ��������� ������
			// ����������� � �������������� ��� � ���������� � Var
			// Var={���, new �������������� ��� ������} ���� ������ Cbool, Cint, Cchar, Cdouble, Cstring, CLoadVect
			  // ��������� - ���������� ��� Var={Cbool, new bool(true)}
			if (MK == 101) // ������ ��
				MkExec(Load, Var);
			else // �������� � ����������
				Load.WriteFromLoad(Var);
			break;
		}
		// ������ �� ������ ������� ������������� ������� � �� � VarOutBuf
//		for (auto& i : VarOutBuf)
//		{
//			// ������ ���������� ����� �� VarOutBuf
//		}
		break;
	}
	case 105: //InputBool ���� �������� ��������
	case 106: //InputBoolMk ���� �������� �������� � ������ �� � ���
	//	if(������������ ������)
		if(false)
			ProgExec(InputFormatErrProg);
		else
			if (MK == 106) // ������ ��
				MkExec(Load, Var);
			else // �������� � ����������
				Load.WriteFromLoad(Var);
		break;
	case 110: //InputInt
	case 111: //InputIntMk
		break;
// ......

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
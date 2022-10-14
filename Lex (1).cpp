#include "stdafx.h"
#include "Lex.h"
#include <string.h>

using namespace std;

void Lex::LexOut(bool Copy, int MK) // ������ ������� �����������
{
	auto uk = UnicAtr.find(LexBuf[ib].atr);
	if (MK < 0)MK = ReceiverMK;
	if (uk != UnicAtr.end())
	{
		if (uk->second.Fu != nullptr && uk->second.Mk != 0)
			if (Copy)
				uk->second.Fu->ProgFU(uk->second.Mk, { TIP, &LexBuf[ib] });
			else
				uk->second.Fu->ProgFU(uk->second.Mk, { TIP, LexBuf[ib].�lone() });
	}
	else
		if (Receiver != nullptr)
			if (Copy)
				Receiver->ProgFU(MK, { TIP, LexBuf[ib].�lone() });
			else
				Receiver->ProgFU(MK, { TIP, &LexBuf[ib] });
}

void Lex::ProgFU(int MK, LoadPoint Load)
{
	// �������� ����� �� � ���������
	switch (MK)
	{
	case 0: // Reset
		S = 0; // ����� ��������� ������������� ��������
			   //		FigureBuf = "";
		UnicAtr.clear();
		ReceiverMK = 0;
		Receiver = Bus;
		ErrProg = nullptr;
		break;
	case 5: //ReceiverMKSet
		if (Load.Type >> 1 == Dint) ReceiverMK = *(int*)Load.Point; break;
	case 10: //ErrProgSet
		ErrProg = (vector<ip> *)Load.Point;
		break;
	case 12: // StartProgSet ���������� ���������, ����������� ����� ������� ���������� 
		StartProg = (IC_type)Load.Point;
		break;
	case 13: // StopProgSet ���������� ���������, ����������� ��� ��������� ���������� ������������ ������� �� �� Stop 
		StopProg = (IC_type)Load.Point;
		break;
	case 14: // FinProgSet  ���������� ���������, ����������� ����� ��������� ������� ������
		FinProg = (IC_type)Load.Point;
		break;
	case 15:// UnicAtrSet ���������� ���������� �������
		UnicAtr[*(int*)Load.Point] = { 0,Bus };
		break;
	case 16:// UnicMkSet ���������� �� ��� ����������� ��������
		if (UnicAtr.size())
			(--UnicAtr.end())->second.Mk = *(int*)Load.Point;
		break;
	case 17:// UnicFuSet ���������� �������� ��� ����������� ��������
		if (UnicAtr.size())
			(--UnicAtr.end())->second.Fu = (FU*)Load.Point;
		break;
	case 18:// UnicReset �������� ������ ���������� ���������
		UnicAtr.clear();
		break;
	case 20: // LexBufSizeSet ���������� ������ ������ ������
		if (Load.Type >> 1 != Dint) break;
		for (int i = 0; i < SizeBuf; LexBuf[i++].Load.Clear());
		delete[] LexBuf;
		SizeBuf = *(int*)Load.Point;
		LexBuf = new ip[SizeBuf];
		for (int i = 0; i < SizeBuf; LexBuf[i++].Load = { 0,nullptr });
		break;
	case 26: // NoUnucToReseiver ������ �������, �� �������� ���������� ��������� (��� ������� �������� �������� ������� �������)
		if (Load.Point == nullptr)
			MkExec(ReceiverMK, LexBuf[ib].Load, Receiver);
		else
		{
			ib = (ib + 1) % SizeBuf;
			LexBuf[ib].Load.Clear(); // ������� �������� ��
			LexBuf[ib].copy(Load);
			MkExec(Load, LexBuf[ib].Load, Receiver);
		}
		break;
	case 27: //Replace �������� ������� ������� � ������
	case 28: //ReplaceToReceiver ������ ������� �� �������� �� ����������, ������� ������� ������� � ������
	case 29: //ReplaceCopyToReceiver ������ ����� ������� �� �������� �� ����������, ������� ������� ������� � ������
	case 30: //ToReceiver ������ ������� �� �������� �� ���������� (���� �������� nil, �� �������� ������� �������)
	case 31: //CopyToReceiver ������ ����� ������� �� �������� �� ����������
		if (MK == 30 && Load.Point == nullptr) {
			LexOut(false);
			break;
		}
		if (MK == 30 || MK == 31) ib = (ib + 1) % SizeBuf;
		LexBuf[ib].Load.Clear(); // ������� �������� ��
		LexBuf[ib].copy(Load);
		if (MK != 27)LexOut(MK == 31);
		break;
		//		case 32: //LastLexemaToReceiver ������ ��������� ������� ����������
		//		case 33: //LastLexemaCopyToReceiver ������ ��������� ������� ����������
		//			if (Receiver != nullptr)
		//				if (MK==33)
		//					Receiver->ProgFU(ReceiverMK, { TIP, LexBuf[ib].�lone() });
		//				else
		//					Receiver->ProgFU(ReceiverMK, { TIP, &LexBuf[ib] });
		//	//		LexOut(MK==33);
		//			break;
	case 35: //OutMk ������ MK c ��������� �������� (���� nil � ��������, �� �������� �� Receiver)
		if (Load.Point == nullptr)
		{
			//LexOut(false);
			Receiver->ProgFU(ReceiverMK, { TIP, &LexBuf[ib] });
		}
		else

			//			if (Load.Type >> 1 == Dint)
			//				LexOut(false,Load.ToInt())
			if (Load.Type >> 1 == Dint)
				Receiver->ProgFU(Load.ToInt(), { TIP, &LexBuf[ib] });
		//	else if(Load.Point==0)

		break;
	case 36: // CopyOutMk ������ �� � ������ ��������� ������� (���� nil � ��������, �� �������� �� Receiver)
		if (Load.Point != nullptr)
			Receiver->ProgFU(*(int*)Load.Point, { TIP, LexBuf[ib].�lone() });
		else
			Receiver->ProgFU(ReceiverMK, { TIP, LexBuf[ib].�lone() });
		break;
	case 37: // LoadOut ������ �������� ������� �������
		Load.Write(LexBuf[ib].Load);
		break;
	case 38: // LoadOutMk ������ �� � ��������� ������� �������
		MkExec(Load, LexBuf[ib].Load);
		break;
	case 39: // LoadCopyOut ������ �������� ������� �������
		Load.Write(LexBuf[ib].Load.Clone());
		break;
	case 40: // LoadCopyOutMk ������ �� � ������ �������� ������� �������
		MkExec(Load, LexBuf[ib].Load.Clone());
		break;
	case 41: // AtrOut ������ ������� ������� �������
		Load.Write(LexBuf[ib].atr);
		break;
	case 42: // AtrOutMk ������ �� � ��������� ������� �������
		MkExec(Load, { Cint, &LexBuf[ib].atr });
		break;
		//		case 40: //PrevToReseiver ������ ���������� ������� ����������
//			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
//				Receiver->ProgFU(*(int*)Load.Point, { TIP, &LexBuf[(ib-1+ SizeBuf)% SizeBuf] });
//			break;
//		case 41: //PrevCopyToReseiver ������ ����� ���������� ������� ����������
//			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
//				Receiver->ProgFU(*(int*)Load.Point, { TIP, LexBuf[(ib - 1 + SizeBuf) % SizeBuf].�lone() });
//			break;
	case 45: //PrevOutMk ������ �� � ���������� �������� (���� �������� nil, �� �������� �� Receiver)
		if (Load.Point == nullptr)
			Receiver->ProgFU(ReceiverMK, { TIP, &LexBuf[(ib - 1 + SizeBuf) % SizeBuf] });
		else
			Receiver->ProgFU(*(int*)Load.Point, { TIP, &LexBuf[(ib - 1 + SizeBuf) % SizeBuf] });
		break;
	case 46: //PrevCopyOutMk ������ �� � ������ ���������� ������� (���� �������� nil, �� �������� �� Receiver)
		if (Load.Point == nullptr)
			Receiver->ProgFU(ReceiverMK, { TIP, LexBuf[(ib - 1 + SizeBuf) % SizeBuf].�lone() });
		else
			Receiver->ProgFU(*(int*)Load.Point, { TIP, LexBuf[(ib - 1 + SizeBuf) % SizeBuf].�lone() });
		break;
	case 50: // AtrSet ���������� ������� ��������� �������
		LexBuf[ib].atr = Load.ToInt();
		break;
	case 51: // PrevAtrSet ���������� ������� ���������� �������
		LexBuf[(ib - 1 + SizeBuf) % SizeBuf].atr = Load.ToInt();
		break;
	case 55: // LoadSet ���������� ��������� � ��������� �������
		LexBuf[ib].Load = Load;
		break;
	case 56: // PrevLoadSet ���������� ��������� � ���������� �������
		LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load = Load;
		break;
	case 57: // LoadCopySet ���������� ����� ��������� � ��������� �������
		LexBuf[ib].Load.Copy(&Load);
		break;
	case 58: // PrevLoadCopySet ���������� ����� ��������� � ���������� �������
		LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load.Copy(&Load);
		break;
	case 65: // VarSet ���������� ��� ���������� ��� �������� ��������� �������
		LexBuf[ib].Load.Type |= 1;
		LexBuf[ib].Load.Type--;
		break;
	case 66: // PrevVarSet ���������� ��� ���������� ��� �������� ���������� �������
		LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load.Type |= 1;
		LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load.Type--;
		break;

	case 70: // CendToReceiver ��������� ������� �� �������� ����������
		MkExec(ReceiverMK, Load);
		break;
	case 71: // CendCopyToReceiver ��������� ����� ������� �� �������� ����������
		MkExec(ReceiverMK, Load.Clone());
		break;

	case 99: // Stop ���������� ����������� ������
		Work = false;
		ProgExec((IC_type)StopProg, Bus, nullptr); // ��������� ��������� �� �������� ������������ �������
		break;
	case 100: // Lexing
	{
		//			ProgExec((IC_type)StartProg,Bus);
		string FigureBuf;
		string str = *(string*)Load.Point;
		S = 0; // --- ��������� ���������� ��������� ��������
		if (Load.Type >> 1 == Dstring && *((string*)Load.Point) == "")
		{
			ib = (ib + 1) % SizeBuf;
			LexBuf[ib].Load.Clear(); // ������� �������� ��
			LexBuf[ib] = { StrAtr,Tstring , new string("") };
			LexOut();
		}
		str += " "; // ���������� ������� ��������� ��������
		Work = true;
		for (auto i = str.begin(); i != str.end() && Work; i++)
			switch (S) //LEXER
			{
				// ��������� ���������
			case 0:
			{
				if (*i == ' ') //�����������; 0 -> 0
				{
					break;
				}
				if (*i == '"') //�������; 0 -> 4
				{
					FigureBuf = ""; //�������� �������
					S = 4; //������� � 4 ���������
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				/*
				auto SepUk = Seps.find(str.substr(distance(str.begin(), i), 1));
				auto SepUk2 = Seps.find(str.substr(distance(str.begin(), i), 2));
				auto SepUk3 = Seps.find(str.substr(distance(str.begin(), i), 3));
				if (SepUk != Seps.end() || SepUk2 != Seps.end() || SepUk3 != Seps.end())
				{
					ib = (ib + 1) % SizeBuf;
					LexBuf[ib].Load.Clear();
					string* tstr=new string;
					if (SepUk3 != Seps.end() && SepUk3->size()==3)
					{
						*tstr = *SepUk3;
						i += 2;
					}
					else if (SepUk2 != Seps.end() && SepUk2->size() == 2)
					{
						*tstr = *SepUk2;
						i += 1;
					}
					else if(SepUk != Seps.end())
						*tstr = *SepUk;

					LexBuf[ib] = { SeperatAtr,Tstring , tstr };
					LexOut();
					break;
				}
				else
				*/
				if ((DigitDEC.count(*i)) && (*i != '0')) // --- ����� (1..9); 0 -> 1
				{
					FigureBuf = *i; //������ � �������� ����������
					S = 1; //������� � ��������� 1
					Debug(*i, S, FigureBuf); // --- �������
					//TO DO numberFormat = DEC (���������� ��������������� ��������)
					break;
				}
				if (ABC.count(*i)) //�����; 0 -> 3
				{
					FigureBuf = *i; //������ � �������� ����������
					S = 3; //������� � ��������� 3
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '0') // ����� (0); 0 -> 9
				{
					FigureBuf = *i; //������ � �������� ����������
					S = 9; //������� � ��������� 9
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (DigitSeps.count(*i))  // --- ������ ���������� ������� � ����� ����� (. ,); 0 -> 2
				{
					FigureBuf = '0'; // --- ���������� ������� 0 � �������� ����������, ��� ��� .5 = 0.5; ,5 = 0,5
					FigureBuf += "."; //���������� ������� � �������� ����������
					//TO DO numberFormat = DEC (���������� ��������������� ��������)
					S = 2; //������� � ��������� 2
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '+' || *i == '-') // ������ (+,-); 0 -> 10
				{
					FigureBuf = *i; //������ � �������� ����������
					S = 10; //������� � ��������� 10
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '/') // ������ (/); 0 -> 5
				{
					FigureBuf = *i; //������ � �������� ����������
					S = 5; //������� � ��������� 5
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '\n') //������� �� ����� ������; 0 -> 12
				{
					FigureBuf = *i; //������ � �������� ����������
					// TO DO tabCounter = 0 (���������� ��������������� ��������)
					S = 12; //������� � ��������� 12
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				Work = false; //��������� ����� �������� ������ ������� �� false
				if (ErrProg != nullptr) ProgExec(ErrProg, Bus, nullptr); //��������� ������
				break;
			}
			// ��������� ����� ����� ���������� �����
			case 1:
				if (DigitDEC.count(*i)) // --- ����� (0..9); 1 -> 1
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '_') //������ ���������� ����� (_); 1 -> 1
				{
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (DigitSeps.count(*i))  // --- ������ ���������� ������� � ����� ����� (. ,); 1 -> 2
				{
					FigureBuf += "."; //���������� ������� � �������� ����������
					S = 2; //������� � ��������� 2
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //�����������; 1 -> 0
				{
					i--; // ��� ��������� ����������
					int* tint = new int;
					*tint = atoi(FigureBuf.c_str()); //������ ������� � ����������
					ib = (ib + 1) % SizeBuf; //���������� �������� ������ ������ �������� ������ �� 1
					LexBuf[ib].Load.Clear(); //�������� �������� ��
					LexBuf[ib] = { IntAtr,Tint , tint }; //���������� ������� � ����� �������� ������ � ���� �� {�������, ���, ���������}
					S = 0; //������� � ��������� 0

					Debug(*i, S, FigureBuf); // --- �������
					std::cout << "LexBuf[" << ib << "] = {IntAtr, Tint, " << *tint << "}\n"; // --- �������
					
					break;
				}
				Work = false; //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				// ��������� ������� ����� ���������� �����
			case 2:
				if (DigitDEC.count(*i))  // --- ����� (0..9); 2 -> 2
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '_') //������ ���������� ����� (_); 2 -> 2
				{
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //�����������; 2 -> 0
				{
					i--; // ��� ��������� ����������
					double* ft = new double;
					*ft = atof(FigureBuf.c_str()); //������ ������� � ����������
					ib = (ib + 1) % SizeBuf; //���������� �������� ������ ������ �������� ������ �� 1
					LexBuf[ib].Load.Clear(); //�������� �������� ��
					LexBuf[ib] = { DoubleAtr, Tdouble, ft }; //���������� ������� � ����� �������� ������ � ���� �� {�������, ���, ���������}
					LexOut();
					S = 0; //������� � ��������� 0

					Debug(*i, S, FigureBuf); // --- �������
					std::cout << "LexBuf[" << ib << "] = {DoubleAtr, Tdouble, " << *ft << "}\n"; // --- �������
					
					break;
				}
				Work = false;  //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				//��������� ���������
			case 3:
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //�����������; 3 -> 0
				{
					i--; // ��� ��������� ����������
					string* st = new string;
					*st = FigureBuf; //������ ������� � ����������
					ib = (ib + 1) % SizeBuf; //���������� �������� ������ ������ �������� ������ �� 1
					LexBuf[ib].Load.Clear(); //�������� �������� ��
					LexBuf[ib] = { MnemoAtr, Tstring, st };  //���������� ������� � ����� �������� ������ � ���� �� {�������, ���, ���������}
					LexOut();
					S = 0; //������� � ��������� 0

					Debug(*i, S, FigureBuf); // --- �������
					std::cout << "LexBuf[" << ib << "] = {MnemoAtr, Tstring, " << *st << "}\n"; // --- �������

					break;
				}
				if (ABC.count(*i) || DigitDEC.count(*i)) // --- ����� ���� �����; 3 -> 3
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				Work = false;  //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				//��������� �������
			case 4:
				if (*i != '"' && *i != '\\') //����� ������ ����� ������� � \; 4 -> 4
				{
					FigureBuf += *i;  //���������� ������� � �������� ����������
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				else
					if (*i == '\\')//������ (\); 4 -> 8
					{
						S = 8; //������� � ��������� 8
						Debug(*i, S, FigureBuf); // --- �������
						break;
					}
					else
						if (*i == '"')//�������; 4 -> 0
						{
							string* st2 = new string;
							*st2 = FigureBuf; //������ ������� � ����������
							ib = (ib + 1) % SizeBuf; //���������� �������� ������ ������ �������� ������ �� 1
							LexBuf[ib].Load.Clear(); //�������� �������� ��
							LexBuf[ib] = { StrAtr, Cstring, st2 }; //���������� ������� � ����� �������� ������ � ���� �� {�������, ���, ���������}
							LexOut();
							S = 0; //������� � ��������� 0

							Debug(*i, S, FigureBuf); // --- �������
							std::cout << "LexBuf[" << ib << "] = {StrAtr, Cstring, " << *st2 << "}\n"; // --- �������
							
							break;
						}
				Work = false;  //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				//��������� ��������� �����������
			case 5:
				if (*i == '/') //������ (/); 5 -> 11
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					S = 11; //������� � ��������� 11
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '*') //������ (*); 5 -> 6
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					S = 6; //������� � ��������� 6
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				Work = false;  //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				//��������� �������������� �����������
			case 6:
				if (*i != '*') //����� ������ ����� (/); 6 -> 6
				{
					FigureBuf += *i;  //���������� ������� � �������� ����������
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '*') //������ (/); 6 -> 7
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					S = 7; //������� � ��������� 7
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				Work = false;  //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				//��������� �������������� �����������
			case 7:
				if (*i != '/') //����� ������ ����� (*); 7 -> 6
				{
					FigureBuf += *i;  //���������� ������� � �������� ����������
					S = 6; //������� � ��������� 6
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '/') //������ (*); 7 -> 0
				{
					string* multiLineComment = new string;
					FigureBuf.erase(0,2); //�������� ������� �������������� ����������� � ������
					FigureBuf.pop_back(); //�������� ������� �������������� ����������� � �����
					*multiLineComment = FigureBuf; //������ ������� � ����������
					ib = (ib + 1) % SizeBuf; //���������� �������� ������ ������ �������� ������ �� 1
					LexBuf[ib].Load.Clear(); //�������� �������� ��
					LexBuf[ib] = { StrAtr, Cstring, multiLineComment }; //���������� ������� � ����� �������� ������ � ���� �� {�������, ���, ���������}
					LexOut();
					S = 0; //������� � ��������� 0

					Debug(*i, S, FigureBuf); // --- �������
					std::cout << "LexBuf[" << ib << "] = {StrAtr, Cstring, " << *multiLineComment << "}\n"; // --- �������
					
					break;
				}
				Work = false;  //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				//��������� �������������
			case 8:
				if (*i == '"' || *i == 'n' || *i == '\'') //������� (", n, '); 8 -> 4
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					//TO DO ��������� ������������� (���������� ��������������� ��������)
					S = 4; //������� � ��������� 4
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				else //����� ������ ����� (n, ", '); 8 -> 4
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					S = 4; //������� � ��������� 4
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				Work = false;  //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				//������������ ������� ���������
			case 9:
				if (*i == '_') //������ ���������� ����� (_); 9 -> 9
				{
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (DigitDEC.count(*i)) // --- ����� (0..9); 9 -> 1
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					//TO DO numberFormat = DEC (���������� ��������������� ��������)
					S = 1; //������� � ��������� 1
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == 'b') //������ (b); 9 -> 13
				{
					//FigureBuf += *i; //���������� ������� � �������� ���������� �� �����, ��� ��� b �������� ������������ BIN �����
					//TO DO numberFormat = BIN (���������� ��������������� ��������)
					S = 13; //������� � ��������� 13
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == 'o') //������ (o); 9 -> 15
				{
					//FigureBuf += *i; //���������� ������� � �������� ���������� �� �����, ��� ��� o �������� ������������ OCT �����
					//TO DO numberFormat = OCT (���������� ��������������� ��������)
					S = 15; //������� � ��������� 15
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == 'x') //������ (x); 9 -> 17
				{
					//FigureBuf += *i; //���������� ������� � �������� ���������� �� �����, ��� ��� x �������� ������������ HEX �����
					//TO DO numberFormat = HEX (���������� ��������������� ��������)
					S = 17; //������� � ��������� 17
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (DigitSeps.count(*i))  // --- ������ ���������� ������� � ����� ����� (. ,); 9 -> 2
				{
					FigureBuf += "."; //���������� ������� � �������� ����������
					//TO DO numberFormat = DEC (���������� ��������������� ��������)
					S = 2; //������� � ��������� 2
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				Work = false;  //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				//��������� ���
			case 10:
				if ((DigitDEC.count(*i)) && (*i != '0')) // --- ����� (1..9); 10 -> 1
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					//TO DO numberFormat = DEC (���������� ��������������� ��������)
					S = 1; //������� � ��������� 1
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '0') //����� 0; 10 -> 9
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					S = 9; //������� � ��������� 9
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //�����������; 10 -> 0
				{
					i--; // ��� ��������� ����������
					string* sep = new string;
					*sep = FigureBuf; //������ ������� � ����������
					ib = (ib + 1) % SizeBuf; //���������� �������� ������ ������ �������� ������ �� 1
					LexBuf[ib].Load.Clear(); //�������� �������� ��
					LexBuf[ib] = { SeperatAtr, Tstring, sep };  //���������� ������� � ����� �������� ������ � ���� �� {�������, ���, ���������}
					LexOut();
					S = 0; //������� � ��������� 0

					Debug(*i, S, FigureBuf); // --- �������
					std::cout << "LexBuf[" << ib << "] = {SeperatAtr, Tstring, " << *sep << "}\n"; // --- �������
					
					break;
				}
				Work = false;  //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				//��������� ��������� �����������
			case 11:
				if (*i != '\n') //����� ������ ����� (\n); 11 -> 11
				{
					FigureBuf += *i;  //���������� ������� � �������� ����������
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '\n') //������ (\n); 11 -> 0
				{
					string* multiLineComment = new string;
					FigureBuf.erase(0, 2); //�������� ������� �������������� ����������� � ������
					*multiLineComment = FigureBuf; //������ ������� � ����������
					ib = (ib + 1) % SizeBuf; //���������� �������� ������ ������ �������� ������ �� 1
					LexBuf[ib].Load.Clear(); //�������� �������� ��
					LexBuf[ib] = { StrAtr, Cstring, multiLineComment }; //���������� ������� � ����� �������� ������ � ���� �� {�������, ���, ���������}
					LexOut();
					S = 0; //������� � ��������� 0

					Debug(*i, S, FigureBuf); // --- �������
					std::cout << "LexBuf[" << ib << "] = {StrAtr, Cstring, " << *multiLineComment << "}\n"; // --- �������

					break;
				}
				Work = false; //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				//��������� ���������
			case 12:
				if (*i != '\n')//����� ������ ����� (\n); 12 -> 0
				{
					/*
					TO DO
					If (bracketAmount > 0)
					������ ��������� �� ���-�� { ������ bracketAmount

					If (bracketAmount < 0)
					������ ��������� �� ���-�� } ������ bracketAmount
					*/
					i--; // ��� ��������� ����������
					string* st3 = new string;
					*st3 = FigureBuf; //������ ������� � ����������
					ib = (ib + 1) % SizeBuf; //���������� �������� ������ ������ �������� ������ �� 1
					LexBuf[ib].Load.Clear(); //�������� �������� ��
					LexBuf[ib] = { StrAtr, Cstring, st3 }; //���������� ������� � ����� �������� ������ � ���� �� {�������, ���, ���������}
					LexOut();
					S = 0; //������� � ��������� 0

					Debug(*i, S, FigureBuf); // --- �������
					std::cout << "LexBuf[" << ib << "] = {IntAtr, Tint, " << *st3 << "}\n"; // --- �������
					
					break;
				}
				if (*i == '\n') //������ (\n); 12 -> 12
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					// TO DO tabCounter = 0 (���������� ��������������� ��������)
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '\t') //������ (\n); 12 -> 12
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					// TO DO tabCounter++ (���������� ��������������� ��������)
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				Work = false; //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				// ��������� ����� ����� �������� �����
			case 13:
				if (DigitBIN.count(*i)) // --- ����� (0..1); 13 -> 13
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '_') //������ ���������� ����� (_); 13 -> 13
				{
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (DigitSeps.count(*i))  // --- ������ ���������� ������� � ����� ����� (. ,); 13 -> 14
				{
					FigureBuf += "."; //���������� ������� � �������� ����������
					S = 2; //������� � ��������� 2
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //�����������; 13 -> 0
				{
					i--; // ��� ��������� ����������
					int* tint = new int;
					*tint = atoi(FigureBuf.c_str()); //������ ������� � ����������
					ib = (ib + 1) % SizeBuf; //���������� �������� ������ ������ �������� ������ �� 1
					LexBuf[ib].Load.Clear(); //�������� �������� ��
					LexBuf[ib] = {IntAtr, TBinInt , tint}; //���������� ������� � ����� �������� ������ � ���� �� {�������, ���, ���������}
					S = 0; //������� � ��������� 0

					Debug(*i, S, FigureBuf); // --- �������
					std::cout << "LexBuf[" << ib << "] = {IntAtr, TBinInt, " << *tint << "}\n"; // --- �������
					
					break;
				}
				Work = false; //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				// ��������� ������� ����� �������� �����
			case 14:
				if (DigitBIN.count(*i))  // --- ����� (0..1); 14 -> 14
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '_') //������ ���������� ����� (_); 14 -> 14
				{
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //�����������; 14 -> 0
				{
					i--; // ��� ��������� ����������
					double* ft = new double;
					*ft = atof(FigureBuf.c_str()); //������ ������� � ����������
					ib = (ib + 1) % SizeBuf; //���������� �������� ������ ������ �������� ������ �� 1
					LexBuf[ib].Load.Clear(); //�������� �������� ��
					LexBuf[ib] = {DoubleAtr, TBinDouble, ft}; //���������� ������� � ����� �������� ������ � ���� �� {�������, ���, ���������}
					LexOut();
					S = 0; //������� � ��������� 0

					Debug(*i, S, FigureBuf); // --- �������
					std::cout << "LexBuf[" << ib << "] = {DoubleAtr, TBinDouble, " << *ft << "}\n"; // --- �������
					
					break;
				}
				Work = false;  //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				// ��������� ����� ����� ������������ �����
			case 15:
				if (DigitOCT.count(*i)) // --- ����� (0..7); 15 -> 15
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '_') //������ ���������� ����� (_); 15 -> 15
				{
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (DigitSeps.count(*i))  // --- ������ ���������� ������� � ����� ����� (. ,); 15 -> 16
				{
					FigureBuf += "."; //���������� ������� � �������� ����������
					S = 2; //������� � ��������� 2
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //�����������; 15 -> 0
				{
					i--; // ��� ��������� ����������
					int* tint = new int;
					*tint = atoi(FigureBuf.c_str()); //������ ������� � ����������
					ib = (ib + 1) % SizeBuf; //���������� �������� ������ ������ �������� ������ �� 1
					LexBuf[ib].Load.Clear(); //�������� �������� ��
					LexBuf[ib] = {IntAtr, TOctInt, tint }; //���������� ������� � ����� �������� ������ � ���� �� {�������, ���, ���������}
					S = 0; //������� � ��������� 0

					Debug(*i, S, FigureBuf); // --- �������
					std::cout << "LexBuf[" << ib << "] = {IntAtr, TOctInt, " << *tint << "}\n"; // --- �������
					
					break;
				}
				Work = false; //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				// ��������� ������� ����� ������������ �����
			case 16:
				if (DigitOCT.count(*i))  // --- ����� (0..7); 16 -> 16
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '_') //������ ���������� ����� (_); 16 -> 16
				{
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //�����������; 16 -> 0
				{
					i--; // ��� ��������� ����������
					double* ft = new double;
					*ft = atof(FigureBuf.c_str()); //������ ������� � ����������
					ib = (ib + 1) % SizeBuf; //���������� �������� ������ ������ �������� ������ �� 1
					LexBuf[ib].Load.Clear(); //�������� �������� ��
					LexBuf[ib] = {DoubleAtr, TOctDouble, ft }; //���������� ������� � ����� �������� ������ � ���� �� {�������, ���, ���������}
					LexOut();
					S = 0; //������� � ��������� 0

					Debug(*i, S, FigureBuf); // --- �������
					std::cout << "LexBuf[" << ib << "] = {DoubleAtr, TOctDouble, " << *ft << "}\n"; // --- �������
					
					break;
				}
				Work = false;  //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				// ��������� ����� ����� ����������������� �����
			case 17:
				if (DigitHEX.count(*i)) // --- ����� (0..9, A..F, a..f); 17 -> 17
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '_') //������ ���������� ����� (_); 17 -> 17
				{
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (DigitSeps.count(*i))  // --- ������ ���������� ������� � ����� ����� (. ,); 17 -> 2
				{
					FigureBuf += "."; //���������� ������� � �������� ����������
					S = 2; //������� � ��������� 2
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //�����������; 17 -> 0
				{
					i--; // ��� ��������� ����������
					string* st2 = new string;
					*st2 = FigureBuf; //������ ������� � ����������
					ib = (ib + 1) % SizeBuf; //���������� �������� ������ ������ �������� ������ �� 1
					LexBuf[ib].Load.Clear(); //�������� �������� ��
					LexBuf[ib] = {IntAtr, THexInt, st2 }; //���������� ������� � ����� �������� ������ � ���� �� {�������, ���, ���������}
					S = 0; //������� � ��������� 0

					Debug(*i, S, FigureBuf); // --- �������
					std::cout << "LexBuf[" << ib << "] = {IntAtr, THexInt, " << *st2 << "}\n"; // --- �������
					
					break;
				}
				Work = false; //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
				// ��������� ������� ����� ����������������� �����
			case 18:
				if (DigitHEX.count(*i))  // --- ����� (0..9, A..F, a..f); 18 -> 18
				{
					FigureBuf += *i; //���������� ������� � �������� ����������
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (*i == '_') //������ ���������� ����� (_); 18 -> 18
				{
					Debug(*i, S, FigureBuf); // --- �������
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //�����������; 18 -> 0
				{
					i--; // ��� ��������� ����������
					string* st2 = new string;
					*st2 = FigureBuf; //������ ������� � ����������
					ib = (ib + 1) % SizeBuf; //���������� �������� ������ ������ �������� ������ �� 1
					LexBuf[ib].Load.Clear(); //�������� �������� ��
					LexBuf[ib] = {DoubleAtr, THexDouble, st2 }; //���������� ������� � ����� �������� ������ � ���� �� {�������, ���, ���������}
					S = 0; //������� � ��������� 0

					Debug(*i, S, FigureBuf); // --- �������
					std::cout << "LexBuf[" << ib << "] = {DoubleAtr, THexDouble, " << *st2 << "}\n"; // --- �������

					break;
				}
				Work = false;  //��������� ����� �������� ������ ������� �� false
				ProgExec(ErrProg, Bus, nullptr); //���������� ������
				break;
			default:
				break;
			}
	}
	ProgExec(FinProg); // ��������� 
	break;
	default:
		CommonMk(MK, Load);
	}
}

void Lex::Debug(char i, int S, string FigureBuf) // --- ��� �������, ����� �������
{
	cout << " Current symbol: " << i << "; "; // --- �������
	cout << "New condition: " << S << "; "; // --- �������
	cout << " FigureBuf: " << FigureBuf << endl; // --- �������
}

Lex::Lex(FU* BusContext, FU* Templ)
{
	Bus = BusContext;
	Receiver = BusContext;
	copy(ABC_templ.begin(), ABC_templ.end(), inserter(ABC, ABC.end()));

	copy(Digit_DEC_templ.begin(), Digit_DEC_templ.end(), inserter(DigitDEC, DigitDEC.end())); // --- ���������� � ��������� 10-���� �����
	copy(Digit_BIN_templ.begin(), Digit_BIN_templ.end(), inserter(DigitBIN, DigitBIN.end())); // --- ���������� � ��������� 2-���� �����
	copy(Digit_OCT_templ.begin(), Digit_OCT_templ.end(), inserter(DigitOCT, DigitOCT.end())); // --- ���������� � ��������� 8-������ �����
	copy(Digit_HEX_templ.begin(), Digit_HEX_templ.end(), inserter(DigitHEX, DigitHEX.end())); // --- ���������� � ���������  16-������ �����

	copy(Digit_seps_templ.begin(), Digit_seps_templ.end(), inserter(DigitSeps, DigitSeps.end())); // --- ���������� � ��������� ������������ ������� � ����� ������ �����
	copy(Seps_templ.begin(), Seps_templ.end(), inserter(Seps, Seps.end()));
	LexBuf = new ip[SizeBuf];
	for (int i = 0; i < SizeBuf; LexBuf[i++].Load = { 0,nullptr });
	ProgFU(0, { 0,nullptr });
}

Lex::Lex() : FU()
{
	Lex(nullptr, nullptr);
}

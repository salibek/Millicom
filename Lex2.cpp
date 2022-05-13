	#include "stdafx.h"
	#include "Lex.h"
	#include <string.h>

	using namespace std;

	void Lex::LexOut(bool Copy,int MK) // Выдача лексемы потребителю
	{
		if (Receiver == nullptr)Receiver = Bus;
		auto uk = UnicAtr.find(LexBuf[ib].atr);
		if (MK < 0)MK = ReceiverMK;
		if (uk!= UnicAtr.end())
		{
			if (uk->second.Fu != nullptr && uk->second.Mk!= 0)
				if(Copy)
					uk->second.Fu->ProgFU(uk->second.Mk, { TIP, &LexBuf[ib] });
				else
					uk->second.Fu->ProgFU(uk->second.Mk, { TIP, LexBuf[ib].Сlone() });
		}
		else
			if (Receiver != nullptr) 
				if (Copy)
					Receiver->ProgFU(MK, { TIP, LexBuf[ib].Сlone() });
				else
					Receiver->ProgFU(MK, { TIP, &LexBuf[ib] });
	}

	void Lex::ProgFU(int MK, LoadPoint Load)
	{
		// Доделать буфер ИП с лексемами
		switch (MK)
 		{
		case 0: // Reset
			S = 0; // Номер состояния распознающего автомата
				   //		FigureBuf = "";
			UnicAtr.clear();
			ReceiverMK = 0;
			Receiver = Bus;
			ErrProg = nullptr;
			SizeBuf = 5;
			if (LexBuf!=nullptr)
				delete[] LexBuf;
			LexBuf = new ip[SizeBuf];

			break;
		case 5: //ReceiverMKSet
			if (Load.Type >> 1 == Dint) ReceiverMK = *(int*)Load.Point; break;
		case 10: //ErrProgSet
			ErrProg = (vector<ip> *)Load.Point;
			break;
		case 12: // StartProgSet Установить программу, запускаемую перед началом компиляции 
			StartProg = (IC_type)Load.Point;
			break;
		case 13: // StopProgSet Установить программу, запускаемую при досрочном завершении лексисческого анализа по МК Stop 
			StopProg = (IC_type)Load.Point;
			break;
		case 14: // FinProgSet  Установить программу, запускаемую после окончания анализа строки
			FinProg = (IC_type)Load.Point;
			break;
		case 15:// UnicAtrSet Установить уникальный атрибут
			UnicAtr[*(int*)Load.Point] = { 0,Bus };
			break;
		case 16:// UnicMkSet Установить МК для уникального атрибута
			if(UnicAtr.size())
				(--UnicAtr.end())->second.Mk= *(int*)Load.Point;
			break;
		case 17:// UnicFuSet Установить контекст для уникального атрибута
			if (UnicAtr.size())
				(--UnicAtr.end())->second.Fu = (FU*)Load.Point;
			break;
		case 18:// UnicReset Сбросить список уникальных атрибутов
			UnicAtr.clear();
			break;
		case 20: // LexBufSizeSet Установить размер буфера лексем
			if (Load.Type >> 1 != Dint) break;
			for (int i = 0; i < SizeBuf; LexBuf[i++].Load.Clear());
			delete[] LexBuf;
			SizeBuf = *(int*)Load.Point;
			LexBuf = new ip[SizeBuf];
			for (int i = 0; i < SizeBuf; LexBuf[i++].Load = { 0,nullptr });
			break;
		case 26: // NoUnucToReseiver Выдать лексему, не учитывая уникальных атрибутов (при нулевой нагрузке выдается текущая лексема)
			if (Load.Point == nullptr)
				MkExec(ReceiverMK, LexBuf[ib].Load, Receiver);
			else
			{
				ib = (ib + 1) % SizeBuf;
				LexBuf[ib].Load.Clear(); // Удаляем нагрузку ИП
				LexBuf[ib].copy(Load);
				MkExec(Load, LexBuf[ib].Load, Receiver);
			}
			break;
		case 27: //Replace Заменить тукущую лексему в буфере
		case 28: //ReplaceToReceiver Выдать лексему из нагрузка МК получателю, заменив тукущую лексему в буфере
		case 29: //ReplaceCopyToReceiver Выдать копию лексемы из нагрузки МК получателю, заменив тукущую лексему в буфере
		case 30: //ToReceiver Выдать лексему из нагрузки МК получателю (Если нагрузка nil, то выдается текущая лексема)
		case 31: //CopyToReceiver Выдать копию лексемы из нагрузки МК получателю
			if(MK==30 && Load.Point==nullptr)	{
				LexOut(false);
				break;
			}
			if(MK==30 || MK==31) ib = (ib + 1) % SizeBuf;
			LexBuf[ib].Load.Clear(); // Удаляем нагрузку ИП
			LexBuf[ib].copy(Load);
			if(MK!=27)LexOut(MK==31);
			break;
//		case 32: //LastLexemaToReceiver Выдать последнюю лексему получателю
//		case 33: //LastLexemaCopyToReceiver Выдать последнюю лексему получателю
//			if (Receiver != nullptr)
//				if (MK==33)
//					Receiver->ProgFU(ReceiverMK, { TIP, LexBuf[ib].сlone() });
//				else
//					Receiver->ProgFU(ReceiverMK, { TIP, &LexBuf[ib] });
//	//		LexOut(MK==33);
//			break;
		case 35: //OutMk Выдать MK c последней лексемой (если nil в нагрузке, то выдается на Receiver)
			if (Load.Point == nullptr)
			{
				//LexOut(false);
				Receiver->ProgFU(ReceiverMK, { TIP, &LexBuf[ib] });
			}
			else

//			if (Load.Type >> 1 == Dint)
//				LexOut(false,Load.ToInt())
				if(Load.Type>>1==Dint)
					Receiver->ProgFU(Load.ToInt(), { TIP, &LexBuf[ib] });
		//	else if(Load.Point==0)

			break;
		case 36: // CopyOutMk Выдать МК с копией последней лексемы (если nil в нагрузке, то выдается на Receiver)
			if (Load.Point!=nullptr) 
				Receiver->ProgFU(*(int*)Load.Point, { TIP, LexBuf[ib].Сlone() });
			else
				Receiver->ProgFU(ReceiverMK, { TIP, LexBuf[ib].Сlone() });
			break;
		case 37: // LoadOut Выдать нагрузку текущей лексемы
			Load.Write(LexBuf[ib].Load);
			break;
		case 38: // LoadOutMk Выдать МК с нагрузкой текущей лексемы
			MkExec(Load, LexBuf[ib].Load);
			break;
		case 39: // LoadCopyOut Выдать нагрузку текущей лексемы
			Load.Write(LexBuf[ib].Load.Clone());
			break;
		case 40: // LoadCopyOutMk Выдать МК с копией нагрузки текущей лексемы
			MkExec(Load, LexBuf[ib].Load.Clone());
			break;
		case 41: // AtrOut Выдать атрибут текущей лексемы
			Load.Write(LexBuf[ib].atr);
			break;
		case 42: // AtrOutMk Выдать МК с атрибутом текущей лексемы
			MkExec(Load, { Cint, &LexBuf[ib].atr });
			break;
			//		case 40: //PrevToReseiver Выдать предыдущую лексему получателю
//			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
//				Receiver->ProgFU(*(int*)Load.Point, { TIP, &LexBuf[(ib-1+ SizeBuf)% SizeBuf] });
//			break;
//		case 41: //PrevCopyToReseiver Выдать копию предыдущей лексемы получателю
//			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
//				Receiver->ProgFU(*(int*)Load.Point, { TIP, LexBuf[(ib - 1 + SizeBuf) % SizeBuf].сlone() });
//			break;
		case 45: //PrevOutMk Выдать МК с предыдущей лексемой (если нагрузка nil, то выдается на Receiver)
			if (Load.Point==nullptr)
				Receiver->ProgFU(ReceiverMK, { TIP, &LexBuf[(ib - 1 + SizeBuf) % SizeBuf] });
			else
				Receiver->ProgFU(*(int*)Load.Point, { TIP, &LexBuf[(ib - 1 + SizeBuf) % SizeBuf] });
			break;
		case 46: //PrevCopyOutMk Выдать МК с копией предыдущей лексемы (если нагрузка nil, то выдается на Receiver)
			if (Load.Point == nullptr)
				Receiver->ProgFU(ReceiverMK, { TIP, LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Сlone() });
			else	
				Receiver->ProgFU(*(int*)Load.Point, { TIP, LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Сlone() });
			break;
		case 50: // AtrSet Установить атрибут последней лексемы
			LexBuf[ib].atr=Load.ToInt();
			break;
		case 51: // PrevAtrSet Установить атрибут предыдущей лексемы
			LexBuf[(ib - 1 + SizeBuf) % SizeBuf].atr = Load.ToInt();
			break;
		case 55: // LoadSet Установить нагрзузку у последней лексемы
			LexBuf[ib].Load = Load;
			break;
		case 56: // PrevLoadSet Установить нагрзузку у предыдущей лексемы
			LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load = Load;
			break;
		case 57: // LoadCopySet Установить копию нагрзузки у последней лексемы
			LexBuf[ib].Load.Copy(&Load);
			break;
		case 58: // PrevLoadCopySet Установить копию нагрзузки у предыдущей лексемы
			LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load.Copy(&Load);
			break;
		case 65: // VarSet Установить тип переменной для нагрузки последней лексемы
			LexBuf[ib].Load.Type |= 1;
			LexBuf[ib].Load.Type --;
			break;
		case 66: // PrevVarSet Установить тип переменной для нагрузки предыдущей лексемы
			LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load.Type |= 1;
			LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load.Type--;
			break;

		case 70: // CendToReceiver Переслать лексему из нагрузки получателю
			MkExec(ReceiverMK, Load);
			break;
		case 71: // CendCopyToReceiver Переслать копию лексемы из нагрузки получателю
			MkExec(ReceiverMK, Load.Clone());
			break;
		case 99: // Stop Остановить лексический анализ
			Work = false;
			ProgExec((IC_type)StopProg); // выполнить программу по останову лексического анализа
			break;
		case 100: // Lexing Лексический анализ
		{
//			ProgExec((IC_type)StartProg,Bus);
			string FigureBuf;
			string str = *(string*)Load.Point;
			if (Load.Type >> 1 == Dstring && *((string*)Load.Point) == "")
			{
				ib = (ib + 1) % SizeBuf;
				LexBuf[ib].Load.Clear(); // Удаляем нагрузку ИП
				LexBuf[ib] = { StrAtr,Tstring , new string("") };
				LexOut();
			}
			str += " "; // Добавление мнимого конечного элемента
			Work = true;
			for (auto i = str.begin(); i != str.end() && Work; i++) //Обход по символам строки
				switch (S)
				{
				case 0: // Стартовое состояние
				{
					if (*i == ' ') break;
					if (*i == '"')
					{
						FigureBuf = "";
						S = 4;
						break;
					}
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

					if (*i >= '0' && *i <= '9')
					{
						FigureBuf = *i;
						S = 1;
						ib = (ib + 1) % SizeBuf; // Индекс буфера лексем
						break;
					}
					if (ABC.count(*i))
					{
						S = 3;
						FigureBuf = *i;
						break;
					}
					Work = false;
					if (ErrProg != nullptr) ProgExec(ErrProg);
					break;
				}
				case 1: // Распознание числа
					if (*i >= '0' && *i <= '9')
					{
						FigureBuf += *i;
						break;
					}
					if (*i == '.')
					{
						FigureBuf = FigureBuf + (*i);
						S = 2;
						break;
					}
					if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ')
					{
						int  *tint = new int;
						*tint = atoi(FigureBuf.c_str());
						ib = (ib + 1) % SizeBuf;
						LexBuf[ib].Load.Clear(); // Удаляем нагрузку ИП
						LexBuf[ib] = { IntAtr,Tint , tint };
						LexOut();

						S = 0;
						i--; // Для обработки сепаратора
						break;
					}
					Work = false;
					ProgExec(ErrProg);
					break;
				case 2: // После десятичной точки
					if (*i >= '0' && *i <= '9')
					{
						FigureBuf = FigureBuf + (*i);
						break;
					}
					if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ')
					{
						double *ft = new double;
						*ft = atof(FigureBuf.c_str());
						ib = (ib + 1) % SizeBuf;
						LexBuf[ib].Load.Clear();
						LexBuf[ib] = { DoubleAtr, Tdouble, ft };
						LexOut();
						S = 0;
						i--;
					}
					Work = false;
					ProgExec(ErrProg);
					break;
				case 3: // Мнемоника
					if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ')
					{
						S = 0;
						i--;
						string *st = new string;
						*st = FigureBuf;
						ib = (ib + 1) % SizeBuf;
						LexBuf[ib].Load.Clear();
						LexBuf[ib] = { MnemoAtr, Tstring, st };
						LexOut();
						break;
					}
					if (ABC.count(*i) || *i >= '0' && *i <= '9')
					{
						FigureBuf += *i;
						break;
					}
					Work = false;
					ProgExec(ErrProg);
					break;
				case 4:
					if (*i != '"'){
						FigureBuf += *i; break;
					}
					else
					{
						S = 0;
						string *st = new string;
						*st = FigureBuf;
						ib = (ib + 1) % SizeBuf; // новый индекс буфера лексем
						LexBuf[ib].Load.Clear();
						LexBuf[ib] = { StrAtr, Cstring, st }; // Добавить строковую константу
						LexOut(); // Выдать лексему потребителю
					}
					Work = false;
					ProgExec(ErrProg);
					break;
				case 5:
					break;
				default:
					break;
				}
		}
		ProgExec(FinProg); // Выполнить 
		break;
		default:
			CommonMk(MK, Load);
		}
	}

	Lex::Lex(FU *BusContext, FU *Templ) 
	{
		Bus = BusContext;
		Receiver = BusContext;
		copy(ABC_templ.begin(), ABC_templ.end(), inserter(ABC, ABC.end()));
		copy(Seps_templ.begin(), Seps_templ.end(), inserter(Seps, Seps.end()));
		LexBuf = new ip[SizeBuf];
		for (int i = 0; i < SizeBuf; LexBuf[i++].Load = { 0,nullptr });
		ProgFU(0, {0,nullptr});
	}

	Lex::Lex() : FU() 
	{
		Lex(nullptr, nullptr); 
	}

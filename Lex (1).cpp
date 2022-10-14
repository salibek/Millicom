#include "stdafx.h"
#include "Lex.h"
#include <string.h>

using namespace std;

void Lex::LexOut(bool Copy, int MK) // Выдача лексемы потребителю
{
	auto uk = UnicAtr.find(LexBuf[ib].atr);
	if (MK < 0)MK = ReceiverMK;
	if (uk != UnicAtr.end())
	{
		if (uk->second.Fu != nullptr && uk->second.Mk != 0)
			if (Copy)
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
		break;
	case 5: //ReceiverMKSet
		if (Load.Type >> 1 == Dint) ReceiverMK = *(int*)Load.Point; break;
	case 10: //ErrProgSet
		ErrProg = (vector<ip> *)Load.Point;
		break;
	case 12: // StartProgSet Установить программу, запускаемую перед началом компиляции 
		StartProg = (IC_type)Load.Point;
		break;
	case 13: // StopProgSet Установить программу, запускаемую при досрочном завершении лексического анализа по МК Stop 
		StopProg = (IC_type)Load.Point;
		break;
	case 14: // FinProgSet  Установить программу, запускаемую после окончания анализа строки
		FinProg = (IC_type)Load.Point;
		break;
	case 15:// UnicAtrSet Установить уникальный атрибут
		UnicAtr[*(int*)Load.Point] = { 0,Bus };
		break;
	case 16:// UnicMkSet Установить МК для уникального атрибута
		if (UnicAtr.size())
			(--UnicAtr.end())->second.Mk = *(int*)Load.Point;
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
		if (MK == 30 && Load.Point == nullptr) {
			LexOut(false);
			break;
		}
		if (MK == 30 || MK == 31) ib = (ib + 1) % SizeBuf;
		LexBuf[ib].Load.Clear(); // Удаляем нагрузку ИП
		LexBuf[ib].copy(Load);
		if (MK != 27)LexOut(MK == 31);
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
			if (Load.Type >> 1 == Dint)
				Receiver->ProgFU(Load.ToInt(), { TIP, &LexBuf[ib] });
		//	else if(Load.Point==0)

		break;
	case 36: // CopyOutMk Выдать МК с копией последней лексемы (если nil в нагрузке, то выдается на Receiver)
		if (Load.Point != nullptr)
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
		if (Load.Point == nullptr)
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
		LexBuf[ib].atr = Load.ToInt();
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
		LexBuf[ib].Load.Type--;
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
		ProgExec((IC_type)StopProg, Bus, nullptr); // выполнить программу по останову лексического анализа
		break;
	case 100: // Lexing
	{
		//			ProgExec((IC_type)StartProg,Bus);
		string FigureBuf;
		string str = *(string*)Load.Point;
		S = 0; // --- установка начального состояния автомата
		if (Load.Type >> 1 == Dstring && *((string*)Load.Point) == "")
		{
			ib = (ib + 1) % SizeBuf;
			LexBuf[ib].Load.Clear(); // Удаляем нагрузку ИП
			LexBuf[ib] = { StrAtr,Tstring , new string("") };
			LexOut();
		}
		str += " "; // Добавление мнимого конечного элемента
		Work = true;
		for (auto i = str.begin(); i != str.end() && Work; i++)
			switch (S) //LEXER
			{
				// Стартовое состояние
			case 0:
			{
				if (*i == ' ') //разделитель; 0 -> 0
				{
					break;
				}
				if (*i == '"') //кавычка; 0 -> 4
				{
					FigureBuf = ""; //буферная лексема
					S = 4; //переход в 4 состояние
					Debug(*i, S, FigureBuf); // --- отладка
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
				if ((DigitDEC.count(*i)) && (*i != '0')) // --- цифра (1..9); 0 -> 1
				{
					FigureBuf = *i; //запись в буферную переменную
					S = 1; //переход в состояние 1
					Debug(*i, S, FigureBuf); // --- отладка
					//TO DO numberFormat = DEC (реализация соответствующих действий)
					break;
				}
				if (ABC.count(*i)) //буква; 0 -> 3
				{
					FigureBuf = *i; //запись в буферную переменную
					S = 3; //переход в состояние 3
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '0') // цифра (0); 0 -> 9
				{
					FigureBuf = *i; //запись в буферную переменную
					S = 9; //переход в состояние 9
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (DigitSeps.count(*i))  // --- символ разделения дробной и целой части (. ,); 0 -> 2
				{
					FigureBuf = '0'; // --- добавление символа 0 в буферную переменную, так как .5 = 0.5; ,5 = 0,5
					FigureBuf += "."; //добавление символа в буферную переменную
					//TO DO numberFormat = DEC (реализация соответствующих действий)
					S = 2; //переход в состояние 2
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '+' || *i == '-') // символ (+,-); 0 -> 10
				{
					FigureBuf = *i; //запись в буферную переменную
					S = 10; //переход в состояние 10
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '/') // символ (/); 0 -> 5
				{
					FigureBuf = *i; //запись в буферную переменную
					S = 5; //переход в состояние 5
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '\n') //переход на новую строку; 0 -> 12
				{
					FigureBuf = *i; //запись в буферную переменную
					// TO DO tabCounter = 0 (реализация соответствующих действий)
					S = 12; //переход в состояние 12
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				Work = false; //установка флага рабочего режима лексера на false
				if (ErrProg != nullptr) ProgExec(ErrProg, Bus, nullptr); //обработка ошибки
				break;
			}
			// Обработка целой части десятичных чисел
			case 1:
				if (DigitDEC.count(*i)) // --- число (0..9); 1 -> 1
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '_') //символ разделения чисел (_); 1 -> 1
				{
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (DigitSeps.count(*i))  // --- символ разделения дробной и целой части (. ,); 1 -> 2
				{
					FigureBuf += "."; //добавление символа в буферную переменную
					S = 2; //переход в состояние 2
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //разделитель; 1 -> 0
				{
					i--; // Для обработки сепаратора
					int* tint = new int;
					*tint = atoi(FigureBuf.c_str()); //запись лексемы в переменную
					ib = (ib + 1) % SizeBuf; //увеличение текущего адреса буфера выходных лексем на 1
					LexBuf[ib].Load.Clear(); //удаление нагрузки ИП
					LexBuf[ib] = { IntAtr,Tint , tint }; //добавление лексемы в буфер выходных лексем в виде ИП {атрибут, тип, указатель}
					S = 0; //переход в состояние 0

					Debug(*i, S, FigureBuf); // --- отладка
					std::cout << "LexBuf[" << ib << "] = {IntAtr, Tint, " << *tint << "}\n"; // --- отладка
					
					break;
				}
				Work = false; //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				// Обработка дробной части десятичных чисел
			case 2:
				if (DigitDEC.count(*i))  // --- число (0..9); 2 -> 2
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '_') //символ разделения чисел (_); 2 -> 2
				{
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //разделитель; 2 -> 0
				{
					i--; // Для обработки сепаратора
					double* ft = new double;
					*ft = atof(FigureBuf.c_str()); //запись лексемы в переменную
					ib = (ib + 1) % SizeBuf; //увеличение текущего адреса буфера выходных лексем на 1
					LexBuf[ib].Load.Clear(); //удаление нагрузки ИП
					LexBuf[ib] = { DoubleAtr, Tdouble, ft }; //добавление лексемы в буфер выходных лексем в виде ИП {атрибут, тип, указатель}
					LexOut();
					S = 0; //переход в состояние 0

					Debug(*i, S, FigureBuf); // --- отладка
					std::cout << "LexBuf[" << ib << "] = {DoubleAtr, Tdouble, " << *ft << "}\n"; // --- отладка
					
					break;
				}
				Work = false;  //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				//Обработка мнемоники
			case 3:
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //разделитель; 3 -> 0
				{
					i--; // Для обработки сепаратора
					string* st = new string;
					*st = FigureBuf; //запись лексемы в переменную
					ib = (ib + 1) % SizeBuf; //увеличение текущего адреса буфера выходных лексем на 1
					LexBuf[ib].Load.Clear(); //удаление нагрузки ИП
					LexBuf[ib] = { MnemoAtr, Tstring, st };  //добавление лексемы в буфер выходных лексем в виде ИП {атрибут, тип, указатель}
					LexOut();
					S = 0; //переход в состояние 0

					Debug(*i, S, FigureBuf); // --- отладка
					std::cout << "LexBuf[" << ib << "] = {MnemoAtr, Tstring, " << *st << "}\n"; // --- отладка

					break;
				}
				if (ABC.count(*i) || DigitDEC.count(*i)) // --- буква либо число; 3 -> 3
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				Work = false;  //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				//Обработка кавычек
			case 4:
				if (*i != '"' && *i != '\\') //любой символ кроме кавычки и \; 4 -> 4
				{
					FigureBuf += *i;  //добавление символа в буферную переменную
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				else
					if (*i == '\\')//символ (\); 4 -> 8
					{
						S = 8; //переход в состояние 8
						Debug(*i, S, FigureBuf); // --- отладка
						break;
					}
					else
						if (*i == '"')//кавычка; 4 -> 0
						{
							string* st2 = new string;
							*st2 = FigureBuf; //запись лексемы в переменную
							ib = (ib + 1) % SizeBuf; //увеличение текущего адреса буфера выходных лексем на 1
							LexBuf[ib].Load.Clear(); //удаление нагрузки ИП
							LexBuf[ib] = { StrAtr, Cstring, st2 }; //добавление лексемы в буфер выходных лексем в виде ИП {атрибут, тип, указатель}
							LexOut();
							S = 0; //переход в состояние 0

							Debug(*i, S, FigureBuf); // --- отладка
							std::cout << "LexBuf[" << ib << "] = {StrAtr, Cstring, " << *st2 << "}\n"; // --- отладка
							
							break;
						}
				Work = false;  //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				//Обработка строчного комментария
			case 5:
				if (*i == '/') //символ (/); 5 -> 11
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					S = 11; //переход в состояние 11
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '*') //символ (*); 5 -> 6
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					S = 6; //переход в состояние 6
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				Work = false;  //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				//Обработка многострочного комментария
			case 6:
				if (*i != '*') //любой символ кроме (/); 6 -> 6
				{
					FigureBuf += *i;  //добавление символа в буферную переменную
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '*') //символ (/); 6 -> 7
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					S = 7; //переход в состояние 7
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				Work = false;  //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				//Обработка многострочного комментария
			case 7:
				if (*i != '/') //любой символ кроме (*); 7 -> 6
				{
					FigureBuf += *i;  //добавление символа в буферную переменную
					S = 6; //переход в состояние 6
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '/') //символ (*); 7 -> 0
				{
					string* multiLineComment = new string;
					FigureBuf.erase(0,2); //удаление символа многострочного комментария в начале
					FigureBuf.pop_back(); //удаление символа многострочного комментария в конце
					*multiLineComment = FigureBuf; //запись лексемы в переменную
					ib = (ib + 1) % SizeBuf; //увеличение текущего адреса буфера выходных лексем на 1
					LexBuf[ib].Load.Clear(); //удаление нагрузки ИП
					LexBuf[ib] = { StrAtr, Cstring, multiLineComment }; //добавление лексемы в буфер выходных лексем в виде ИП {атрибут, тип, указатель}
					LexOut();
					S = 0; //переход в состояние 0

					Debug(*i, S, FigureBuf); // --- отладка
					std::cout << "LexBuf[" << ib << "] = {StrAtr, Cstring, " << *multiLineComment << "}\n"; // --- отладка
					
					break;
				}
				Work = false;  //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				//Обработка экранирования
			case 8:
				if (*i == '"' || *i == 'n' || *i == '\'') //символы (", n, '); 8 -> 4
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					//TO DO Обработка экранирования (реализация соответствующих действий)
					S = 4; //переход в состояние 4
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				else //любой символ кроме (n, ", '); 8 -> 4
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					S = 4; //переход в состояние 4
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				Work = false;  //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				//Определитель системы счисления
			case 9:
				if (*i == '_') //символ разделения чисел (_); 9 -> 9
				{
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (DigitDEC.count(*i)) // --- число (0..9); 9 -> 1
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					//TO DO numberFormat = DEC (реализация соответствующих действий)
					S = 1; //переход в состояние 1
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == 'b') //символ (b); 9 -> 13
				{
					//FigureBuf += *i; //добавление символа в буферную переменную не нужно, так как b является обозначением BIN числа
					//TO DO numberFormat = BIN (реализация соответствующих действий)
					S = 13; //переход в состояние 13
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == 'o') //символ (o); 9 -> 15
				{
					//FigureBuf += *i; //добавление символа в буферную переменную не нужно, так как o является обозначением OCT числа
					//TO DO numberFormat = OCT (реализация соответствующих действий)
					S = 15; //переход в состояние 15
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == 'x') //символ (x); 9 -> 17
				{
					//FigureBuf += *i; //добавление символа в буферную переменную не нужно, так как x является обозначением HEX числа
					//TO DO numberFormat = HEX (реализация соответствующих действий)
					S = 17; //переход в состояние 17
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (DigitSeps.count(*i))  // --- символ разделения дробной и целой части (. ,); 9 -> 2
				{
					FigureBuf += "."; //добавление символа в буферную переменную
					//TO DO numberFormat = DEC (реализация соответствующих действий)
					S = 2; //переход в состояние 2
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				Work = false;  //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				//Обработка АЛВ
			case 10:
				if ((DigitDEC.count(*i)) && (*i != '0')) // --- число (1..9); 10 -> 1
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					//TO DO numberFormat = DEC (реализация соответствующих действий)
					S = 1; //переход в состояние 1
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '0') //число 0; 10 -> 9
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					S = 9; //переход в состояние 9
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //разделитель; 10 -> 0
				{
					i--; // Для обработки сепаратора
					string* sep = new string;
					*sep = FigureBuf; //запись лексемы в переменную
					ib = (ib + 1) % SizeBuf; //увеличение текущего адреса буфера выходных лексем на 1
					LexBuf[ib].Load.Clear(); //удаление нагрузки ИП
					LexBuf[ib] = { SeperatAtr, Tstring, sep };  //добавление лексемы в буфер выходных лексем в виде ИП {атрибут, тип, указатель}
					LexOut();
					S = 0; //переход в состояние 0

					Debug(*i, S, FigureBuf); // --- отладка
					std::cout << "LexBuf[" << ib << "] = {SeperatAtr, Tstring, " << *sep << "}\n"; // --- отладка
					
					break;
				}
				Work = false;  //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				//Обработка строчного комментария
			case 11:
				if (*i != '\n') //любой символ кроме (\n); 11 -> 11
				{
					FigureBuf += *i;  //добавление символа в буферную переменную
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '\n') //символ (\n); 11 -> 0
				{
					string* multiLineComment = new string;
					FigureBuf.erase(0, 2); //удаление символа многострочного комментария в начале
					*multiLineComment = FigureBuf; //запись лексемы в переменную
					ib = (ib + 1) % SizeBuf; //увеличение текущего адреса буфера выходных лексем на 1
					LexBuf[ib].Load.Clear(); //удаление нагрузки ИП
					LexBuf[ib] = { StrAtr, Cstring, multiLineComment }; //добавление лексемы в буфер выходных лексем в виде ИП {атрибут, тип, указатель}
					LexOut();
					S = 0; //переход в состояние 0

					Debug(*i, S, FigureBuf); // --- отладка
					std::cout << "LexBuf[" << ib << "] = {StrAtr, Cstring, " << *multiLineComment << "}\n"; // --- отладка

					break;
				}
				Work = false; //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				//Обработка табуляции
			case 12:
				if (*i != '\n')//любой символ кроме (\n); 12 -> 0
				{
					/*
					TO DO
					If (bracketAmount > 0)
					Замена табуляции на кол-во { равное bracketAmount

					If (bracketAmount < 0)
					Замена табуляции на кол-во } равное bracketAmount
					*/
					i--; // Для обработки сепаратора
					string* st3 = new string;
					*st3 = FigureBuf; //запись лексемы в переменную
					ib = (ib + 1) % SizeBuf; //увеличение текущего адреса буфера выходных лексем на 1
					LexBuf[ib].Load.Clear(); //удаление нагрузки ИП
					LexBuf[ib] = { StrAtr, Cstring, st3 }; //добавление лексемы в буфер выходных лексем в виде ИП {атрибут, тип, указатель}
					LexOut();
					S = 0; //переход в состояние 0

					Debug(*i, S, FigureBuf); // --- отладка
					std::cout << "LexBuf[" << ib << "] = {IntAtr, Tint, " << *st3 << "}\n"; // --- отладка
					
					break;
				}
				if (*i == '\n') //символ (\n); 12 -> 12
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					// TO DO tabCounter = 0 (реализация соответствующих действий)
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '\t') //символ (\n); 12 -> 12
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					// TO DO tabCounter++ (реализация соответствующих действий)
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				Work = false; //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				// Обработка целой части двоичных чисел
			case 13:
				if (DigitBIN.count(*i)) // --- число (0..1); 13 -> 13
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '_') //символ разделения чисел (_); 13 -> 13
				{
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (DigitSeps.count(*i))  // --- символ разделения дробной и целой части (. ,); 13 -> 14
				{
					FigureBuf += "."; //добавление символа в буферную переменную
					S = 2; //переход в состояние 2
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //разделитель; 13 -> 0
				{
					i--; // Для обработки сепаратора
					int* tint = new int;
					*tint = atoi(FigureBuf.c_str()); //запись лексемы в переменную
					ib = (ib + 1) % SizeBuf; //увеличение текущего адреса буфера выходных лексем на 1
					LexBuf[ib].Load.Clear(); //удаление нагрузки ИП
					LexBuf[ib] = {IntAtr, TBinInt , tint}; //добавление лексемы в буфер выходных лексем в виде ИП {атрибут, тип, указатель}
					S = 0; //переход в состояние 0

					Debug(*i, S, FigureBuf); // --- отладка
					std::cout << "LexBuf[" << ib << "] = {IntAtr, TBinInt, " << *tint << "}\n"; // --- отладка
					
					break;
				}
				Work = false; //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				// Обработка дробной части двоичных чисел
			case 14:
				if (DigitBIN.count(*i))  // --- число (0..1); 14 -> 14
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '_') //символ разделения чисел (_); 14 -> 14
				{
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //разделитель; 14 -> 0
				{
					i--; // Для обработки сепаратора
					double* ft = new double;
					*ft = atof(FigureBuf.c_str()); //запись лексемы в переменную
					ib = (ib + 1) % SizeBuf; //увеличение текущего адреса буфера выходных лексем на 1
					LexBuf[ib].Load.Clear(); //удаление нагрузки ИП
					LexBuf[ib] = {DoubleAtr, TBinDouble, ft}; //добавление лексемы в буфер выходных лексем в виде ИП {атрибут, тип, указатель}
					LexOut();
					S = 0; //переход в состояние 0

					Debug(*i, S, FigureBuf); // --- отладка
					std::cout << "LexBuf[" << ib << "] = {DoubleAtr, TBinDouble, " << *ft << "}\n"; // --- отладка
					
					break;
				}
				Work = false;  //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				// Обработка целой части восьмеричных чисел
			case 15:
				if (DigitOCT.count(*i)) // --- число (0..7); 15 -> 15
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '_') //символ разделения чисел (_); 15 -> 15
				{
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (DigitSeps.count(*i))  // --- символ разделения дробной и целой части (. ,); 15 -> 16
				{
					FigureBuf += "."; //добавление символа в буферную переменную
					S = 2; //переход в состояние 2
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //разделитель; 15 -> 0
				{
					i--; // Для обработки сепаратора
					int* tint = new int;
					*tint = atoi(FigureBuf.c_str()); //запись лексемы в переменную
					ib = (ib + 1) % SizeBuf; //увеличение текущего адреса буфера выходных лексем на 1
					LexBuf[ib].Load.Clear(); //удаление нагрузки ИП
					LexBuf[ib] = {IntAtr, TOctInt, tint }; //добавление лексемы в буфер выходных лексем в виде ИП {атрибут, тип, указатель}
					S = 0; //переход в состояние 0

					Debug(*i, S, FigureBuf); // --- отладка
					std::cout << "LexBuf[" << ib << "] = {IntAtr, TOctInt, " << *tint << "}\n"; // --- отладка
					
					break;
				}
				Work = false; //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				// Обработка дробной части восьмеричных чисел
			case 16:
				if (DigitOCT.count(*i))  // --- число (0..7); 16 -> 16
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '_') //символ разделения чисел (_); 16 -> 16
				{
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //разделитель; 16 -> 0
				{
					i--; // Для обработки сепаратора
					double* ft = new double;
					*ft = atof(FigureBuf.c_str()); //запись лексемы в переменную
					ib = (ib + 1) % SizeBuf; //увеличение текущего адреса буфера выходных лексем на 1
					LexBuf[ib].Load.Clear(); //удаление нагрузки ИП
					LexBuf[ib] = {DoubleAtr, TOctDouble, ft }; //добавление лексемы в буфер выходных лексем в виде ИП {атрибут, тип, указатель}
					LexOut();
					S = 0; //переход в состояние 0

					Debug(*i, S, FigureBuf); // --- отладка
					std::cout << "LexBuf[" << ib << "] = {DoubleAtr, TOctDouble, " << *ft << "}\n"; // --- отладка
					
					break;
				}
				Work = false;  //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				// Обработка целой части шестнадцатеричных чисел
			case 17:
				if (DigitHEX.count(*i)) // --- число (0..9, A..F, a..f); 17 -> 17
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '_') //символ разделения чисел (_); 17 -> 17
				{
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (DigitSeps.count(*i))  // --- символ разделения дробной и целой части (. ,); 17 -> 2
				{
					FigureBuf += "."; //добавление символа в буферную переменную
					S = 2; //переход в состояние 2
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //разделитель; 17 -> 0
				{
					i--; // Для обработки сепаратора
					string* st2 = new string;
					*st2 = FigureBuf; //запись лексемы в переменную
					ib = (ib + 1) % SizeBuf; //увеличение текущего адреса буфера выходных лексем на 1
					LexBuf[ib].Load.Clear(); //удаление нагрузки ИП
					LexBuf[ib] = {IntAtr, THexInt, st2 }; //добавление лексемы в буфер выходных лексем в виде ИП {атрибут, тип, указатель}
					S = 0; //переход в состояние 0

					Debug(*i, S, FigureBuf); // --- отладка
					std::cout << "LexBuf[" << ib << "] = {IntAtr, THexInt, " << *st2 << "}\n"; // --- отладка
					
					break;
				}
				Work = false; //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
				break;
				// Обработка дробной части шестнадцатеричных чисел
			case 18:
				if (DigitHEX.count(*i))  // --- число (0..9, A..F, a..f); 18 -> 18
				{
					FigureBuf += *i; //добавление символа в буферную переменную
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (*i == '_') //символ разделения чисел (_); 18 -> 18
				{
					Debug(*i, S, FigureBuf); // --- отладка
					break;
				}
				if (Seps.count(str.substr(distance(str.begin(), i), 1)) || *i == ' ') //разделитель; 18 -> 0
				{
					i--; // Для обработки сепаратора
					string* st2 = new string;
					*st2 = FigureBuf; //запись лексемы в переменную
					ib = (ib + 1) % SizeBuf; //увеличение текущего адреса буфера выходных лексем на 1
					LexBuf[ib].Load.Clear(); //удаление нагрузки ИП
					LexBuf[ib] = {DoubleAtr, THexDouble, st2 }; //добавление лексемы в буфер выходных лексем в виде ИП {атрибут, тип, указатель}
					S = 0; //переход в состояние 0

					Debug(*i, S, FigureBuf); // --- отладка
					std::cout << "LexBuf[" << ib << "] = {DoubleAtr, THexDouble, " << *st2 << "}\n"; // --- отладка

					break;
				}
				Work = false;  //установка флага рабочего режима лексера на false
				ProgExec(ErrProg, Bus, nullptr); //обработчик ошибок
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

void Lex::Debug(char i, int S, string FigureBuf) // --- для отладки, позже удалить
{
	cout << " Current symbol: " << i << "; "; // --- отладка
	cout << "New condition: " << S << "; "; // --- отладка
	cout << " FigureBuf: " << FigureBuf << endl; // --- отладка
}

Lex::Lex(FU* BusContext, FU* Templ)
{
	Bus = BusContext;
	Receiver = BusContext;
	copy(ABC_templ.begin(), ABC_templ.end(), inserter(ABC, ABC.end()));

	copy(Digit_DEC_templ.begin(), Digit_DEC_templ.end(), inserter(DigitDEC, DigitDEC.end())); // --- Добавление в множество 10-чных чисел
	copy(Digit_BIN_templ.begin(), Digit_BIN_templ.end(), inserter(DigitBIN, DigitBIN.end())); // --- Добавление в множество 2-чных чисел
	copy(Digit_OCT_templ.begin(), Digit_OCT_templ.end(), inserter(DigitOCT, DigitOCT.end())); // --- Добавление в множество 8-ричных чисел
	copy(Digit_HEX_templ.begin(), Digit_HEX_templ.end(), inserter(DigitHEX, DigitHEX.end())); // --- Добавление в множество  16-ричных чисел

	copy(Digit_seps_templ.begin(), Digit_seps_templ.end(), inserter(DigitSeps, DigitSeps.end())); // --- Добавление в множество разделителей дробных и целых частей чисел
	copy(Seps_templ.begin(), Seps_templ.end(), inserter(Seps, Seps.end()));
	LexBuf = new ip[SizeBuf];
	for (int i = 0; i < SizeBuf; LexBuf[i++].Load = { 0,nullptr });
	ProgFU(0, { 0,nullptr });
}

Lex::Lex() : FU()
{
	Lex(nullptr, nullptr);
}

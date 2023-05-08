// ФУ Консоль
#include "stdafx.h"
#include "Console.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <regex>


void Console::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	MK %= FUMkRange; // Оставить только свои МК
	switch (MK)
	{
	case 0: // Reset
		AtrMnemo.clear();
		break;
	case 1: // Out Вывод
	case 2: // OutLn Вывод и перевод строки
	case 3: // LnOut Перевод строки и вывод
	case 4: // LnOutLn Перевод строки, вывод и снова перевод строки
		cout << prefix;
		if (MK == 3 || MK == 4) cout << endl;
		if (Load.Point != nullptr)
			Load.print(AtrMnemo,"",Sep,End, quote, ArrayBracketStart,ArrayBracketFin);
		if (MK == 2 || MK == 4) cout << endl;
		break;
	case 5: // LoadInfoOut Вывести сведения о нагрузке
		cout << "LoadInfo : Type " << Load.Type << " Ind " << Load.Ind;
		if (Load.Point == nullptr) cout << " Point=null";
		else cout << Load.Type % 2 ? " Const" : "Var";
		cout << endl;
		break;
	case 10: // Ln Перевод строки
		cout << endl;
		break;
	case 15: //SepSet Установить строку-разделитель
		Sep = Load.toStr();
		break;
	case 16: //EndSet Установить строку в конце вывода
		End = Load.toStr();
		break;
	case 17: // ArrayBracketStartSet Установить строку, обозначающую открывающуюся скобку при вывод вектора
		ArrayBracketStart = Load.toStr();
		break;
	case 18: // ArrayBracketStartSet Установить строку, обозначающую закрывающуюся скобку при вывод вектора
		ArrayBracketFin = Load.toStr();
		break;
	case 19: // QuoteSet Установить символ кавычек при воводе строки или символа
		quote = Load.toStr();
		break;
	case 20: // PrefixSet Установить префикс перед выводом
		if ((Load.Type) >> 1 == Dstring)
			prefix = *(string*)Load.Point;
		break;
	case 25: // FileNameSet
		filename = Load.toStr();
		break;
	case 30: // OutFileSet Установить файл для вывода
		if(Load.Point==nullptr)
			ostream& out = cout;
		else
			freopen_s(&streamOut, Load.toStr().c_str(), "w", stdout);
		break;

	case 31: // StdOutFileAppend Установить файл для дополнения
		if (Load.Point == nullptr)
			ostream& out = cout;
		else
			freopen_s(&streamOut, Load.toStr().c_str(), "a", stdout);
		break;

	case 35: // StdInFileSet Установить файл для ввода
		if (Load.Point == nullptr)
//			std::istream& in == cin;
			;
		else
			freopen_s(&streamIn, Load.toStr().c_str(), "r", stdin);
		break;
	case 40: // AtrMnemoAdd Добавить мнемоники атрибутов
		if (Load.Type >> 1 == DIP)
			AtrMnemo[((ip*)Load.Point)->atr] = ((ip*)Load.Point)->Load.toStr();
		else if (Load.isIC())
			for (auto& i : *(IC_type)Load.Point)
				AtrMnemo[i.atr] = i.Load.toStr();
		break;
	case 41: // AtrMnemoClear Очистить мнемоники атрибутов
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

	case 50: //VectIn ввод вектора 
		break;
	case 55: //MatrIn ввод матрицы	
		break;
	case 60: //VarClear Очистить буфер адресов для результата ввода
		VarOutBuf.clear();
		break;
	case 61: //VarSet Записать адрес переменной для записи результата ввода
		VarOutBuf.clear();
		VarOutBuf.push_back({ -1, Load });
		break;
	case 62: //VarAdd Добавить адрес переменной для записи результата ввода
		VarOutBuf.push_back({-1, Load});
		break;
	case 80: //VarOut – выдать адрес переменной
		Load.Write(Var);
		break;
	case 85: // VarOutMk
		MkExec(Load, Var);
		break;
	case 86: // InStrOut Выдать последнюю введенную строку
		Load.Write(inStr);
		break;
	case 87: // InStrOutMk  Выдать МК с последней введенной строкой
		MkExec(Load, {Cstring, &inStr});
		break;
	case 90: // TrueFalseClear Очистить буфер наименований true и false
		False.clear();
		break;
	case 91: // TrueAdd
		True.insert(Load.toStr());
		break;
	case 92: // FalseAdd
		False.insert(Load.toStr());
		break;
	case 94: // InputErrProgSet Установить программу обработки ошибки неправильного формата входных данных
		InputFormatErrProg = Load.Point;
	case 100: // Input Ввод данных
	case 101: // InputMk Ввод данных и выдача МК с ними
	{
		std::cmatch results;
		regex regular_str("[\\D\\s]+");
		regex regular_int("([\\d]+)");
		regex regular_float("\\d+\\.\\d+");
		regex regular_true("[Tt][Rr][Uu][Ee]");
		regex regular_false("[Ff][Aa][Ll][Ss][Ee]");
		regex regular_vector("^\\[(\\d+,?\\s?)+\\]$"); //интовый вектор
		regex regular_matrix("^\\[(\\d+[,;]?\\s?)+\\]$"); //интовая матрица
		regex regular_char("\\w{1}");





		getline(cin, inStr);
		Var.Clear(); // Очистить предыдущее значение
	//	if (MK == 101 && Load.Point != nullptr) // Выдать МК
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
			// Распознание вектора
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

			// Опеределяем тип введенных данных
			// Преобразуем в соответсвующий тип и записываем в Var
			// Var={Тип, new Соотвествующий тип данных} Типы данных Cbool, Cint, Cchar, Cdouble, Cstring, CLoadVect
			  // Регулятка - определить тип Var={Cbool, new bool(true)}
			if (MK == 101) // Выдать МК
				MkExec(Load, Var);
			else // Записать в переменную
				Load.WriteFromLoad(Var);
			break;
		}
		// Выдать по списку заранее установленных адресов и МК в VarOutBuf
//		for (auto& i : VarOutBuf)
//		{
//			// Выдача результата ввода на VarOutBuf
//		}
		break;
	}
	case 105: //InputBool Ввод буленова значения
	case 106: //InputBoolMk Ввод буленова значения и выдача МК с ним
	//	if(неправильный формат)
		if(false)
			ProgExec(InputFormatErrProg);
		else
			if (MK == 106) // Выдать МК
				MkExec(Load, Var);
			else // Записать в переменную
				Load.WriteFromLoad(Var);
		break;
	case 110: //InputInt
	case 111: //InputIntMk
		break;
// ......

	case 200: // NoVarToOutProgSet Установить подрограмму реакции на ошибку "Нет переменной для ввода"
		NoVarToOutProg = Load.Point;
		break;
	case 201: // WrongFormatProgSet Установить подрограмму реакции на ошибку "Неправильный формат входных данных"
		WrongFormatProg = Load.Point;
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
}
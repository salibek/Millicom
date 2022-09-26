// ФУ Консоль
#include "stdafx.h"
#include "Console.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>

void Console::ProgFU(int MK, LoadPoint Load)
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
			Load.print(AtrMnemo,"",Sep,End,ArrayBracketStart,ArrayBracketFin);
		if (MK == 2 || MK == 4) cout << endl;
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
	case 90: // Input Ввод данных
//	case 91: // Input Ввод данных и запись их в переменную
	case 92: // InputMk Ввод данных и выдача МК с ними
	{
		getline(cin, inStr);
		Var.Clear(); // Очистить предыдущее значение
		if (MK == 92 && Load.Point != nullptr) // Выдать МК
		{
			// Опеределяем тип введенных данных
			// Преобразуем в соответсвующий тип и записываем в Var
			// Var={Тип, new Соотвествующий тип данных} Типы данных Cbool, Cint, Cchar, Cdouble, Cstring, CLoadVect
			  // Регулятка - определить тип Var={Cbool, new bool(true)}
			MkExec(Load, Var);
			break;
		}
		// ---

		if (MK == 90 && Load.Point!=nullptr)
		{
			if (Load.isBool()) // Булев тип
			{
				// Проверка формата
				// Сообщерние об ошибке
				// Преобразование текста в переменную
				Load.WriteFromLoad(Var); // Запись резульатата распознания введенной строки
			}
			else if (Load.isInt()) // Целый тип
			{

			}
			else if (Load.isFloatDouble()) // Дробный тип
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
				ProgExec(WrongFormatProg); // Сообщение о неправильном формате данных
				break;
			}
			break;
		}
		// Выдать по списку заранее установленных адресов и МК в VarOutBuf
		for (auto i : VarOutBuf)
		{
			// Выдача результата ввода на VarOutBuf
		}
		break;
	}
	case 95: // TrueFalseClear Очистить буфер наименований true и false
		False.clear();
		break;
	case 96: // TrueAdd
		True.insert(Load.toStr());
		break;
	case 97: // FalseAdd
		False.insert(Load.toStr());
		break;

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
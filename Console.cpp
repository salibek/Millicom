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
		Sep = Load.ToStr();
		break;
	case 16: //EndSet Установить строку в конце вывода
		End = Load.ToStr();
		break;
	case 17: // ArrayBracketStartSet Установить строку, обозначающую открывающуюся скобку при вывод вектора
		ArrayBracketStart = Load.ToStr();
		break;
	case 18: // ArrayBracketStartSet Установить строку, обозначающую закрывающуюся скобку при вывод вектора
		ArrayBracketFin = Load.ToStr();
		break;
	case 20: // PrefixSet Установить префикс перед выводом
		if ((Load.Type) >> 1 == Dstring)
			prefix = *(string*)Load.Point;
		break;
	case 25: // FileNameSet
		filename = Load.ToStr();
		break;
	case 30: // OutFileSet Установить файл для вывода
		if(Load.Point==nullptr)
			ostream& out = cout;
		else
			freopen_s(&streamOut, Load.ToStr().c_str(), "w", stdout);
		break;

	case 31: // StdOutFileAppend Установить файл для дополнения
		if (Load.Point == nullptr)
			ostream& out = cout;
		else
			freopen_s(&streamOut, Load.ToStr().c_str(), "a", stdout);
		break;

	case 35: // StdInFileSet Установить файл для ввода
		if (Load.Point == nullptr)
//			std::istream& in == cin;
			;
		else
			freopen_s(&streamIn, Load.ToStr().c_str(), "r", stdin);
		break;
	case 40: // AtrMnemoAdd Добавить мнемоники атрибутов
		if (Load.Type >> 1 == DIP)
			AtrMnemo[((ip*)Load.Point)->atr] = ((ip*)Load.Point)->Load.ToStr();
		else if (Load.isIC())
			for (auto& i : *(IC_type)Load.Point)
				AtrMnemo[i.atr] = i.Load.ToStr();
		break;
	case 41: // AtrMnemoClear Очистить мнемоники атрибутов
		AtrMnemo.clear();
		break;

	case 44: // TrueValSet
		TrueVar.clear();
		if(Load.Point!=nullptr)
			TrueVar.push_back(Load.ToStr());
		break;
	case 45: // TrueValAdd
		if (Load.Point != nullptr)
			TrueVar.push_back(Load.ToStr());
		break;
	case 46: // TrueValSet
		FalseVar.clear();
		if (Load.Point != nullptr)
			FalseVar.push_back(Load.ToStr());
		break;
	case 47: // TrueValAdd
		if (Load.Point != nullptr)
			FalseVar.push_back(Load.ToStr());
		break;

	case 50: //VectIn ввод вектора 
		break;
	case 55: //MatrIn ввод матрицы	
		break;
	case 60: //VarClear Очистить буфер адресов для результата ввода
		VarBuf.clear();
		break;
	case 61: //VarSet Записать адрес переменной для записи результата ввода
		VarBuf.clear();
		VarBuf.push_back(Load);
		break;
	case 62: //VarAdd Добавить адрес переменной для записи результата ввода
		VarBuf.push_back(Load);
		break;
	case 80: //VarOut – выдать адрес переменной (если в буфере несколько переменных, то выдается адрес самой последней добавленной переменной)
		if (!VarBuf.size())
			ProgExec(NoVarToOutProg);// Сообщение об ошибке
		else
			Load.Write(VarBuf.back());
		break;
//	case 81: //VarOutAll выдать адрес всех переменных из буфера
//		if(!VarBuf.size())
//			ProgExec(NoVarToOutProg);// Сообщение об ошибке
//		else
//			for(auto i: VarBuf) // Запись всех переменных
//				Load.Write(i);
	case 85: // VarOutMk
		MkExec(Load, VarBuf[0]);
		break;
	case 90: // Input Ввод данных
	{
		string inStr;
		getline(cin, inStr);
		if (Load.isBool()) // Булев тип
		{
			// Проверка формата
			// Сообщерние об ошибке
			bool t=0;
			// Преобразование текста в переменную
			Load.Write(t); // Запись резульатата распознания введенной строки
		}
		else if (Load.isInt()) // Целый тип
		{

		}
		else if (Load.isFloatDouble()) // Дробный тип
		{
		}
		else if (Load.IsChar())   //
		{
		}
		else if (Load.IsStr())    //
		{
		}
		else if (Load.IsVector()) //
		{
		}
		else
			ProgExec(WrongFormatProg); // Сообщение о неправильном формате данных
		break;
	}
	case 92: // InputMk Вввод данных и выдача Мк с ними (тип введенных данных определяется автоматически)
		// Автоматическое распознание типа
	{
		int t = 0;
		MkExec(Load, { Cint, &t });
		break;
	}
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
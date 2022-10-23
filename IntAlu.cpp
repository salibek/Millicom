#include "IntAlu.h"

void IntAlu::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case 0:// Reset
		AccumulatUk = &Accumulat;
		Accumulat = 0;
		AutoInc = 0;
		Compare = 0;
		CompareUk = &Compare;
		EmptyStackErrProg = nullptr; // Программа обработки ошибки pop из пустого стека
		BiggerProg = nullptr; LessProg = nullptr; EQProg = nullptr; NEQProg = nullptr; BiggerEQProg = nullptr; LessEQProg = nullptr;
		break;
	case 1: // Set Установить значение аккумулятора
		*AccumulatUk = Load.toInt(0);
		if (!Load.isDigit()) ProgExec(NoCorrectTypeErrProg);
		if (!Load.isInt()) ProgExec(NoIntTypeErrProg);
		if (!Load.isIntBool()) ProgExec(NoBoolIntTypeErrProg);
		break;
	case 2: // AccumulatREfOut Выдать ссылку на аккумулятор
		{
			LoadPoint t = { Tint, AccumulatUk };
			Load.Write(t);
		}
		break;
	case 3: // AccumulatREfOutMk Выдать МК со ссылкой на аккумулятор
		MkExec(Load,{Cint, AccumulatUk});
		break;
	case 5: // Push Положить значенпие аккумулятора. Если нагрузка nil, то в аккумуляторе остается прежнее значение
		Stack.push_back({ AccumulatUk,Accumulat, AutoInc, Fin });
     	*AccumulatUk = Load.toInt(0);
		break;
	case 8: // Pop Вытолкнуть значение аккумулятора
		Load.Write(*AccumulatUk);
		AccumulatUk = Stack.back().AccumulatUk;
		Accumulat = Stack.back().Accumulat;
		AutoInc = Stack.back().AutoInc;
		Fin = Stack.back().Fin;
		Stack.pop_back();
		break;
	case 9: // PopMk Вытолкнуть значение аккумулятора
	{
		auto t = AccumulatUk;
		AccumulatUk = Stack.back().AccumulatUk;
		Accumulat = Stack.back().Accumulat;
		AutoInc = Stack.back().AutoInc;
		Fin = Stack.back().Fin;
		Stack.pop_back();
		MkExec(Load, { Tint, t });
		break;
	}
	case 20: // Out
		Load.Write(*AccumulatUk);
		*AccumulatUk += AutoInc;
		break;
	case 21: //OutMk
		MkExec(Load, { Cint, AccumulatUk });
		*AccumulatUk += AutoInc;
		break;
	case 25: // AutoIncSet Установить значение автоматического инкремента при операции считывания значения
		AutoInc = Load.toInt();
		break;
	case 27: // For Выполнить цикл с началом в аккумуляторе, шагом Autoinc и концом последовательностьи, заданным значением из аккумулятора, тело цикла в prog
	{
		void* t = Prog;
		if (Load.isDigitBool())
			Fin = Load.toInt();
		if (Load.isProg())
			t = Load.Point;
		if (AutoInc > 0)
			for (; *AccumulatUk < Fin; *AccumulatUk += AutoInc)
				ProgExec(t);
		else
			for (; *AccumulatUk > Fin; *AccumulatUk += AutoInc)
				ProgExec(t);
		break;
	}
	case 28: // ForInclude Выполнить цикл с началом в аккумуляторе, шагом Autoinc и концом последовательности включительно, заданным значением из аккумулятора, тело цикла в prog
	{	
		void* t = Prog;
		if (Load.isDigitBool())
			Fin = Load.toInt();
		if (Load.isProg())
			t = Load.Point;
		if (AutoInc > 0)
			for (; *AccumulatUk <= Fin; *AccumulatUk += AutoInc)
				ProgExec(t);
		else
			for (; *AccumulatUk >= Fin; *AccumulatUk += AutoInc)
				ProgExec(t);
		break;
	}
	case 29: //ForSet Установить значение для цикла for (цикл запускается с помощью МК  for или forInclude без нагрузки)
		Fin = Load.toInt();
		break;
	case 30: // ForOut Выдать конечное значение для цикла for
		Load.Write(Fin);
		break;
	case 31: // ForOutMk Выдать МК с конечным значением цикла for
		MkExec(Load, {Cint,&Fin});
		break;
	case 40: // CounterExec Выполнить программу столько раз, сколько записано в аккумуляторе
	{
		void* t = Prog;
		if (Load.isProg())
			t = Load.Point;
		if (Load.isDigitBool())
			*AccumulatUk = Load.toInt();
		if (*AccumulatUk >= 0)
			for (; *AccumulatUk > 0; (*AccumulatUk)--)
				ProgExec(t);
		break;
	}
	case 50: // NoCorrectTypeErrProgSet Программа для ошибки записи некорректного (нечислового типа)
		NoCorrectTypeErrProg = Load.Point;
		break;
	case 51: // NoIntTypeErrProg Программа для ошибки записи нецелого типа
		NoIntTypeErrProg = Load.Point;
		break;
	case 52: // NoBoolIntTypeErrProg Программа для ошибки записи нецелого типа или boolean
		NoBoolIntTypeErrProg = Load.Point;
		break;

	case 60: // EQExec
		if (*AccumulatUk != *CompareUk)
			ProgExec(Load);
		break;
	case 61: // NEQExec
		if (*AccumulatUk != *CompareUk)
			ProgExec(Load);
		break;
	case 62: // BiggerExec
		if (*AccumulatUk > *CompareUk)
			ProgExec(Load);
		break;
	case 63: // LessExec
		if (*AccumulatUk < *CompareUk)
			ProgExec(Load);
		break;
	case 64: // BiggerEQExec
		if (*AccumulatUk >= *CompareUk)
			ProgExec(Load);
		break;
	case 65: // LessEQExec
		if (*AccumulatUk <= *CompareUk)
			ProgExec(Load);
		break;
	case 66: // StackEmptyExec Выполнить, если в стеке ничего нет
	case 67: // StackFullExec Выполнить, если стек непустой
		if (MK == 66 && Stack.size() || MK == 67 && !Stack.size())
			ProgExec(Load);
		break;

	case 70: // EQProgSet
		EQProg = Load.Point;
		break;
	case 71: // NEQProgSet
		NEQProg = Load.Point;
		break;
	case 72: // BiggerProgSet
		BiggerProg = Load.Point;
		break;
	case 73: // LessProgSet
		LessProg = Load.Point;
		break;
	case 74: // BiggerEQProgSet
		BiggerEQProg = Load.Point;
		break;
	case 75: // LessEQProgSet
		LessEQProg = Load.Point;
		break;
	case 130: // Inc
		++ *AccumulatUk;
		break;
	case 131: // Dec
		-- *AccumulatUk;
		break;
	case 140: // Add
		if (!Load.isDigit()) ProgExec(NoCorrectTypeErrProg);
		if (!Load.isInt()) ProgExec(NoIntTypeErrProg);
		if (!Load.isIntBool()) ProgExec(NoBoolIntTypeErrProg);
		*AccumulatUk += Load.toInt();
		break;
	case 141: // Sub
		if (!Load.isDigit()) ProgExec(NoCorrectTypeErrProg);
		if (!Load.isInt()) ProgExec(NoIntTypeErrProg);
		if (!Load.isIntBool()) ProgExec(NoBoolIntTypeErrProg);
		*AccumulatUk -= Load.toInt();
		break;
	case 142: // Mul
		if (!Load.isDigit()) ProgExec(NoCorrectTypeErrProg);
		if (!Load.isInt()) ProgExec(NoIntTypeErrProg);
		if (!Load.isIntBool()) ProgExec(NoBoolIntTypeErrProg);
		*AccumulatUk *= Load.toInt();
		break;
	case 143: // Div
		if (!Load.isDigit()) ProgExec(NoCorrectTypeErrProg);
		if (!Load.isInt()) ProgExec(NoIntTypeErrProg);
		if (!Load.isIntBool()) ProgExec(NoBoolIntTypeErrProg);
		*AccumulatUk /= Load.toInt();
		break;
	case 144: // Mod
		if (!Load.isDigit()) ProgExec(NoCorrectTypeErrProg);
		if (!Load.isInt()) ProgExec(NoIntTypeErrProg);
		if (!Load.isIntBool()) ProgExec(NoBoolIntTypeErrProg);
		*AccumulatUk %= Load.toInt();
		break;
	default:
		CommonMk(MK, Load);
		break;
	}

	if (MK >= 130 || (MK == 8 || MK == 9) && AutoInc != 0) // Отработка программ сравнения значений
	{
		if (*AccumulatUk == *CompareUk) ProgExec(EQProg);
		if (*AccumulatUk != *CompareUk) ProgExec(NEQProg);
		if (*AccumulatUk >  *CompareUk) ProgExec(BiggerProg);
		if (*AccumulatUk <  *CompareUk) ProgExec(LessProg);
		if (*AccumulatUk >= *CompareUk) ProgExec(BiggerEQProg);
		if (*AccumulatUk <= *CompareUk) ProgExec(LessEQProg);
	}
}
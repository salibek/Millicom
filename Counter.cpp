#include "Counter.h"

void Counter::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case 0:// Reset
		AccumUk = &Accum;
		Accum = 0;
		AutoInc = 0;
		Compare = 0;
		CompareUk = &Compare;
		EmptyStackErrProg = nullptr; // Программа обработки ошибки pop из пустого стека
		BiggerProg = nullptr; LessProg = nullptr; EQProg = nullptr; NEQProg = nullptr; BiggerEQProg = nullptr; LessEQProg = nullptr;
		break;
	case 1: // Set Установить значение аккумулятора
		*AccumUk = Load.ToInt();
		break;
	case 5: // Push Положить значенпие аккумулятора. Если нагрузка nil, то в аккумуляторе остается прежнее значение
		Stack.push_back(*AccumUk);
		if (Load.Point != nullptr)
			*AccumUk = Load.ToInt();
	case 8: // Pop Вытолкнуть значение аккумулятора
		Load.Write(*AccumUk);
		*AccumUk = Stack.back();
		Stack.pop_back();
		break;
	case 9: // PopMk Вытолкнуть значение аккумулятора
		MkExec(Load, { Tint, AccumUk });
		*AccumUk = Stack.back();
		Stack.pop_back();
		break;
	case 20: // Out
		Load.Write(*AccumUk);
		*AccumUk += AutoInc;
		break;
	case 21: //OutMk
		MkExec(Load, { Tint, AccumUk });
		*AccumUk += AutoInc;
		break;
	case 25: // AutoIncSet Установить значение автоматического инкремента при операции считывания значения
		AutoInc = Load.ToInt();
		break;
	case 30: // Inc
		*AccumUk++;
		break;
	case 31: // Dec
		*AccumUk--;
		break;
	case 40: // Add
		*AccumUk += Load.ToInt();
		break;
	case 41: // Sub
		*AccumUk -= Load.ToInt();
		break;
	case 42: // Mul
		*AccumUk *= Load.ToInt();
		break;
	case 43: // Div
		*AccumUk /= Load.ToInt();
		break;
	case 44: // Mod
		*AccumUk %= Load.ToInt();
		break;
	case 50: // EQProgSet
		EQProg = Load.Point;
		break;
	case 51: // NEQProgSet
		NEQProg= Load.Point;
		break;
	case 52: // BiggerProgSet
		BiggerProg= Load.Point;
		break;
	case 53: // LessProgSet
		LessProg= Load.Point;
		break;
	case 54: // BiggerEQProgSet
		BiggerEQProg= Load.Point;
		break;
	case 55: // LessEQProgSet
		LessEQProg = Load.Point;
		break;

	case 60: // EQExec
		if (*AccumUk != *CompareUk)
			ProgExec(Load);
		break;
	case 61: // NEQExec
		if (*AccumUk != *CompareUk)
			ProgExec(Load);
		break;
	case 62: // BiggerExec
		if (*AccumUk > *CompareUk)
			ProgExec(Load);
		break;
	case 63: // LessExec
		if (*AccumUk < *CompareUk)
			ProgExec(Load);
		break;
	case 64: // BiggerEQExec
		if (*AccumUk >= *CompareUk)
			ProgExec(Load);
		break;
	case 65: // LessEQExec
		if (*AccumUk <= *CompareUk)
			ProgExec(Load);
		break;
	default:
		CommonMk(MK, Load);
		break;
	}

	if (MK >= 30 && MK <= 44 || (MK == 8 || MK == 9) && AutoInc != 0) // Отработка программ сравнения значений
	{
		if (*AccumUk == *CompareUk) ProgExec(EQProg);
		if (*AccumUk != *CompareUk) ProgExec(NEQProg);
		if (*AccumUk >  *CompareUk) ProgExec(BiggerProg);
		if (*AccumUk <  *CompareUk) ProgExec(LessProg);
		if (*AccumUk >= *CompareUk) ProgExec(BiggerEQProg);
		if (*AccumUk <= *CompareUk) ProgExec(LessEQProg);
	}
}
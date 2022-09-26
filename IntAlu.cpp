#include "IntAlu.h"

void IntAlu::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case 0:// Reset
		//AccumUk = &Accum;
		Accum = 0;
		AutoInc = 0;
		Compare = 0;
		CompareUk = &Compare;
		EmptyStackErrProg = nullptr; // ��������� ��������� ������ pop �� ������� �����
		BiggerProg = nullptr; LessProg = nullptr; EQProg = nullptr; NEQProg = nullptr; BiggerEQProg = nullptr; LessEQProg = nullptr;
		break;
	case 1: // Set ���������� �������� ������������
		*AccumUk = Load.toInt(0);
		if (!Load.isDigit()) ProgExec(NoCorrectTypeErrProg);
		if (!Load.isInt()) ProgExec(NoIntTypeErrProg);
		if (!Load.isIntBool()) ProgExec(NoBoolIntTypeErrProg);
		break;
	case 2: // AccumREfOut ������ ������ �� �����������
		{
			LoadPoint t = { Tint, &Accum };
			Load.Write(t);
		}
		break;
	case 3: // AccumREfOutMk ������ �� �� ������� �� �����������
		MkExec(Load,{Cint, &Accum});
		break;
	case 5: // Push �������� ��������� ������������. ���� �������� nil, �� � ������������ �������� ������� ��������
		Stack.push_back(*AccumUk);
     	*AccumUk = Load.toInt(0);

	case 8: // Pop ���������� �������� ������������
		Load.Write(*AccumUk);
		*AccumUk = Stack.back();
		Stack.pop_back();
		break;
	case 9: // PopMk ���������� �������� ������������
		MkExec(Load, { Tint, AccumUk });
		*AccumUk = Stack.back();
		Stack.pop_back();
		break;
	case 20: // Out
		Load.Write(*AccumUk);
		*AccumUk += AutoInc;
		break;
	case 21: //OutMk
		MkExec(Load, { Cint, AccumUk });
		*AccumUk += AutoInc;
		break;
	case 25: // AutoIncSet ���������� �������� ��������������� ���������� ��� �������� ���������� ��������
		AutoInc = Load.toInt();
		break;
	case 30: // CounterExec ��������� ��������� ������� ���, ������� �������� � ������������
		if (Accum >= 0)
			for (; Accum > 0; Accum--)
				ProgExec(Prog);
		break;
	case 50: // NoCorrectTypeErrProgSet ��������� ��� ������ ������ ������������� (����������� ����)
		NoCorrectTypeErrProg = Load.Point;
		break;
	case 51: // NoIntTypeErrProg ��������� ��� ������ ������ �������� ����
		NoIntTypeErrProg = Load.Point;
		break;
	case 52: // NoBoolIntTypeErrProg ��������� ��� ������ ������ �������� ���� ��� boolean
		NoBoolIntTypeErrProg = Load.Point;
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
	case 66: // StackEmptyExec ���������, ���� � ����� ������ ���
	case 67: // StackFullExec ���������, ���� ���� ��������
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
		++ *AccumUk;
		break;
	case 131: // Dec
		-- *AccumUk;
		break;
	case 140: // Add
		if (!Load.isDigit()) ProgExec(NoCorrectTypeErrProg);
		if (!Load.isInt()) ProgExec(NoIntTypeErrProg);
		if (!Load.isIntBool()) ProgExec(NoBoolIntTypeErrProg);
		*AccumUk += Load.toInt();
		break;
	case 141: // Sub
		if (!Load.isDigit()) ProgExec(NoCorrectTypeErrProg);
		if (!Load.isInt()) ProgExec(NoIntTypeErrProg);
		if (!Load.isIntBool()) ProgExec(NoBoolIntTypeErrProg);
		*AccumUk -= Load.toInt();
		break;
	case 142: // Mul
		if (!Load.isDigit()) ProgExec(NoCorrectTypeErrProg);
		if (!Load.isInt()) ProgExec(NoIntTypeErrProg);
		if (!Load.isIntBool()) ProgExec(NoBoolIntTypeErrProg);
		*AccumUk *= Load.toInt();
		break;
	case 143: // Div
		if (!Load.isDigit()) ProgExec(NoCorrectTypeErrProg);
		if (!Load.isInt()) ProgExec(NoIntTypeErrProg);
		if (!Load.isIntBool()) ProgExec(NoBoolIntTypeErrProg);
		*AccumUk /= Load.toInt();
		break;
	case 144: // Mod
		if (!Load.isDigit()) ProgExec(NoCorrectTypeErrProg);
		if (!Load.isInt()) ProgExec(NoIntTypeErrProg);
		if (!Load.isIntBool()) ProgExec(NoBoolIntTypeErrProg);
		*AccumUk %= Load.toInt();
		break;
	default:
		CommonMk(MK, Load);
		break;
	}

	if (MK >= 130 || (MK == 8 || MK == 9) && AutoInc != 0) // ��������� �������� ��������� ��������
	{
		if (*AccumUk == *CompareUk) ProgExec(EQProg);
		if (*AccumUk != *CompareUk) ProgExec(NEQProg);
		if (*AccumUk >  *CompareUk) ProgExec(BiggerProg);
		if (*AccumUk <  *CompareUk) ProgExec(LessProg);
		if (*AccumUk >= *CompareUk) ProgExec(BiggerEQProg);
		if (*AccumUk <= *CompareUk) ProgExec(LessEQProg);
	}
}
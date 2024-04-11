#include "IntAlu.h"

void IntAlu::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	switch (MK)
	{
	case 0:// Reset
		AccumulatUk = &Accumulat;
		Accumulat = 0;
		AutoInc = 0;
		Compare = 0;
		CompareUk = &Compare;
		EmptyStackErrProg = nullptr; // ��������� ��������� ������ pop �� ������� �����
		BiggerProg = nullptr; LessProg = nullptr; EQProg = nullptr; NEQProg = nullptr; BiggerEQProg = nullptr; LessEQProg = nullptr;
		break;
	case 1: // Set ���������� �������� ������������
		*AccumulatUk = Load.toInt(0);
		if (!Load.isDigit()) ProgExec(NoCorrectTypeErrProg);
		if (!Load.isInt()) ProgExec(NoIntTypeErrProg);
		if (!Load.isIntBool()) ProgExec(NoBoolIntTypeErrProg);
		break;
	case 2: // AccumREfOut ������ ������ �� �����������
		{
			LoadPoint t = { Cint, AccumulatUk };
			Load.Write(t);
		}
		break;
	case 3: // AccumulatREfOutMk ������ �� �� ������� �� �����������
		MkExec(Load,{Cint, AccumulatUk});
		break;
	case 5: // Push �������� ��������� ������������. ���� �������� nil, �� � ������������ �������� ������� ��������
		Stack.push_back({ AccumulatUk,Accumulat, AutoInc, Fin });
     	*AccumulatUk = Load.toInt(0);
		break;
	case 8: // Pop ���������� �������� ������������
		Load.Write(*AccumulatUk);
		AccumulatUk = Stack.back().AccumulatUk;
		Accumulat = Stack.back().Accumulat;
		AutoInc = Stack.back().AutoInc;
		Fin = Stack.back().Fin;
		Stack.pop_back();
		break;
	case 9: // PopMk ���������� �������� ������������
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
	case 10: // RandFromSet ���������� ��������� �������� ��������� ��������� �����
		RandFrom = Load.toInt();
		break;
	case 11: // RandToSet ���������� �������� �������� ��������� ��������� �����
		RandTo = Load.toInt()+1;
		break;
	case 12: // RandOut ������ ��������������� �����, ���� � Load=nil, �� �������� � �����������
		if (Load.Point == nullptr)
			*Stack.back().AccumulatUk = RandFrom + rand() % (RandFrom - RandTo);
		else
			Load.Write(RandFrom + rand()%(RandFrom-RandTo));
		break;
	case 13: // RandOutMk ������ �� �� ��������������� ������, ���� � Load=nil, �� �������� � �����������
	{
		int t = RandFrom + rand() % (RandFrom - RandTo);
		if (Load.Point == nullptr)
			*Stack.back().AccumulatUk = t;
		else
			MkExec(Load, { Cint, &t });
	}
		break;
	case 14: // Rand ������������� ��������� ����� � ��������� �� 0 �� ����� �� ��������
		*Stack.back().AccumulatUk = rand() % (Load.toInt()+1);
		break;

	case 20: // Out
		Load.Write(*AccumulatUk);
		*AccumulatUk += AutoInc;
		break;
	case 21: //OutMk
		MkExec(Load, { Cint, AccumulatUk });
		*AccumulatUk += AutoInc;
		break;
	case 25: // AutoIncSet ���������� �������� ��������������� ���������� ��� �������� ���������� ��������
		AutoInc = Load.toInt();
		break;
	case 27: // For ��������� ���� � ������� � ������������, ����� Autoinc � ������ �������������������, �������� ��������� �� ������������, ���� ����� � prog
	{
		int AutoIncArch = AutoInc; // ��������� �������������
		AutoInc = 0; // ��������� �����������������
		void* t = Prog;
		if (Load.isDigitBool())
			Fin = Load.toInt();
		if (Load.isProg())
			t = Load.Point;
		if (AutoIncArch > 0)

			for (; *AccumulatUk < Fin; *AccumulatUk += AutoIncArch)
				ProgExec(t);
		else
			for (; *AccumulatUk > Fin; *AccumulatUk += AutoIncArch)
				ProgExec(t);
		AutoInc = AutoIncArch; // ������������ ��������� �����������������
		break;
	}
	case 28: // ForInclude ��������� ���� � ������� � ������������, ����� Autoinc � ������ ������������������ ������������, �������� ��������� �� ������������, ���� ����� � prog
	{	
		int AutoIncArch = AutoInc; // ��������� �������������
		AutoInc = 0; // ��������� �����������������
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
		AutoInc = AutoIncArch; // ������������ ��������� �����������������
		break;
	}
	case 29: //ForSet ���������� �������� ��� ����� for (���� ����������� � ������� ��  for ��� forInclude ��� ��������)
		Fin = Load.toInt();
		break;
	case 30: // ForOut ������ �������� �������� ��� ����� for
		Load.Write(Fin);
		break;
	case 31: // ForOutMk ������ �� � �������� ��������� ����� for
		MkExec(Load, {Cint,&Fin});
		break;
	case 40: // CounterExec ��������� ��������� ������� ���, ������� �������� � ������������
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
		CommonMk(MK, Load, Sender);
		break;
	}

	if (MK >= 130 || (MK == 8 || MK == 9) && AutoInc != 0) // ��������� �������� ��������� ��������
	{
		if (*AccumulatUk == *CompareUk) ProgExec(EQProg);
		if (*AccumulatUk != *CompareUk) ProgExec(NEQProg);
		if (*AccumulatUk >  *CompareUk) ProgExec(BiggerProg);
		if (*AccumulatUk <  *CompareUk) ProgExec(LessProg);
		if (*AccumulatUk >= *CompareUk) ProgExec(BiggerEQProg);
		if (*AccumulatUk <= *CompareUk) ProgExec(LessEQProg);
	}
}

FU* IntAlu::Copy() // ��������� ����������� ��
{
	return new IntAlu(Bus, this);
}

FU* IntAlu::TypeCopy() // ������� �� ������ �� ���� (�� ������� ��������
{
	return new IntAlu(Bus, nullptr);
}

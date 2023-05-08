#include "stdafx.h"
#include "Consts.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include "LocationTable.h"
#include "ALU.h"
#include "IntAlu.h"
#include "FUini.h"

using namespace std;

unsigned int LoadPoint::getType() // ������ ��� ��������
{
	if (Type >> 1 == DLoadVectInd)
		return ((LoadVect_type)Point)->at(Ind).Type;
	if (Type >> 1 != DICInd)
		return Type;
	else
		switch (Ind % 3) {
		case 0: return TIP;
		case 1: return Tint;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return -1; return ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
}

int LoadPoint::DataSize() // ������ ������ ������ � ��������
{
	LoadPoint LP = *this;
	while (LP.Type >> 1 == DLoadVectInd || LP.Type >> 1 == DICInd)
	{
		if (LP.Type >> 1 == DLoadVectInd)
			LP = ((LoadVect_type)Point)->at(LP.Ind);
		else
			if (LP.Ind < 0 || LP.Ind / 3 >= ((IC_type)LP.Point)->size()) return 2; // ������ �������
			else switch (LP.Ind % 3) {
			case 0: LP = { TIP, &((IC_type)LP.Point)->at(Ind / 3) }; break;
			case 1: LP = { Tint, &((IC_type)LP.Point)->at(Ind / 3).atr }; break;
			case 2: if (((IC_type)LP.Point)->at(Ind / 3).Load.Point == nullptr) return 1; LP = ((IC_type)LP.Point)->at(Ind / 3).Load;
			}
	}
	switch (Type >> 1)
	{
	case Dbool: return 1;
	case Dchar: return 2;
	case Tmk:
	case Dint:
	case Dfloat: return 4;
	case Ddouble: return 8;
	case DFU: return SizeOfFUType(((FU*)LP.Point)->FUtype);
	case DIP: return 4 + ((ip*)LP.Point)->Load.DataSize();
	case DLoad: return sizeof(LoadPoint);
	case Dstring: return sizeof(string) + ((string*)LP.Point)->length();
	case DLoadVect: {
		register int s = 0, t = sizeof(LoadPoint);
		for (auto& i : *((LoadVect_type)LP.Point))
			s += t + i.DataSize();
	}
	case TCalc:
	case TProg:
	case TGraph:
	case DIC: {
		register int s = 0, t = sizeof(ip);
		for (auto& i : *((IC_type)LP.Point))
			s += t + i.Load.DataSize();
	}
	}
	return 0;
}

bool LoadPoint::isDigit() {
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return true;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	return t >> 1 == Dint || t >> 1 == Dchar || t >> 1 == Dfloat || t >> 1 == Ddouble;
}; // �����?}

bool LoadPoint::isDigitBool() {
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return true;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	return t >> 1 == Dint || t >> 1 == Dchar || t >> 1 == Dfloat || t >> 1 == Ddouble || t >> 1 == Dbool;
}; // ����� ��� ������?

bool LoadPoint::isBool() {
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return false;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	return t >> 1 == Dbool;
}; // ������?

bool LoadPoint::isInt() {
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return true;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	return t >> 1 == Dint;
}; // ����� �����?

bool LoadPoint::isIntBool() {
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return true;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	return t >> 1 == Dint || t >> 1 == Dbool;
}; // ������ ��� ����� �����?

bool LoadPoint::isFloatDouble() {
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return false;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	return t >> 1 == Ddouble || t >> 1 == Dfloat;
};

bool LoadPoint::isFloat() {
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return false;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	return t >> 1 == Dfloat;
};

bool LoadPoint::isDouble() {
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return false;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	return t >> 1 == Ddouble;
};

bool LoadPoint::isProg() {
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return false;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	return t >> 1 == DIC || t >> 1 == DCalc || t >> 1 == DProg;
}; // ����������� ����� �� ���� �������� ����������

bool LoadPoint::isStrChar() {
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return false;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	return Point != nullptr && (t >> 1 == Dstring || t >> 1 == Dchar);
}; // ������ ��� ������?

bool LoadPoint::isStr() {
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return false;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	return Point != nullptr && t >> 1 == Dstring;
}; // ������?

bool LoadPoint::isChar() {
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return false;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	return Point != nullptr && t >> 1 == Dchar;
}; // ������?

bool LoadPoint::isMk() {
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return (Ind / 3 >= ((IC_type)Point)->at(Ind / 3).atr >= 0) ? true : false; // ���� ������� ���������������, �� �� ��������� �������������
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	return Point != nullptr && t >> 1 == DMk;
}; // ������������?

bool LoadPoint::isVect() {
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return false;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	return t >> 1 == DLoadVect;
}; // ������ ?

bool LoadPoint::isVectIndVectInd()  // ��������������� ������� ������� �������� �� ���������������� ������� ��������
{
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return false;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	return (t >> 1) == DLoadVectInd;
}

bool LoadPoint::isIC() // ���������� ��������� �� ������ �� ��
{
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return false;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	set<unsigned int> IC_Types = { DIC, DCalc, DProg }; // ���������� ����� �� � ��-������
	return IC_Types.count(t >> 1);
}

bool LoadPoint::isIP() // ���������� ��������� �� ������ �� ��
{
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return true;
		case 1: return false;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	set<unsigned int> IP_Types = { DIP }; // ���������� ����� �� � ��-������
	return IP_Types.count(t >> 1);
}

bool LoadPoint::isCalc() // ���������� ��������� �� ������ �� �� � ���
{
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return false;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	set<unsigned int> IP_Types = { DCalc }; // ���������� ����� �� � ��-������
	return IP_Types.count(t >> 1);
}

bool LoadPoint::isICSet() // ���������� ��������� �� ������ �� �� � ��������������� ����������
{
	if (Point == nullptr) return false;
	register unsigned int t = Type;
	if (Type >> 1 == DLoadVect) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return false;
		t = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Type;
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return false;
		else switch (Ind % 3) {
		case 0: return false;
		case 1: return false;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return false; t = ((IC_type)Point)->at(Ind / 3).Load.Type;
		}
	set<unsigned int> IP_Types = { DIC }; // ���������� ����� �� � ��-������
	return IP_Types.count(t >> 1);
}

LoadPoint LoadPoint::IndLoadReturn() // ���������� ��������� �� �������� ���������������� ��������
{
	if (Point == nullptr)
		return *this;
	LoadPoint LP = *this;
	while (LP.isInd())
	{
		if (LP.Type >> 1 == DLoadVectInd) {
			register int N = ((LoadVect_type)LP.Point)->size();
			if (N <= LP.Ind || N < -LP.Ind || ((LoadVect_type)LP.Point)->at(N * (LP.Ind < 0) + LP.Ind).Point == nullptr) return {};
			LP = ((LoadVect_type)LP.Point)->at(N * (LP.Ind < 0) + LP.Ind);
		}
		else if (LP.Type >> 1 == DICInd)
			if (LP.Ind < 0 || LP.Ind / 3 >= ((IC_type)LP.Point)->size()) return {};
			else switch (LP.Ind % 3) {
			case 0: LP = { TIP, &((IC_type)LP.Point)->at(LP.Ind / 3) }; break;
//			case 1: LP = LP.IC()->at(LP.Ind / 3).Load;  break;
			case 1:LP = { Tint, &((IC_type)LP.Point)->at(LP.Ind / 3).atr }; break;
			case 2: LP = ((IC_type)LP.Point)->at(LP.Ind / 3).Load;  break;
			}
	}
	return LP;
}

LoadPoint LoadPoint::IndLoadReturn(LoadPoint LP) // ���������� ��������� �� �������� ���������������� ��������
{
	return LP.IndLoadReturn();
}

string LoadPoint::toStr(string define) // ������� � bool
{
	if (Point == nullptr)
		return define;
	LoadPoint LP = *this;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return "";
		LP = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind);
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return define;
		else switch (Ind % 3) {
		case 0: return define;
		case 1: return to_string(*(int*)((IC_type)Point)->at(Ind / 3).Load.Point);
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return ""; LP = ((IC_type)Point)->at(Ind / 3).Load;
		}
	switch (LP.Type >> 1) {
	case Dstring: return *((string*)LP.Point);
	case Ddouble: return to_string(*(double*)LP.Point); break;
	case Dfloat: return to_string(*(float*)LP.Point); break;
	case Dint: return to_string(*(int*)LP.Point); break;
	case Dbool: return to_string(*(bool*)LP.Point); break;
	default:
		return define;
		break;
	}
}

bool LoadPoint::toBool(bool define) // ������� � bool (�� ��������� false)
{
	if (Point == nullptr)
		return define;
	LoadPoint LP = *this;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return define;
		LP = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind);
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return define;
		else switch (Ind % 3) {
		case 0: return define;
		case 1: return define;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return define; LP = ((IC_type)Point)->at(Ind / 3).Load;
		}
	switch (LP.Type >> 1)
	{
	case Ddouble: return *(double*)LP.Point;
	case Dfloat: return *(float*)LP.Point;
	case Dint: return *(int*)LP.Point;
	case Dbool: return *(bool*)LP.Point;
	case Dstring: return !((*(string*)LP.Point) == "");
	default:
		return define;
	}
}

int LoadPoint::toInt(int define) { // ������� � integer
	if (Point == nullptr)
		return define;
	LoadPoint LP = *this;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return define;
		LP = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind);
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return define;
		else switch (Ind % 3) {
		case 0: return define;
		case 1: return ((IC_type)Point)->at(Ind / 3).atr;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return define; LP = ((IC_type)Point)->at(Ind / 3).Load;
		}
	switch (LP.Type >> 1)
	{
	case Ddouble: return (int)*(double*)LP.Point; break;
	case Dfloat: return (int)*(float*)LP.Point; break;
	case Dint: return *(int*)LP.Point; break;
	case Dbool: return *(bool*)LP.Point; break;
	case Dstring: return atoi((*(string*)LP.Point).c_str());
	default:
		return define;
		break;
	}
}
double LoadPoint::toDouble(double define) {// ������� � integer
	if (Point == nullptr)
		return define;
	LoadPoint LP = *this;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return define;
		LP = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind);
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return define;
		else switch (Ind % 3) {
		case 0: return define;
		case 1: return ((IC_type)Point)->at(Ind / 3).atr;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return define; LP = ((IC_type)Point)->at(Ind / 3).Load;
		}
	switch (LP.Type >> 1)
	{
	case Ddouble: return *(double*)LP.Point; break;
	case Dfloat: return *(float*)LP.Point; break;
	case Dint: return *(int*)LP.Point; break;
	case Dbool: return *(bool*)LP.Point; break;
	case Dstring: return atof((*(string*)LP.Point).c_str());
	default:
		return define;
		break;
	}
}

float LoadPoint::toFloat(float define) {// ������� � integer
	if (Point == nullptr)
		return define;
	LoadPoint LP = *this;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return define;
		LP = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind);
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return define;
		else switch (Ind % 3) {
		case 0: return define;
		case 1: return ((IC_type)Point)->at(Ind / 3).atr;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return define; LP = ((IC_type)Point)->at(Ind / 3).Load;
		}
	switch (LP.Type >> 1)
	{
	case Ddouble: return *(double*)LP.Point; break;
	case Dfloat: return *(float*)LP.Point; break;
	case Dint: return *(int*)LP.Point; break;
	case Dbool: return *(bool*)LP.Point; break;
	case Dstring: return atof((*(string*)LP.Point).c_str());
	default:
		return define;
		break;
	}
};
// ������ � �������� ��
int LoadPoint::Write(vector<double> x) //
{
	if (Point == nullptr)
		return 1;
	if (Type == CdoubleArray)
	{
		*(vector<double>*)Point = x;
		return 0;
	}
	else
		return 1;
}
int LoadPoint::Write(vector<float> x) //
{
	if (Point == nullptr)
		return 1;
	if (Type == CdoubleArray)
	{
		*(vector<float>*)Point = x;
		return 0;
	}
	else
		return 1;
}
int LoadPoint::Write(vector<bool> x) // ����������� �������
{
	if (Point == nullptr)
		return 1;
	if (Type == CdoubleArray)
	{
		*(vector<bool>*)Point = x;
		return 0;
	}
	else
		return 1;
}
int LoadPoint::Write(vector<char> x) // ����������� �������
{
	if (Point == nullptr)
		return 1;
	if (Type == CdoubleArray)
	{
		*(vector<char>*)Point = x;
		return 0;
	}
	else
		return 1;
}
int  LoadPoint::Write(vector<int> x) // ����������� �������
{
	if (Point == nullptr)
		return 1;
	if (Type == CdoubleArray)
	{
		*(vector<int>*)Point = x;
		return 0;
	}
	else
		return 1;
}
// -----

int LoadPoint::WriteFromLoad(LoadPoint Load) // �������� �������� �� ��������
{
	if (Point == nullptr) return 1;
	LoadPoint LP = *this;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return 2; // ������ �������
		LP = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind);
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return 2; // ������ �������
		else switch (Ind % 3) {
		case 0: LP = { TIP, &((IC_type)Point)->at(Ind / 3) }; break;
		case 1: LP = { Tint, &((IC_type)Point)->at(Ind / 3).atr }; break;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return 1; LP = ((IC_type)Point)->at(Ind / 3).Load;
		}
	switch (LP.Type)
	{
	case Tdouble:
		*((double*)LP.Point) = Load.toDouble();
		break;
	case Tfloat:
		*((float*)LP.Point) = Load.toFloat();
		break;
	case Tint:
		*((int*)LP.Point) = Load.toInt();
		break;
	case Tbool:
		*((bool*)LP.Point) = Load.toBool();
		break;
	case Tchar:
		*((char*)LP.Point) = Load.toChar();
		break;
	case Tstring:
		*((string*)LP.Point) = Load.toStr();
		break;
	case TIP:
		//...
		break;
	case TIC:
		//...
		break;
	default: // ���������� ���������
		if (Type % 2 != 0) // ���� ��� ����������
			return 2; // ������: ������� ������ � ���������
		else
			if (Type >> 1 != Load.Type >> 1)
				return 1; // ������: ������������� �����
			else
				Point = Load.Point;
	}
	return 0;
	/*
		if (Point == nullptr)
			return 1;
		register unsigned int t = Type;
		register void* P = Point;
		if (Ind >= 0)
			if (Type == TLoadVect) {
				if (Ind >= ((LoadVect_type)Point)->size()) return 2; // ������ �������
				P = ((LoadVect_type)Point)->at(Ind).Point;
				t = ((LoadVect_type)Point)->at(Ind).Type;
			}
			else if (Type == TIC){
				if (Ind/3 >= ((IC_type)Point)->size()) return 2; // ������ �������
				switch (Ind % 3) {
					case 0: P = &((IC_type)P)->at(Ind / 3); t = TIP; break;
					case 1: P = &((IC_type)P)->at(Ind / 3).atr; t = Tint; break;
					case 2: P = ((IC_type)P)->at(Ind / 3).Load.Point; t = ((IC_type)P)->at(Ind / 3).Load.Type;
				}
			}
			else return 2; // ������ �������
		if (P == nullptr || t % 2 != 0) return 1;
		switch (t)
		{
		case Tdouble:
			*((double*)P) = Load.toDouble();
			break;
		case Tfloat:
			*((float*)P) = Load.toFloat();
			break;
		case Tint:
			*((int*)P) = Load.toInt();
			break;
		case Tbool:
			*((bool*)P) = Load.toBool();
			break;
		case Tchar:
			*((char*)P) = Load.toChar();
			break;
		case Tstring:
			*((string*)P) = Load.toStr();
			break;
		default: // ���������� ���������
			if (Type % 2 != 0) // ���� ��� ����������
				return 2; // ������: ������� ������ � ���������
			else
				if (Type >> 1 != Load.Type >> 1)
					return 1; // ������: ������������� �����
				else
					Point = Load.Point;
		}
		*/
	return 0;
}

int LoadPoint::Write(size_t x)
{
	if (Point == nullptr) return 1;
	LoadPoint LP = *this;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return 2; // ������ �������
		LP = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind);
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return 2; // ������ �������
		else switch (Ind % 3) {
		case 0: LP = { TIP, &((IC_type)Point)->at(Ind / 3) }; break;
		case 1: LP = { Tint, &((IC_type)Point)->at(Ind / 3).atr }; break;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return 1; LP = ((IC_type)Point)->at(Ind / 3).Load;
		}
	switch (LP.Type)
	{
	case Tdouble:
		*((double*)LP.Point) = x;
		break;
	case Tfloat:
		*((float*)LP.Point) = x;
		break;
	case Tint:
		*((int*)LP.Point) = x;
		break;
	case Tbool:
		*((bool*)LP.Point) = x;
		break;
	case Tchar:
		*((char*)LP.Point) = x;
		break;
	case Tstring:
		*((string*)LP.Point) = to_string(x);
		break;

	default:
		return 2; // �������������� �����
	}
	return 0;
}

int LoadPoint::Write(int x)
{
	if (Point == nullptr) return 1;
	LoadPoint LP = *this;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return 2; // ������ �������
		LP = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind);
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return 2; // ������ �������
		else switch (Ind % 3) {
		case 0: LP = { TIP, &((IC_type)Point)->at(Ind / 3) }; break;
		case 1: LP = { Tint, &((IC_type)Point)->at(Ind / 3).atr }; break;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return 1; LP = ((IC_type)Point)->at(Ind / 3).Load;
		}
	switch (LP.Type)
	{
	case Tdouble:
		*((double*)LP.Point) = x;
		break;
	case Tfloat:
		*((float*)LP.Point) = x;
		break;
	case Tint:
		*((int*)LP.Point) = x;
		break;
	case Tbool:
		*((bool*)LP.Point) = x;
		break;
	case Tchar:
		if (x >= 0 && x < 256)
			*((char*)LP.Point) = x;
		else
			return 1; // �������������� �����
		break;
	case Tstring:
		*((string*)LP.Point) = to_string(x);
		break;
	default:
		return 1; // �������������� �����
	}
	return 0;
}

int LoadPoint::Write(double x)
{
	if (Point == nullptr) return 1;
	LoadPoint LP = *this;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return 2; // ������ �������
		LP = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind);
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return 2; // ������ �������
		else switch (Ind % 3) {
		case 0: LP = { TIP, &((IC_type)Point)->at(Ind / 3) }; break;
		case 1: LP = { Tint, &((IC_type)Point)->at(Ind / 3).atr }; break;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return 1; LP = ((IC_type)Point)->at(Ind / 3).Load;
		}
	switch (LP.Type)
	{
	case Tdouble:
		*((double*)LP.Point) = x;
		break;
	case Tfloat:
		*((float*)LP.Point) = x;
		break;
	case Tint:
		*((int*)LP.Point) = x;
		break;
	case Tbool:
		*((bool*)LP.Point) = x;
		break;
	case Tchar:
		*((char*)LP.Point) = x;
		break;
	case Tstring:
		*((string*)LP.Point) = to_string(x);
		break;
	defoult:
		return 1; // �������������� �����
	}
	return 0;
}

int LoadPoint::Write(float x)
{
	if (Point == nullptr) return 1;
	LoadPoint LP = *this;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return 2; // ������ �������
		LP = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind);
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return 2; // ������ �������
		else switch (Ind % 3) {
		case 0: LP = { TIP, &((IC_type)Point)->at(Ind / 3) }; break;
		case 1: LP = { Tint, &((IC_type)Point)->at(Ind / 3).atr }; break;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return 1; LP = ((IC_type)Point)->at(Ind / 3).Load;
		}
	switch (LP.Type >> 1)
	{
	case Tdouble:
		*((double*)LP.Point) = x;
		break;
	case Tfloat:
		*((float*)LP.Point) = x;
		break;
	case Tint:
		*((int*)LP.Point) = x;
		break;
	case Tbool:
		*((bool*)LP.Point) = x;
		break;
	case Tchar:
		*((char*)LP.Point) = x;
		break;
	case Tstring:
		*((string*)LP.Point) = to_string(x);
		break;
	defoult:
		return 1; // �������������� �����
	}
	return 0;
}

int LoadPoint::Write(bool x)
{
	if (Point == nullptr) return 1;
	LoadPoint LP = *this;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return 2; // ������ �������
		LP = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind);
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return 2; // ������ �������
		else switch (Ind % 3) {
		case 0: LP = { TIP, &((IC_type)Point)->at(Ind / 3) }; break;
		case 1: LP = { Tint, &((IC_type)Point)->at(Ind / 3).atr }; break;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return 1; LP = ((IC_type)Point)->at(Ind / 3).Load;
		}
	switch (LP.Type)
	{
	case Tdouble:
		*((double*)LP.Point) = x;
		break;
	case Tfloat:
		*((float*)LP.Point) = x;
		break;
	case Tint:
		*((int*)LP.Point) = x;
		break;
	case Tbool:
		*((bool*)LP.Point) = x;
		break;
	case Tchar:
		*((char*)LP.Point) = x;
		break;
	case Tstring:
		*((string*)LP.Point) = to_string(x);
		break;
	defoult:
		return 1; // �������������� �����
	}
	return 0;
}

int LoadPoint::Write(char x)
{
	if (Point == nullptr) return 1;
	LoadPoint LP = *this;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return 2; // ������ �������
		LP = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind);
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return 2; // ������ �������
		else switch (Ind % 3) {
		case 0: LP = { TIP, &((IC_type)Point)->at(Ind / 3) }; break;
		case 1: LP = { Tint, &((IC_type)Point)->at(Ind / 3).atr }; break;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return 1; LP = ((IC_type)Point)->at(Ind / 3).Load;
		}
	switch (LP.Type)
	{
	case Tdouble:
		*((double*)LP.Point) = x;
		break;
	case Tfloat:
		*((float*)LP.Point) = x;
		break;
	case Tint:
		*((int*)LP.Point) = x;
		break;
	case Tbool:
		*((bool*)LP.Point) = x;
		break;
	case Tchar:
		*((char*)LP.Point) = x;
		break;
	case Tstring:
		*((string*)LP.Point) = x;
		break;
	defoult:
		return 1; // �������������� �����
	}
	return 0;
}

int LoadPoint::Write(string x)
{
	if (Point == nullptr) return 1;
	LoadPoint LP = *this;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return 2; // ������ �������
		LP = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind);
	}
	else if (Type >> 1 == DICInd)
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return 2; // ������ �������
		else switch (Ind % 3) {
		case 0: LP = { TIP, &((IC_type)Point)->at(Ind / 3) }; break;
		case 1: LP = { Tint, &((IC_type)Point)->at(Ind / 3).atr }; break;
		case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return 1; LP = ((IC_type)Point)->at(Ind / 3).Load;
		}
	switch (LP.Type)
	{
	case Tdouble:
		*((double*)LP.Point) = atof(x.c_str());
		break;
	case Tfloat:
		*((float*)LP.Point) = atof(x.c_str());
		break;
	case Tint:
		*((int*)LP.Point) = atoi(x.c_str());
		break;
	case Tbool:
		*((bool*)LP.Point) = atoi(x.c_str());
		break;
	case Tchar:
		*((char*)LP.Point) = atoi(x.c_str());
		break;
	case Tstring:
		*((string*)LP.Point) = x;
		break;
	defoult:
		return 1; // �������������� �����
	}
	return 0;
}

int LoadPoint::Write(LoadPoint x) // �������� �������� �� ��������
{
	if (Point == nullptr) return 1;
	LoadPoint LP = *this;
	if (Type >> 1 == DLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return 2; // ������ �������
		((LoadVect_type)Point)->at(N * (Ind < 0) + Ind) = x; return 0;
	}
	else if (Type >> 1 == DICInd) {
		if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return 2; // ������ �������
		if (Ind % 3 != 2)
			return 1;
		else
		{
			((IC_type)Point)->at(Ind / 3).Load = x.Clone();
			return 0;
		}
	}
	LP = x.Clone();
	return 0;
}
template <typename T>
int LoadPoint::WriteByVector(T x) // ������ �� ��������� � �������
{
	if (Point == nullptr) return 1;
	if (!isVect()) return 1;
	for (auto& i : *((vector<LoadPoint>*) Point))
		Write(i);
	return 0;
};

int LoadPoint::Write(vector<LoadPoint>* x)
{
	register unsigned int t = Type;
	register void* P = Point;
	if (Type == TLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return 2; // ������ �������
		P = ((LoadVect_type)Point)->at(Ind).Point;
		t = ((LoadVect_type)Point)->at(Ind).Type;
	}
	else if (Type == TICInd) {
		if (Ind / 3 >= ((LoadVect_type)Point)->size()) return 2; // ������ �������
		switch (Ind % 3) {
		case 0: P = &((IC_type)P)->at(Ind / 3); t = TIP; break;
		case 1: P = &((IC_type)P)->at(Ind / 3).atr; t = Tint; break;
		case 2: P = ((IC_type)P)->at(Ind / 3).Load.Point; t = ((IC_type)P)->at(Ind / 3).Load.Type;
		}
	}
	else return 2; // ������ �������
	if (P == nullptr || t % 2 != 0) return 1;
	P = x; t = TLoadVect;
	return 0;
}

int LoadPoint::Write(vector<LoadPoint> x) // ����������� ������� �������� ��������
{
	if (Point == nullptr || !isVect() || Type % 2 != 0)
		return 1;
	register unsigned int t = Type;
	register void* P = Point;

	if (Type == TLoadVectInd) {
		register int N = ((LoadVect_type)Point)->size();
		if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return 2; // ������ �������
		P = ((LoadVect_type)Point)->at(Ind).Point;
		t = ((LoadVect_type)Point)->at(Ind).Type;
	}
	else if (Type == TICInd) {
		if (Ind / 3 >= ((LoadVect_type)Point)->size()) return 2; // ������ �������
		switch (Ind % 3) {
		case 0: P = &((IC_type)P)->at(Ind / 3); t = TIP; break;
		case 1: P = &((IC_type)P)->at(Ind / 3).atr; t = Tint; break;
		case 2: P = ((IC_type)P)->at(Ind / 3).Load.Point; t = ((IC_type)P)->at(Ind / 3).Load.Type;
		}
	}
	else return 2; // ������ �������
	if (P == nullptr || t % 2 != 0) return 1;

	((LoadVect_type)P)->resize(x.size());
	copy(x.begin(), x.end(), ((LoadVect_type)P)->begin());
	return 0;
}

unsigned int  LoadPoint::TypeMinimize(double x) // �������������� ���, �.�. ���� ����� ����� - ������������ int � �.�.
{
	if (x == int(x) and abs(x) < 2147483647)
		return Tint;
	else if (x == 1 || x == 0)
		return Tbool;
	else
		return Tdouble;

}

LoadPoint LoadPoint::TypeMinimizeOut(double x, bool var) // �������������� ��� (������������ LoadPoint), �.�. ���� ����� ����� - ������������ int � �.�.
{
	if (x == int(x) and abs(x) < 2147483647)
	{
		int* t = new int;
		return { (var ? Tint : Cint), t };
	}
	else if (x == 1 || x == 0)
	{
		bool* t = new bool;
		return { (var ? Tbool : Cbool), t };
	}
	else
	{
		double* t = new double;
		return { (var ? Tdouble : Cdouble), t };
	}
}

LoadPoint LoadPoint::Clone(LoadPoint LP) // ����������� �������� (������� � ������������ � �������� ��������� ��������)
{
	return LP.Clone();
}

LoadPoint LoadPoint::Clone(bool All) // ������� ������������� ��������
{
	if (Point == nullptr)
		return { 0,nullptr };
	if (Type % 2 == 0 && !All) // ���� ����������, �� ���������� ���������
		return *this;

	LoadPoint LP = *this;
	while (LP.Point != nullptr && (Type >> 1 == DLoadVectInd || Type >> 1 == DICInd))
		if (Type >> 1 == DLoadVectInd)
			if (Type == TLoadVectInd && !All)
				return LP;
			else
				LP = ((LoadVect_type)Point)->at(Ind);
		else if (LP.Type >> 1 == DICInd)
			if (LP.Ind < 0 || LP.Ind / 3 >= ((IC_type)Point)->size()) return {}; // ������ �������
			else if (LP.Type == CICInd && !All)
				return LP;
			else switch (Ind % 3) {
			case 0: LP = { TIP, &((IC_type)Point)->at(Ind / 3) }; break;
			case 1: LP = { Tint, &((IC_type)Point)->at(Ind / 3).atr }; break;
			case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return {}; LP = ((IC_type)Point)->at(Ind / 3).Load;
			}
	switch (LP.Type >> 1)
	{
	case Dstring: return { LP.Type, new string(*(string*)LP.Point) };
	case Dint: return { LP.Type, new int(*(int*)LP.Point) };
	case Dfloat: return { LP.Type, new float(*(float*)LP.Point) };
	case Ddouble: return { LP.Type,new double(*(double*)LP.Point) };
	case Dchar: return { LP.Type,new char(*(char*)LP.Point) };
	case Dbool: return { LP.Type,new bool(*(bool*)LP.Point) };
	case DPPoint: return { LP.Type,new (void*)(*(void**)LP.Point) };
	case DLoadVect:
	{
		vector<LoadPoint>* t = new vector<LoadPoint>;
		t->resize(((vector<LoadPoint>*)Point)->size());
		for (auto i = t->begin(), j = ((vector<LoadPoint>*)Point)->begin(); i != t->end(); i++, j++)
			*i = j->Clone(); // ������������ ������ ��������
		return { CLoadVect, t };
	}
	case DIC:

		break;
	case DIP: //return { Type, (*(ip*)Point).�lone() };
	{
		vector<ip>* t = new vector<ip>;
		t->push_back(*((ip*)Point));
		//		((ip*)Point)->Load.Clone();
		return { Type, t };
	}
	default: return *this;
	}
}

void* LoadPoint::VarClone() // ������� ������ �� ������������� �������� �� ��������
{
	if (Point == nullptr)
		return nullptr;
	switch (Type >> 1)
	{
	case Dstring: return new string(*(string*)Point);
	case Dint: return new int(*(int*)Point);
	case Dfloat: return new float(*(float*)Point);
	case Ddouble: return new double(*(double*)Point);
	case Dchar: return new char(*(char*)Point);
	case Dbool: return new bool(*(bool*)Point);
	case DPPoint: return new (void*)(*(void**)Point);
	case DIP: {IC_type t = new vector<ip>; t->push_back(*(ip*)Point); return t; }
	case DIC: return ICCopy(*this).Point;
	case DLoadVect: {
		LoadVect_type t = new vector<LoadPoint>;
		t->resize(((LoadVect_type)Point)->size());
		copy(((LoadVect_type)Point)->begin(), ((LoadVect_type)Point)->end(), t->begin());
	}
	}
}

void LoadPoint::VarDel() // �������� �������� ��
{
	if (Point == nullptr)
		return;
	switch (Type)
	{
	case Cstring: delete (string*)Point; break;
		//	case Cint: delete (int*)Point; break;
	case Cfloat: delete (float*)Point; break;
		//	case Cdouble: delete (double*)Point; break;
	case Cchar: delete (char*)Point; break;
	case Cbool: delete (bool*)Point; break;
	case CPPoint: delete (void**)Point; break;
	case CLoadVect: ((vector<LoadPoint>*)Point)->resize(0); delete ((vector<LoadPoint>*)Point); break;
	}
	Point = nullptr; Type = 0;
}

LoadPoint LoadPoint::Copy()
{
	LoadPoint LP;
	LP.Copy(*this);
	return LP;
}

void LoadPoint::Copy(LoadPoint LP)
{
	//	*this = LP.Clone();
	Type = LP.Type;
	if (LP.Type % 2 == 0)
	{
		Point = LP.Point;
		return;
	}
	switch (Type)
	{
	case Cstring: Point = new string(*(string*)LP.Point); break;
	case Cint: Point = new int(*(int*)LP.Point); break;
	case Cfloat: Point = new float(*(float*)LP.Point); break;
	case Cdouble: Point = new double(*(double*)LP.Point); break;
	case Cchar: Point = new char(*(char*)LP.Point); break;
	case Cbool: Point = new bool(*(bool*)LP.Point); break;
	case CPPoint: Point = new (void*)(*(void**)LP.Point); break;
	case CIP: // ???
		break;
	case CIC:
		Point = ICCopy(LP).Point;
		break;
	}
}

void LoadPoint::Clear() // �������� ��������
{
	if (Type % 2 == 0) return;
	switch (Type)
	{
	case Cstring: delete (string*)Point; break;
	case Cint: delete (int*)Point; break;
	case Cfloat: delete (float*)Point; break;
	case Cdouble: delete (double*)Point; break;
	case Cchar: delete (char*)Point; break;
	case Cbool: delete (bool*)Point; break;
	case CPPoint: delete (void**)Point; break;
	case CIP: // ???
		break;
	case CIC:
		ICDel(Point);
		break;
	}
	Type = 0; Point = nullptr;
}
// ������ �������
void LoadPoint::LoadPoint::VarClear() // ����� �������� �� � ��� ����� � � ���������� (���������� ���������)
{
	Type -= Type % 2;
	Type += 1;
	Clear();
}

void LoadPoint::VectorPrint(unsigned int Type, void* P, map<int, string > AtrMnemo, string offset, string Sep, string End, string ArrayBracketStart, string ArrayBracketFin)
{
	switch ((Type % 1000) >> 1)
	{
	case Dstring: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector<string>*)P)).begin(); t != (*(vector<string>*)P).end(); t++)
			cout << *t << (t == (*((vector<string>*)P)).begin() - 1 ? Sep : "");
		cout << ArrayBracketFin;
		break; }
	case Dint: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector<int>*)P)).begin(); t != (*(vector<int>*)P).end(); t++)
			cout << *t << (t == (*((vector<int>*)P)).begin() - 1 ? Sep : "");
		cout << ArrayBracketFin;
	}break;
	case Dfloat: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector<float>*)P)).begin(); t != (*(vector<float>*)P).end(); t++)
			cout << *t << (t == (*((vector<float>*)P)).begin() - 1 ? Sep : "");
		cout << ArrayBracketFin;
	}break;
	case Ddouble: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector<double>*)P)).begin(); t != (*(vector<double>*)P).end(); t++)
		{
			cout << *t << ((t != (*((vector<double>*)P)).end() - 1) ? Sep : "");
		}
		cout << ArrayBracketFin;
	}break;
	case Dchar: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector<char>*)P)).begin(); t != (*(vector<char>*)P).end(); t++)
			cout << *t << (t == (*((vector<char>*)P)).begin() - 1 ? Sep : "");
		cout << ArrayBracketFin;
	}break;
	case Dbool: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector<bool>*)P)).begin(); t != (*(vector<bool>*)P).end(); t++)
			cout << *t << (t == (*((vector<bool>*)P)).begin() - 1 ? Sep : "");
		cout << ArrayBracketFin;
	}break;
	case DIP: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector<ip>*)P)).begin(); t != (*(vector<ip>*)P).end(); t++)
			t->Load.print(AtrMnemo, offset, Sep, End);
		cout << ArrayBracketFin;
	}break;
	case DIC: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector<ip>*)P)).begin(); t != (*(vector<ip>*)P).end(); t++)
			t->Load.print(AtrMnemo, offset, Sep, End);
		cout << ArrayBracketFin;
	}break;
	default:
		cout << "unknown load\n";
	}
}
// ������ �������
void LoadPoint::MatrixPrint(unsigned int Type, void* P, map<int, string > AtrMnemo, string offset, string Sep, string End, string ArrayBracketStart, string ArrayBracketFin)
{
	switch ((Type % 1000) >> 1)
	{
	case Dstring: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector< vector<string>>*)P)).begin(); t != (*(vector < vector<string>>*)P).end(); t++)
			VectorPrint(Type - 1000, (void*)&(*t), AtrMnemo, offset, Sep, End, ArrayBracketStart, ArrayBracketFin);
		cout << ArrayBracketFin;
		break; }
	case Dint: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector < vector<int>>*)P)).begin(); t != (*(vector < vector<int>>*)P).end(); t++)
			VectorPrint(Type - 1000, (void*)&(*t), AtrMnemo, offset, Sep, End, ArrayBracketStart, ArrayBracketFin);
		cout << ArrayBracketFin;
	}break;
	case Dfloat: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector < vector<float>>*)P)).begin(); t != (*(vector < vector<float>>*)P).end(); t++)
			VectorPrint(Type - 1000, (void*)&(*t), AtrMnemo, offset, Sep, End, ArrayBracketStart, ArrayBracketFin);
		cout << ArrayBracketFin;
	}break;
	case Ddouble: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector < vector<double>>*)P)).begin(); t != (*(vector < vector<double>>*)P).end(); t++)
		{
			VectorPrint(Type - 1000, (void*)&(*t), AtrMnemo, offset, Sep, End, ArrayBracketStart, ArrayBracketFin);
			cout << End;
		}
		cout << ArrayBracketFin;
	}break;
	case Dchar: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector < vector<char>>*)P)).begin(); t != (*(vector < vector<char>>*)P).end(); t++)
			VectorPrint(Type - 1000, (void*)&(*t), AtrMnemo, offset, Sep, End, ArrayBracketStart, ArrayBracketFin);
		cout << ArrayBracketFin;
	}break;
	case Dbool: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector < vector<bool>>*)P)).begin(); t != (*(vector < vector<bool>>*)P).end(); t++)
			VectorPrint(Type - 1000, (void*)&(*t), AtrMnemo, offset, Sep, End, ArrayBracketStart, ArrayBracketFin);
		cout << ArrayBracketFin;
	}break;
	case DIP: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector < vector<ip>>*)P)).begin(); t != (*(vector < vector<ip>>*)P).end(); t++)
			VectorPrint(Type - 1000, (void*)&(*t), AtrMnemo, offset, Sep, End, ArrayBracketStart, ArrayBracketFin);
		cout << ArrayBracketFin;
	}break;
	case DIC: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector < vector<ip>>*)P)).begin(); t != (*(vector < vector<ip>>*)P).end(); t++)
			VectorPrint(Type - 1000, (void*)&(*t), AtrMnemo, offset, Sep, End, ArrayBracketStart, ArrayBracketFin);
		cout << ArrayBracketFin;
	}break;
	default:
		cout << "unknown load\n";
	}
}
// AtrMnemo - ������� �������� ���������
// AdrMap - ������ ������ ��� ���������� �� ��� ������ ��-�����
void LoadPoint::print(map<int, string > AtrMnemo, string offset, string Sep, string End, string quote, string ArrayBracketStart, string ArrayBracketFin, map<void*, int>* AdrMap)
{
	LoadPoint LP =IndLoadReturn();
	if (LP.Point == nullptr)
	{
		cout << "null";
		return;
	}
	switch (LP.Type)
	{
	case Tstring:
	case Cstring: cout << quote << LP.toStr() << quote; break;
	case Tint:
	case Cint:	  cout << LP.toInt(); break;
	case Tfloat:
	case Cfloat:  cout << LP.toFloat(); break;
	case Tdouble:
	case Cdouble: cout << LP.toDouble(); break;
	case Tchar:
	case Cchar:   cout << LP.toChar(); break;
	case Tbool:
	case Cbool:   cout << LP.toBool(); break;
	case TIP:
	case CIP:
	case TIC:
	case CIC:
		/*		if (((ip*)Point)->Load.Type >> 1 == DIP || isIC())
					if(AtrMnemo.count(((ip*)Point)->atr))
						cout << AtrMnemo[((ip*)Point)->atr] << " ->\n";
					else
						cout << ((ip*)Point)->atr << " ->\n";
				else
					if (AtrMnemo.count(((ip*)Point)->atr))
						cout << AtrMnemo[((ip*)Point)->atr] << ((((ip*)Point)->Load.Type % 2) ? " # " : "=");
					else
						cout << ((ip*)Point)->atr << ((((ip*)Point)->Load.Type%2)? " # ": "=");
				((ip*)Point)->Load.print();
				break;
		*/
	{
/*		LoadPoint LP = *this;
		while (LP.isVectInd() || LP.isICInd())
			if (Type >> 1 == DLoadVectInd) {
				register int N = ((LoadVect_type)Point)->size();
				if (N <= Ind || N < -Ind || ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind).Point == nullptr) return; // ������ �������
				LP = ((LoadVect_type)Point)->at(N * (Ind < 0) + Ind);
			}
			else if (Type >> 1 == DICInd)
				if (Ind < 0 || Ind / 3 >= ((IC_type)Point)->size()) return; // ������ �������
				else switch (Ind % 3) {
				case 0: LP = { TIP, &((IC_type)Point)->at(Ind / 3) }; break;
				case 1: LP = { Tint, &((IC_type)Point)->at(Ind / 3).atr }; break;
				case 2: if (((IC_type)Point)->at(Ind / 3).Load.Point == nullptr) return; LP = ((IC_type)Point)->at(Ind / 3).Load;
				}
*/
		if (LP.Type >> 1 == DIP)
		{
			if (AtrMnemo.count(((ip*)LP.Point)->atr))
				cout << AtrMnemo[((ip*)LP.Point)->atr] <<  " = ";
			else
				cout << ((ip*)LP.Point)->atr << ((((ip*)LP.Point)->Load.Type%2==0) ? " = " : " # ");
			((ip*)LP.Point)->Load.print();
			break;
		}
		bool FMap = false; // ���� �������� ������ ���������� ������� ��-�����
		if (AdrMap == nullptr)
		{
			AdrMap = new map<void*, int>;
			FMap = true;
		}
		if (AdrMap->count(LP.Point)) // ����������� ������������ ��-�����
		{
			cout << offset << "IC id: " << (*AdrMap)[LP.Point] << endl;
			break;
		}
		(*AdrMap)[LP.Point] = AdrMap->size(); // ��������� ���������� �� ��� ��������� ������������

		if (!((IC_type)LP.Point)->size()) return;
		for (auto i = ((IC_type)LP.Point)->begin(); i != ((IC_type)LP.Point)->end(); i++)
		{
			if (i->Load.Type >> 1 == DIP || i->Load.isIC())
				if (AtrMnemo.count(i->atr))
					cout << offset << AtrMnemo[i->atr] << " ->\n";
				else
					cout << offset << i->atr << " ->\n";
			else
				if (AtrMnemo.count(i->atr))
					cout << offset << AtrMnemo[i->atr] << ((i->Load.Type % 2) ? " # " : " = ");
				else
					cout << offset << i->atr << ((i->Load.Type % 2) ? " # " : " = ");
			i->Load.print(AtrMnemo, offset + "  ", Sep, End, quote, ArrayBracketStart, ArrayBracketFin, AdrMap);
			if (i != ((IC_type)LP.Point)->end() - 1)
				cout << endl;
		}
		if (FMap)  // ������� ������� ���������� �������
		{
			AdrMap->clear();
			delete AdrMap;
		}
		break;
	}
	case TLoadVect:
	case CLoadVect: // ������ ��������
	{
		cout << ArrayBracketStart;
		register int c = 1;
		for (auto i : *(vector<LoadPoint>*) Point)
		{
			i.print(AtrMnemo, offset, Sep, End, quote, ArrayBracketStart, ArrayBracketFin);
			if (c < ((vector<LoadPoint>*) Point)->size()) cout << Sep;
			c++;
		}
		cout << ArrayBracketFin << endl;
		break;
	}
	case TLoadVectInd:
	case CLoadVectInd: // ������ ��������
	{
		register int i = Ind;
		cout << "Vect Ind[" << Ind << "] ";
		if (((LoadVect_type)Point)->size() > abs(i) or -i == ((LoadVect_type)Point)->size())
			((LoadVect_type)Point)->at(i).print(AtrMnemo, offset, Sep, End, quote, ArrayBracketStart, ArrayBracketFin);
		break;
	}
	default:
		if (Type >= 2000) // ������ ������� � �������
		{
			MatrixPrint(Type, Point, AtrMnemo, offset, Sep, End, ArrayBracketStart, ArrayBracketFin);
		}
		else if (Type >= 1000)
			VectorPrint(Type, Point, AtrMnemo, offset, Sep, End, ArrayBracketStart, ArrayBracketFin);
	}
}

// ������ � ��
void FU::CommonMk(int Mk, LoadPoint Load, FU* Sender)
{
	Mk %= FUMkRange;
	if (Mk < 0) // ������� ��� ���
	{

		if (Alu == nullptr) // ���� ��� �� ������ ���
		{ // ������� ���
			Alu = (FU*)new ALU(Bus);
			//			cout << "FUType: " << FUtype << endl;
			Alu->ProgFU(0, { 0,nullptr }, this);
			((FU*)Alu)->Parent = this;
			Accum = { Cdouble, &((ALU*)Alu)->accum };
		}
		Alu->ProgFU(-Mk, Load, this);
		return;
	}
	switch (Mk)
	{
	case 902: // ActiveSet ���������� ���������� �� (true �� ���������)
		Active = Load.toBool(true);
		break;
	case ProgMk: // 958 Prog ����� ������������
	case ProgCycleMk: //959 CycleProg ����� �����
	case ProgPostCycleMk: //960 PostCycleProg ����� ���� �����
		if (Alu != nullptr)
			((ALU*)Alu)->Stack.push_back({}); //������������ �������� �����
		if (Load.Point == nullptr)
			ProgExec(Prog, Mk - ProgMk);
		else
			ProgExec(Load, Mk - ProgMk);
		if (Alu != nullptr)
			((ALU*)Alu)->Stack.pop_back(); // ������ ����������� �������� �����
		break;
	case YesMk: //961 YesProg ����� ������������ �� ��
	case YesCycleMk: //962 YesCycleProg ����� ����� �� ��
	case YesPostCycleMk: //963 YesPostCycleProg ����� ���� ����� �� ��
	case YesBreakMk: //967 YesProgBreak ����� ������������ �� �� � ����������� ����������� �������� ���������
		if (Accum.toBool())
		{
			int PB = Mk == YesBreakMk;
			if (Alu != nullptr)
				((ALU*)Alu)->Stack.push_back({}); //������������ �������� �����
			if (Mk == YesBreakMk) {
				Mk = YesMk;
			}
			if (Load.Point == nullptr)
				ProgExec(Prog, Mk - YesMk);
			else
				ProgExec(Load.Point, Mk - YesMk);
			if (Alu != nullptr)
				((ALU*)Alu)->Stack.pop_back(); // ������ ����������� �������� �����
			ProgStop += PB; // ����� �� ������� ���������
		}
		break;
	case NoMk: //964 NoProg ����� ������������ �� ���
	case NoCycleMk: //965 NoCycleProg ����� ����� �� ���
	case NoPostCycleMk: //966 NoPostCycleProg ����� ���� ����� �� ���
	case NoBreakMk: //968 NoProgBreak ����� ������������ �� ��� � ����������� ����������� �������� ���������
		if (!Accum.toBool())
		{
			int PB = Mk == NoBreakMk;
			if (Alu != nullptr)
				((ALU*)Alu)->Stack.push_back({}); //������������ �������� �����
			if (Mk == NoBreakMk) {
				ProgStop += 1;
				Mk = NoMk;
			}
			if (Load.Point == nullptr)
				ProgExec(Prog, Mk - NoMk);
			else
				ProgExec(Load.Point, Mk - NoMk);
			if (Alu != nullptr)
				((ALU*)Alu)->Stack.pop_back(); // ������ ����������� �������� �����
			ProgStop += PB; // ����� �� ������� ���������
		}
		break;
	case 919: // AccumPointerSet ���������� ������ �� �����������
		if (Load.Type >> 1 == Ddouble)
			Accum = Load;
		break;
	case 920: // AccumPointerOut ������ ������ �� �����������
		Load.Write(Accum);
		break;
	case 921: // AccumPointerOutMk ������ �� �� ������� �� �����������
		MkExec(Load, Accum);
		break;
	case 931: // AccumSet ���������� �������� ������������
		if (Accum.Point = nullptr)
		{
			Accum = { Cdouble,new double };
			AccumCreating = true; // ������������� ���� ���������������� �������� ������������ ��-��
		}
		Accum.Write(Load.toDouble());
		AccumCreating = false;
		break;
	case 923: // AccumOut ������ �������� ������������
		Load.WriteFromLoad(Accum);
		break;
	case 925: // AccumOutMk ������ �� �� ��������� ������������
		MkExec(Load, Accum);
		break;
	case FUIndSetMk: // 933 FUIndSet ���������� ������ ��
		FUInd = Load.toInt();
		break;
	case 992: // FUIndOut ������ ������ ��
		Load.Write(FUInd);
		break;
	case 993: //FUIndOutMk ������ ������������ � �������� ��
		MkExec(Load, { Cint, &FUInd });
		break;
	case CalcMk: // 927 Calc ��������� ���
	case 929: // CalcContinue ��������� ��� � ���������� ���������� ���������, ���� ��������� true
	case 930: // CalcBreak ��������� ��� � ���������� ���������� ���������, ���� ��������� false
		if (Alu == nullptr)
		{
			Alu = (FU*)new ALU(this->Bus);
			Accum = { Cdouble, &((ALU*)Alu)->accum };
			AccumCreating = false;
			ALUCreating = true;
		}
		((FU*)Alu)->ProgFU(ProgExecMk, Load, this);
		break;
		if (Mk == 929 && Accum.toBool() || Mk == 930 && !Accum.toBool())
			ProgStop = 1; // ����� �� ���������
	case 932: // IntAluCalc ������� ������������� ���
		Alu = new IntAlu(this->Bus);
		Accum = { Tint, &((IntAlu*)Alu)->Accum };
		AccumCreating = false;
		ALUCreating = true;
		((FU*)Alu)->ProgFU(ProgExecMk, Load, this);
		break;
	case 989: // ProgStop ��������� ��������� (� �������� ���������� ������� ������; ���� �������� nil, �� ������������� 1)
		ProgStop = Load.toInt(2);
		break;
	case 988: // ProgStopAll ��������� ���� ���������� �� ���������� �������������� ��� ������� ��
		ProgStopAll = Load.toBool(true);
		break;
	case 987: //Next ������� � ��������� �������� �����
		CycleStop = Load.toInt();
		break;
	case 916:// ManualModeSet ���������� ����� ������� ����������
		if (Modeling == nullptr) Modeling = new FUModeling();
		Modeling->ManualMode = Load.toBool();
		break;
	case SchedulerSetMk: //918: // SchedulerSet ���������� �������� ������������ ����������
		if (Load.Point == nullptr) break;
		if (Modeling == nullptr) Modeling = new FUModeling();
		Modeling->scheduler = (FU*)Load.Point;
		Modeling->ManualMode = true;
		break;
	case 924: // Pr�fixProgSet
		PrefixProg = (IC_type)Load.Point;
		break;
	case 922: // PostfixProgSet
		PostfixProg = (IC_type)Load.Point;
		break;
	case 926: // BusSet
		Bus = (FU*)Load.Point;
		break;
	case ProgExecMk: // ProgExec ��������� ��������� (���� � �������� null, �� ��������� �� �������� Prog
		if (Load.Point == nullptr)
			ProgExec((vector<ip>*)Prog);
		else
			ProgExec((vector<ip>*)Load.Point);
		break;
	case 991: // ProgSet // ���������� ��������� �� ���������
		Prog = (IC_type)Load.Point;
		break;
	case 953: // ElseProgSet
		ElseProg = (IC_type)Load.Point;
		break;
	case 954: // ProgSetExec ���������� ��������� �� ��������� � ��������� ��
		Prog = (IC_type)Load.Point;
		ProgExec((vector<ip>*)Prog);
		break;
	case ContextOutMk:  // 995 ContextOut ������ ��������� �� �������� ��
		//		if (Load.Type >> 1 == Dvoid || Load.Type >> 1 == DPPoint || Load.Type >> 1 == DFU)
		//			Load.Point = this;
		Load.Write((FU*)this);
		break;
	case 928: // ALUSet ���������� ������ �� ���
		if (ALUCreating)
			delete Alu;
		Alu = (FU*)Load.Point;
		ALUCreating = false;
		break;
	case 998: //FUNameSet
		FUName = Load.toStr();
		break;
	case 996: // FUNameOut
		Load.Write(FUName);
		break;
	case 997: // FUNameOutMk
		if (Load.Point != nullptr)
			MkExec(Load.toInt(), { Cstring,&FUName });
		break;
	case ContextOutMkMk: // 999 ContextOutMK ������ ������������ � ���������� �� �������� ��
		if (Load.Type >> 1 == Dint)
			Bus->ProgFU(*(int*)Load.Point, { TFU, this }, this);
		break;
	case BreakMk: //����� �� ������
	    CycleStop = Load.toInt(1);
		if (CycleStop < 0) CycleStop = 0;
		break;
	case NextMk: // ����������� ������
		CycleStop = - Load.toInt(1); // ��� CycleStop < 0 ����������� �������� Continue
		if (CycleStop > 0) CycleStop = 0;
		break;
	case MkGlobalRangeSet: // ���������� ���������� ����� �� ��� ��
		FUMkGloabalRange = Load.toInt();
		break;
	case MkGlobalRangeOutMk:  // ������ ���������� ����� �� ��� ��
		Load.Write(FUMkGloabalRange);
		break;
	case MkGlobalRangeOutMkMK: // ������ �� � ���������� ������� �� ��� ��
		MkExec(Load, { Cint, &FUMkGloabalRange });
		break;
	case 917: // EventserSet Установить указатель на контроллер событий
		if (Modeling == nullptr) Modeling = new FUModeling();
		Modeling->eventser = (FU*)Load.Point;
		break;	}
}

// ������ ���������
// CycleType ��� �����: 0 - ��� �����, 1 - ����, 2 - ���� � ������������
void FU::ProgExec(void* UK, unsigned int CycleMode, FU* ProgBus, vector<ip>::iterator* Start) // ���������� ��������� �� ��
{
	if (UK == nullptr) return;
	vector<ip>* Uk = (IC_type)UK;
	if (ProgBus == nullptr) ProgBus = Bus;
	bool RepeatF = false; // ���� ������������ ���������� ��������� � ��
	do
	{
		ProgStop = 0; // ������� ��������� (�������� ���������� �������, �� ������� ���������� �����
		RepeatF = false; // ���� ������������� �������� �� ����� ���� ������� �� � ��
		ProgStopAll = false; // ���� ��������� ���� ���������
		CycleStop = 0; // ������� ������� �� ������ (���� ������������� ��������, �� ����������� �����)
		for (auto i = Start == nullptr ? Uk->begin() : *Start; i != Uk->end(); i++)
		{
			if (i->atr == GotoAtr) // ������� �� ������ ��
			{
				ProgExec(i->Load, CycleMode, ProgBus, Start);
				return;
			}
			if (i->atr >= FUMkRange)
				ProgBus->ProgFU(i->atr, i->Load, this); // ���� �������� �� �� ����������� �� (������ �� Bus)
			else // �� ��� ������� ��
			{
				if (i->atr == YesContinueAtr || i->atr == NoContinueAtr)
				{
					if (CycleMode == 2) // �������� ����� � ������������ (���������� ������ ��������)
					{
						CycleMode = 1;
						continue;
					}
					if (i->Load.isProg() && Alu != nullptr) // ������ ���������� ���

						// ������� � ��������� �������� ����� continue
						if (i->Load.isProg())
						{
							if (Alu == nullptr)
							{
								Alu = (FU*)new ALU(this);
								((FU*)Alu)->Bus = Bus;
								ALUCreating = true;
								AccumCreating = false;
								if (Accum.Point != nullptr && AccumCreating)
									Accum.Clear();
								AccumCreating = false;
								Accum = { Cdouble,&((ALU*)Alu)->accum };
							}
							((FU*)Alu)->ProgExec(i->Load);
						}
					if (i->atr == YesContinueAtr && !Accum.toBool() || i->atr == NoContinueAtr && Accum.toBool())
					{
						CycleMode = 0; // ����� �� �����
						ProgStop += 1; // ����� �� �����
						break;
					} // ����� �� �����
					continue;
				}
				if (i->atr == RepeatAtr) { // ��������� ��������� ������
					RepeatF = true; break;
				}
				ProgFU(i->atr, i->Load, this); // ���������� �������
			}

			if (CycleStop != 0) // ��������� ������
				if (!CycleMode) // ���� �� � ������ �����, �� ������ ������� �� ������
					return;
				else {
					if (CycleStop > 0)
					{
						CycleStop--; // ���������� �������� ������� �� �����
						if(!CycleStop) return;
					}
					else
					{
						CycleStop++; // ���������� �������� ������� �� �����
						if (CycleStop) 
							return;
					}
				}
			if (ProgStop > 0) { ProgStop--; return; }
			if (ProgStopAll) { return; } // ������������ ����� �� ������������
		}
	} while (RepeatF || CycleMode > 0);
}

// ������ ��������� �� ��������� �� ������� ��
void FU::ProgExec(LoadPoint Uk, unsigned int CycleMode, FU* Bus, vector<ip>::iterator* Start) // ���������� ��������� �� ��
{
	if (Uk.isIC())
		ProgExec(Uk.Point, CycleMode, Bus, Start);
}

void FU::MkExec(int MK, LoadPoint Load, FU* BusContext, bool Ext) // ������ �� � ���������
{
	if (MK < FUMkRange && !Ext) // ���� �� ���������� ������ ��
		ProgFU(MK, Load, this);
	else
		if (BusContext != nullptr)
			BusContext->ProgFU(MK, Load, this);
		else
			Bus->ProgFU(MK, Load, this);
}

void FU::MkExec(LoadPoint Mk, LoadPoint Load, FU* BusContext, bool Ext) // ������ �� � ���������
{
	if (Mk.Type >> 1 == Dint && Mk.Point != nullptr)
	{
		int MK = *(int*)Mk.Point;
		if (MK < FUMkRange && !Ext) // ���� �� ���������� ������ ��
			ProgFU(MK, Load, this);
		else
			if (BusContext != nullptr)
				BusContext->ProgFU(MK, Load, this);
			else
				Bus->ProgFU(MK, Load, this);
	}
}

void FU::MkAwait(int MK, LoadPoint Load, FU* Sender, double Delay) // ?????????? ?? ??? ???????? ?? ??????? ??? ?????????????
{
	if (Modeling == nullptr || Modeling->eventser == nullptr)
	{
		ProgFU(MK, Load, Sender);
		return;
	}
	double t;
	ip IP= {MK, Load};
//	LoadPoint LP = { Tdouble, &t };
//	Modeling->eventser->ProgFU(EventserCurrentTimeOutMk, LP, this); // ????? ??????? ????? ?? ??????????? ???????
//	Modeling->qAwaitMk.insert({ t + Delay, {MK, Load, Sender} }); // ????????? ????? ?????????? ??????? ?? ? ??????? ????????

	Modeling->eventser->ProgFU(EventserFUSetMk, { CFU, this }, this); // ????????? ????? ??????? ? ?????? ????????? ???????
	Modeling->eventser->ProgFU(EventTimeSetMk, { Cdouble, &Delay }, this); // Установить вреям задержки ожидаемой МК
	Modeling->eventser->ProgFU(AwaitMkSetMk, { CIP, &IP}, this); // ????????? ????? ??????? ? ?????? ????????? ???????
}
/*
void FU::Scheduling(bool SchedulerFlag)
{
	if (Modeling == nullptr) return;
	if (Modeling->qmk.size() != 0)
	{
		Modeling->SchedulerFlag = SchedulerFlag;
		if (Modeling->qmk.size() == 0)
			cout << "Modeling error\n";
		else
		{
			ip t = Modeling->qmk.back();
			Modeling->qmk.pop_back();
			ProgFU(t.atr, t.Load, this);
			if (t.Load.Type % 2 == 1 && t.Load.Point != nullptr)
				t.Load.VarDel();
		}
	}
}
*/

void FU::Scheduling(bool SchedulerFlag)
{
	if (Modeling == nullptr)
	{
		cout << "Modeling error: Modeling=null\n";
		return;
	}
	Modeling->SchedulerFlag = SchedulerFlag;
	if (SchedulerFlag) {
		if (Modeling->qmk.size() != 0)
		{
			if (Modeling->qmk.size() == 0)
				cout << "Modeling error: qmk.size() = 0\n";
			else
			{
				ipSender t = Modeling->qmk.back();
				Modeling->qmk.pop_back();
				ProgFU(t.atr, t.Load, t.Sender);
				if (t.Load.Type % 2 == 1 && t.Load.Point != nullptr)
					t.Load.VarDel();
			}
		}
	}
	else
	{// MkReceive awaiting Приход ожидаемой МК
		if (Modeling->qAwaitMk.size() != 0)
		{
			if (Modeling->qAwaitMk.size() == 0)
				cout << "Modeling error: qAwaitMk.size() = 0\n";
			else
			{
				ipSender t = Modeling->qAwaitMk.begin()->second;
				Modeling->qAwaitMk.erase(Modeling->qAwaitMk.begin());
				ProgFU(t.atr, t.Load, t.Sender);
				if (t.Load.Type % 2 == 1 && t.Load.Point != nullptr)
					t.Load.VarDel();
			}
		}
	}
}

void FUModeling::EventModelingPrint() // Вывести состояние моделирования ФУ
{
	cout << "qmk: " << endl;
	for (auto i : qmk)
		cout << i.atr << "=" << i.Load.Type << endl;
	cout << "qAwaitMk: " << endl;
	for (auto i : qAwaitMk)
		cout << "Time: " << i.first << " MK: "<< i.second.atr << "=" << i.second.Load.Type << endl;
}

bool LoadCmp(LoadPoint x, LoadPoint y) // ��������� ���� �������� ��
{
	// ���������� � ����������� �� �� � ��
	if (x.Point == nullptr || y.Point == nullptr)
		return true;
	int xt = x.Type >> 1, yt = y.Type >> 1;
	if ((xt == Dint || xt == Dfloat || xt == Ddouble || xt == Dchar) && (yt == Dint || yt == Dfloat || yt == Ddouble || yt == Dchar))
		switch (xt)
		{
		case Dint:
			switch (yt)
			{
			case Dint: return *(int*)x.Point == *(int*)y.Point;
			case Dfloat: return *(int*)x.Point == *(float*)y.Point;
			case Ddouble: return *(int*)x.Point == *(double*)y.Point;
			case Dchar: return *(int*)x.Point == *(char*)y.Point;
			}
		case Dfloat:
			switch (yt)
			{
			case Dint: return *(float*)x.Point == *(int*)y.Point;
			case Dfloat: return *(float*)x.Point == *(float*)y.Point;
			case Ddouble: return *(float*)x.Point == *(double*)y.Point;
			case Dchar: return *(float*)x.Point == *(char*)y.Point;
			}
		case Ddouble:
			switch (yt)
			{
			case Dint: return *(double*)x.Point == *(int*)y.Point;
			case Dfloat: return *(double*)x.Point == *(float*)y.Point;
			case Ddouble: return *(double*)x.Point == *(double*)y.Point;
			case Dchar: return *(double*)x.Point == *(char*)y.Point;
			}
		case Dchar:
			switch (yt)
			{
			case Dint: return *(char*)x.Point == *(int*)y.Point;
			case Dfloat: return *(char*)x.Point == *(float*)y.Point;
			case Ddouble: return *(char*)x.Point == *(double*)y.Point;
			case Dchar: return *(char*)x.Point == *(char*)y.Point;
			}
		}
	if (xt == Dbool && yt == Dbool)
		return *(bool*)x.Point == *(bool*)y.Point;
	if (xt == Dstring && yt == Dstring)
		return *(string*)x.Point == *(string*)y.Point;

	return false;
}

bool IPCmp(ip* x, ip* y) // ��������� ����  ��
{
	if (!LoadCmp(x->Load, y->Load))
		return false;
	else
		if (x->atr == y->atr || x->atr == 0 || y->atr == 0)
			return true;
		else
			return false;
}

// ������ � ��
bool AtrSearch(void* uk, int Atr) // ����� ��������� � ��
{
	for (auto& i : *(IC_type)uk)
		if (i.atr == Atr)
			return true;
	return false;
}

int AtrCounter(void* uk, int Atr) // ������� ���������� �� � ��������� ���������� � ��
{
	int c = 0;
	for (auto& i : *(IC_type)uk)
		if (i.atr == Atr)
			c++;
	return c;
}

void ICDel(void* Uk) // �������� ��
{
	for (auto&& i : *(IC_type)Uk)
		i.Load.Clear();
	delete (IC_type)Uk;
}

void ICDel(LoadPoint& Uk)// �������� ��
{
	if (!Uk.isIC()) return;
	ICDel(Uk.Point);
	Uk = { 0,nullptr };
}

LoadPoint ICCopy(LoadPoint uk, bool Copy) // ����������� �� (Copy - ���� ��������������� ����������� ���� �� � ��-�����)
{
	IC_type CapsNew = new vector<ip>;
	if (uk.Type >> 1 == DIP) // ���� ���������� ��
	{
		CapsNew->push_back(*(*(ip*)uk.Point).�lone());
		return { uk.Type, CapsNew };
	}
	IC_type Uk = (IC_type)uk.Point;
	CapsNew->resize(((IC_type)Uk)->size());
	for (auto i = ((IC_type)Uk)->begin(), j = CapsNew->begin(); j != CapsNew->end(); i++, j++)
	{
		j->atr = i->atr;
		if (Copy and i->Load.isIC())
			j->Load = ICCopy(i->Load, true);
		//j->Load.Copy({ i->Load.Type | 1, i->Load.Point, i->Load.Ind }); // ������������� � ���������, ����� ��������� �����������
		else
			j->Load.Copy(i->Load);
	}
	return { uk.Type, CapsNew };
}

void* GraphCopy(void* Uk, LocatTable* Table = nullptr) // ����������� ��-�����
{
	return nullptr;
}

void GraphDel(void* Uk, LocatTable* Table = nullptr) // �������� ��-�����
{
	return;
}
// ------------------------



// ����� � �� �� � ��������� Atr � ��������� ��������� ���� �� ���. � ��������, ���� ����� ��������� ��, ���� ������� �� ������, ������������ true
bool AtrProgExec(vector<ip>* Prog, int Atr, FU* Bus, bool AfterContinue)
{
	auto i = Prog->begin();
	for (; i != Prog->end() && i->atr != Atr; i++);
	if (i != Prog->end())
	{
		if (i->Load.Point != nullptr)
			Bus->ProgExec((vector<ip> *)i->Load.Point);
		if ((++i) != Prog->end() && AfterContinue)
			Bus->ProgExec(Prog, 0, nullptr, &i);
		return true;
	}
	return false;
}

ip* AtrFind(void* IC, int Atr) // ����� � �� �� � �������� �������
{
	if (IC == nullptr) return nullptr;
	auto uk = (*(IC_type)IC).begin();
	for (; uk != ((IC_type)IC)->end() && uk->atr != Atr; uk++);
	if (uk == ((IC_type)IC)->end())
		return nullptr;
	else
		return uk._Ptr;
	return nullptr;
}

bool isIPinIC(void* iP, void* iC) //��������, ��� �� ������ � ��
{
	for (auto i = ((IC_type)iC)->begin(); i != ((IC_type)iC)->end(); i++) {
		if (IPCmp(i._Ptr, ((ip*)iP)))
			return true;
	}
	return false;
}

void ICCopyConcat(void* uk, void* uk2) // ������������ ���� ��
{
	//copy( ((IC_type)uk2)->begin(), ((IC_type)uk2)->end(), inserter(*((IC_type)uk), ((IC_type)uk)->end()));
	for (auto& i : *(IC_type)uk2)
		((IC_type)uk)->push_back(i.Copy());
}

int ICLen(void* uk) // ������������ ����� ��
{
	return (((IC_type)uk)->size());
}

vector<ip>::iterator IPSearch(void* ic, ip IP) // ����� �� � �� (������������ ��������� �� ������ ��������� ��
{
	for (auto i = ((IC_type)ic)->begin(); i != ((IC_type)ic)->end(); i++)
		if (IPCmp(i._Ptr, &IP))
			return i;
	return ((IC_type)ic)->end();
}

vector<ip>::iterator IPSearch(void* ic, LoadPoint IP) // ����� �� � �� (������������ ��������� �� ������ ��������� ��
{
	if (IP.isIP())
		return IPSearch(ic, *((ip*)IP.Point));
	else if (IP.isIC())
		return IPSearch(ic, *((IC_type)IP.Point)->begin());
	return ((IC_type)ic)->end();
}
void IPAdd(void* IC, ip IP) // �������� �� � ����� ��
{
	((vector<ip>*)IC)->push_back(IP.Copy());
}

void IPAdd(LoadPoint IC, ip IP) // �������� �� � ����� ��
{
	if (!IC.isIC())return;
	IPAdd(IC.Point, IP);
}

void* MakeLoadFromDouble(double x, unsigned int Type) // ������� �������� �� ���� double
{
	if (Type > 0) // ����� ���
	{
		LoadPoint P = { Type, nullptr };
		if (!P.isDigitBool()) return nullptr;
	}
	else // �������������� ������������ ����
	{
		if (x == 0 || x == 1)
			Type = Tbool;
		else if (x == int(x))
			Type = Tint;
		else
			Type = Tdouble;
	}

	switch (Type >> 1)
	{
	case Dbool:
		return new bool((bool)x);
		break;
	case Dint:
		return new int((int)x);
		break;
	case Dfloat:
		return new float((float)x);
		break;
	case Ddouble:
		return new double((double)x);
		break;
	}
};

LoadVect_type LoadPoint::Vect() // ���������� ���������� �� ������
{
//	if (!isVect()) return nullptr;
	return (LoadVect_type)Point;
}
IC_type LoadPoint::IC() // ���������� ��������� �� ��
{
//	if (!isIC()) return nullptr;
	return (IC_type)Point;
}

LoadPoint LoadNew(int t) //�������� �������� �� ����������
{
	return { Cint, (void*)&t };
}
LoadPoint LoadNew(double t) //�������� �������� �� ����������
{
	return { Cdouble, (void*)&t };
}
LoadPoint LoadNew(bool t) //�������� �������� �� ����������
{
	return { Cbool, (void*)&t };
}
LoadPoint LoadNew(string t) //�������� �������� �� ����������
{
	return { Cstring, (void*)&t };
}
LoadPoint LoadNew(float t) //�������� �������� �� ����������
{
	return { Cfloat, (void*)&t };
}

LoadPoint CopyAdrCorrect(LoadPoint OriginalIC, LoadPoint CopyIC, LoadPoint Adr, set<void*> buf) // ������������� ������ � ������������ �� (���������� ��������)
{
	if (buf.count(OriginalIC.Point)) return {};
	buf.insert(OriginalIC.Point);
	if (Adr.Point == OriginalIC.Point)
		if (Adr.isICInd())
			return { TICInd + (CopyIC.Type & 1) , CopyIC.Point, Adr.Ind };
		else if(Adr.isVectInd())
			return { TLoadVectInd + (CopyIC.Type & 1) , CopyIC.Point, Adr.Ind };
		else
			return CopyIC;
	for (auto i = ((IC_type)OriginalIC.Point)->begin(), j = ((IC_type)CopyIC.Point)->begin();
		i != ((IC_type)OriginalIC.Point)->end() && j != ((IC_type)CopyIC.Point)->end(); i++, j++)
	{
		if (i._Ptr == Adr.Point) return { TIP, j._Ptr };
		if (&(i->atr) == Adr.Point) return { Tint, &(j->atr) };
		if (&(i->Load) == Adr.Point) return { TLoad, &(j->Load) };
//		if (i->Load.Point == Adr.Point) return j->Load;
		if (i->Load.isIC())
		{
			LoadPoint LP = CopyAdrCorrect(i->Load, j->Load, Adr, buf);
			if (LP.Point == nullptr)
				continue;
			return LP;
		}
	}
	return {};
}
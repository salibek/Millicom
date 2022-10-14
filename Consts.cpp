#include "stdafx.h"
#include "Consts.h"
#include <iostream>
#include <string>
#include "LocationTable.h"
#include "ALU.h"
#include "IntAlu.h"

using namespace std;

bool LoadPoint::isIC() // Определить указывает ли ссылка на ИК
{
	set<unsigned int> IC_Types = {DIC, DCalc}; // Множевство типов ИК и ОА-графов
	return IC_Types.count(Type>>1);
}

bool LoadPoint::isIP() // Определить указывает ли ссылка на ИП
{
	set<unsigned int> IP_Types = { DIP }; // Множевство типов ИК и ОА-графов
	return IP_Types.count(Type >> 1);
}

string LoadPoint::toStr(string define) // Перевод в bool
{
	if (Point == nullptr)
		return define;
	switch (Type >> 1)
	{
	case Dstring: return *((string*)Point);
	case Ddouble: return to_string(*(double*)Point); break;
	case Dfloat: return to_string(*(float*)Point); break;
	case Dint: return to_string(*(int*)Point); break;
	case Dbool: return to_string(*(bool*)Point); break;
	default:
		return "";
		break;
	}
}

bool LoadPoint::toBool(bool define) // Перевод в bool (по умолчания false)
{
	if (Point == nullptr)
		return define;
	switch (Type >> 1)
	{
	case Ddouble: return *(double*)Point;
	case Dfloat: return *(float*)Point;
	case Dint: return *(int*)Point;
	case Dbool: return *(bool*)Point;
	case Dstring: return !((*(string*)Point) == "");
	default:
		return false;
	}
}

int LoadPoint::toInt(int define) { // Перевод в integer
	if (Point == nullptr)
		return define;
	switch (Type >> 1)
	{
	case Ddouble: return (int)*(double*)Point; break;
	case Dfloat: return (int)*(float*)Point; break;
	case Dint: return *(int*)Point; break;
	case Dbool: return *(bool*)Point; break;
	case Dstring: return atoi((*(string*)Point).c_str());
	default:
		return 0;
		break;
	}
}
double LoadPoint::toDouble(double define) {// Перевод в integer
	if (Point == nullptr) return define;
	switch (Type >> 1)
	{
	case Ddouble: return *(double*)Point; break;
	case Dfloat: return *(float*)Point; break;
	case Dint: return *(int*)Point; break;
	case Dbool: return *(bool*)Point; break;
	case Dstring: return atof((*(string*)Point).c_str());
	default:
		return 0;
		break;
	}
}

float LoadPoint::toFloat(float define) {// Перевод в integer
	if (Point == nullptr)
		return define;
	switch (Type >> 1)
	{
	case Ddouble: return *(double*)Point; break;
	case Dfloat: return *(float*)Point; break;
	case Dint: return *(int*)Point; break;
	case Dbool: return *(bool*)Point; break;
	case Dstring: return atof((*(string*)Point).c_str());
	default:
		return 0;
		break;
	}
};
// Запись в нагрузку ИП
int LoadPoint::Write(vector<double> x) // Копирование вектора
{
	if (Point == nullptr)
		return 1;
	if (Type == CdoubleArray)
		*(vector<double>*)Point = x;
	else
		return 0;
}
int LoadPoint::Write(vector<float> x) // Копирование вектора
{
	if (Point == nullptr)
		return 1;
	if (Type == CdoubleArray)
		*(vector<float>*)Point = x;
	else
		return 0;
}
int LoadPoint::Write(vector<bool> x) // Копирование вектора
{
	if (Point == nullptr)
		return 1;
	if (Type == CdoubleArray)
		*(vector<bool>*)Point = x;
	else
		return 0;
}
int LoadPoint::Write(vector<char> x) // Копирование вектора
{
	if (Point == nullptr)
		return 1;
	if (Type == CdoubleArray)
		*(vector<char>*)Point = x;
	else
		return 0;
}
int  LoadPoint::Write(vector<int> x) // Копирование вектора
{
	if (Point == nullptr)
		return 1;
	if (Type == CdoubleArray)
		*(vector<int>*)Point = x;
	else
		return 0;
}
// -----

int LoadPoint::WriteFromLoad(LoadPoint Load) // Записать величину из нагрузки
{
	if (Point == nullptr)
		return 1;
	switch (Type)
	{
	case Tdouble:
		*((double*)Point) = Load.toDouble();
		break;
	case Tfloat:
		*((float*)Point) = Load.toFloat();
		break;
	case Tint:
		*((int*)Point) = Load.toInt();
		break;
	case Tbool:
		*((bool*)Point) = Load.toBool();
		break;
	case Tchar:
		*((char*)Point) = Load.toChar();
		break;
	default: // Перезапись указателя
		if (Type % 2 != 0) // Если тип переменной
			return 2; // Ошибка: попытка запись в константу
		else
			if (Type >> 1 != Load.Type >> 1)
				return 1; // Ошибка: несоотвествие типов
			else
				Point = Load.Point;
	}
	return 0;
}

int LoadPoint::Write(size_t x)
{
	if (Point == nullptr)
		return 1;
	switch (Type)
	{
	case Tdouble:
		*((double*)Point) = x;
		break;
	case Tfloat:
		*((float*)Point) = x;
		break;
	case Tint:
		*((int*)Point) = x;
		break;
	case Tbool:
		*((bool*)Point) = x;
		break;
	case Tchar:
		*((char*)Point) = x;
		break;
	default:
		return 2; // Несоответствие типов
	}
	return 0;
}

int LoadPoint::Write(int x)
{
	if (Point == nullptr)
		return 1;
	switch (Type)
	{
	case Tdouble:
		*((double*)Point) = x;
		break;
	case Tfloat:
		*((float*)Point) = x;
		break;
	case Tint:
		*((int*)Point) = x;
		break;
	case Tbool:
		*((bool*)Point) = x;
		break;
	case Tchar:
		if (x >= 0 && x < 256)
			*((char*)Point) = x;
		else
			return 1; // Несоответствие типов
		break;
	defoult:
		return 1; // Несоответствие типов
	}
	return 0;
}

int LoadPoint::Write(double x)
{
	if (Point == nullptr)
		return 1;
	switch (Type)
	{
	case Tdouble:
		*((double*)Point) = x;
		break;
	case Tfloat:
		*((float*)Point) = x;
		break;
	case Tint:
		*((int*)Point) = x;
		break;
	case Tbool:
		*((bool*)Point) = x;
		break;
	case Tchar:
		*((char*)Point) = x;
		break;
	defoult:
		return 1; // Несоответствие типов
	}
	return 0;
}

int LoadPoint::Write(float x)
{
	if (Point == nullptr)
		return 1;
	switch (Type)
	{
	case Tdouble:
		*((double*)Point) = x;
		break;
	case Tfloat:
		*((float*)Point) = x;
		break;
	case Tint:
		*((int*)Point) = x;
		break;
	case Tbool:
		*((bool*)Point) = x;
		break;
	case Tchar:
		*((char*)Point) = x;
		break;
	defoult:
		return 1; // Несоответствие типов
	}
	return 0;
}

int LoadPoint::Write(bool x)
{
	if (Point == nullptr)
		return 1;
	switch (Type)
	{
	case Tdouble:
		*((double*)Point) = x;
		break;
	case Tfloat:
		*((float*)Point) = x;
		break;
	case Tint:
		*((int*)Point) = x;
		break;
	case Tbool:
		*((bool*)Point) = x;
		break;
	case Tchar:
		*((char*)Point) = x;
		break;
	defoult:
		return 1; // Несоответствие типов
	}
	return 0;
}

int LoadPoint::Write(char x)
{
	if (Point == nullptr)
		return 1;
	switch (Type)
	{
	case Tdouble:
		*((double*)Point) = x;
		break;
	case Tfloat:
		*((float*)Point) = x;
		break;
	case Tint:
		*((int*)Point) = x;
		break;
	case Tbool:
		*((bool*)Point) = x;
		break;
	case Tchar:
		*((char*)Point) = x;
		break;
	defoult:
		return 1; // Несоответствие типов
	}
	return 0;
}

int LoadPoint::Write(string x)
{
	if (Point == nullptr)
		return 1;
	switch (Type)
	{
	case Tstring:
		*((string*)Point) = x;
		break;
	defoult:
		return 1; // Несоответствие типов
	}
	return 0;
}

int LoadPoint::Write(LoadPoint x) // Записать величино из нагрузки
{
	if (Point == nullptr)
		return 1;
	switch (Type)
	{
	case TLoad:
		if (x.Type >> 1 != DLoad)
			return 1;
		*(LoadPoint*)Point = *(LoadPoint*)x.Point;
		break;
	case Tstring:
		if (x.Type >> 1 != Dstring)
			return 1;
		*(string*)Point = *(string*)x.Point;	
	case Tdouble:
		if (!x.isDigitBool())
			return 1;
		*((double*)Point) = x.toDouble();
		break;
	case Tfloat:
		if (!x.isDigitBool())
			return 1;
		*((float*)Point) = x.toFloat();
		break;
	case Tint:
		if(x.Type>>1==Dint || x.Type >> 1 == Dbool || x.Type >> 1 == Dchar)
			*((int*)Point) = x.toInt();
		else return 1;
		break;
	case Tbool:
		if (!x.isDigitBool() || x.Type >> 1 == Dstring)
			*((bool*)Point) = x.toBool();
		else return 1;
		break;
	case Tchar:
		if(x.Type >> 1 == Dchar || x.Type >> 1 == Dbool ||
			x.Type >> 1 == Dint && *(int*)x.Point>=0 && *(int*)x.Point <256)
		*((char*)Point) = x.toChar();
		break;

	defoult:
		WriteFromLoad(x); // Попытка записать по нагрузке
	}
	return 0;
}

int LoadPoint::Write(vector<LoadPoint> x)
{
	if (Point == nullptr || !isVector())
		return 1;
	*(LoadVect_type)Point = x;
	return 0;
}

unsigned int  LoadPoint::TypeMinimize(double x) // Минимизировать тип, т.е. было целое число - возвращается int и т.д.
{
	if (x == int(x) and abs(x) < 2147483647)
		return Tint;
	else if (x == 1 || x == 0)
		return Tbool;
	else
		return Tdouble;

}

LoadPoint LoadPoint::TypeMinimizeOut(double x, bool var) // Минимизировать тип (возвращается LoadPoint), т.е. было целое число - возвращается int и т.д.
{
	if (x == int(x) and abs(x) < 2147483647)
	{
		int* t = new int;
		return { (var ? Tint : Cint), t };
	}
	else if(x==1 || x==0)
	{
		bool* t = new bool;
		return { (var ? Tbool : Cbool), t };
	}
	else
	{
		double* t = new double;
		return { (var? Tdouble:  Cdouble), t };
	}
}

LoadPoint LoadPoint::Clone() // Вернуть клонированную нагрузку
{
	if (Point == nullptr)
		return { 0,nullptr };
	if (Type % 2 == 0) // Если переменная, то возвращаем указатель
		return *this;
	
	switch (Type >> 1)
	{
	case Dstring: return { Type, new string(*(string*)Point) };
	case Dint: return { Type, new int(*(int*)Point) };
	case Dfloat: return { Type, new float(*(float*)Point) };
	case Ddouble: return { Type,new double(*(double*)Point) };
	case Dchar: return { Type,new char(*(char*)Point) };
	case Dbool: return { Type,new bool(*(bool*)Point) };
	case DPPoint: return { Type,new (void*)(*(void**)Point) };
	case DIC: return { Type, ICCopy(*this).Point };
	case DIP: //return { Type, (*(ip*)Point).Сlone() };
	{
		vector<ip>* t = new vector<ip>;
		t->push_back(*((ip*)Point));
		//		((ip*)Point)->Load.Clone();
		return { Type, t };
	}
	case DLoadVect: // Копирование вектора нагрузок
	{
		vector<LoadPoint> t;
		for (auto& i : *(vector<LoadPoint>*)Point)
			t.push_back(i.Clone());
	}
	default: return *this;
	}
}

void* LoadPoint::VarClone() // Вернуть ссылку на клонированное значение из нагрузки
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
	}
}

void LoadPoint::VarDel() // Удаление нагрузки ИП
{
	if (Point == nullptr)
		return;
	switch (Type >> 1)
	{
	case Dstring: delete (string*)Point; break;
	case Dint: delete (int*)Point; break;
	case Dfloat: delete (float*)Point; break;
	case Ddouble: delete (double*)Point; break;
	case Dchar: delete (char*)Point; break;
	case Dbool: delete (bool*)Point; break;
	case DPPoint: delete (void**)Point; break;
	case DLoadVect: ((vector<LoadPoint>*)Point)->resize(0); delete ((vector<LoadPoint>*)Point); break;
		//	case DIP: return (*(ip*)Point).сlone();
		//	case DIC: return Point = ICCopy(*this);
	}
	Point = nullptr; Type = 0;
}

LoadPoint LoadPoint::Copy()
{
//	if (!this->isIC())
//		return { 0,nullptr };
//	IC_type t=new vector<ip>;
	
	LoadPoint LP;
	LP.Copy(*this);
	return LP;
}

void LoadPoint::Copy(LoadPoint LP)
{
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

void LoadPoint::Clear() // Сбросить нагрузку
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
// Печать вектора
void LoadPoint::LoadPoint::VarClear() // Сброс нагрузки ИП в том числе и с переменной (переменная стирается)
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
// Печать матрицы
void LoadPoint::MatrixPrint(unsigned int Type, void* P, map<int, string > AtrMnemo, string offset, string Sep, string End, string ArrayBracketStart, string ArrayBracketFin)
{
	switch ((Type % 1000) >> 1)
	{
	case Dstring: {
		cout << ArrayBracketStart;
		for (auto t = (*((vector< vector<string>>*)P)).begin(); t != (*(vector < vector<string>>*)P).end(); t++)
			VectorPrint(Type-1000, (void*)&(*t), AtrMnemo, offset, Sep, End, ArrayBracketStart, ArrayBracketFin);
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
		cout << ArrayBracketFin ;
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

void LoadPoint::print(map<int, string > AtrMnemo, string offset, string Sep, string End, string ArrayBracketStart, string ArrayBracketFin, map<void*, int> *AdrMap)
{
	if (Point == nullptr)
	{
		cout << "null\n";
		return;
	}
	switch (Type)
	{
	case Tstring:
	case Cstring: cout << "\"" << * (string*)Point << "\""; break;
	case Tint:
	case Cint:	  cout << *(int*)Point; break;
	case Tfloat:
	case Cfloat:  cout << *(float*)Point; break;
	case Tdouble:
	case Cdouble: cout << *(double*)Point; break;
	case Tchar:
	case Cchar:   cout << *(char*)Point; break;
	case Tbool:
	case Cbool:   cout << *(bool*)Point; break;
	case TIP:
	case CIP:
		if (((ip*)Point)->Load.Type >> 1 == DIP || isIC())
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
	case TIC:
	case CIC:
	{
		bool FMap = false; // Флаг создания списка пройденных адресов ОА-графа
		if (AdrMap == nullptr)
		{
			AdrMap = new map<void*, int>;
			FMap = true;
		}
		if (AdrMap->count(Point)) // Обнаружение зацикливания ОА-графа
		{
			cout << offset << "IC id: " << (*AdrMap)[Point] << endl;
			break;
		}
		(*AdrMap)[Point] = AdrMap->size(); // Запомнить пройденную ИК для избежания зацикливания

		for (auto i = ((IC_type)Point)->begin(); i != ((IC_type)Point)->end(); i++)
		{
			if (i->Load.Type >> 1 == DIP || i->Load.isIC())
				if (AtrMnemo.count(i->atr))
					cout << offset << AtrMnemo[i->atr] << " ->\n";
				else
					cout << offset << i->atr << " ->\n";
			else
				if (AtrMnemo.count(i->atr))
					cout << offset << AtrMnemo[i->atr] <<  ((i->Load.Type % 2) ? " # " : " = ");
				else
					cout << offset << i->atr << ((i->Load.Type % 2) ? " # " : " = ");
			i->Load.print(AtrMnemo, offset + "  ", Sep, End, ArrayBracketStart, ArrayBracketFin, AdrMap);
			if (i != ((IC_type)Point)->end() - 1)
				cout << endl;
		}
		if (FMap)  // Удачить таблицу пройденных адресов
		{
			AdrMap->clear();
			delete AdrMap;
		}
		break;
	}
	case TLoadVect:
	case CLoadVect: // Вектор нагрузок
	{
		cout << ArrayBracketStart;
		int c = 1;
		for (auto i : *(vector<LoadPoint>*) Point)
		{
			i.print(AtrMnemo, offset, Sep, End, ArrayBracketStart, ArrayBracketFin);
			if(c<((vector<LoadPoint>*) Point)->size()) cout << Sep;
			c++;
		}
		cout << ArrayBracketFin << endl;
		break;
	}
	default:
		if (Type >= 2000) // Печать матрицы и вектора
		{
			MatrixPrint(Type, Point, AtrMnemo, offset, Sep, End, ArrayBracketStart, ArrayBracketFin);
		}
		else if(Type >= 1000)
			VectorPrint(Type, Point, AtrMnemo, offset, Sep, End, ArrayBracketStart, ArrayBracketFin);
	}
}

// Работа с ФУ
void FU::CommonMk(int Mk, LoadPoint Load)
{
	switch (Mk)
	{
	case 902: // ActiveSet Установить активность ФУ (true по умолчанию)
		Active = Load.toBool(true);
		break;
	case ProgMk: // 958 Prog Вызов подпрограммы
	case ProgCycleMk: //959 CycleProg Вызов цикла
	case ProgPostCycleMk: //960 PostCycleProg Вызов пост цикла
		if (Alu != nullptr)
			((ALU*)Alu)->Stack.push_back({}); //Буферизиация текущего стека
		if (Load.Point == nullptr)
			ProgExec(Prog, Mk - ProgMk);
		else
			ProgExec(Load, Mk - ProgMk);
		if (Alu != nullptr)
			((ALU*)Alu)->Stack.pop_back(); // Отмена буферизации текущего стека
		break;
	case YesMk: //961 YesProg Вызов подпрограммы по ДА
	case YesCycleMk: //962 YesCycleProg Вызов цикла по ДА
	case YesPostCycleMk: //963 YesPostCycleProg Вызов пост цикла по ДА
		if (Accum.toBool())
		{
			if (Alu != nullptr)
				((ALU*)Alu)->Stack.push_back({}); //Буферизиация текущего стека
			if (Load.Point == nullptr)
				ProgExec(Prog, Mk - YesMk);
			else
				ProgExec(Load.Point, Mk - YesMk);
			if (Alu != nullptr)
				((ALU*)Alu)->Stack.pop_back(); // Отмена буферизации текущего стека
		}
		break;
	case NoMk: //964 NoProg Вызов подпрограммы по НЕТ
	case NoCycleMk: //965 NoCycleProg Вызов цикла по НЕТ
	case NoPostCycleMk: //966 NoPostCycleProg Вызов пост цикла по НЕТ
		if (Accum.toBool())
		{
			if (Alu != nullptr)
				((ALU*)Alu)->Stack.push_back({}); //Буферизиация текущего стека
			if (Load.Point == nullptr)
				ProgExec(Prog, Mk - NoMk);
			else
				ProgExec(Load.Point, Mk - NoMk);
			if (Alu != nullptr)
				((ALU*)Alu)->Stack.pop_back(); // Отмена буферизации текущего стека
		}
		break;
	case 919: // AccumPointerSet Установить ссылку на аккумулятор
		if (Load.Type >> 1 == Ddouble)
			Accum = Load;
		break;
	case 920: // AccumPointerOut Выдать ссылку на аккумулятор
		Load.Write(Accum);
		break;
	case 921: // AccumPointerOutMk Выдать МК со ссылкой на аккумулятор
		MkExec(Load,Accum);
		break;
	case 931: // AccumSet Установить значение аккумулятора
		if (Accum.Point = nullptr)
		{
			Accum = { Cdouble,new double };
			AccumCreating = true; // Устанавливаем флаг самостоятельного создания аккумулятора ФУ-ом
		}
		Accum.Write(Load.toDouble());
		AccumCreating = false;
		break;
	case 923: // AccumOut Выдать значение аккумулятора
		Load.WriteFromLoad(Accum);
		break;
	case 925: // AccumOutMk Выдать МК со значением аккумулятора
		MkExec(Load,Accum);
		break;
	case CalcMk: // 927 Calc вычислить АЛВ
	case 929: // CalcContinue Вычислить АЛВ и продолжеть выполнение программы, если результат true
	case 930: // CalcBreak Вычислить АЛВ и продолжеть выполнение программы, если результат false
		if (Alu == nullptr)
		{
			Alu = new ALU(this->Bus);
			Accum = { Cdouble, &((ALU*)Alu)->accum };
			AccumCreating = false;
			ALUCreating = true;
		}
		((FU*)Alu)->ProgFU(ProgExecMk,Load);
		break;
		if (Mk == 929 && Accum.toBool() || Mk == 930 && !Accum.toBool())
			ProgStop = 1; // Выход из программы
	case 932: // IntAluCalc Создать челочисленное АЛУ
		Alu = new IntAlu(this->Bus);
		Accum = { Tint, &((IntAlu*)Alu)->Accum };
		AccumCreating = false;
		ALUCreating = true;
		((FU*)Alu)->ProgFU(ProgExecMk, Load);
		break;
	case 989: // ProgStop Остановка программы (в нагрузке количество уровней выхода; если нагрузка nil, то присваивается 1)
		ProgStop = Load.toInt(1);
		break;
	case 988: // ProgStopAll Остановка всех запущенных на выполнение миллипрограммы для данного ФУ
		ProgStopAll = Load.toBool(true);
		break;
	case 987: //Next Переход к следующей итерации цикла
		CycleStop = Load.toInt();
		break;
	case 916:// ManualModeSet Установить режим ручного управления
		if (Modeling == nullptr) Modeling = new FUModeling();
		Modeling->ManualMode = Load.toBool();
		break;
	case 918: // SchedulerSet Установить контекст планировщика вычислений
		if (Modeling == nullptr) Modeling = new FUModeling();
		Modeling->scheduler = (FU*)Load.Point;
		Modeling->ManualMode = true;
		break;
	case 924: // PrеfixProgSet
		PrefixProg = (IC_type)Load.Point;
		break;
	case 922: // PostfixProgSet
		PostfixProg = (IC_type)Load.Point;
		break;
	case 926: // BusSet
		Bus = (FU*)Load.Point;
		break;
	case ProgExecMk: // ProgExec Выполнить программу (если в нагрузке null, То выполнить из регистра Prog
		if (Load.Point == nullptr)
			ProgExec((vector<ip>*)Prog);
		else
			ProgExec((vector<ip>*)Load.Point);
		break;
	case 991: // ProgSet // Установить указатель на программу
		Prog = (IC_type)Load.Point;
		break;
	case 953: // ElseProgSet
		ElseProg = (IC_type)Load.Point;
		break;
	case 954: // ProgSetExec Установить указатель на программу и выполнить ее
		Prog = (IC_type)Load.Point;
		ProgExec((vector<ip>*)Prog);
		break;
	case 995: //ContextOut Выдать указатель на контекст ФУ
		if (Load.Type >> 1 == Dvoid || Load.Type >> 1 == DPPoint || Load.Type >> 1 == DFU)
			Load.Point = this;
		break;
	case 928: // ALUSet Установить ссылку на АЛУ
		if (ALUCreating)
			delete Alu;
		Alu = Load.Point;
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
	case 999: //ContextOutMK Выдать милликоманду с указателем на контекст ФУ
		if (Load.Type >> 1 == Dint)
			Bus->ProgFU(*(int*)Load.Point, { TFU, this });
		break;
	}
}

// Запуск программы
//void FU::ProgExec(void* UK, FU* ProgBus, vector<ip>::iterator* Start) // Исполнение программы из ИК{
// CycleType тип цикла: 0 - без цикла, 1 - цикл, 2 - цикл с постусловием
void FU::ProgExec(void* UK, unsigned int CycleMode, FU* ProgBus, vector<ip>::iterator* Start) // Исполнение программы из ИК
{
	if (UK==nullptr) return;
	vector<ip>* Uk = (IC_type)UK;
	if (ProgBus == nullptr) ProgBus = Bus;
	bool RepeatF = false; // Флаг циклического повторения программы в ИК
	do
	{
		ProgStop = 0; // Останов программы (содержит количество уровней, из которых необходимо выйти
		RepeatF = false; // Флаг необходимости перехода на новый цикл выборки МК в ИК
		ProgStopAll = false; // Флаг остановки всей программы
		CycleStop = 0; // Счетчик выходов из циклов (если отрицательная величина, то Продолжение цикла)
		for (auto i = Start == nullptr ? Uk->begin() : *Start; i != Uk->end(); i++)
		{
			if (i->atr == GotoAtr) // Переход на другую ИК
			{
				ProgExec(i->Load, CycleMode, ProgBus, Start);
				return;
			}
			if (i->atr >= FUMkRange)
				ProgBus->ProgFU(i->atr, i->Load); // Если диапазон МК не принадлежит ФУ (выдаем на Bus)
			else // МК для данного ФУ
			{
				if (i->atr == BreakAtr) { ProgStop = i->Load.toInt()-1; return; } // Прервать программу
				if (i->atr == NextAtr)
					if (!i->Load.toInt())
					{
						i = Uk->begin();  // Продолжение текущего цикла
						continue;
					}
					else
					{
						CycleStop = i->Load.toInt(); 
						return;
					} // Переход к следующим итерациям циклов
				// Выход из цикла по условию
				if (i->atr == YesContinueAtr || i->atr == NoContinueAtr) 
				{
					if (CycleMode == 2) // Проверка цикла с постусловием (пропускаем первую проверку)
					{
						CycleMode = 1;
						continue;
					}
					if (i->Load.isProg() && Alu != nullptr) // Запуск вычисления АЛВ
//						((FU*)Alu)->ProgExec(i->Load.Point);
//
					// Перейти к следующей итерации цикла continue
					if (i->Load.isProg())
					{
						if (Alu == nullptr)
						{
							Alu = new ALU(this);
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
						CycleMode = 0; // Выход из цикла
						break;
					} // Выход из цикла
					continue;
				}
				if (i->atr == RepeatAtr) { // Запустить программу заново
					RepeatF = true; break;
				}

				if (i->atr == ProgMkAtr || // Переход к подрограмме
					i->atr == YesAtr && Accum.toBool() ||
					i->atr == NoAtr && !Accum.toBool()) {
					((ALU*)Alu)->Stack.push_back(((ALU*)Alu)->Stack.back());
					ProgExec(i->Load, 0, ProgBus);
					((ALU*)Alu)->Stack.pop_back();
					((ALU*)Alu)->accum = ((ALU*)Alu)->Stack.back().accum;
					continue;
				}
				if (i->atr == YesCycleAtr && Accum.toBool() || // Переход к циклу с постусловием
					i->atr == NoCycleAtr && !Accum.toBool() ||
					i->atr == ProgCycleAtr) {
					((ALU*)Alu)->Stack.push_back(((ALU*)Alu)->Stack.back());
					ProgExec(i->Load, 1, ProgBus);
					((ALU*)Alu)->Stack.pop_back();
					((ALU*)Alu)->accum = ((ALU*)Alu)->Stack.back().accum;
					continue;
				}
				if (i->atr == YesPostCycleAtr && Accum.toBool() || // Переход к циклу
					i->atr == NoPostCycleAtr && !Accum.toBool() ||
					i->atr == ProgPostCycleAtr) {
					((ALU*)Alu)->Stack.push_back(((ALU*)Alu)->Stack.back());
					ProgExec(i->Load, 2, ProgBus);
					((ALU*)Alu)->Stack.pop_back();
					((ALU*)Alu)->accum = ((ALU*)Alu)->Stack.back().accum;
					continue;
				}

				ProgFU(i->atr, i->Load); // Выполнение команды
			}

			if (CycleStop != 0) // Остановка циклов
				if (!CycleMode) // Если не в режиме цикла, то просто выходим из уровня
					return;
				else
					if (CycleStop > 0)
						CycleStop--; // уменьшение счетчика выходов из цикла
					else
					{
						CycleStop++;
						if (!CycleStop) // уменьшение счетчика выходов из цикла
							i = Uk->begin(); // Продолжение цикла
						return;
					}
			if (ProgStop > 0) { ProgStop--; return; }
			if (ProgStopAll) { return; } // Внеочередной выход из подпрограммы
		}
	} while (RepeatF || CycleMode>0);
}

// Запуск программы по указателю из нарузки ИП
void FU::ProgExec(LoadPoint Uk, unsigned int CycleMode, FU* Bus, vector<ip>::iterator* Start) // Исполнение программы из ИК
{
	if (Uk.isIC())
		ProgExec(Uk.Point, CycleMode, Bus, Start);
}

void FU::MkExec(int MK, LoadPoint Load, FU* BusContext, bool Ext) // Выдача МК с нагрузкой
{
	if (MK < FUMkRange && !Ext) // Если МК адресована сомому ФУ
		ProgFU(MK, Load);
	else
		if (BusContext != nullptr)
			BusContext->ProgFU(MK, Load);
		else
			Bus->ProgFU(MK, Load);
}

void FU::MkExec(LoadPoint Mk, LoadPoint Load, FU* BusContext, bool Ext) // Выдача МК с нагрузкой
{
	if (Mk.Type >> 1 == Dint && Mk.Point != nullptr)
	{
		int MK = *(int*)Mk.Point;
		if (MK < FUMkRange && !Ext) // Если МК адресована сомому ФУ
			ProgFU(MK, Load);
		else
			if (BusContext != nullptr)
				BusContext->ProgFU(MK, Load);
			else
				Bus->ProgFU(MK, Load);
	}
}

void FU::Scheduling()
{
	if (Modeling == nullptr) return;
	if (Modeling->qmk.size() != 0)
	{
		Modeling->SchedulerFlag = true;
		if (Modeling->qmk.size() == 0)
			cout << "Modeling error\n";
		else
		{
			ip t = Modeling->qmk.back();
			Modeling->qmk.pop_back();
			ProgFU(t.atr, t.Load);
			if (t.Load.Type % 2 == 1 && t.Load.Point != nullptr)
				t.Load.VarDel();
		}
	}
}

bool LoadCmp(LoadPoint x, LoadPoint y) // Сравнение двух нагрузок ИП
{
	// Доработать с указателями на ИК и ИП
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

bool IPCmp(ip* x, ip* y) // Сравнение двух  ИП
{
	if (!LoadCmp(x->Load, y->Load))
		return false;
	else
		if (x->atr == y->atr || x->atr == 0 || y->atr == 0)
			return true;
		else
			return false;
}

// Работа с ИК
bool AtrSearch(void* uk, int Atr) // Поиск атриубута в ИК
{
	for (auto& i : *(IC_type)uk)
		if (i.atr == Atr)
			return true;
	return false;
}

int AtrCounter(void* uk, int Atr) // Подсчет количества ИП с заданнным атриубутом в ИК
{
	int c = 0;
	for (auto& i : *(IC_type)uk)
		if (i.atr == Atr)
			c++;
	return c;
}

void ICDel(void* Uk) // Удаление ИК
{
	for (auto&& i : *(IC_type)Uk)
		i.Load.Clear();
	delete (IC_type)Uk;
}

void ICDel(LoadPoint &Uk)// Удаление ИК
{
	if (!Uk.isIC()) return;
	ICDel(Uk.Point);
	Uk = { 0,nullptr };
}

LoadPoint ICCopy(LoadPoint uk) // Копирование ИК
{
	IC_type CapsNew = new vector<ip>;
//	if (uk.Point = nullptr)
//		return {TIC, CapsNew};
	if (uk.Type >> 1 == DIP) // Если передается ИП
	{
		CapsNew->push_back(*(*(ip*)uk.Point).Сlone());
		return { uk.Type, CapsNew };
	}
	IC_type Uk = (IC_type)uk.Point;
	CapsNew->resize(((IC_type)Uk)->size());
	for (auto i = ((IC_type)Uk)->begin(), j = CapsNew->begin(); j != CapsNew->end(); i++, j++)
	{
		j->atr = i->atr;
		j->Load.Copy(i->Load);
	}
	return { uk.Type, CapsNew };
}

void* GraphCopy(void* Uk, LocatTable* Table = nullptr) // Копирование ОА-графа
{
	return nullptr;
}

void GraphDel(void* Uk, LocatTable* Table = nullptr) // Удаление ОА-графа
{
	return;
}
// ------------------------



// Найти в ИК ИП с атрибутом Atr и выполнить программу либо по адр. в нагрузке, либо после найденной ИП, если атрибут не найден, возвращается true
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

ip* AtrFind(void* IC, int Atr) // Поиск в ИК ИП с заданным атрутом
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

bool isIPinIC(void* iP, void* iC) //проверка, что ИП входит в ИК
{
	for (auto i = ((IC_type)iC)->begin(); i != ((IC_type)iC)->end(); i++) {
		if (IPCmp(i._Ptr, ((ip*)iP)))
			return true;
	}
	return false;
}

void ICCopyConcat(void* uk, void* uk2) // Конкатенация двух ИК
{
	//copy( ((IC_type)uk2)->begin(), ((IC_type)uk2)->end(), inserter(*((IC_type)uk), ((IC_type)uk)->end()));
	for (auto& i : *(IC_type)uk2)
		((IC_type)uk)->push_back(i.Copy());
}

int ICLen(void* uk) // Определитель длины ИК
{
	return (((IC_type)uk)->size());
}

vector<ip>::iterator IPSearch(void* ic, ip IP) // Поиск ИП в ИК (возвращается указатель на персую найденную ИП
{
	for (auto i = ((IC_type)ic)->begin(); i != ((IC_type)ic)->end(); i++)
		if (IPCmp(i._Ptr, &IP))
			return i;
	return ((IC_type)ic)->end();
}

vector<ip>::iterator IPSearch(void* ic, LoadPoint IP) // Поиск ИП в ИК (возвращается указатель на персую найденную ИП
{
	if (IP.isIP())
		return IPSearch(ic, *((ip*)IP.Point));
	else if(IP.isIC())
		return IPSearch(ic, *((IC_type)IP.Point)->begin());
	return ((IC_type)ic)->end();
}
void IPAdd(void* IC, ip IP) // Добавить ИП в конец ИК
{
	((vector<ip>*)IC)->push_back(IP.Copy());
}

void IPAdd(LoadPoint IC, ip IP) // Добавить ИП в конец ИК
{
	if (!IC.isIC())return;
	IPAdd(IC.Point, IP);
}

void* MakeLoadFromDouble(double x, unsigned int Type) // Создать нагрузку из типа double
{
	if (Type > 0) // Задан тип
	{
		LoadPoint P = { Type, nullptr };
		if (!P.isDigitBool()) return nullptr;
	}
	else // Автоматическое определерние типа
	{
		if (x == 0 || x == 1)
			Type = Tbool;
		else if (x == int(x))
			Type = Tint;
		else
			Type = Tdouble;
	}

	switch (Type>>1)
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

/*
function <LoadPoint(LoadPoint, LoadPoint) > Add = [](LoadPoint x, LoadPoint y)->LoadPoint // Сложение
{
	if (x.IsStrChar() && y.IsStrChar()) // Две строки
		return {Tstring,new string(x.ToStr()+y.ToStr())};

	if(!(x.isDigitBool() && y.isDigitBool())) // Два числа
		return { 0, nullptr }; // Не числа
	double t = x.ToDouble() + y.ToDouble();
	switch (max(x.Type,y.Type)>>1)
	{
	case Dbool:
		return { Tbool,(void*)new bool((bool)t) };
		break;
	case Dint:
		return { Tint,(void*)new int((int)t) };
		break;
	case Dfloat:
		return { Tfloat,(void*)new float((float)t) };
		break;
	case Ddouble:
		return { Tdouble,(void*)new double(t)};
		break;
	}
	return { 0,nullptr };
};
*/


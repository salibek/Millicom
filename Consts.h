// Описание основных констант и информационных конструкций
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <set>
#include <string>
//#include "LocationTable.h"

using namespace std;

// Типы переменных
const int Tvoid = 0, Tbool = 2, Tchar = 4, Tint = 6, Tfloat = 8, Tdouble = 10,  Tstring = 12, TIP = 14, TIC = 16;
const int TPPoint = 18, TGraph = 20, TFU = 22, TLoad = 24, Tmk = 26, TLoadVect=28;
const int TvoidArray = 1000, TboolArray = 1002, TCharArray=1004, TintArray = 1006, TfloatArray = 1008, TdoubleArray = 1010, TstringArray = 1012;
const int TIPArray = 1014, TICArray = 1016, TPPointArray = 1018, TGrapgArray = 1020, TFUArray = 1022, TLoadArray = 1024;
const int TboolArray2 = 2002, TcharArray2 = 2004, TintArray2 = 2006, TfloatArray2 = 2008, TdoubleArray2 = 2010, TstringArray2 = 2012;
const int TIPArray2 = 2014, TICArray2 = 2016, TPPointArray2 = 2018, TGrapgArray2 = 2020, TFUArray2 = 2022, TLoadArray2 = 2024;
// Типы констант
const int Cvoid = 1, Cbool = 3, Cchar = 5, Cint = 7, Cfloat = 9, Cdouble = 11, Cstring = 13, CIP = 15, CIC = 17;
const int CPPoint = 19, CGraph = 21, CFU = 23, CLoad = 25, Cmk=27, CLoadVect=29;
const int CvoidArray = 1001, CboolArray=1003, CcharArray = 1005, CintArray = 1007, CfloatArray = 1009, CdoubleArray = 1011, CstringArray = 1013;
const int CIPArray = 1015, CICArray = 1017, CPPointArray = 1019, CGrapgArray = 1021, CFUArray = 1023, CLoadArray = 1025;
const int CvoidArray2 = 2001, CboolArray2 = 2003, CcharArray2 = 2005, CintArray2 = 2007, CfloatArray2 = 2009, CdoubleArray2 = 2011, CstringArray2 = 2013;
const int CIPArray2 = 2015, CICArray2 = 2017, CPPointArray2 = 2019, CGrapgArray2 = 2021, CFUArray2 = 2023, CLoadArray2 = 2025;
// Общие типы данных (остаток от целочисленного деления на 2 типа переменной или константы)
const int Dvoid=0, Dbool = 1, Dchar = 2, Dint = 3, Dfloat = 4, Ddouble = 5, Dstring = 6, DIP = 7,  DIC = 8;
const int DPPoint = 9, DGraph = 10, DFU = 11, DLoad = 12, Dmk=14, DLoadVect=16;
const int DVoidArray = 500, DboolArray = 501, DcharArray = 502, DintArray = 503, DfloatArray = 504, DdoubleArray = 505, DstringArray = 506;
const int DIPArray = 507, DICArray = 508, DPPointArray = 509, DGrapgArray = 510, DFUArray = 511, DLoadArray = 512;
const int DvoidArray2 = 1000, DboolArray2 = 1001, DcharArray2 = 1002, DintArray2 = 1003, DfloatArray2 = 1004, DdoubleArray2 = 1005, DstringArray2 = 1006;
const int DIPArray2 = 1007, DICArray2 = 1008, DPPointArray2 = 1009, DGrapgArray2 = 1010, DFUArray2 = 1011, DLoadArray2 = 1012;

// Типы ФУ
const int FUBus = 0, FUCons = 1, FUStrGen = 2, FULex = 3, FUList = 4, FUFind = 5;

// Веса типов
const vector<int> w_type = { 10, 30, 40, 50, 60, 70, 20 };

// Общие атрибуты
const int ProgAtr = -100, Atr = -60, SubObj = -6;
const int ListLine = -80, ListSub = -90; // Атрибуты линии списка и подсписка
// Общие МК
const int ProgMk=958, ProgCycleMk = 959, ProgPostCycleMk = 960;
const int YesMk = 961, YesCycleMk = 962, YesPostCycleMk = 963;
const int NoMk = 964, NoCycleMk = 965, NoPostCycleMk = 966;
const int CalcMk = 927; // Милликоманда вычисления АЛВ
const int BreakMk = 909, NextMk = 910; // МК прерывания программы и продолжения цикла
const int RepeatMk=911; // Начать выполнение ИК заново
const int YesContinueMk = 969, NoContinueMk = 970;
const int ProgExecMk = 990; // МК выполнения программы
bool isIPinIC(void* iP, void* iC); //проверка, что ИК входит в ИП

class LoadPoint
{
	void VectorPrint(unsigned int Type, void* P, map<int, string > AtrMnemo, string offset, string Sep, string End, string ArrayBracketStart, string ArrayBracketFin); // Печать вектора
	void MatrixPrint(unsigned int Type, void* P, map<int, string > AtrMnemo, string offset, string Sep, string End, string ArrayBracketStart, string ArrayBracketFin); // Пачать матрицы
public:
	unsigned int Type = 0; // Неизвестный тип
	void *Point=nullptr;
	bool isDigit(int type = -1) { int t = type < 0 ? Type : type;  return t >> 1 == Dint || t >> 1 == Dchar || t >> 1 == Dfloat || t >> 1 == Ddouble; }; // 
	bool isDigitBool(int type=-1) { int t = type < 0 ? Type : type; return t >> 1 == Dint || t >> 1 == Dchar || t >> 1 == Dfloat || t >> 1 == Ddouble || t >> 1 == Dbool; }; // Число или булеан?
	bool isIC(); // Определить указывает ли ссылка на ИК
	bool isIP(); // Определить указывает ли ссылка на ИП
	bool IpTest() { return (Type >> 1 == DIP || Type >> 1 == DIC); } // Является ли нагрузка ИП?
	bool IsConvert(unsigned int T) {}; // Тест на возможность конвертации значения из Point в определенный тип
	bool IsProg() { return Point!=nullptr && Type >> 1 == DIC; }; // Определение может ли быть нагрузка программой
	int Write(int x); // return 0 - корректная запись, 1 - несоотвествие типов
	int Write(size_t x);
	int Write(double x);
	int Write(float x);
	int Write(bool x);
	int Write(char x);
	int Write(string x);
	int Write(LoadPoint x);
	int WriteFromLoad(LoadPoint x); // Записать величину из нагрузки
	void Write(vector<double> x);
	void Write(vector<float> x);
	void Write(vector<bool> x);
	void Write(vector<char> x);
	void Write(vector<int> x);

	void Write(void* x) { Point = x; };
	void WriteVar(LoadPoint x) { Point = x.Point; Type = x.Type; Type |= 1; Type--; }; //Записать ссылку и сделать ее переменной
	void WriteConst(LoadPoint x) {Point = x.Point; Type = x.Type; Type |= 1;}; // Записать ссылку и сделать ее константой

	string ToStr(string define=""); // Первод в string
	bool ToBool(bool define = false); // Перевод в bool
	int ToInt(int define=0); // Перевод в integer
	double ToDouble(double define=0); // Перевод в double
	float ToFloat(float define=0);// Перевод во float
	char ToChar() { return Point == nullptr ? 0 : *(char*)Point; }; // Перевод в integer
	LoadPoint Copy();
	void Copy(LoadPoint LP);
	void Clear(); // Сброс нагрузки ИП
	void VarClear(); // Сброс нагрузки ИП в том числе и с переменной (переменная стирается)
	void* VarClone(); // Копирование значения нагрузки
	void VarDel();// Удаление нагрузки ИП
	void print(map<int, string > AtrMnemo = {}, string offset = "", string Sep = "", string End = "\n", string ArrayBracketStart = "[", string ArrayBracketFin = "]", map<void*, int> *AdrMap = nullptr); // Параметр - указатель на табл. мнемоник атрибутов
	LoadPoint Clone(); // Дублировать нагрузку
	void ConstTypeSet(bool F = true) { if (F)Type |= 1; else VarTypeSet(); }; // Установить тип 'константа'
    // Установить тип 'переменная'
	void VarTypeSet(bool F = true) {
		if (!F) { ConstTypeSet(); return; } Type |= 1; Type -= 1;}
	LoadPoint IpOut() // Возвращается указатель на ИП или на первую ИП из ИК, иначе null
	{
		if (Type >> 1 == DIP) return *this;
//		if (Type >> 1 == DIC) return { DIP,((IC_type)Point)->begin()._Ptr};
//		return { 0,nullptr };
	};
};

// struct TAtrMnemo

class ip // Информационная пара
{
public:
	int atr = 0;
	LoadPoint Load = {0, nullptr }; // Указатель на нагрузку с типом даннных
	~ip() { }//Load.Clear(); };
	void copy(ip *IP)// Копирование ИП
	{
		atr = IP->atr;
		Load.Point = IP->Load.Point;
		Load.Type = IP->Load.Type;
	};
	void copy(void *IP) { copy(*(ip*)IP); }// Копирование ИП
	void copy(ip &IP)// Копирование ИП
	{
		atr = IP.atr;
		Load.Copy(IP.Load);
	};
	void copy(LoadPoint &LP)// Копирование ИП
	{
		switch (LP.Type >> 1)
		{
		case DIP:
			atr = ((ip*)LP.Point)->atr;
			Load.Copy(((ip*)LP.Point)->Load);
			break;
		case DIC:
			atr = ((vector<ip>*)LP.Point)->begin()->atr;
			Load.Copy(((vector<ip>*)LP.Point)->begin()->Load);
		}
	};

	ip copy() // Копирование ИП
	{
		ip* ip_new = new ip;
		ip_new->copy(*this);
		return *ip_new;
	};

	ip* Сlone()
	{
		ip* ip_new = new ip;
		ip_new->copy(*this);
		return ip_new;
	};

	vector<ip>* СloneToIC()
	{
		vector<ip>* t = new vector<ip>;
		t->push_back(*this);
		return t;
	};
};

typedef  vector<vector<ip>*> ICVect;
typedef  vector<ip>* IC_type;

struct deletedIC //удаленная ИП
{
	vector<ip*>* IC; // информационная капсула, содержащая удаленную ИП
	void* programm; // указатель на милипрограмму (ИК, содержащую набор милликоманд)
};

class FUModeling
{
public:
	bool SchedulerFlag = false; // Флаг запуска МК планироващиком
	vector<ip> qmk; // Очередь МК для моделирования
	bool ManualMode = false; // Режим ручного управления (для моделирования)
	map<int, double> MkTime; // Время выполнения операций (для моделирования)
	void* scheduler = nullptr; // Указатель на контекст планировщика вычислений
};

class FU {  // Ядро функционального устройства
public:
	virtual void ProgFU(int MK, LoadPoint Load) {}; // Реализация логики работы ФУ
	void Scheduling(); // Запуск МК после разрешенрия планировщика
	int FUtype = 0; // Тип ФУ
	string FUName; //  Имя ФУ
	bool Active = true;
	LoadPoint Accum = { 0,nullptr }; // Указатель на аккумулятор
	bool AccumCreating = false; // Флаг создания аккумулятора (аккумулятор создается при первой записи в него какого-либо значения)
	void* Alu = nullptr; // Ссылка на АЛУ
	bool ALUCreating = false; // Флаг создания АЛУ

	FUModeling *Modeling=nullptr; // Моделирование

	FU() { Bus = nullptr; ProgFU(0, { 0,nullptr }); };
	FU(FU* BusContext) { Bus = BusContext; ProgFU(0, { 0,nullptr }); };
	~FU() { 
		if (AccumCreating) Accum.Clear(); // Уничтожаем самостоятельно созданнный ФУ-ом аккумулятор
		if (ALUCreating) delete Alu; // Уничтожаем самостоятельно созданнного АЛУ
	};

	void MkExec(int MK, LoadPoint Load, FU* BusContext = nullptr, bool Ext = false); // Выполнить одну милликоманду 
	void MkExec(LoadPoint MK, LoadPoint Load, FU* BusContext = nullptr, bool Ext=false); // Выдача МК с нагрузкой
	void ProgExec(void* Uk, unsigned int CycleMode = 0, FU* Bus = nullptr, vector<ip>::iterator* Start = nullptr); // Исполнение программы из ИК
	void ProgExec(LoadPoint Uk, unsigned int CycleMode = 0, FU* Bus = nullptr, vector<ip>::iterator* Start = nullptr); // Исполнение программы из ИК
	int SubAtr=ProgMk; // Атрибут входа в подпрограмму

	FU *Bus; // Ссылка на контекст Шины
	int FUMkRange = 1000; // Диапазон МК для каждого ФУ
	int ProgStop = 0; // Флаг остановки программы, выполняемой ProgExec
	int CycleStop = 0; // Флаг остановки цикла программы, выполняемой ProgExec
	bool ProgStopAll = false; // Флаг остановки всех запущенных на выполнение миллипрограммы для данного ФУ
	int YesAtr=YesMk, NoAtr=NoMk, YesCycleAtr = YesCycleMk, NoCycleAtr = NoCycleMk, YesPostCycleAtr = YesPostCycleMk, NoPostCycleAtr = NoPostCycleMk;
	int RepeatAtr = RepeatMk;//Мк повтория программы
	int BreakAtr = BreakMk;//Мк повторения программы
	int NextAtr = NextMk;//Мк повторения программы
	int ProgMkAtr=ProgMk, ProgCycleAtr= ProgCycleMk, ProgPostCycleAtr = ProgPostCycleMk;
	int YesContinueAtr = YesContinueMk, NoContinueAtr = NoContinueMk; // МК условного выхода из программы
	void CommonMk(int Mk, LoadPoint Uk); // Выполнение общих МК для ФУ
	IC_type PrefixProg = nullptr, PostfixProg = nullptr, Prog = nullptr, ElseProg = nullptr; // Программы презапуска и послезапуска во время прихода МК, просто программа, альтернативная программа
private:
//	int ProgSetFaze = 0; // Фаза для установки программы ProgSet, ElseProgSet
};

//void GraphDel(void* Uk, LocatTable* Table = nullptr); // Удаление ОА-графа
void ICDel(void* Uk);// Удаление ИК
void ICDel(LoadPoint &Uk);// Удаление ИК

LoadPoint ICCopy(LoadPoint uk);// Копирование ИК
void ICCopyConcat(void* uk, void* uk2); // Конкатенация двух ИК
int ICLen(void* uk); // Определитель длины ИК

//void ProgExec(void *Uk, FU* Bus, vector<ip>::iterator *Start=nullptr); // Исполнение программы из ИК

bool LoadCmp(LoadPoint x, LoadPoint y); // Сравнение двух нагрузок ИП
bool IPCmp(ip* x, ip* y); // Сравнение двух  ИП
vector<ip>::iterator IPSearch(void* ic, ip IP); // Поиск ИП в ИК (возвращается указатель на персую найденную ИП
vector<ip>::iterator IPSearch(void* ic, LoadPoint IP); // Поиск ИП в ИК (возвращается указатель на персую найденную ИП
void AtrProgExec(vector<ip>* Prog, int Atr, FU* Bus=nullptr, bool AfterContinue = false); // Найти в ИК ИП с атрибутом Atr и выполнить программу либо по адр. в нагрузке, либо после найденной ИП
//void AddOrReplIPAtr(vector<ip>* UK, ip* IP); // 
ip* AtrFind(void* IC, int Atr); // Поиск в ИК ИП с заданным атрибутом. На выходе указатель на ИП или NULLL
bool AtrSearch(void* uk, int Atr); // Поиск атриубута в ИК
int AtrCounter(void* uk, int Atr); // Подсчет количества ИП с заданнным атриубутом в ИК

void IPAdd(void* IC, ip IP); // Добавить ИП в конец ИК
void IPAdd(LoadPoint IC, ip IP); // Добавить ИП в конец ИК

// �������� �������� �������� � �������������� �����������
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <functional>
//#include "LocationTable.h"

using namespace std;
// ���� ����������
const unsigned int Tvoid = 0, Tbool = 2, Tchar = 4, Tint = 6, Tfloat = 8, Tdouble = 10,  Tstring = 12, TIP = 14, TIC = 16;
const unsigned int TPPoint = 18, TGraph = 20, TFU = 22, TLoad = 24, Tmk = 26, TLoadVect = 28, TLoadVectInd = 30, TICInd = 32, THashList=34;
const unsigned int TCalc = 100, TProg=102; // C����� �� �������������� ��-����, ���������
const unsigned int TvoidArray = 1000, TboolArray = 1002, TCharArray=1004, TintArray = 1006, TfloatArray = 1008, TdoubleArray = 1010, TstringArray = 1012;
const unsigned int TIPArray = 1014, TICArray = 1016, TPPointArray = 1018, TGrapgArray = 1020, TFUArray = 1022, TLoadArray = 1024;
const unsigned int TboolArray2 = 2002, TcharArray2 = 2004, TintArray2 = 2006, TfloatArray2 = 2008, TdoubleArray2 = 2010, TstringArray2 = 2012;
const unsigned int TIPArray2 = 2014, TICArray2 = 2016, TPPointArray2 = 2018, TGrapgArray2 = 2020, TFUArray2 = 2022, TLoadArray2 = 2024;
// ���� ��������
const unsigned int Cvoid = 1, Cbool = 3, Cchar = 5, Cint = 7, Cfloat = 9, Cdouble = 11, Cstring = 13, CIP = 15, CIC = 17;
const unsigned int CPPoint = 19, CGraph = 21, CFU = 23, CLoad = 25, Cmk=27, CLoadVect = 29, CLoadVectInd = 31, CICInd = 33, CHashList = 35;
const unsigned int CCalc = 101, CProg=103; // C����� �� �������������� ��-����, ���������
const unsigned int CvoidArray = 1001, CboolArray=1003, CcharArray = 1005, CintArray = 1007, CfloatArray = 1009, CdoubleArray = 1011, CstringArray = 1013;
const unsigned int CIPArray = 1015, CICArray = 1017, CPPointArray = 1019, CGrapgArray = 1021, CFUArray = 1023, CLoadArray = 1025;
const unsigned int CvoidArray2 = 2001, CboolArray2 = 2003, CcharArray2 = 2005, CintArray2 = 2007, CfloatArray2 = 2009, CdoubleArray2 = 2011, CstringArray2 = 2013;
const unsigned int CIPArray2 = 2015, CICArray2 = 2017, CPPointArray2 = 2019, CGrapgArray2 = 2021, CFUArray2 = 2023, CLoadArray2 = 2025;
// ����� ���� ������ (������� �� �������������� ������� �� 2 ���� ���������� ��� ���������)
const unsigned int Dvoid=0, Dbool = 1, Dchar = 2, Dint = 3, Dfloat = 4, Ddouble = 5, Dstring = 6, DIP = 7,  DIC = 8;
const unsigned int DPPoint = 9, DGraph = 10, DFU = 11, DLoad = 12, DMk=13, DLoadVect = 14, DLoadVectInd = 15, DICInd = 16, DHashList = 17;
const unsigned int DCalc = 50, DProg=51; // C����� �� �������������� ��-����, ���������
const unsigned int DVoidArray = 500, DboolArray = 501, DcharArray = 502, DintArray = 503, DfloatArray = 504, DdoubleArray = 505, DstringArray = 506;
const unsigned int DIPArray = 507, DICArray = 508, DPPointArray = 509, DGrapgArray = 510, DFUArray = 511, DLoadArray = 512;
const unsigned int DvoidArray2 = 1000, DboolArray2 = 1001, DcharArray2 = 1002, DintArray2 = 1003, DfloatArray2 = 1004, DdoubleArray2 = 1005, DstringArray2 = 1006;
const unsigned int DIPArray2 = 1007, DICArray2 = 1008, DPPointArray2 = 1009, DGrapgArray2 = 1010, DFUArray2 = 1011, DLoadArray2 = 1012;

// ���� ��
const int FUBus = 0, FUCons = 1, FUStrGen = 2, FULex = 3, FUList = 4, FUFind = 5;

// ����� ��������
const int ProgAtr = -100, Atr = -60, SubObj = -6, GotoAtr=-99;
const int ListLine = -80, ListSub = -90; // �������� ����� ������ � ���������
// ����� ��
const int ProgMk=958, ProgCycleMk = 959, ProgPostCycleMk = 960;
const int YesMk = 961, YesCycleMk = 962, YesPostCycleMk = 963, YesBreakMk =967;
const int NoMk = 964, NoCycleMk = 965, NoPostCycleMk = 966, NoBreakMk =968;
const int CalcMk = 927; // ������������ ���������� ���
const int BreakMk = 934, NextMk = 935; // �� ���������� ��������� � ����������� �����
const int RepeatMk=911; // ������ ���������� �� ������
const int YesContinueMk = 969, NoContinueMk = 970;
const int ProgExecMk = 990; // �� ���������� ���������
const int FUIndSetMk = 933; // �� ��������� ������� ��
const int ContextOutMkMk = 999; // �� ������ �� � ���������� ��
const int ContextOutMk = 995; // �� ������ ��������� ��
const int SchedulerSetMk = 918; // �� ��������� ������������ ����������
const int MkGlobalRangeSet = 949; // ���������� ���������� ����� �� ��� ��
const int MkGlobalRangeOutMk = 948; // ������ ���������� ����� �� ��� ��
const int MkGlobalRangeOutMkMK = 947; // ������ �� � ���������� ������� �� ��� ��
const int EventserCurrentTimeOutMk = 50; // Ìê äëÿ êîíòðîëëåðà ñîáûòèé, ÷òîáû âûäàòü òåêóùåå ìîäåëüíîå âðåìÿ
const int EventserFUSetMk = 10; // ÌÊ êîíòðîëëåðà ñîáûòèé äëÿ óñòàíîâêè êîíòåêñòà ÔÓ äëÿ îïèñàíèÿ ñîáûòèÿ
const int EventTimeSetMk = 11; // ÌÊ êîíòðîëëåðà ñîáûòèé äëÿ óñòàíîâêè âðåìåíè ñîáûòèÿ, èíèöèèðîâàííîãî ïëàíèðîâùèêîì âû÷èñëèòåëüíîãî ïðîöåññà
const int AwaitMkSetMk = 15; //  ÌÊ êîíòðîëëåðà ñîáûòèé äëÿ óñòàíîâêè âðåìåíè ïðèõîäà óäàëåííîé ÌÊ

bool isIPinIC(void* iP, void* iC); //��������, ��� �� ������ � ��

class FU;
class LoadPoint;
class ip;

typedef  vector<vector<ip>*> ICVect;
typedef  vector<ip>* IC_type;
typedef  vector<LoadPoint>* LoadVect_type;

class LoadPoint
{
	void VectorPrint(unsigned int Type, void* P, map<int, string > AtrMnemo, string offset, string Sep, string End, string ArrayBracketStart, string ArrayBracketFin); // ������ �������
	void MatrixPrint(unsigned int Type, void* P, map<int, string > AtrMnemo, string offset, string Sep, string End, string ArrayBracketStart, string ArrayBracketFin); // ������ �������
public:
	unsigned int Type = 0; // ����������� ���
	void *Point=nullptr; // ��������� �� ������� ������
	int Ind = -1; // ������ ���� � �� ��� �������  ��� �� �� ������ 3. 0 - ����� ��, 1- ����� ��������, 2 - ����� ��������
	unsigned int getType(); // ������ ��� ��������
	int DataSize(); // ������ ������ ������ � ��������
	bool isDigit(); // �����?}
	static bool isDigit(unsigned int type) { unsigned int t = type; return t >> 1 == Dint || t >> 1 == Dchar || t >> 1 == Dfloat || t >> 1 == Ddouble; }; // 
	bool isDigitBool(); // ����� ��� ������?
	static bool isDigitBool(int type) { unsigned int t = type; return t >> 1 == Dint || t >> 1 == Dchar || t >> 1 == Dfloat || t >> 1 == Ddouble || t >> 1 == Dbool; }; // ����� ��� ������?

	bool isBool(); // ������?
	static bool isBool(unsigned int type) { unsigned int t = type; return type >> 1 == Dbool; }; // ����� ��� ������?

	bool isInt(); // ����� �����?
	static bool isInt(int type) { unsigned int t = type; return type >> 1 == Dint; }; // ����� �����?
	bool isIntBool(); // ������ ��� ����� �����?
	static bool isIntBool(int type) { unsigned int t = type; return type >> 1 == Dint || type >> 1 == Dbool; }; // ����� ��� ������?
	bool isFloatDouble(); // ������?
	static bool isFloatDouble(int type) { unsigned int t = type; return type >> 1 == Ddouble || type >> 1 == Dfloat; }; // ������
	bool isFloat(); // ����� ��� ������?
	static bool isFloat(int type) { unsigned int t = type; return type >> 1 == Dfloat; }; // ����� ��� ������?
	bool isDouble(); // ������?
	static bool isDouble(int type) { unsigned int t = type; return type >> 1 == Ddouble; };
	static bool isDoubleInt(int type) { return type >> 1 == Ddouble || type>>1==Ddouble; }; // ����� ��� ������?
	bool isIC(); // ���������� ��������� �� ������ �� ��
	bool isIP(); // ���������� ��������� �� ������ �� ��
	bool isCalc(); // ���������� ��������� �� ������ �� �� � ���
	bool isProg(); // ���������� ��������� �� ������ �� �� � ����������
	bool isICSet(); // ���������� ��������� �� ������ �� �� � ��������������� ����������
	bool IpTest() { return (Type >> 1 == DIP || Type >> 1 == DIC); } // �������� �� �������� ��?
	bool IsConvert(unsigned int T) {}; // ���� �� ����������� ����������� �������� �� Point � ������������ ���
	static bool isProg(unsigned int type) { return type >> 1 == DIC; }; // ����������� ����� �� ���� �������� ����������
	bool isStrChar(); // ������ ��� ������?
	static bool isStrChar(unsigned int type) { return type >> 1 == Dstring || type >> 1 == Dchar; }; // ������ ��� ������?
	bool isStr(); // ������?
	static bool isStr(unsigned int type) { return type >> 1 == Dstring; }; // ������?
	bool isChar(); // ������?
	static bool isChar(unsigned int type) {return type >> 1 == Dchar; }; // ������?
	bool isMk();// ������������?
	static bool isMk(unsigned int type) { return type >> 1 == DMk; }; // ������������?
	bool isVectInd() { return Type >> 1 == DLoadVectInd; }; // ��������������� ������� ������� ��������
	static bool isVectInd(int type) { unsigned int t = type; return t >> 1 == DLoadVectInd; }; //��������������� ������� ������� ��������
	bool isICInd() { return Type >> 1 == DICInd; }; // ��������������� ������� ��
	static bool isICInd(int type) { unsigned int t = type; return t >> 1 == DICInd; }; //��������������� ������� ��
	bool isInd() { return Type >> 1 == DICInd || Type >> 1 == DLoadVectInd; }; // ��������������� �������
	static bool isInd(int type) { unsigned int t = type; return t >> 1 == DICInd || t >> 1 == DLoadVectInd; }; // ��������������� �������
	LoadPoint IndLoadReturn(); // ���������� ��������� �� �������� ���������������� ��������
	bool isVectIndVectInd();  // ��������������� ������� ������� �������� �� ���������������� ������� ��������
	static LoadPoint IndLoadReturn(LoadPoint LP); // ���������� ��������� �� �������� ���������������� ��������

	bool isVect(); // ������ �� ��������
	static bool isVect(unsigned int type) { return (type >> 1) == DLoadVect; }; // ������ �� ��������
	int Write(int x); // return 0 - ���������� ������, 1 - ������������� �����
	int Write(size_t x);
	int Write(double x);
	int Write(float x);
	int Write(bool x);
	int Write(char x);
	int Write(string x);
	int Write(LoadPoint x);
	int WriteFromLoad(LoadPoint x); // �������� �������� �� ��������
	int Write(vector<double> x);
	int Write(vector<float> x);
	int Write(vector<bool> x);
	int Write(vector<char> x);
	int Write(vector<int> x);
	int Write(vector<LoadPoint> x); // ����������� ������� �������� ��������
	int Write(vector<LoadPoint>* x); // ������ ������ �� ������ ��������
	template <typename T>
	int WriteByVector(T x); // ������ �� ��������� � �������

	static LoadPoint TypeMinimizeOut(double x, bool var = false); // �������������� ��� (������������ LoadPoint), �.�. ���� ����� ����� - ������������ int � �.�.
	static unsigned int  TypeMinimize(double x); // �������������� ���, �.�. ���� ����� ����� - ������������ int � �.�.

	int Write(void* x) { Point = x; return 0; };
	int Write(FU* x) { if (Type == TFU) Point = x; else return 1; return 0;}; // ������ �� �������� ��

	void WriteVar(LoadPoint x) { Point = x.Point; Type = x.Type; Type |= 1; Type--; }; //�������� ������ � ������� �� ����������
	void WriteConst(LoadPoint x) {Point = x.Point; Type = x.Type; Type |= 1;}; // �������� ������ � ������� �� ����������
	vector<LoadPoint>* LoadVect() { return (vector<LoadPoint>*)Point; }; // ������� ������ �� ������ ��������
//	LoadPoint Operation(function <LoadPoint(LoadPoint, LoadPoint) > F, LoadPoint y) // �������� (�� ���� ���������� ������-�������)
//	{
//		return F(*this, y);
//	};

	string toStr(string define=""); // ������ � string
	bool toBool(bool define = false); // ������� � bool
	int toInt(int define=0); // ������� � integer
	double toDouble(double define=0); // ������� � double
	float toFloat(float define=0);// ������� �� float
	char toChar() { return Point == nullptr ? 0 : *(char*)Point; }; // ������� � integer
	LoadPoint Copy();
	void Copy(LoadPoint LP);
	void Clear(); // ����� �������� ��
	void VarClear(); // ����� �������� �� � ��� ����� � � ���������� (���������� ���������)
	void* VarClone(); // ����������� �������� ��������
	void VarDel();// �������� �������� ��
	void print(map<int, string > AtrMnemo = {}, string offset = "", string Sep = " ", string End = "\n", string quote = """",  string ArrayBracketStart = "[", string ArrayBracketFin = "]", map<void*, int> *AdrMap = nullptr); // �������� - ��������� �� ����. �������� ���������
	LoadPoint Clone(bool All=false); // ����������� �������� (All - ���� ����������� ����� ��������, � ��� ����� � ����������)
	static LoadPoint Clone(LoadPoint LP); // // ����������� �������� (������� � ������������ � �������� ��������� ��������)
	void ConstTypeSet(bool F = true) { if (F)Type |= 1; else VarTypeSet(); }; // ���������� ��� '���������'
    // ���������� ��� '����������'
	void VarTypeSet(bool F = true) {
		if (!F) { ConstTypeSet(); return; } Type |= 1; Type -= 1;}
	LoadPoint IpOut() // ������������ ��������� �� �� ��� �� ������ �� �� ��, ����� null
	{
		if (Type >> 1 == DIP) return *this;
		if (isIC()) return {TIC, IC()->begin()._Ptr };
	};
	LoadVect_type Vect(); // ���������� ���������� �� ������
	IC_type IC();
};

// struct TAtrMnemo

class ip // �������������� ����
{
public:
	int atr = 0;
	LoadPoint Load = {0, nullptr }; // ��������� �� �������� � ����� �������
	~ip() { }//Load.Clear(); };
	void copy(ip *IP)// ����������� ��
	{
		atr = IP->atr;
		Load.Point = IP->Load.Point;
		Load.Type = IP->Load.Type;
	};
	void copy(void *IP) { copy(*(ip*)IP); }// ����������� ��
	void copy(ip &IP)// ����������� ��
	{
		atr = IP.atr;
		Load.Copy(IP.Load);
	};
	void copy(LoadPoint &LP)// ����������� ��
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

	ip Copy() // ����������� ��
	{
		return { atr,Load.Clone() };
	};

	ip* �lone()
	{
		ip* ip_new = new ip;
		ip_new->copy(*this);
		return ip_new;
	};

	vector<ip>* �loneToIC()
	{
		vector<ip>* t = new vector<ip>;
		t->push_back(*this);
		return t;
	};
};

class ipSender : public ip // ����� ��� ���������� �� � ����������� �� (��� ���������� � ������� �������� �� ��� �������������
{
public:
	FU* Sender=nullptr; // ����������� ��
};


struct deletedIC //��������� ��
{
	vector<ip*>* IC; // �������������� �������, ���������� ��������� ��
	void* programm; // ��������� �� ������������� (��, ���������� ����� �����������)
};

class ipSender : public ip // Êëàññ äëÿ ðàçìåùåíèÿ ÌÊ è îòïðàâèòåëÿ ÌÊ (äëÿ ïðèìåíåíèÿ â î÷åðåäè îæèäàíèÿ ÌÊ ïðè ìîäåëèðîâàíèè
{
public:
	FU* Sender = nullptr; // Îòïðàâèòåëü ÌÊ
};

class Event { // Описание события
public:
	bool SchedulerFlag; // Флаг того, что МК была передана от планировщика событий, т.е. она должна выполняться. В противном случае она поступает в очередь ожидания МК
	FU* Receiver;       // Получатель МК
};

class FUModeling
{
public:
	bool SchedulerFlag = false; // ���� ������� �� ��������������
	vector<ipSender> qmk; // Очередь МК для моделирования
	multimap<double, ipSender> qAwaitMk; // Î÷åðåäü îæèäàþùèõ ÌÊ äëÿ ìîäåëèðîâàíèÿ (íà íàõîäÿòñÿ â ïðîöåññå ïåðåäà÷è ê ÔÓ, íàïðèìåð, âî âðåìÿ ïåðåäà÷è ïî ñåòè)
	bool ManualMode = false; // ����� ������� ���������� (��� �������������)
	map<int, double> MkTime; // ����� ���������� �������� (��� �������������)
	void* scheduler = nullptr; // Указатель на контекст планировщика вычислений
	FU* eventser = nullptr; // Ссылка на контроллер событий
	void EventModelingPrint(); // Вывести состояние моделирования ФУ
};

class FU {  // ���� ��������������� ����������
public:
	virtual void ProgFU(int MK, LoadPoint Load, FU* Sender) {}; // Реализация логики работы ФУ
	void Scheduling(bool SchedulerFlag); // Запуск МК после разрешенрия планировщика
	void MkAwait(int MK, LoadPoint Load, FU* Sender, double Delay); // Постановка МК для ожидания прихода
	int FUtype = 0; // ��� ��
	string FUName; //  ��� ��
	bool Active = true;
	LoadPoint Accum = { 0,nullptr }; // ��������� �� �����������
	bool AccumCreating = false; // ���� �������� ������������ (����������� ��������� ��� ������ ������ � ���� ������-���� ��������)
	FU* Alu = nullptr; // ������ �� ���
	bool ALUCreating = false; // ���� �������� ���
	FU* Parent = nullptr; // ������ �� ������������ ��
	int FUInd = -1; // ������ ��
	int  FUMkGlobalRange = 0; // ���������� ����� ��

	FUModeling *Modeling=nullptr; // �������������

	FU() { Bus = nullptr; ProgFU(0, { 0,nullptr }, nullptr); };
	FU(FU* BusContext) { Bus = BusContext; ProgFU(0, { 0,nullptr }, nullptr); };
	~FU() {
		if (AccumCreating) Accum.Clear(); // ���������� �������������� ���������� ��-�� �����������
		if (ALUCreating) delete Alu; // ���������� �������������� ����������� ���
	};

	void MkExec(int MK, LoadPoint Load, FU* BusContext = nullptr, bool Ext = false); // ��������� ���� ������������ 
	void MkExec(LoadPoint MK, LoadPoint Load, FU* BusContext = nullptr, bool Ext=false); // ������ �� � ���������
	void ProgExec(void* Uk, unsigned int CycleMode = 0, FU* Bus = nullptr, vector<ip>::iterator* Start = nullptr); // ���������� ��������� �� ��
	void ProgExec(LoadPoint Uk, unsigned int CycleMode = 0, FU* Bus = nullptr, vector<ip>::iterator* Start = nullptr); // ���������� ��������� �� ��
	int SubAtr=ProgMk; // ������� ����� � ������������

	FU *Bus; // ������ �� �������� ����
	int FUMkRange = 1000; // �������� �� ��� ������� ��
	int FUMkGloabalRange = 0; // ������ ����������� ��������� �� (�.�. ������� ��, ������� ��������� � ������)
	int ProgStop = 0; // ���� ��������� ���������, ����������� ProgExec
	int CycleStop = 0; // ���� ��������� ����� ���������, ����������� ProgExec
	bool ProgStopAll = false; // ���� ��������� ���� ���������� �� ���������� �������������� ��� ������� ��
	int YesAtr=YesMk, NoAtr=NoMk, YesCycleAtr = YesCycleMk, NoCycleAtr = NoCycleMk, YesPostCycleAtr = YesPostCycleMk, NoPostCycleAtr = NoPostCycleMk;
	int RepeatAtr = RepeatMk;//�� �������� ���������
	int BreakAtr = BreakMk;//�� ���������� ���������
	int NextAtr = NextMk;//�� ���������� ���������
	int ProgMkAtr=ProgMk, ProgCycleAtr= ProgCycleMk, ProgPostCycleAtr = ProgPostCycleMk;
	int YesContinueAtr = YesContinueMk, NoContinueAtr = NoContinueMk; // �� ��������� ������ �� ���������
	void CommonMk(int Mk, LoadPoint Uk, FU* Sender=nullptr); // ���������� ����� �� ��� ��
	IC_type PrefixProg = nullptr, PostfixProg = nullptr, Prog = nullptr, ElseProg = nullptr; // ��������� ���������� � ������������ �� ����� ������� ��, ������ ���������, �������������� ���������
private:
//	int ProgSetFaze = 0; // ���� ��� ��������� ��������� ProgSet, ElseProgSet
};

LoadPoint LoadNew(int t); //�������� �������� �� ����������
LoadPoint LoadNew(double t); //�������� �������� �� ����������
LoadPoint LoadNew(bool t); //�������� �������� �� ����������
LoadPoint LoadNew(string t); //�������� �������� �� ����������
LoadPoint LoadNew(float t); //�������� �������� �� ����������
//void GraphDel(void* Uk, LocatTable* Table = nullptr); // �������� ��-�����
void ICDel(void* Uk);// �������� ��
void ICDel(LoadPoint &Uk);// �������� ��

LoadPoint ICCopy(LoadPoint uk, bool Copy=false); // ����������� �� (Copy - ���� ��������������� ����������� ���� �� � ��-�����)
void ICCopyConcat(void* uk, void* uk2); // ������������ ���� ��
int ICLen(void* uk); // ������������ ����� ��
LoadPoint CopyAdrCorrect(LoadPoint Adr, LoadPoint OriginalIC, LoadPoint CopyIC, set<void*> buf); // ������������� ������ � ������������ ��  (���������� ��������)

//void ProgExec(void *Uk, FU* Bus, vector<ip>::iterator *Start=nullptr); // ���������� ��������� �� ��

bool LoadCmp(LoadPoint x, LoadPoint y); // ��������� ���� �������� ��
bool IPCmp(ip* x, ip* y); // ��������� ����  ��
vector<ip>::iterator IPSearch(void* ic, ip IP); // ����� �� � �� (������������ ��������� �� ������ ��������� ��
vector<ip>::iterator IPSearch(void* ic, LoadPoint IP); // ����� �� � �� (������������ ��������� �� ������ ��������� ��
bool AtrProgExec(vector<ip>* Prog, int Atr, FU* Bus=nullptr, bool AfterContinue = false); // ����� � �� �� � ��������� Atr � ��������� ��������� ���� �� ���. � ��������, ���� ����� ��������� ��
//void AddOrReplIPAtr(vector<ip>* UK, ip* IP); // 
ip* AtrFind(void* IC, int Atr); // ����� � �� �� � �������� ���������. �� ������ ��������� �� �� ��� NULLL
bool AtrSearch(void* uk, int Atr); // ����� ��������� � ��
int AtrCounter(void* uk, int Atr); // ������� ���������� �� � ��������� ���������� � ��

void IPAdd(void* IC, ip IP); // �������� �� � ����� ��
void IPAdd(LoadPoint IC, ip IP); // �������� �� � ����� ��
void* MakeLoadFromDouble(double x, unsigned int Type); // ������� �������� �� ���� double

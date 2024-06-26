#pragma once
#include "Consts.h"
#include "Search.h"
#include "ALU.h"
#include <set>

class Search
{
private:
	bool null_check();// �������� �� ���� (false, ���� �� � �������)
	void MkAtrExec();
public:
	~Search() { if (Alu != nullptr) delete Alu; };
//	vector<ip>* Template; // ������ ��� ������
	long int _CalcMk = CalcMk; // ������� ���������� ���
	bool CalcMode = false; // ����� ���������� ���������
	FU* Alu = nullptr; // ������ �� ���
	bool PredicatCalc(IC_type Templ); // ������� ���������� ���������, ���� �� ������������ � ��
	LoadPoint Template={0, nullptr}; // ������ ��� ������
	LoadPoint Obj = { 0, nullptr }; //�� ��� ������
	bool MkMode = true; //  ����� ���������� ���� �� � ��-������� (��-�� ��������� ����� �������, ������ �������� ������ 0)
	set<long int> MkAtr; // ������ ��������, �������������� ��, ������������� � ������ ��������� ������
	FU *MainFU; // ������ �� ������� ��, ��������� ���������� ������������ searcher-�
	bool Rez=false; // ��������� ������
	vector<ip> *SuccessProg = nullptr, *FailProg = nullptr; // ��������� �� ��������� �� ������ � �������� ������
	vector<ip> *SuccessAfterProg = nullptr, *FailAfterProg = nullptr; // ��������� �� ��������� �� ������ � �������� ������, ������������� ����� ��������� ��������� �����
	ip *IPTemplRezPoint = nullptr, *IPRezPoint = nullptr; // ��������� �� ��������� �� � ������� � � ��
	long int IP_Num = -1, IPTempl_Num=-1; // ����� ������ ��������� �� � �� � �������
	vector<ip>* BiggerProg = nullptr, *EqProg = nullptr, * SmallerProg = nullptr; // ��������� �� ��������� �� ������� ������������ �� ������ ��������� �� � �������
	long int Prog_atr = ProgAtr; // ������� ���������, ������������� ��� �������� ������ (<0 ������� �� �����������)
	long int Sub_atr = ProgMk; // ������� ���������, ������������� ��� �������� ������ (<0 ������� �� �����������)
	void Clear();
	void MkAtrAdd(int MK) { MkAtr.insert(MK); };
	void MkAtrClear() { MkAtr.clear(); };
	bool FindIPObj(LoadPoint obj, LoadPoint Templ, bool XOR=false); // �����, ���� obj �������� �� (��� Or And)
	bool FindOr(LoadPoint obj); // �����
	bool FindXor(LoadPoint obj); // �����
	bool FindAnd(LoadPoint obj); // �����
	bool FindAndSource(LoadPoint obj); // �����
	bool FindIP(LoadPoint obj) {}; // �������� ��
};
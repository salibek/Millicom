// ������ ��� ������ � ��, ������������ � FUFind � FUList
#include "stdafx.h"
#include <string.h>
#include <algorithm>
#include "Search.h"

using namespace std;

bool Search::PredicatCalc(IC_type Templ) // ������� ���������� ���������, ���� �� ������������ � ��
{
	ip* t = AtrFind(Templ, _CalcMk);
	if (t == nullptr) return true;
	if (Alu == nullptr) // �������� ���
	{
		Alu = (FU*)new ALU({});
		Alu->ProgFU(0, { 0,nullptr }, nullptr);
	}
	double tt = 1;
	Alu->ProgExec(t->Load);
	Alu->ProgFU(2, {Tdouble, &tt}, nullptr); // ������ ���������� �� ���
	return tt;
}

bool Search::null_check()// �������� �� ���� (false, ���� �� � �������)
{
	if (Template.Point != nullptr && Obj.Point != nullptr)
		return false;
	else
	{
		IPTemplRezPoint = nullptr;
		IPRezPoint = nullptr;
		Rez = false;
		return true;
	}
}

// ����������, ����� ��������� ���������� � ������� ProgExec !!!!!!!!!!!!
void  Search::MkAtrExec() // ��������� ������������ �� ��-�������
{

	if (Template.Point == nullptr || Template.Type>>1!=DIC) return;
	MainFU->ProgStopAll = false;
	MainFU->ProgStop = 0;
	MainFU->CycleStop = 0;

	if (MkMode)
	{
		auto i = ((IC_type)Template.Point)->begin();
		for (; i < ((IC_type)Template.Point)->end(); i++)
			if (i->atr > 0) break;
		if(i!= ((IC_type)Template.Point)->end())
			MainFU->ProgExec(Template, 0, nullptr, &i);
	}
	else
		for (auto i = ((IC_type)Template.Point)->begin(); i < ((IC_type)Template.Point)->end(); i++)
			if (MkAtr.count(i->atr))
				MainFU->MkExec(i->atr, i->Load);
/*
	for (auto i = ((IC_type)Template.Point)->begin(); i < ((IC_type)Template.Point)->end(); i++)
		if (MainFU->ProgStopAll)
			break;
		else if (MainFU->ProgStop > 0){
			MainFU->ProgStop--;
			break;
		}
		else
		if (MkMode)
		{
			if (i->atr == ProgAtr && i->Load.Point==nullptr)
				break;
			if(i->atr>0) MainFU->MkExec(i->atr, i->Load);
		}
		else
			if (MkAtr.count(i->atr))
				MainFU->MkExec(i->atr, i->Load);
*/
}

bool Search::FindIPObj(LoadPoint Templ, LoadPoint obj, bool XOR) // �����, ���� obj �������� �� (��� Or And)
{
	if(Templ.Type>>1==DIP && obj.Type >> 1 == DIP)// �������� � ������ - ��� ��
		if(IPCmp((ip*)Templ.Point, (ip*)obj.Point))
		{
			Rez = true;
			IPTemplRezPoint = (ip*)Templ.Point;
			IPRezPoint = (ip*)obj.Point;
			return true;
		}
		else
		{
			Rez = false; IPTemplRezPoint = nullptr; IPRezPoint = nullptr;
			MainFU->ProgExec(FailProg);
			return false;
		}
	int Count = 0; // ������� ���������� ���������� ��
	auto i = ((IC_type)Templ.Point)->begin();
	for (; i != ((IC_type)Templ.Point)->end() && (Prog_atr < 0 || i->atr != Prog_atr); i++)
	{
		if (MkAtr.count(i->atr)) continue;
		if (IPCmp(i._Ptr, (ip*)obj.Point))
			if(!XOR)
				break;
			else
			{
				Count++;
				if (Count > 2) break;
			}
	}
	if (i == ((IC_type)Templ.Point)->end() || i->atr == Prog_atr || Count==2)
	{
		Rez = false; IPTemplRezPoint = nullptr; IPRezPoint = nullptr;
		MainFU->ProgExec(FailProg);
		return false;
	}
	else
	{
		Rez = true;
		if (CalcMode) Rez &= PredicatCalc((IC_type)Template.Point); // ����� ���������� ���������
		IPTemplRezPoint = i._Ptr;
		IPRezPoint = (ip*)obj.Point;
		return true;
	}
}
bool Search::FindOr(LoadPoint obj)
{
	Obj = obj; // ��������� �� ��� ������
	if (null_check()) return false;
	// ����� ���� ��
	Rez = false;
	IPTemplRezPoint = nullptr;
	IPRezPoint = nullptr;
	if (Obj.Type >> 1 == DIP)
		if (!FindIPObj(Template, Obj))
			return false;
		else
		{
			MainFU->ProgExec(SuccessProg);
			AtrProgExec((IC_type)Template.Point, Prog_atr, MainFU->Bus, true);
			MkAtrExec();
			MainFU->ProgExec(SuccessAfterProg);
			return true;
		}
	auto i = ((IC_type)Template.Point)->begin();
	for (; i != ((IC_type)Template.Point)->end() && (Prog_atr < 0 || i->atr != Prog_atr); i++)
	{
		if (MkAtr.count(i->atr)) continue;
		auto j = ((vector<ip>*)(Obj.Point))->begin();
		for (; j != ((vector<ip>*)(Obj.Point))->end() && (Prog_atr < 0 || j->atr != Prog_atr); j++)
		{
			if (MkAtr.count(j->atr)) continue;
			if (IPCmp(i._Ptr, j._Ptr))
			{
				IPTemplRezPoint = i._Ptr;
				IPRezPoint = j._Ptr;
				break;
			}
		}
		if (j != ((vector<ip>*)(Obj.Point))->end() || i->atr == Prog_atr)
			break;
	}
	if (i == ((IC_type)Template.Point)->end() || i->atr == Prog_atr)
	{
		Rez = false; IPTemplRezPoint = nullptr; IPRezPoint = nullptr;
		MainFU->ProgExec(FailProg);
		return false;
	}
	else
	{
			Rez = true;
			if (CalcMode) Rez &= PredicatCalc((IC_type)Template.Point); // ����� ���������� ���������
			if (!Rez)
			{
				IPTemplRezPoint = nullptr; IPRezPoint = nullptr;
				MainFU->ProgExec(FailProg);
				return false;
			}
			MainFU->ProgExec(SuccessProg);
			AtrProgExec((IC_type)Template.Point, Prog_atr, MainFU->Bus, true);
			MkAtrExec();
			MainFU->ProgExec(SuccessAfterProg);
			return true;
	}
}

bool Search::FindXor(LoadPoint obj)
{
	int  Count = 0; // ������� ���������� ����������
	Obj = obj;
	Rez = false;

	if (Obj.Type >> 1 == DIP)
		if (!FindIPObj(Template, Obj, true))
			return false;
		else
		{
			MainFU->ProgExec(SuccessProg);
			AtrProgExec((IC_type)Template.Point, Prog_atr, MainFU->Bus, true);
			MkAtrExec();
			MainFU->ProgExec(SuccessAfterProg);
			return true;
		}

	if (null_check()) return false;
	for (auto i = ((IC_type)Template.Point)->begin();
	Count<2 && i != ((IC_type)Template.Point)->end() && (Prog_atr < 0 || i->atr != Prog_atr); i++)
	{
		if (MkAtr.count(i->atr)) continue;
		for (auto j = ((vector<ip>*)(Obj.Point))->begin();
		Count<2 && j != ((vector<ip>*)(Obj.Point))->end() && (Prog_atr < 0 || j->atr != Prog_atr); j++)
		{
			if (MkAtr.count(j->atr)) continue;
			if (IPCmp(i._Ptr, j._Ptr))
			{
				Count++;
				IPTemplRezPoint = i._Ptr;
				IPRezPoint = j._Ptr;
			}
		}
	}
	if (Count == 1)
	{// ����������� �������
		Rez = true;
		if (CalcMode) Rez &= PredicatCalc((IC_type)Template.Point); // ����� ���������� ���������
		if (!Rez)
		{
			IPTemplRezPoint = nullptr; IPRezPoint = nullptr;
			MainFU->ProgExec(FailProg);
			return false;
		}
		MainFU->ProgExec(SuccessProg);
		AtrProgExec((IC_type)Template.Point, Prog_atr, MainFU->Bus, true);
		MkAtrExec();
		MainFU->ProgExec(SuccessAfterProg);
		return true;
	}
	else
	{
		Rez = false;
		IPTemplRezPoint = nullptr;
		IPRezPoint = nullptr;
		MainFU->ProgExec(FailProg);
		return false;
	}
}

bool Search::FindAnd(LoadPoint obj) // �����
{
	Obj = obj;
	if (null_check()) return false;
	IPTemplRezPoint = nullptr;
	IPRezPoint = nullptr;
	Rez = false;
	int IP_NumOld = IP_Num; // ��������� ������ ����� ��������� ��

	if (Obj.Type >> 1 == DIP)
		if (!FindIPObj(Template, Obj))
			return false;
		else
		{
			MainFU->ProgExec(SuccessProg);

			AtrProgExec((IC_type)Template.Point, Prog_atr, MainFU->Bus, true);
			MkAtrExec();
			MainFU->ProgExec(SuccessAfterProg);
		//	IP_Num = ;
			return true;
		}

	auto i = ((vector<ip>*)(Obj.Point))->begin();
	for (; i != ((vector<ip>*)(Obj.Point))->end(); i++)
	{
		if (MkAtr.count(i->atr)) continue;
		auto j = ((IC_type)Template.Point)->begin();
		for (; j != ((IC_type)Template.Point)->end() && (!IPCmp(i._Ptr,j._Ptr) || (MkAtr.count(j->atr))); j++);
		if (j == ((IC_type)Template.Point)->end()) break;
		if (IPTemplRezPoint ==nullptr)
		{
			IPTemplRezPoint = j._Ptr;
			IPRezPoint = i._Ptr;
		}
	}
	if (i==((vector<ip>*)(Obj.Point))->end())
	{// ����������� �������
		Rez = true;
		if (CalcMode) Rez &= PredicatCalc((IC_type)Template.Point); // ����� ���������� ���������
		if (!Rez) // ���� �������� false
		{
			IPTemplRezPoint = nullptr; IPRezPoint = nullptr;
			MainFU->ProgExec(FailProg);
			return false;
		}
		MainFU->ProgExec(SuccessProg);
		AtrProgExec((IC_type)Template.Point, Prog_atr, MainFU->Bus);
		MkAtrExec();
		MainFU->ProgExec(SuccessAfterProg);
		return true;
	}
	else
	{
		Rez = false;
		IPTemplRezPoint = nullptr;
		IPRezPoint = nullptr;
		MainFU->ProgExec(FailProg);
		return false;
	}
}

bool Search::FindAndSource(LoadPoint obj) // �����
{
	Obj = obj;
	Rez = false;
	if (null_check()) return false;
	IPTemplRezPoint = nullptr;
	IPRezPoint = nullptr;

	if (Template.Type >> 1 == DIP && obj.Type >> 1 == DIP)
		if (!FindIPObj(Template, Obj))
			return false;
		else
		{
			MainFU->ProgExec(SuccessProg);
			AtrProgExec((IC_type)Template.Point, Prog_atr, MainFU->Bus, true);
			MkAtrExec();
			MainFU->ProgExec(SuccessAfterProg);
			return true;
		}

	if (Template.Type >> 1 == DIP)
		if (!FindIPObj(Obj, Template))
			return false;
		else
		{
			swap(IPTemplRezPoint,IPRezPoint);
			MainFU->ProgExec(SuccessProg, 0, MainFU->Bus);
			AtrProgExec((IC_type)Template.Point, Prog_atr, MainFU->Bus, true);
			MkAtrExec();
			MainFU->ProgExec(SuccessAfterProg);
			return true;
		}

	if (Obj.Type >> 1 == DIP)
	{
		auto i = ((IC_type)Template.Point)->begin();
		for (; i != ((IC_type)Template.Point)->end() && IPCmp(i._Ptr,(ip*)obj.Point); i++);
		if (i == ((IC_type)Template.Point)->end())
		{
			Rez = true;
			if (CalcMode) Rez &= PredicatCalc((IC_type)Template.Point); // ����� ���������� ���������
			if (!Rez) // ���� �������� false
			{
				IPTemplRezPoint = nullptr; IPRezPoint = nullptr;
				MainFU->ProgExec(FailProg);
				return false;
			}
			IPTemplRezPoint = (ip*)Template.Point;
			IPRezPoint = (ip*)obj.Point;
			return true;
		}
		else
		{
			Rez = false;
			IPTemplRezPoint = nullptr;
			IPRezPoint = nullptr;
			return false;
		}
		
	}


	auto i = ((IC_type)Template.Point)->begin();
	for (; i != ((IC_type)Template.Point)->end(); i++)
	{
		if (MkAtr.count(i->atr)) continue;
		auto j = ((vector<ip>*)(Obj.Point))->begin();
		for (; j != ((vector<ip>*)(Obj.Point))->end() && (!IPCmp(i._Ptr, j._Ptr) || (MkAtr.count(j->atr))); j++);
		if (j == ((vector<ip>*)(Obj.Point))->end()) break;
		if (IPTemplRezPoint == nullptr)
		{
			IPTemplRezPoint = i._Ptr;
			IPRezPoint = j._Ptr;
		}
	}
	if (i != ((IC_type)Template.Point)->end())
	{// ����������� �������
		Rez = true;
		if (CalcMode) Rez &= PredicatCalc((IC_type)Template.Point); // ����� ���������� ���������
		if (!Rez) // ���� �������� false
		{
			IPTemplRezPoint = nullptr; IPRezPoint = nullptr;
			MainFU->ProgExec(FailProg);
			return false;
		}
		MainFU->ProgExec(SuccessProg, 0, MainFU->Bus);
		AtrProgExec((IC_type)Template.Point, Prog_atr, MainFU->Bus, true);
		MkAtrExec();
		MainFU->ProgExec(SuccessAfterProg);
		return true;
	}
	else
	{
		Rez = false;
		IPTemplRezPoint = nullptr;
		IPRezPoint = nullptr;
		MainFU->ProgExec(FailProg);
		return false;
	}
}
void Search::Clear()
{
	IP_Num = -1; IPTempl_Num = -1;
	Template = { 0, nullptr };
	Obj = { 0, nullptr };
	MkMode = true;
	MkAtr.clear();
	Rez = false;
	SuccessProg = nullptr;  FailProg = nullptr;
	SuccessAfterProg = nullptr;  FailAfterProg = nullptr;
	IPTemplRezPoint = nullptr;  IPRezPoint = nullptr;
	IP_Num = -1; IPTempl_Num = -1;
	BiggerProg = nullptr; EqProg = nullptr;  SmallerProg = nullptr;
	Prog_atr = ProgAtr;
}
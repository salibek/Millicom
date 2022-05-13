// Объект для поиска в ИК, встраивается в FUFind и FUList
#include "stdafx.h"
#include <string.h>
#include <algorithm>
#include "Search.h"

using namespace std;

bool Search::null_check()// Проверка на нуль (false, если всё в порядке)
{
	if (Template.Point != nullptr && Obj.Point != nullptr)
		return false;
	else
	{
		IPTemplPoint = nullptr;
		IPPoint = nullptr;
		Rez = false;
		return true;
	}
}

void  Search::MkAtrExec() // Выполнить Милликоманды из ИК-шаблона
{
	if (Template.Point == nullptr || Template.Type>>1!=DIC) return;
	for (auto i=((IC_type)Template.Point)->begin();i<((IC_type)Template.Point)->end();i++)
		if (MkMode)
		{
			if (i->atr == ProgAtr && i->Load.Point==nullptr)
				break;
			if(i->atr>0) MainFU->MkExec(i->atr, i->Load);
		}
		else
			if (MkAtr.count(i->atr))
				MainFU->MkExec(i->atr, i->Load);
}

bool Search::FindIPObj(LoadPoint Templ, LoadPoint obj, bool XOR) // Поиск, если obj является ИП (Для Or And)
{
	if(Templ.Type>>1==DIP && obj.Type >> 1 == DIP)// Нагрузка и объект - оба ИП
		if(IPCmp((ip*)Templ.Point, (ip*)obj.Point))
		{
			Rez = true;
			IPTemplPoint = (ip*)Templ.Point;
			IPPoint = (ip*)obj.Point;
			return true;
		}
		else
		{
			Rez = false; IPTemplPoint = nullptr; IPPoint = nullptr;
			MainFU->ProgExec(FailProg);
			return false;
		}
	int Count = 0; // Счетчик количества совпадений ИП
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
		Rez = false; IPTemplPoint = nullptr; IPPoint = nullptr;
		MainFU->ProgExec(FailProg);
		return false;
	}
	else
	{
		Rez = true;
		IPTemplPoint = i._Ptr;
		IPPoint = (ip*)obj.Point;
		return true;
	}
}
bool Search::FindOr(LoadPoint obj)
{
	Obj = obj; // Запомнить ИК для поиска
	if (null_check()) return false;
	// Поиск одно ИП
	Rez = false;
	IPTemplPoint = nullptr;
	IPPoint = nullptr;
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
				IPTemplPoint = i._Ptr;
				IPPoint = j._Ptr;
				break;
			}

		}
		if (j != ((vector<ip>*)(Obj.Point))->end() || i->atr == Prog_atr)
			break;
	}
	if (i == ((IC_type)Template.Point)->end() || i->atr == Prog_atr)
	{
		Rez = false; IPTemplPoint = nullptr; IPPoint = nullptr;
		MainFU->ProgExec(FailProg);
		return false;
	}
	else
	{
		Rez = true;
		MainFU->ProgExec(SuccessProg);
		AtrProgExec((IC_type)Template.Point, Prog_atr, MainFU->Bus, true);
		MkAtrExec();
		MainFU->ProgExec(SuccessAfterProg);
		return true;
	}
}

bool Search::FindXor(LoadPoint obj)
{
	int  Count = 0; // Счетчик количества совпадений
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
				IPTemplPoint = i._Ptr;
				IPPoint = j._Ptr;
			}
		}
	}
	if (Count == 1)
	{// Выполнилось условие
		Rez = true;
		MainFU->ProgExec(SuccessProg);
		AtrProgExec((IC_type)Template.Point, Prog_atr, MainFU->Bus, true);
		MkAtrExec();
		MainFU->ProgExec(SuccessAfterProg);
		return true;
	}
	else
	{
		Rez = false;
		IPTemplPoint = nullptr;
		IPPoint = nullptr;
		MainFU->ProgExec(FailProg);
		return false;
	}
}

bool Search::FindAnd(LoadPoint obj) // Поиск
{
	Obj = obj;
	if (null_check()) return false;
	IPTemplPoint = nullptr;
	IPPoint = nullptr;
	Rez = false;
	int IP_NumOld = IP_Num; // Запомнить старый номер найденной ИП

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
		if (IPTemplPoint==nullptr)
		{
			IPTemplPoint = j._Ptr;
			IPPoint = i._Ptr;
		}
	}
	if (i==((vector<ip>*)(Obj.Point))->end())
	{// Выполнилось условие
		Rez = true;
		MainFU->ProgExec(SuccessProg);
		AtrProgExec((IC_type)Template.Point, Prog_atr, MainFU->Bus);
		MkAtrExec();
		MainFU->ProgExec(SuccessAfterProg);
		return true;
	}
	else
	{
		Rez = false;
		IPTemplPoint = nullptr;
		IPPoint = nullptr;
		MainFU->ProgExec(FailProg);
		return false;
	}
}

bool Search::FindAndSource(LoadPoint obj) // Поиск
{
	Obj = obj;
	Rez = false;
	if (null_check()) return false;
	IPTemplPoint = nullptr;
	IPPoint = nullptr;

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
			swap(IPTemplPoint,IPPoint);
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
			IPTemplPoint = (ip*)Template.Point;
			IPPoint = (ip*)obj.Point;
			return true;
		}
		else
		{
			Rez = false;
			IPTemplPoint = nullptr;
			IPPoint = nullptr;
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
		if (IPTemplPoint == nullptr)
		{
			IPTemplPoint = i._Ptr;
			IPPoint = j._Ptr;
		}
	}
	if (i != ((IC_type)Template.Point)->end())
	{// Выполнилось условие
		Rez = true;
		MainFU->ProgExec(SuccessProg, 0, MainFU->Bus);
		AtrProgExec((IC_type)Template.Point, Prog_atr, MainFU->Bus, true);
		MkAtrExec();
		MainFU->ProgExec(SuccessAfterProg);
		return true;
	}
	else
	{
		Rez = false;
		IPTemplPoint = nullptr;
		IPPoint = nullptr;
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
	IPTemplPoint = nullptr;  IPPoint = nullptr;
	IP_Num = -1; IPTempl_Num = -1;
	BiggerProg = nullptr; EqProg = nullptr;  SmallerProg = nullptr;
	Prog_atr = ProgAtr;
}
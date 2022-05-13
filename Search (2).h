#pragma once
#include "Consts.h"
#include "Search.h"
class Search
{
public:
	vector<ip>*Template; // Шаблон для поиска
	vector<ip>*Obj; //ИК для поиска
	FU Bus;
	bool Rez; // Результат поиска
	vector<ip> *SuccessProg = nullptr, *FailProg = nullptr; // Указатели на программы по успеху и неуспеху поиска
	ip *IPTemplPoint = nullptr, *IPPoint = nullptr; // Указатели на найденные ИП в шаблоне и в ИК
	int SuccessProgAtr = -1; // Атрибут программы, выполняющейся при успешном поиске (<0 атрибут не учитывается)
	bool Search::FindOr(vector<ip> *obj); // Поиск
	bool Search::FindXor(vector<ip> *obj); // Поиск
	bool Search::FindAnd(vector<ip> *obj); // Поиск
};
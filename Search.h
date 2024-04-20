#pragma once
#include "Consts.h"
#include "Search.h"
#include "ALU.h"
#include <set>

class Search
{
private:
	bool null_check();// Проверка на нуль (false, если всё в порядке)
	void MkAtrExec();
public:
	~Search() { if (Alu != nullptr) delete Alu; };
//	vector<ip>* Template; // Шаблон для поиска
	long int _CalcMk = CalcMk; // Атрибут вычисления АЛВ
	bool CalcMode = false; // Режим вычисления предиката
	FU* Alu = nullptr; // Ссылка на АЛУ
	bool PredicatCalc(IC_type Templ); // Функция вычисления предиката, если он присутствует в ИК
	LoadPoint Template={0, nullptr}; // Шаблон для поиска
	LoadPoint Obj = { 0, nullptr }; //ИК для поиска
	bool MkMode = true; //  Режим выполнения всех МК в ИК-шаблоне (МК-ой считается любой атрибут, индекс которого больше 0)
	set<long int> MkAtr; // Список атрибтов, представляющих МК, выполняющиеся в случае успешного поиска
	FU *MainFU; // Ссылка на главное ФУ, короторое пользуется функционалом searcher-а
	bool Rez=false; // Результат поиска
	vector<ip> *SuccessProg = nullptr, *FailProg = nullptr; // Указатели на программы по успеху и неуспеху поиска
	vector<ip> *SuccessAfterProg = nullptr, *FailAfterProg = nullptr; // Указатели на программы по успеху и неуспеху поиска, выполняющиеся после обработки программы линии
	ip *IPTemplRezPoint = nullptr, *IPRezPoint = nullptr; // Указатели на найденные ИП в шаблоне и в ИК
	long int IP_Num = -1, IPTempl_Num=-1; // Номер первой найденной ИП и ИП в шаблоне
	vector<ip>* BiggerProg = nullptr, *EqProg = nullptr, * SmallerProg = nullptr; // Указатели на программы по запуску подпрограммы по номеру найденной ИП в шаблоне
	long int Prog_atr = ProgAtr; // Атрибут программы, выполняющейся при успешном поиске (<0 атрибут не учитывается)
	long int Sub_atr = ProgMk; // Атрибут программы, выполняющейся при успешном поиске (<0 атрибут не учитывается)
	void Clear();
	void MkAtrAdd(int MK) { MkAtr.insert(MK); };
	void MkAtrClear() { MkAtr.clear(); };
	bool FindIPObj(LoadPoint obj, LoadPoint Templ, bool XOR=false); // Поиск, если obj является ИП (Для Or And)
	bool FindOr(LoadPoint obj); // Поиск
	bool FindXor(LoadPoint obj); // Поиск
	bool FindAnd(LoadPoint obj); // Поиск
	bool FindAndSource(LoadPoint obj); // Поиск
	bool FindIP(LoadPoint obj) {}; // Сравнеие ИП
};
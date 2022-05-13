#pragma once
#include "Consts.h"
#include "LocationTable.h"
#include "Search.h"
#include "Navigator.h"

class GraphTransf : public FU
{
private:
	LocatTable *LTable; // Таблица преобразования адресов
	deletedIC delIC; //буфер удаленной ИП
	Search Searcher;  //поисковик
	Navigator navigator; //бегунок
	void ProgFU(int MK, LoadPoint Load) override;
	vector<vector<ip>*> Accum; // Аккумулятор для операций (первый операнд)
public:
	GraphTransf(FU *BusContext, FU *Templ) : FU(BusContext) { 
		Bus = BusContext; 
		LTable = new LocatTable(); 
		navigator.currentPos = new vector<Position>(); 
		ProgFU(0, { 0,nullptr }); 
	};
	GraphTransf() : FU() {
		Bus = nullptr; 
	};
};
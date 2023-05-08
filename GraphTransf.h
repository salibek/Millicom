#pragma once
#include "Consts.h"
#include "LocationTable.h"
#include "Search.h"
#include "Navigator.h"

class GraphTransf : public FU
{
private:
	LocatTable *LTable=nullptr; // Таблица преобразования адресов
	deletedIC delIC = { nullptr, nullptr }; //буфер удаленной ИП
	Search Searcher;  //поисковик
	Navigator navigator = {nullptr}; //бегунок
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	vector<IC_type> Operands; // Аккумулятор для операций (первый операнд)
public:
	GraphTransf(FU *BusContext, FU *Templ) : FU(BusContext) {
		FUtype = 6;
		Bus = BusContext; 
		LTable = new LocatTable(); 
		navigator.currentPos = new vector<Position>(); 
		ProgFU(0, { 0,nullptr }); 
	};
	GraphTransf() : FU() {
		Bus = nullptr; 
	};
};
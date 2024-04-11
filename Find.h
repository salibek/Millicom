#pragma once
#include "Consts.h"
#include "Search.h"

class Find: public FU
{
	public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст
	Find(FU *BusContext, FU *Templ) : FU(BusContext)
	{
		ProgFU(0, { 0,nullptr }); Bus = BusContext; Searcher.MainFU = this;
		FUtype = 4;
	};
	Find() : FU() { Find(nullptr, nullptr); };
	Search Searcher;
};
#pragma once
#include "Consts.h"
#include "Search.h"

class Find: public FU
{
	public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	Find(FU *BusContext, FU *Templ) : FU(BusContext)
	{
		ProgFU(0, { 0,nullptr }); Bus = BusContext; Searcher.MainFU = this;
		FUtype = 4;
	};
	Find() : FU() { Find(nullptr, nullptr); };
	Search Searcher;
};
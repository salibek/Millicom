#pragma once
#include "Consts.h"

class MatPlot : public FU
{
private:
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	MatPlot(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 26;
		ProgFU(0, { 0,nullptr });
	};
	MatPlot() { MatPlot(nullptr, nullptr); };
};
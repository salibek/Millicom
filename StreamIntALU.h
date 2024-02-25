#pragma once
#include "Consts.h"
#include "StreamFloatALUManager.h"

using namespace std;

class StreamIntALUManager : public FU
{
private:
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	StreamIntALUManager(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 24;
		ProgFU(0, { 0,nullptr });
	};
	StreamIntALUManager() { StreamIntALUManager(nullptr, nullptr); };
};


class StreamIntALU : public FU
{
private:
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	StreamIntALU(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
		FUtype = 25;
		ProgFU(0, { 0,nullptr });
	};
	StreamIntALU() { StreamIntALU(nullptr, nullptr); };
};

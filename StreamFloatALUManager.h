#pragma once
#include "Consts.h"
#include "StreamIntALU.h"

using namespace std;

class StreamFloatALUManager : public FU
{
private:
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	StreamFloatALUManager(FU* BusContext, FU* Templ) 
	{
		Bus = BusContext;
		FUtype = 23;
		ProgFU(0, { 0,nullptr },nullptr);
	};
	StreamFloatALUManager() { StreamFloatALUManager(nullptr, nullptr); };
};

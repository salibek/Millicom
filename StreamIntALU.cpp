#include "StreamIntALU.h"

void StreamIntALU::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	switch (MK)
	{
	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}

void StreamIntALUManager::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	switch (MK)
	{
	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}

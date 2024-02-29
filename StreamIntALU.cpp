#include "StreamIntALU.h"

void StreamIntALU::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	switch (MK)
	{
	case 0: //Reset
		break;
	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}

void StreamIntALUManager::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	switch (MK)
	{
	case 0: //Reset
		break;
	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}

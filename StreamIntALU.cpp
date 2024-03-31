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
	if (!Active && MK < 900) return; //При сброшенном флаге активности выполняются общие МК
	int MKinitial = MK;
	MK %= FUMkRange;
	switch (MK)
	{
	case 0: //Reset
		break;
	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}

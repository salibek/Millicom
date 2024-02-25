#include "StreamFloatALUManager.h"

using namespace std;

void StreamFloatALUManager::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	switch (MK)
	{
	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}

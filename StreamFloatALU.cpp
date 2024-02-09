#include "StreamFloatALU.h"

using namespace std;

void StreamFloatALU::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	switch (MK)
	{
	case 0: //Reset
		Rez = 0;
		OperandsCounter = 0;
		Redy = false;
		RezStack.clear();
		ReseiverMk.clear();
		ReseiverContext.clear();
		AngleMode = 0;
		break;
	case 1: // RezSet Установить результат
		Rez = Load.toDouble();
		break;
	case 5: //RezOut
		Load.WriteVar({Cdouble, &Rez});
		break;
	case 6: //RezOutMk
		MkExec(Load, { Cdouble, &Rez });
		break;
	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}
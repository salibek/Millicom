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
	case 5: //RezOut Выдать результат
		Load.Write(Rez);
		break;
	case 6: //RezOutMk Выдать МК с результатом
		MkExec(Load, { Cdouble, &Rez });
		break;
	case 10: // ZProgSet
		ZProg = Load.Point;
		break;
	case 20: // ZExec
		if (Rez == 0)
			ProgExec(Load);
	case 30: // AngleModeSet
		AngleMode = Load.toInt();
		break;

	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}
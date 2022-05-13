#include "ConfineSimple.h"


void ConfineSimple::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
		
	case 0: // Reset
		break;
	case 1: // iniSet
		ini = Load;
		break;		
	case 2: // iniOut
		Load.WriteFromLoad(Load);
	case 3: // iniOutMk
		MkExec(Load, ini);
		break;
	case 20: //FpostSet
		Fpost = Load.ToBool();
		break;
		
	case 21: //FpostOut
		Load.Write(Fpost);
		break;

	case 22: // FpostOutMk
	{
		LoadPoint LP = { Cbool, &Fpost };
		MkExec(Load, LP);
		break;
	}
	
		// è ò.ä.
	default:
		CommonMk(MK, Load);
		break;
	}
	
}
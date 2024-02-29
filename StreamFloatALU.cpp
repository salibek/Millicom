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
	case 1: // Set  
		Rez = Load.toDouble();
		break;
	case 5: //Out  
		Load.Write(Rez);
		break;
	case 6: //OutMk    
		MkExec(Load, { Cdouble, &Rez });
		break;
	case 10: // ZProgSet
		ZProg = Load.Point;
		break;
	case 11: // ZExec
		if (Rez == 0)
			ProgExec(Load);
		break;
	case 21: // AngleModeSet     (0  , 1 - )
		AngleMode = Load.toInt();
		break;
	case 30: // OperandsReset  
		OperandsCounter = 0;
		for (size_t i = 0; i < Foperands.size(); ++i) {
			Foperands[i] = false;
		}
		break;
	case 70: // ReadySet        true
		Redy = Load.toBool(true);
		break;
	case 71: // ReadyOut     
		Load.Write(Redy);
		break;
	case 72: // ReadyOutMk     
		MkExec(Load, { Cbool, &Ready });
		break;
	case 75: // ReadyExec   ,     
		if (Redy)
			ProgExec(Load);
		break;
	case 76: // ReadyNotExec   ,      
		if (!Redy)
			ProgExec(Load);
		break;
	case 80: // OutRezBlockSet      
		OutRezBlock = Load.toBool();
		break;
	case 90:// Push     
		if (Load.Point == nullptr)
			RezStack.push_back(Rez);
		else
			RezStack.push_back(Load.toDouble());
		break;
	case 130: // Pop      
		if (RezStack.size() == 0) {
			ProgExec(RezStackIsEmpyProg);
			ProgExec(ErrProg);
			break;
		}
		if (Load.Point == nullptr)
			Rez = RezStack.back();
		else
			Load.Write(RezStack.back());
		RezStack.pop_back();
		break;
	case 131: // PopMk        
	{
		if (RezStack.size() == 0)
		{
			ProgExec(RezStackIsEmpyProg);
			ProgExec(ErrProg);
			break;
		}
		if (Load.Point == nullptr)
			Rez = RezStack.back();
		else {
			double temp = RezStack.back();
			RezStack.pop_back();
			MkExec(Load, { Cdouble, &temp });
		}
	}
	break;
	case 140: // StackOut     
		Load.Write(RezStack.back());
		break;
	case 141: // StackOutMk      
	{
		auto temp = RezStack.back();
		MkExec(Load, { Cdouble, &temp });
		break;
	}
	case 150: // NOperandSet   
		OperandsCounter = Load.toInt();
		break;
	case 160: // ReceiverReset    
		ReseiverMk.clear();
		ReseiverContext.clear();
		break;
	case 170: // ReceiverSet     
		// Implement logic
		break;
	case 180: // ReceiverMkSet     
		if (ReseiverMk.size() == ReseiverContext.size())
			ReseiverContext.push_back(nullptr);
		ReseiverMk.push_back(Load.toInt());
		break;
	case 190: // RezProgSet    ,    
		ReseiverContext.push_back((FU*)Load.Point);
		break;
	case 200: // Op1Out   
		if (Operands.size() == 0)
			break;
		else {
			Load.Write(Operands[0]);
		}
		break;
	case 201: // Op1OutMk   
		if (Operands.size() == 0)
			break;
		else {
			double temp = Operands[0];
			MkExec(Load, { Cdouble, &temp });
		}
	case 210: // Op1Out   
		if (Operands.size() == 1)
			break;
		else {
			Load.Write(Operands[1]);
		}
	case 211: // Op1OutMk   
		if (Operands.size() == 1)
			break;
		else {
			double temp = Operands[1];
			MkExec(Load, { Cdouble, &temp });
		}
		break;
	case 220: // OperetionProgSet     
		OperetionProg = nullptr;
		break;
	case 230: // Operand1Set    (  )
		Operands.clear();
		OperandsCounter = 1;
		Foperands.clear();
		Foperands.push_back(true);
		break;
	case 240: // Operand2Set    (  )
		if (Operands.size() < 2) {
			Operands.push_back(Load.toDouble());
			Foperands.push_back(true);
		}
		break;
	case 250: // Operand3Set    (  )
		if (Operands.size() < 3) {
			Operands.push_back(Load.toDouble());
			Foperands.push_back(true);
		}
		break;
	case 260: // PiOut,   EOut[Mk]   , 2, /2, e.
	{
		double t = 1;
		Load.Write(1);
	}
	break;

	case 261: // PiOutMk
	{
		double t = 1;
		MkExec(Load, { Cdouble, &t });
		break;
	}
	case 262: //Pi2Out
	{
		double t = 1;
		Load.Write(1);
	}
	break;
	case 263: //Pi2OutMk
	{
		double t = 1;
		MkExec(Load, { Cdouble, &t });
		break;
	}
	case 264: // EOut
	{
		double t = 1;
		Load.Write(1);
		break;
	}
	case 265: // EOutMk
	{
		double e = 1;
		MkExec(Load, { Cdouble, &e });
		break;
	}

	case 270: // ZRrogSet      
		ZProg = Load.Point;
		break;
	case 280: // NZRrogSet      !=
		BZProg = Load.Point;
		break;
	case 290: // LRrogSet     <
		LProg = Load.Point;
		break;
	case 300: // BRrogSet     >
		BProg = Load.Point;
		break;
	case 310: // LZRrogSet     <=
		LZProg = Load.Point;
		break;
	case 320: // BZRrogSet     >=
		BZProg = Load.Point;
		break;
	case 330: // ZExec    ==
		if (Rez == 0)
			ProgExec(Load);
		break;
	case 340: // LExec    <
		if (Rez < 0)
			ProgExec(Load);
		break;
	case 350: // BExec    >
		if (Rez > 0)
			ProgExec(Load);
		break;
	case 360: // LZExec    <=
		if (Rez <= 0)
			ProgExec(Load);
		break;
	case 370: // BZExec    >=
		if (Rez >= 0)
			ProgExec(Load);
		break;
	case 380: // NZExec    !=
		if (Rez != 0)
			ProgExec(Load);
		break;
	case 390: // ZFOut[Mk]   
	{
		bool temp = Rez == 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 400: // BOut[Mk]   
	{
		bool temp = Rez > 0;
		MkExec(Load, { Cbool, &temp });
	}
	break;
	case 410: // LOut[Mk]   
	{
		bool temp = Rez < 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 420: // BZOut   
	{
		bool temp = Rez >= 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 430: // LZOutMk    
	{
		bool temp = Rez <= 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 440: // NZOut[Mk]    
	{
		bool temp = Rez != 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 500: // Add
	case 501: // AddSqr
		if (Ready) {
			Ready = false;
			Operands.clear();
			Foperands.clear();
			Operands.clear();
			OperandsCounter = 0;
		}
		Operands.push_back(Load.toDouble());
		Foperands.push_back(true);
		OperandsCounter++;
		if (OperandsCounter == Noperands) { //     ->  
			Rez = 0;
			for (auto i : Operands) {
				Rez += i;
			}
			Ready = true;
			for (int i = 0; i < ReseiverMk.size(); i++) {
				MkExec(ReseiverMk[i], { Cdouble, &Rez }, ReseiverContext[i]);
			}
			ProgExec(RezProg);
			if (Rez == 0) ProgExec(ZProg);
			if (Rez != 0) ProgExec(NZProg);
			if (Rez >= 0) ProgExec(BZProg);
			if (Rez <= 0) ProgExec(LZProg);
			if (Rez > 0) ProgExec(BProg);
			if (Rez < 0) ProgExec(LProg);
		}
		break;
	case 505: // Sub1
	case 506: // Sub2
		if (Ready) {
			Ready = false;
			Operands.clear();
			Foperands.clear();
			Operands.clear();
			OperandsCounter = 0;
		}
		if (MK == 505)
		{
			if (Operands.size() < 1)
			{
				Operands.resize(1);
				Foperands.resize(1);
				Foperands[0] = false;
				
			}
			Operands[0] = Load.toDouble();
			if (Foperands[0] == false)
			{
				Foperands[0] = true;
				OperandsCounter++;
			}
			Foperands[0] = Load.toDouble();
		}
		else // Второй операнд
		{
			if (Operands.size() <1) // Первого операнда нет
			{
				Operands.resize(1);
				Foperands.resize(1);
				Foperands[0] = false;
			}
			Foperands.push_back(true);
			Operands.push_back(Load.toDouble());
			OperandsCounter++;
		}
		if (Foperands[0] && OperandsCounter>=Noperands)
		{
			Ready = true;
			Rez = Operands[0];
			// Добавить проверку на ошибку
			for (int i=1;i<Noperands;i++)
				Rez -= Operands[i];
			// --------------------------
			ProgExec(RezProg);
			if (Rez == 0) ProgExec(ZProg);
			if (Rez != 0) ProgExec(NZProg);
			if (Rez >= 0) ProgExec(BZProg);
			if (Rez <= 0) ProgExec(LZProg);
			if (Rez > 0)  ProgExec(BProg);
			if (Rez < 0)  ProgExec(LProg);
		}
		break;
	case 510: //  
		if (!Foperands[0]) {
			//      
			OperandsCounter++;
			Foperands[0] = true;
			Operands[0] = Load.toDouble(); //    
		}
		else { //     
			Operands[0] *= Load.toDouble(); //       
		}
		break;

	case 511: //  
		if (!Foperands[0]) {
			//      
			OperandsCounter++;
			Foperands[0] = true;
			Operands[0] = Load.toDouble(); //    
		}
		else {
			double divisor = Load.toDouble();
			if (divisor != 0) {
				Operands[0] /= divisor; //       
			}
			else {
				//    
				// ,        
			}
		}
		break;

	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}

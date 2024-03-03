#include "StreamFloatALU.h"

using namespace std;

void StreamFloatALU::RezExec() // Выполнение подпрограмм при получении результата
{
	if (Ready == 2)return;
	Ready = true;
	if (!OutRezBlock)//Если не заблокирована рассылка МК с результатами вычислений
		for (int i = 0; i < ReseiverMk.size(); i++) { // Рассылка МК с результатами
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
	case 10: //OpCounterOut Выдать счетчик накопленных операндов
		Load.Write(OperandsCounter);
		break;
	case 11: //OpCounterOutMk Выдать МК со счетчиком накопленных операндов
	{
		int t = OperandsCounter;
		MkExec(Load, { Cdouble, &t });
	}
		break;
	case 20: // AngleModeSet     Установить режим измерения угла (0 - радианы, 1 -градусы)
		AngleMode = Load.toInt();
		break;
	case 30: // OperandsReset
		OperandsCounter = 0;
		for (size_t i = 0; i < Foperands.size(); ++i) {
			Foperands[i] = false;
		}
		break;
	case 40: //ErrProgSet
		ErrProg = Load.Point;
		break;
	case 41: //WrongFormatErrProgSet
		WrongFormatErrProg = Load.Point;
		break;
	case 42: //OveflowErrProgSet
		OveflowErrProg = Load.Point;
		break;
	case 43: //DivZeroErrProgSet
		DivZeroErrProg = Load.Point;
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
		OutRezBlock = Load.toBool(true);
		break;
	case 90:// Push 
		if (Load.Point == nullptr)
			RezStack.push_back(Rez);
		else
			RezStack.push_back(Load.toDouble());
		break;
	case 91: // Pop      
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
	case 92: // PopMk        
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
	case 95: // StackOut     
		Load.Write(RezStack.back());
		break;
	case 96: // StackOutMk      
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
	case 161: // ReceiverSet     
		// Implement logic
		break;
	case 162: // ReceiverMkSet     
		if (ReseiverMk.size() == ReseiverContext.size())
			ReseiverContext.push_back(nullptr);
		ReseiverMk.push_back(Load.toInt());
		break;
	case 190: // RezProgSet    ,    
		RezProg=Load.Point;
		break;
	case 200: // Op0Out   
		if (Operands.size() == 0)
			break;
		else {
			Load.Write(Operands[0]);
		}
		break;
	case 201: // Op0OutMk   
		if (Operands.size() == 0)
			break;
		else {
			double temp = Operands[0];
			MkExec(Load, { Cdouble, &temp });
		}
		break;
	case 202: // Op0Set    (  )
		if (Ready)
			Operands.clear();
		if (Operands.size() < 1)
		{
			Operands.push_back(Load.toDouble());
			Foperands.push_back(true);
			break;
		}
		Operands[0] = Load.toDouble();
		if (!Foperands[0])
			OperandsCounter++;
		Foperands[0] = true;
		break;
	case 205: // Op1Out   
		if (Operands.size() < 2)
			break;
		else {
			Load.Write(Operands[1]);
		}
		break;
	case 206: // Op1OutMk   
		if (Operands.size() < 2)
			break;
		else {
			double temp = Operands[1];
			MkExec(Load, { Cdouble, &temp });
		}
		break;
	case 207: // Op1Set    (  )
		if (Ready)
			Operands.clear();
		if (Operands.size() < 2)
		{
			Operands.resize(2);
			while (Foperands.size() < Operands.size())
				Foperands.push_back(false);
		}
		Operands[1] = Load.toDouble();
		if (!Foperands[0])
			OperandsCounter++;
		Foperands[1] = true;
		break;
	case 210: // Op2Out   
		if (Operands.size() < 3)
			break;
		else {
			Load.Write(Operands[2]);
		}
		break;
	case 211: // Op2OutMk   
		if (Operands.size() == 1)
			break;
		else {
			double temp = Operands[2];
			MkExec(Load, { Cdouble, &temp });
		}
		break;
	case 212: // Op2Set    (  )
		if (Ready)
			Operands.clear();
		if (Operands.size() < 3)
		{
			Operands.resize(3);
			while (Foperands.size() < Operands.size())
				Foperands.push_back(false);
		}
		Operands[2] = Load.toDouble();
		if (!Foperands[2])
			OperandsCounter++;
		Foperands[2] = true;
		break;
	case 220: //	OpIndSet
		OpInd = Load.toInt();
		break;
	case 221: // OpIndInc;
		OpInd++;
		break;
	case 222: // OperandByIndSet
		if (OpInd >= Operands.size()) break;
		Operands[OpInd] = Load.toDouble();
		if (!Foperands[OpInd])
			OperandsCounter++;
		Foperands[OpInd] = true;
		break;
	case 223: // OperandByIndOut Выдать операнд по индексу
		if (Operands.size() >=OpInd)
			break;
		Load.Write(Operands[OpInd]);
		break;
	case 224: // OperandByIndOutMk Выдать МК с операндом по индексу
	{
		if (Operands.size() >= OpInd)
			break;
		double temp = Operands[OpInd];
		MkExec(Load, { Cdouble, &temp });
		break;
	}
	case 230: // OperetionProgSet     
		OperetionProg = Load.Point;
		break;
	case 260: // PiOut,   EOut[Mk]   , 2, /2, e.
		Load.Write((double)3.141592653589793);
		break;
	case 261: // PiOutMk
	{
		double t = 3.141592653589793;
		MkExec(Load, { Cdouble, &t });
		break;
	}
	case 262: //Pi2Out
		Load.Write((double)6.283185307179586);
		break;
	case 263: //Pi2OutMk
	{
		double t = 6.283185307179586;
		MkExec(Load, { Cdouble, &t });
		break;
	}
	case 264: // EOut
		Load.Write((double)2.718281828459045);
		break;
	case 265: // EOutMk
	{
		double e = 2.718281828459045;
		MkExec(Load, { Cdouble, &e });
		break;
	}
	case 270: // ZRrogSet      
		ZProg = Load.Point;
		break;
	case 280: // NZRrogSet      !=
		BZProg = Load.Point;
		break;
	case 281: // LRrogSet     <
		LProg = Load.Point;
		break;
	case 282: // BRrogSet     >
		BProg = Load.Point;
		break;
	case 283: // LZRrogSet     <=
		LZProg = Load.Point;
		break;
	case 284: // BZRrogSet     >=
		BZProg = Load.Point;
		break;
	case 290: // ZExec    ==
		if (Rez == 0)
			ProgExec(Load);
		break;
	case 291: // LExec    <
		if (Rez < 0)
			ProgExec(Load);
		break;
	case 292: // BExec    >
		if (Rez > 0)
			ProgExec(Load);
		break;
	case 293: // LZExec    <=
		if (Rez <= 0)
			ProgExec(Load);
		break;
	case 294: // BZExec    >=
		if (Rez >= 0)
			ProgExec(Load);
		break;
	case 296: // NZExec    !=
		if (Rez != 0)
			ProgExec(Load);
		break;
	case 300: // ZFOut 
		Load.Write(Rez == 0);
		break;
	case 301: // ZFOutMk  
	{
		bool temp = Rez == 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 305: // BOut  
		Load.Write(Rez > 0);
		break;
	case 306: // BOutMk
	{
		bool temp = Rez > 0;
		MkExec(Load, { Cbool, &temp });
	}
	break;
	case 310: // LOut  
		Load.Write(Rez < 0);
		break;
	case 311: // LOutMk
	{
		bool temp = Rez < 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 315: // BZOut   
		Load.Write(Rez >= 0);
		break;
	case 316: // BZOutMk   
	{
		bool temp = Rez >= 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 320: // LZOutMk    
		Load.Write(Rez <= 0);
		break;
	case 321: // LZOutMk    
	{
		bool temp = Rez <= 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 325: // NZOut    
		Load.Write(Rez != 0);
		break;
	case 326: // NZOutMk    
	{
		bool temp = Rez != 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 500: // Add
	case 501: // AddSqr
	case 510: // Mul
		if (Ready || OpCode!=MK) {
			OpCode = MK; // Запомнить код операции
			Ready = false;
			Operands.clear();
			Foperands.clear();
			Operands.clear();
			OperandsCounter = 0;
		}
		if(Load.isEmpty())
			Operands.push_back(Rez); // Накопление из буфера результата
		else if(Load.isDigit())
			Operands.push_back(Load.toDouble()); // Накопление операндов
		else
		{
			Ready = 2; // Код ошибки
			ProgExec(ErrProg); // Неправильный формат операнда
			ProgExec(WrongFormatErrProg);
			break;
		}
		Foperands.push_back(true);
		OperandsCounter++;
		if (OperandsCounter == Noperands) 
		{ //     ->  
			Rez = 0;
			switch (OpCode) {
			case 500: //Add
				for (auto i : Operands)
					Rez += i;
					break;
			case 501: //AddSqrt
				for (auto i : Operands)
					Rez += i*i;
					break;
			case 510: //Mul
				for (auto i : Operands)
					Rez *= i;
					break;
			}
			RezExec(); // Действия при получении результата
		}
		break;
	case 505: // Sub1
	case 506: // Sub2
	case 515: // Div1
	case 516: // Div2
		if (Ready || OpCode!=MK && OpCode != MK-1) {
			OpCode = 505;
			Ready = 0;
			Operands.clear();
			Foperands.clear();
			OperandsCounter = 0;
		}
		if (!Load.isEmpty() && !Load.isDigit()) // Ошибка формата операнда
		{
			Ready = 2; // Код ошибки
			ProgExec(ErrProg); // Неправильный формат операнда
			ProgExec(WrongFormatErrProg);
			break;
		}
		if (MK%2) // Первый операнд (МК нечетная)
		{
			if (Operands.size() < 1)
			{
				Operands.resize(1);
				Foperands.resize(1);
				Foperands[0] = false;
				
			}
			if (Foperands[0] == false)
			{
				Foperands[0] = true;
				OperandsCounter++;
			}
			Operands[0] = Load.toDouble(Rez);
		}
		else // Второй операнд
		{
			if (Operands.size() <1) // Первого операнда нет
			{
				Operands.resize(1);
				Foperands.push_back(false);
			}
			Operands.push_back(Load.toDouble(Rez)); // Поместить операнд в стек операндов
			Foperands.push_back(true);
			OperandsCounter++;
		}
		if (Foperands[0] && OperandsCounter >= Noperands)
		{
			Ready = 1;
			Rez = Operands[0];
			// Добавить проверку на ошибку
			switch (OpCode) {
			case 505:
				for (int i = 1; i < Noperands; i++)
					Rez -= Operands[i];
				break;
			case 515:
				for (int i = 1; i < Noperands; i++)
					if (Operands[i] != 0)
						Rez /= Operands[i];
					else
					{
						Ready = 2; // Код ошибки
						ProgExec(ErrProg); // Деление на ноль
						ProgExec(DivZeroErrProg);
						break;
					}
			}
			// --------------------------
			RezExec(); // Действия при получении результата
		}
		break;
	case 520: // DivInt1 Целочисленное деление
	case 521: // DivInt2 Целочисленное деление
	case 522:	// Rem Остаток от целочисленного деления
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
	case 525: //Sqrt Квадратный корень
		Rez = Load.toDouble(Rez); // Поместить в аккумулятор нагрузку, если нагрузка нулевая, то поместить Rez
		Rez = sqrt(Rez);
		Ready = 1;
		break;
	case 526: //Sqr Квадрат
		Rez = Load.toDouble(Rez); // Поместить в аккумулятор нагрузку, если нагрузка нулевая, то поместить Rez
		Rez = Rez*Rez;
		Ready = 1;
		break;
	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}

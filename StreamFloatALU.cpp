#include "StreamFloatALU.h"

using namespace std;

void StreamFloatALU::RezExec() // Выполнение подпрограмм при получении результата
{
	if (Ready == 2) return;
	Ready = 1;
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
		Ready = 0;
		RezStack.clear();
		ReseiverMk.clear();
		ReseiverContext.clear();
		AngleMode = 0;
		break;
	case 1: // Set Установить результат вычислений
		Rez = Load.toDouble();
		break;
	case 5: //Out  Выдать результат
		Load.Write(Rez);
		break;
	case 6: //OutMk Выдать МК с результатом
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
	case 30: // OperandsReset Сброс операндов (все те операнды, что пришли, сбрасываются и накопление операндов начинается заново)
		OperandsCounter = 0;
		for (size_t i = 0; i < Foperands.size(); ++i) {
			Foperands[i] = false;
		}
		break;
	case 40: //ErrProgSet Установить программу при ошибке
		ErrProg = Load.Point;
		break;
	case 41: //WrongFormatErrProgSet Установить программу при неправильном формате операнда
		WrongFormatErrProg = Load.Point;
		break;
	case 42: //OveflowErrProgSet Установить программу при переполнении
		OveflowErrProg = Load.Point;
		break;
	case 43: //DivZeroErrProgSet Установить программу при делении на 0
		DivZeroErrProg = Load.Point;
		break;
	case 44: //OpIndErrProgSet Установить программу при ошибке индекса операнда
		OpIndErrProg = Load.Point;
		break;
	case 45: //RezStackIsEmpyProgSet Установить программу при чтении из пустого стека результатов
		RezStackIsEmpyProg = Load.Point;
		break;
	case 70: // ReadySet Установить флаг готовности результата (по умолчанию true)
		Ready = Load.toInt(1);
		break;
	case 71: // ReadyOut Выдать флаг готовности результата    
		Load.Write(Ready);
		break;
	case 72: // ReadyOutMk Выдать МК с флагом готовности результата   
		MkExec(Load, { Cbool, &Ready });
		break;
	case 75: // ReadyExec Запуск программы по флагу готовности резульатата     
		if (Ready==1)
			ProgExec(Load);
		break;
	case 76: // ReadyNotExec   Запуск программы при сброшенном флаге готовности резульатат,      
		if (Ready==0)
			ProgExec(Load);
		break;
	case 77: //ErrExec Выполнить если ошибка
		if (Ready == 2)
			ProgExec(Load);
		break;
	case 80: // OutRezBlockSet Установить блокировку выдачи результата (при нулевой нагрузке true)
		OutRezBlock = Load.toBool(true);
		break;
	case 90:// Push Положить в стек (при нулевой нагрузке в стек помещается Rez)
		if (Load.Point == nullptr)
			RezStack.push_back(Rez);
		else
			RezStack.push_back(Load.toDouble());
		break;
	case 91: // Pop Вынуть из стека (при при нулевой нагрузке величина помещается в Rez)
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
	case 92: // PopMk Вынуть из стека и выдать МК (при при нулевой нагрузке величина помещается в Rez)        
	{
		if (RezStack.size() == 0)
		{
			ProgExec(RezStackIsEmpyProg);
			ProgExec(ErrProg);
			break;
		}
		if (Load.Point == nullptr)
		{
			Rez = RezStack.back();
			RezStack.pop_back();
		}
		else {
			double temp = RezStack.back();
			RezStack.pop_back();
			MkExec(Load, { Cdouble, &temp });
		}
	}
	break;
	case 93: // StackCounterOut Выдать количество элементов в стеке
		Load.Write((int)RezStack.size());
		break;
	case 94: // StackCounterOut Выдать МК с количеством элементов в стеке
	{
		int t = RezStack.size();
		MkExec(Load,{Cint,&t});
		break;
	}
	case 95: // StackOut Выдать из стека (при при нулевой нагрузке величина помещается в Rez) 
		if (Load.isEmpty())
			Rez = RezStack.back();
		else
			Load.Write(RezStack.back());
		break;
	case 96: // StackOutMk Выдать из стека с МК (при при нулевой нагрузке величина помещается в Rez)
	{
		if (Load.isEmpty())
			Rez = RezStack.back();
		else {
			auto temp = RezStack.back();
			MkExec(Load, { Cdouble, &temp });
		}
		break;
	}
	case 97: //StackEmptyExec Выполнить, если стек пустой
		if (!RezStack.size()) ProgExec(Load);
		break;
	case 98: //StackNotEmptyExec Выполнить, если стек не пустой
		if (RezStack.size()) ProgExec(Load);
		break;

	case 150: // NOperandSet Установить количество операндов (по умолчанию 2)
		OperandsCounter = Load.toInt();
		break;
	case 160: // ReceiverReset Сброс установок получателей результата
		ReseiverMk.clear();
		ReseiverContext.clear();
		break;
	case 161: // ReceiverSet Установить ссылку на приемника результата (Устанавливается перед установкой МК)
		ReseiverContext.push_back((FU*)Load.Point);
		break;
	case 162: // ReceiverMkSet Установить МК для приемника результата 
		if (ReseiverMk.size() == ReseiverContext.size())
			ReseiverContext.push_back(nullptr);
		ReseiverMk.push_back(Load.toInt());
		break;
	case 190: // RezProgSet Установить ссылку на подпрограмму, запускаемую при получении результата   
		RezProg=Load.Point;
		break;


	case 200: // Op0Out Выдать операнд   
	case 205: // Op1Out   
	case 210: // Op2Out   
	case 215: // Op3Out   
	case 220: // Op4Out   
	case 225: // Op5Out   
	case 230: // Op6Out   
	case 235: // Op7Out   
	case 240: // Op8Out   
	case 245: // Op9Out   
		if (Operands.size() < (MK - 200) / 5 )
		{
			ProgExec(ErrProg);
			ProgExec(NoOperandErrProg);
			break;
		}
		Load.Write(Operands[(MK - 200) / 5]);
		break;
	case 201: // Op0OutMk  Выдать МК с операндом 0 
	case 206: // Op1OutMk   
	case 211: // Op2OutMk   
	case 216: // Op3OutMk   
	case 221: // Op4OutMk   
	case 226: // Op5OutMk   
	case 231: // Op6OutMk   
	case 236: // Op7OutMk   
	case 241: // Op8OutMk   
	case 246: // Op9OutMk   
		if (Operands.size() < (MK - 200) / 5)
		{
			ProgExec(ErrProg);
			ProgExec(NoOperandErrProg);
		}
		else {
			double temp = Operands[(MK - 200) / 5];
			MkExec(Load, { Cdouble, &temp });
		}
		break;
	case 202: // Op0Set  Установить операнд 0
	case 207: // Op1Set
	case 212: // Op2Set
	case 217: // Op3Set
	case 222: // Op4Set
	case 227: // Op5Set
	case 232: // Op6Set
	case 237: // Op7Set
	case 242: // Op8Set
	case 247: // Op9Set
		if (Ready)
		{
			Operands.clear();
			Foperands.clear();
			OperandsCounter = 0;
		}
		{int t = (MK - 200) / 5;
		while (Operands.size() < t)
		{
			Operands.push_back(0);
			Foperands.push_back(false);
		}

		Operands[t] = Load.toDouble();
		if (!Foperands[t])
			OperandsCounter++;
		Foperands[t] = true;
		}
		break;
	case 250: //	OpIndSet Установить индекс операнда
		OpInd = Load.toInt(0);
		break;
	case 251: // OpIndAdd Прибавить к индексу операнда (по умолчанию 1)
		OpInd+=Load.toInt(1);
		break;
	case 252: // OperandByIndSet Установить операнд по индексу
		while (Operands.size() <= OpInd)
		{
			Operands.push_back(0);
			Foperands.push_back(false);
		}
		Operands[OpInd] = Load.toDouble();
		if (!Foperands[OpInd])
			OperandsCounter++;
		Foperands[OpInd] = true;
		if (OperandsCounter >= Noperands) // Выполнение программы
		{
			ProgExec(OperetionProg);
			RezExec();
		}
		break;
	case 253: // OperandByIndOut Выдать операнд по индексу
		if (Operands.size() <= OpInd || OpInd<0)
		{
			ProgExec(ErrProg);
			ProgExec(OpIndErrProg);
			break;
		}
		Load.Write(Operands[OpInd]);
		break;
	case 254: // OperandByIndOutMk Выдать МК с операндом по индексу
	{
		if (Operands.size() <= OpInd || OpInd < 0)
		{
			ProgExec(ErrProg);
			ProgExec(OpIndErrProg);
			break;
		}
		double temp = Operands[OpInd];
		MkExec(Load, { Cdouble, &temp });
		break;
	}
	case 255: // OperandAdd Добавить операнд для специальной МК
		Operands.push_back(Load.toDouble());
		Foperands.push_back(true);
		OperandsCounter++;
		break;
	case 260: // OperationProgSet Установить специальную операцию    
		OperetionProg = Load.Point;
		break;

	case 270: // PiOut Выдать число ПИ (при нулевой нагрузке записать в аккумулятор)
		if (Load.isEmpty()) {
			Rez = 3.141592653589793; break;
		}
		Load.Write((double)3.141592653589793);
		break;
	case 271: // PiOutMk Выдать МК с числом ПИ (при нулевой нагрузке записать в аккумулятор)
	{
		if (Load.isEmpty()){
			Rez = 3.141592653589793; break;
		}
		double t = 3.141592653589793;
		MkExec(Load, { Cdouble, &t });
		break;
	}
	case 272: //Pi2Out Выдать число 2ПИ (при нулевой нагрузке записать в аккумулятор)
		if (Load.isEmpty()){
			Rez = 6.283185307179586; break;
		}
		Load.Write((double)6.283185307179586);
		break;
	case 273: //Pi2OutMk Выдать МК с числом 2ПИ (при нулевой нагрузке записать в аккумулятор)
	{
		if (Load.isEmpty()){
			Rez = 6.283185307179586; break;
		}
		double t = 6.283185307179586;
		MkExec(Load, { Cdouble, &t });
		break;
	}
	case 274: // EOut Выдать число е (при нулевой нагрузке записать в аккумулятор)
		if (Load.isEmpty()){
			Rez = 2.718281828459045; break;
		}
		Load.Write((double)2.718281828459045);
		break;
	case 275: // EOutMk Выдать МК с числом е (при нулевой нагрузке записать в аккумулятор)
	{
		if (Load.isEmpty()) {
			Rez = 2.718281828459045; break;
		}
		double e = 2.718281828459045;
		MkExec(Load, { Cdouble, &e });
		break;
	}
	case 280: // ZProgSet Установить программу при == 
		ZProg = Load.Point;
		break;
	case 281: // NZProgSet Установить программу при !=
		BZProg = Load.Point;
		break;
	case 282: // LProgSet   Установить программу при <
		LProg = Load.Point;
		break;
	case 283: // BProgSet   Установить программу при >
		BProg = Load.Point;
		break;
	case 284: // LZProgSet   Установить программу при <=
		LZProg = Load.Point;
		break;
	case 285: // BZProgSet   Установить программу при >=
		BZProg = Load.Point;
		break;
	case 290: // ZExec   Выполнить программу при ==
		if (Rez == 0)
			ProgExec(Load);
		break;
	case 291: // LExec    Выполнить программу при <
		if (Rez < 0)
			ProgExec(Load);
		break;
	case 292: // BExec    Выполнить программу при >
		if (Rez > 0)
			ProgExec(Load);
		break;
	case 293: // LZExec    Выполнить программу при <=
		if (Rez <= 0)
			ProgExec(Load);
		break;
	case 294: // BZExec    Выполнить программу при >=
		if (Rez >= 0)
			ProgExec(Load);
		break;
	case 296: // NZExec    Выполнить программу при !=
		if (Rez != 0)
			ProgExec(Load);
		break;
	case 300: // ZFOut Выдать флаг нуля
		Load.Write(Rez == 0);
		break;
	case 301: // ZFOutMk Выдать МК с флагом нуля
	{
		bool temp = Rez == 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 305: // BOut  Выдать флаг больше
		Load.Write(Rez > 0);
		break;
	case 306: // BOutMk Выдать МК с флагом больше
	{
		bool temp = Rez > 0;
		MkExec(Load, { Cbool, &temp });
	}
	break;
	case 310: // LOut  Выдать флаг меньше
		Load.Write(Rez < 0);
		break;
	case 311: // LOutMk BOutMk Выдать МК с флагом меньше
	{
		bool temp = Rez < 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 315: // BZOut Выдать флаг больше или нуль
		Load.Write(Rez >= 0);
		break;
	case 316: // BZOutMk BOutMk Выдать МК с флагом больше или нуль
	{
		bool temp = Rez >= 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 320: // LZOut Выдать флаг меньше или нуль
		Load.Write(Rez <= 0);
		break;
	case 321: // LZOutMk BOutMk Выдать МК с флагом меньше или нуль
	{
		bool temp = Rez <= 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 325: // NZOut  Выдать флаг не нуль
		Load.Write(Rez != 0);
		break;
	case 326: // NZOutMk BOutMk Выдать МК с флагом не нуль   
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
	case 520: // DivInt1 Целочисленное деление
	case 521: // DivInt2 Целочисленное деление
		if (Ready || OpCode!=MK && OpCode != MK-1) {
			OpCode = MK;
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
				break;
			case 520: // DivInt
				Rez = int(Rez);
				for (int i = 1; i < Noperands; i++)
					if (int(Operands[i]) != 0)
						Rez /= int(Operands[i]);
					else
					{
						Ready = 2; // Код ошибки
						ProgExec(ErrProg); // Деление на ноль
						ProgExec(DivZeroErrProg);
						break;
					}
				break;
			}
			// --------------------------
			RezExec(); // Действия при получении результата
		}
		break;
	case 522:	// Rem1 Число из которого извлекается остаток от целочисленного деления
	case 523:	// Rem2 Остаток от целочисленного деления
		if (!Load.isEmpty() && !Load.isDigit()) // Ошибка формата операнда
		{
			Ready = 2; // Код ошибки
			ProgExec(ErrProg); // Неправильный формат операнда
			ProgExec(WrongFormatErrProg);
			break;
		}
		if (Ready || OpCode != MK && OpCode != MK - 1) {
			OpCode = MK;
			Ready = 0;
			Operands.clear();
			Foperands.clear();
			OperandsCounter = 0;
		}
		if (MK == 522)
		{
			if (Operands.size() == 0)
			{
				Operands.push_back(Load.toDouble());
				Foperands.push_back(true);
				OperandsCounter = 1;
				break;
			}
			Operands[0] = Load.toDouble();
			if (Foperands[0] == false)
				OperandsCounter += 1;
			Foperands[0] = true;
		}
		else
		{
			while (Operands.size() < 2)
			{
				Operands.push_back(0);
				Foperands.push_back(false);
			}
			Operands[1] = Load.toDouble();
			if (Foperands[1] == false)
				OperandsCounter++;
			Foperands[1] = true;
		}
		if (OperandsCounter == 2)
		{
			Rez = int(Operands[0]) % int(Operands[1]);
			RezExec();
		}
		break;

	case 525: //Sqrt Квадратный корень
		Rez = Load.toDouble(Rez); // Поместить в аккумулятор нагрузку, если нагрузка нулевая, то поместить Rez
		Rez = sqrt(Rez);
		Ready = 1;
		RezExec(); // Действия при получении результата
		break;
	case 526: //Sqr Квадрат
		Rez = Load.toDouble(Rez); // Поместить в аккумулятор нагрузку, если нагрузка нулевая, то поместить Rez
		Rez = Rez*Rez;
		Ready = 1;
		RezExec(); // Действия при получении результата
		break;
	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}

#include "StreamFloatALU.h"

using namespace std;

void StreamFloatALU::ProgFU(long int MK, LoadPoint Load, FU* Sender)
{
	if (PrefixProg != nullptr) ProgExec(PrefixProg); // Запуск предварительной программы
	if (!Active && MK<900) return; //При сброшенном флаге активности выполняются общие МК
	if (MK >= FUMkRange && MK < FUMkGlobalAdr && MK >= FUMkGlobalAdr + FUMkRange)
	{
		
		ProgExec(RoutProg);
		return;
	}
	else
		ProgExec(FUSelfAdrProg);
	int MKinitial = MK;
	MK %= FUMkRange;
	if (Load.isEmpty()) Load = { Cdouble,&Rez }; // Если нулевая нагрузка, то операндом является аккумулятор
	switch (MK)
	{
	case 0: //Reset
		Rez = 0;
		OperandsCounter = 0;
		Ready = 0;
		RezStack.clear();
		RezExtStack.clear();
		ReceiverMk.clear();
		ReceiverContexts.clear();
		AngleMode = 0;
		break;
	case 1: // Set Установить результат вычислений
		Rez = Load.toDouble();
		break;
	case 3: // RoutProg Программа при несовпадении адреса ФУ с его собственным адресом
		RoutProg = Load.Point;
		break;
	case 4: // SelfAdrProg Программа при совпадении адреса ФУ с его собственным адресом
		FUSelfAdrProg = Load.Point;
		break;
	case 5: //Out   Выдать результат
		Load.Write(Rez);
		break;
	case 6: //OutMk  Выдать МК с результатом
		MkExec(Load, { Cdouble, &Rez });
		break;
	case 10: //OpCounterOut Выдать счетчик накопленных операндов
		Load.Write(OperandsCounter);
		break;
	case 11: //OpCounterOutMk Выдать МК со счетчиком накопленных операндов
		MkExec(Load, { Cint, &OperandsCounter });
		break;
	case 20: // AngleModeSet   Установить режим измерения угла (0 - радианы, 1 -градусы)
		AngleMode = Load.toInt();
		break;
	case 30: // OperandsReset Сброс операндов (все те операнды, что пришли, сбрасываются и накопление операндов начинается заново)
		OperandsCounter = 0;
		for (size_t i = 0; i < FOperands.size(); ++i) {
			FOperands[i] = false;
		}
		break;
	case 40: //ErrProgSet Установить программу при ошибке
		ErrProg = Load.Point;
		break;
	case 41: //WrongFormatErrProgSet  Установить программу при неправильном формате операнда
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
	case 46: //MatErrProgSet Установить программу обработки ошибки математической операции
		MatErrProg = Load.Point;
		break;
	case 70: // ReadySet Установить флаг готовности результата (по умолчанию true)
		Ready = Load.toInt(1);
		break;
	case 71: // ReadyOut Выдать флаг готовности результата   
		Load.Write(Ready);
		break;
	case 72: // ReadyOutMk Выдать МК с флагом готовности результата    
		MkExec(Load, { Cint, &Ready });
		break;
	case 75: // ReadyExec Запуск программы по флагу готовности резульатата    
		if (Ready==1)
			ProgExec(Load);
		break;
	case 76: // ReadyNotExec   Запуск программы при сброшенном флаге готовности резульатата      
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
	case 91: // Pop Р’С‹РЅСѓС‚СЊ РёР· СЃС‚РµРєР° (РїСЂРё РїСЂРё РЅСѓР»РµРІРѕР№ РЅР°РіСЂСѓР·РєРµ РІРµР»РёС‡РёРЅР° РїРѕРјРµС‰Р°РµС‚СЃСЏ РІ Rez)
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
	case 92: // PopMk Вынуть из стека (при при нулевой нагрузке величина помещается в Rez)        
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
		long int t = RezStack.size();
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
	case 100: //RezExtOut Выдать значение из расширенного стека результата
	case 101: // RezExtOutMk Выдать МК со значением из расширенного стера результата
	case 102: // RezExtPop Выдавить расширенный результат (при нулевой нагрузке записывается в Rez)
	case 103: // RezExtPopMk Выдавить МК с расширенным результатом (при нулевой нагрузке записывается в Rez)
		if (!RezExtStack.size())
		{
			ProgExec(RezExtStackIsEmpyProg);
			break;
		}
		switch(MK)
		{
			case 100:
				Load.Write(RezExtStack.back());
				break;
			case 101:
			{
				double t = RezExtStack.back();
				MkExec(Load, {Cdouble,&t});
			}
			case 102:
			{
				Load.Write(RezExtStack.back());
				RezExtStack.pop_back();
			}
			case 103:
			{
				double t = RezExtStack.back();
				RezExtStack.pop_back();
				MkExec(Load, { Cdouble,&t });
			}
			break;
		}
		break;
	case 105: // RezExtStackIsEmpyProgSet Установить программу при ошибке чтения из пустого стека расширенного результата
		RezExtStackIsEmpyProg = Load.Point;
		break;

	case 150: // NOperandSet Установить количество операндов (по умолчанию 2)
		OperandsCounter = Load.toInt();
		break;
	case 160: // ReceiverReset Сброс установок получателей результата
		ReceiverMk.clear();
		ReceiverContexts.clear();
		break;
	case 161: // ReceiverSet Установить ссылку на приемника результата (Устанавливается перед установкой МК)
		ReceiverContexts.push_back((FU*)Load.Point);
		break;
	case 162: // ReceiverMkSet Установить МК для приемника результата 
		if (ReceiverMk.size() == ReceiverContexts.size())
			ReceiverContexts.push_back(nullptr);
		ReceiverMk.push_back(Load.toInt());
		break;
	case 165: // OutVarReset Очистить список переменных для записи результата
		OutVars.clear();
		break;
	case 166: // OutVarAdd Установить адрес для записи результата
		OutVars.push_back(Load);
		break;

	case 190: // RezProgSet Установить ссылку на подпрограмму, запускаемую при получении результата   
		RezProg=Load.Point;
		break;
	case 191: //PreRezProgSet Программа, запускаемая перед получением результата
		PreRezProg = Load.Point;
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
			FOperands.clear();
			OperandsCounter = 0;
		}
		{long int t = (MK - 200) / 5;
		while (Operands.size() < t)
		{
			Operands.push_back(0);
			FOperands.push_back(false);
		}

		Operands[t] = Load.toDouble();
		if (!FOperands[t])
			OperandsCounter++;
		FOperands[t] = true;
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
			FOperands.push_back(false);
		}
		Operands[OpInd] = Load.toDouble();
		if (!FOperands[OpInd])
			OperandsCounter++;
		FOperands[OpInd] = true;
		if (OperandsCounter >= Noperands) // Р’С‹РїРѕР»РЅРµРЅРёРµ РїСЂРѕРіСЂР°РјРјС‹
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
		FOperands.push_back(true);
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
	case 280: // ZeroProgSet Установить программу при == 
		ZProg = Load.Point;
		break;
	case 281: // NotZeroProgSet Установить программу при !=
		BZProg = Load.Point;
		break;
	case 282: // LessProgSet   Установить программу при <
		LProg = Load.Point;
		break;
	case 283: // BiggerProgSet   Установить программу при <
		BProg = Load.Point;
		break;
	case 284: // LessZeroProgSet   Установить программу при <=
		LZProg = Load.Point;
		break;
	case 285: // BiggerZeroProgSet  Установить программу при >=
		BZProg = Load.Point;
		break;
	case 290: // ZeroExec   Выполнить программу при ==
		if (Rez == 0)
			ProgExec(Load);
		break;
	case 291: // LessExec    Выполнить программу при <
		if (Rez < 0)
			ProgExec(Load);
		break;
	case 292: // BiggerExec    Выполнить программу при >
		if (Rez > 0)
			ProgExec(Load);
		break;
	case 293: // LessZeroExec    Выполнить программу при <=
		if (Rez <= 0)
			ProgExec(Load);
		break;
	case 294: // BiggerZeroExec    Выполнить программу при >=
		if (Rez >= 0)
			ProgExec(Load);
		break;
	case 296: // NotZeroExec    Выполнить программу при !=
		if (Rez != 0)
			ProgExec(Load);
		break;
	case 300: // ZeroOut Выдать флаг нуля
		Load.Write(Rez == 0);
		break;
	case 301: // ZeroOutMk Выдать МК с флагом нуля
	{
		bool temp = Rez == 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 305: // BiggerOut  Выдать флаг больше
		Load.Write(Rez > 0);
		break;
	case 306: // BiggerOutMk Выдать МК с флагом больше
	{
		bool temp = Rez > 0;
		MkExec(Load, { Cbool, &temp });
	}
	break;
	case 310: // LessOut  Выдать флаг меньше
		Load.Write(Rez < 0);
		break;
	case 311: // LessOutMk BOutMk Выдать МК с флагом меньше
	{
		bool temp = Rez < 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 315: // BiggerZeroOut Выдать флаг больше или нуль
		Load.Write(Rez >= 0);
		break;
	case 316: // BiggerZeroOutMk Выдать МК с флагом больше или нуль
	{
		bool temp = Rez >= 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 320: // LessZeroOut Выдать флаг меньше или нуль
		Load.Write(Rez <= 0);
		break;
	case 321: // LessZeroOutMk Выдать МК с флагом меньше или нуль
	{
		bool temp = Rez <= 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 325: // NotZeroOut  Выдать флаг не нуль
		Load.Write(Rez != 0);
		break;
	case 326: // NotZeroOutMk Выдать МК с флагом не нуль    
	{
		bool temp = Rez != 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}

	case 340: // RoutMkOut Выдать маршрутизированную МК
		Load.Write(IpForMkAdrOut.atr);
		break;
	case 341: // RoutMkOutMk
		MkExec(Load, { Cint, &IpForMkAdrOut.atr });
		break;
	case 342: // RoutLoadOut
		Load.Write(IpForMkAdrOut.atr);
		break;
	case 343: // RoutLoadOutMk
		MkExec(Load, IpForMkAdrOut.Load);
		break;

	case 350: // AdrBuf_0_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 0
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[0]); else MkExec(Load, Load, (FU*)AdrBuf[0]);
		break;
	case 351: // AdrBuf_1_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 1
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[1]); else MkExec(Load, Load, (FU*)AdrBuf[1]);
		break;
	case 352: // AdrBuf_2_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 2
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[2]); else MkExec(Load, Load, (FU*)AdrBuf[2]);
		break;
	case 353: // AdrBuf_3_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 3
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[3]); else MkExec(Load, Load, (FU*)AdrBuf[3]);
		break;
	case 354: // AdrBuf_4_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 4
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[4]); else MkExec(Load, Load, (FU*)AdrBuf[4]);
		break;
	case 355: // AdrBuf_5_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 5
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[5]); else MkExec(Load, Load, (FU*)AdrBuf[5]);
		break;
	case 356: // AdrBuf_6_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 6
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[6]); else MkExec(Load, Load, (FU*)AdrBuf[6]);
		break;
	case 357: // AdrBuf_7_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 7
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[7]); else MkExec(Load, Load, (FU*)AdrBuf[7]);
		break;
	case 358: // AdrBuf_8_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 8
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[8]); else MkExec(Load, Load, (FU*)AdrBuf[8]);
		break;
	case 359: // AdrBuf_9_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 9
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[9]); else MkExec(Load, Load, (FU*)AdrBuf[9]);
		break;
	case 360: // AdrBuf_10_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 10
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[10]); else MkExec(Load, Load, (FU*)AdrBuf[10]);
		break;
	case 361: // AdrBuf_11_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 11
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[11]); else MkExec(Load, Load, (FU*)AdrBuf[11]);
		break;
	case 362: // AdrBuf_12_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 12
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[12]); else MkExec(Load, Load, (FU*)AdrBuf[12]);
		break;
	case 363: // AdrBuf_13_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 13
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[13]); else MkExec(Load, Load, (FU*)AdrBuf[13]);
		break;
	case 364: // AdrBuf_14_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 14
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[14]); else MkExec(Load, Load, (FU*)AdrBuf[14]);
		break;
	case 365: // AdrBuf_15_OutMk Выдать пришедшую МК на ФУ с адресом из AdrBuf с индексом 15
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[15]); else MkExec(Load, Load, (FU*)AdrBuf[15]);
		break;
		
	// Генерация случайных чисел
	case 498: // Rand Генерация дробного числа от 0 до Load
	case 499: // RandInt Генерация случайного числа от 0 до Load (по умолчанию от 0 до Rez)
		if (WrongFormatCheck(Load)) break;
		if (Load.isEmpty()) Load = { Cdouble,&Rez }; // РџСЂРё РЅСѓР»РµРІРѕР№ РЅР°РіСЂСѓР·РєРµ Р±РµСЂРµРј РѕРїРµСЂР°РЅРґ РёР· СЂРµРіРёСЃС‚СЂР° СЂРµР·СѓР»СЊР°С‚С‚Р°
		OperandsClear(MK);
		Rez = Load.toInt();
		Operands.push_back(Rez);
		FOperands.push_back(true);
		if (Rez < 1)
		{
			Ready = 2;
			ProgExec(ErrProg);
			ProgExec(MatErrProg);
		}
		if (MK == 999)
			Rez = rand() % int(Rez);
		else
			Rez = (double)(rand()) / RAND_MAX * Rez;
		break;
	// Арифметические операции
	case 500: // Add
	case 501: // AddSqr
	case 510: // Mul
		if (WrongFormatCheck(Load)) break;
		if (Ready || OpCode != MK && OpCode != MK - 1)
			OperandsClear(MK); // РЎР±СЂРѕСЃ РѕРїРµСЂР°РЅРґРѕРІ РїСЂРё РЅР°С‡Р°Р»Рµ РѕР±РѕСЂР°Р±РѕС‚РєРё РЅРѕРІРѕР№ РѕРїРµСЂР°С†РёРё
		if (Load.isEmpty()) Load = { Cdouble,&Rez }; // РџСЂРё РЅСѓР»РµРІРѕР№ РЅР°РіСЂСѓР·РєРµ Р±РµСЂРµРј РѕРїРµСЂР°РЅРґ РёР· СЂРµРіРёСЃС‚СЂР° СЂРµР·СѓР»СЊР°С‚С‚Р°

		if(Load.isEmpty())
			Operands.push_back(Rez); // РќР°РєРѕРїР»РµРЅРёРµ РёР· Р±СѓС„РµСЂР° СЂРµР·СѓР»СЊС‚Р°С‚Р°
		else
			Operands.push_back(Load.toDouble()); // РќР°РєРѕРїР»РµРЅРёРµ РѕРїРµСЂР°РЅРґРѕРІ
		FOperands.push_back(true);
		OperandsCounter++;
		if (OperandsCounter == Noperands) 
		{ //     ->  
			switch (OpCode) {
			case 500: //Add
				Rez = 0;
				for (auto i : Operands)
					Rez += i;
					break;
			case 501: //AddSqr
				Rez = 0;
				for (auto i : Operands)
					Rez += i*i;
					break;
			case 510: //Mul
				Rez = 1;
				for (auto i : Operands)
					Rez *= i;
					break;
			}
			RezExec(); // Р”РµР№СЃС‚РІРёСЏ РїСЂРё РїРѕР»СѓС‡РµРЅРёРё СЂРµР·СѓР»СЊС‚Р°С‚Р°
		}
		break;
	case 505: // Sub1
	case 506: // Sub2
	case 515: // Div1
	case 516: // Div2
	case 520: // DivInt1 Р¦РµР»РѕС‡РёСЃР»РµРЅРЅРѕРµ РґРµР»РµРЅРёРµ
	case 521: // DivInt2 Р¦РµР»РѕС‡РёСЃР»РµРЅРЅРѕРµ РґРµР»РµРЅРёРµ
	case 540: // Pow1 РЎС‚РµРїРµРЅСЊ (РѕСЃРЅРѕРІР°РЅРёРµ)
	case 541: // Pow2 РЎС‚РµРїРµРЅСЊ С‡РёСЃР»Р°
	case 542: // Log Р›РѕРіР°СЂРёС„Рј
	case 543: // LogBase Р›РѕРіР°СЂРёС„Рј (РїРµСЂРµРґР°РµС‚СЃСЏ РѕСЃРЅРѕРІР°РЅРёРµ Р»РѕРіР°СЂРёС„РјР°)
		if (WrongFormatCheck(Load)) break;
		if (Ready || OpCode != MK && OpCode != MK - 1)
			OperandsClear(MK); // РЎР±СЂРѕСЃ РѕРїРµСЂР°РЅРґРѕРІ РїСЂРё РЅР°С‡Р°Р»Рµ РѕР±РѕСЂР°Р±РѕС‚РєРё РЅРѕРІРѕР№ РѕРїРµСЂР°С†РёРё
		if (Load.isEmpty()) Load = { Cdouble,&Rez }; // РџСЂРё РЅСѓР»РµРІРѕР№ РЅР°РіСЂСѓР·РєРµ Р±РµСЂРµРј РѕРїРµСЂР°РЅРґ РёР· СЂРµРіРёСЃС‚СЂР° СЂРµР·СѓР»СЊР°С‚С‚Р°
		if (MK%5==0) // РџРµСЂРІС‹Р№ РѕРїРµСЂР°РЅРґ (РњРљ РєСЂР°С‚РЅР° 5)
		{
			if (Operands.size() < 1)
			{
				Operands.resize(1);
				FOperands.resize(1);
				FOperands[0] = false;
			}
			if (FOperands[0] == false)
			{
				FOperands[0] = true;
				OperandsCounter++;
			}
			Operands[0] = Load.toDouble();
		}
		else // Р’С‚РѕСЂРѕР№ РѕРїРµСЂР°РЅРґ
		{
			if (Operands.size() <1) // РџРµСЂРІРѕРіРѕ РѕРїРµСЂР°РЅРґР° РЅРµС‚
			{
				Operands.resize(1);
				FOperands.push_back(false);
			}
			Operands.push_back(Load.toDouble(Rez)); // РџРѕРјРµСЃС‚РёС‚СЊ РѕРїРµСЂР°РЅРґ РІ СЃС‚РµРє РѕРїРµСЂР°РЅРґРѕРІ
			FOperands.push_back(true);
			OperandsCounter++;
		}
		if (FOperands[0] && OperandsCounter >= Noperands)
		{
			ProgExec(PreRezProg);// РџСЂРѕРіСЂР°РјРјР° РїРµСЂРµРґ РїРѕР»СѓС‡РµРЅРёРµРј СЂРµР·СѓР»СЊС‚Р°С‚Р°
			Ready = 1;
			Rez = Operands[0];
			// Р”РѕР±Р°РІРёС‚СЊ РїСЂРѕРІРµСЂРєСѓ РЅР° РѕС€РёР±РєСѓ
			switch (OpCode) {
			case 505:
				for (int i = 1; i < Noperands; i++)
					Rez -= Operands[i];
				break;
			case 515:
				for (int i = 1; i < Noperands; i++)
					if (Operands[i] != 0)
					{
						Rez /= Operands[i];
					}
					else
					{
						Ready = 2; // РљРѕРґ РѕС€РёР±РєРё
						ProgExec(DivZeroErrProg);
						ProgExec(MatErrProg);// РћС€РёР±РєР° РјР°С‚РµРјР°С‚РёС‡РµСЃРєРѕР№ РѕРїРµСЂР°С†РёРё
						ProgExec(ErrProg); // Р”РµР»РµРЅРёРµ РЅР° РЅРѕР»СЊ
						break;
					}
				break;
			case 520: // DivInt
				Rez = int(Operands[0]);
				RezExtStack.push_back((int)Rez % int(Operands[1])); // РћСЃС‚Р°С‚РѕРє РѕС‚ РґРµР»РµРЅРёСЏ Р·Р°РїРёСЃСЊ РІ СЃС‚РµРє СЂР°СЃС€РёСЂРµРЅРЅС‹С… СЂРµР·СѓР»СЊС‚Р°С‚РѕРІ
				for (int i = 1; i < Noperands; i++)
					if (int(Operands[i]) != 0)
					{
						if (!RezExtStack.size()) // РЎРѕС…СЂР°РЅРµРЅРёРµ РѕСЃС‚Р°С‚РєР° РѕС‚ РґРµР»РµРЅРёСЏ РІ СЂР°СЃС€РёСЂРµРЅРЅРѕРј СЃРїРёСЃРєРµ СЂРµР·СѓР»СЊС‚Р°С‚Р°
							RezExtStack.push_back(int(Rez)% int(Operands[i]));
						else
							RezExtStack[0]=int(Rez) % int(Operands[i]);
						Rez /= int(Operands[i]);
					}
					else
					{
						Ready = 2; // РљРѕРґ РѕС€РёР±РєРё
						ProgExec(DivZeroErrProg);
						ProgExec(MatErrProg);// РћС€РёР±РєР° РјР°С‚РµРјР°С‚РёС‡РµСЃРєРѕР№ РѕРїРµСЂР°С†РёРё
						ProgExec(ErrProg); // Р”РµР»РµРЅРёРµ РЅР° РЅРѕР»СЊ
						break;
					}
				break;
			case 540: // Pow1 РЎС‚РµРїРµРЅСЊ (РѕСЃРЅРѕРІР°РЅРёРµ)
				Rez = pow(Operands[0], Operands[1]);
				break;
			case 542: // Log Р›РѕРіР°СЂРёС„Рј
				Rez = log(Operands[0]) / log(Operands[1]);
				break;
			}
			// --------------------------
			RezExec(); // Р”РµР№СЃС‚РІРёСЏ РїСЂРё РїРѕР»СѓС‡РµРЅРёРё СЂРµР·СѓР»СЊС‚Р°С‚Р°
		}
		break;
	case 522:	// Rem1 Р§РёСЃР»Рѕ РёР· РєРѕС‚РѕСЂРѕРіРѕ РёР·РІР»РµРєР°РµС‚СЃСЏ РѕСЃС‚Р°С‚РѕРє РѕС‚ С†РµР»РѕС‡РёСЃР»РµРЅРЅРѕРіРѕ РґРµР»РµРЅРёСЏ
	case 523:	// Rem2 РћСЃС‚Р°С‚РѕРє РѕС‚ С†РµР»РѕС‡РёСЃР»РµРЅРЅРѕРіРѕ РґРµР»РµРЅРёСЏ
		if (WrongFormatCheck(Load)) break;
		if (Ready || OpCode != MK && OpCode != MK - 1)
			OperandsClear(MK); // РЎР±СЂРѕСЃ РѕРїРµСЂР°РЅРґРѕРІ РїСЂРё РЅР°С‡Р°Р»Рµ РѕР±РѕСЂР°Р±РѕС‚РєРё РЅРѕРІРѕР№ РѕРїРµСЂР°С†РёРё
		if (Load.isEmpty()) Load = { Cdouble,&Rez }; // РџСЂРё РЅСѓР»РµРІРѕР№ РЅР°РіСЂСѓР·РєРµ Р±РµСЂРµРј РѕРїРµСЂР°РЅРґ РёР· СЂРµРіРёСЃС‚СЂР° СЂРµР·СѓР»СЊР°С‚С‚Р°
		if (MK == 522)
		{
			if (Operands.size() == 0)
			{
				Operands.push_back(Load.toDouble());
				FOperands.push_back(true);
				OperandsCounter = 1;
				break;
			}
			Operands[0] = Load.toDouble();
			if (FOperands[0] == false)
				OperandsCounter += 1;
			FOperands[0] = true;
		}
		else
		{
			while (Operands.size() < 2)
			{
				Operands.push_back(0);
				FOperands.push_back(false);
			}
			Operands[1] = Load.toDouble();
			if (FOperands[1] == false)
				OperandsCounter++;
			FOperands[1] = true;
		}
		if (OperandsCounter == 2)
		{
			ProgExec(PreRezProg);// РџСЂРѕРіСЂР°РјРјР° РїРµСЂРµРґ РїРѕР»СѓС‡РµРЅРёРµРј СЂРµР·СѓР»СЊС‚Р°С‚Р°
			Rez = int(Operands[0]) % int(Operands[1]);
			RezExec();
		}
		break;
	// РћРґРЅРѕРѕРїРµСЂР°РЅРґРЅС‹Рµ РґРµР№СЃС‚РІРёСЏ
	case 525: // Sqrt РљРІР°РґСЂР°С‚РЅС‹Р№ РєРѕСЂРµРЅСЊ
	case 526: // Sqr РљРІР°РґСЂР°С‚
	case 527: // Log10 Р›РѕРіР°СЂРёС„Рј РїРѕ РѕСЃРЅРѕРІР°РЅРёСЋ 10
	case 528: // Ln РќР°С‚СѓСЂР°Р»СЊРЅС‹Р№ Р»РѕРіРёСЂРёС„Рј
	case 529: // Log2 Р›РѕРіР°СЂРёС„Рј РїРѕ РѕСЃРЅРѕРІР°РЅРёСЋ 2
	case 530: // Exp Р­РєСЃРїРѕРЅРµРЅС‚Р°
	case 531: // Abs РњРѕРґСѓР»СЊ С‡РёСЃР»Р°
	case 532: // Round РћРєСЂСѓРіР»РµРЅРёРµ
	case 533: // Int Р¦РµР»Р°СЏ С‡Р°СЃС‚СЊ С‡РёСЃР»Р°
	case 534: // Ceil РћРєСЂСѓРіР»РµРЅРёРµ РґРѕ Р±РѕР»СЊС€РµРіРѕ С†РµР»РѕРіРѕ С‡РёСЃР»Р°
	case 535: // SignReverse РРЅС„РµРІРµСЂСЃРёСЏ Р·РЅР°РєР°
	case 536: // Reverse РћР±СЂР°С‚РЅРѕРµ С‡РёСЃР»Рѕ (1/x)
	case 600: // Sin
	case 601: // Cos
	case 602: // tan
	case 603: // ctan
	case 610: // ASin
	case 611: // ACos
	case 612: // Atan
		if (WrongFormatCheck(Load)) break;
		ProgExec(PreRezProg);// РџСЂРѕРіСЂР°РјРјР° РїРµСЂРµРґ РїРѕР»СѓС‡РµРЅРёРµРј СЂРµР·СѓР»СЊС‚Р°С‚Р°
		Rez = Load.toDouble(Rez); // РџРѕРјРµСЃС‚РёС‚СЊ РІ Р°РєРєСѓРјСѓР»СЏС‚РѕСЂ РЅР°РіСЂСѓР·РєСѓ, РµСЃР»Рё РЅР°РіСЂСѓР·РєР° РЅСѓР»РµРІР°СЏ, С‚Рѕ РїРѕРјРµСЃС‚РёС‚СЊ Rez
		Operands.clear();
		FOperands.clear();
		Operands.push_back(Rez);
		FOperands.push_back(true);
		switch (MK)
		{
		case 525: // Sqrt РљРІР°РґСЂР°С‚РЅС‹Р№ РєРѕСЂРµРЅСЊ
		case 526: // Sqr РљРІР°РґСЂР°С‚
		case 527: // Ln
		case 528: // Log2
		case 529: // Log10 Р›РѕРіР°СЂРёС„Рј РїРѕ РѕСЃРЅРѕРІР°РЅРёСЋ 10
			if(MK!=526 && (!Load.isEmpty() && Load.toDouble() < 0))
				{
					Ready = 2; // РљРѕРґ РѕС€РёР±РєРё
					ProgNExec({ ErrProg, MatErrProg }); // Р”РµР»РµРЅРёРµ РЅР° РЅРѕР»СЊ  // РћС€РёР±РєР° РјР°С‚РµРјР°С‚РёС‡РµСЃРєРёС… РІС‹С‡РёСЃР»РµРЅРёР№
					break;
				}
			switch (MK) {
			case 525: //Sqrt РљРІР°РґСЂР°С‚РЅС‹Р№ РєРѕСЂРµРЅСЊ
				Rez = sqrt(Rez);
				break;
			case 526: //Sqr РљРІР°РґСЂР°С‚
				Rez = Rez * Rez;
				break;
			case 527: // Ln
				Rez = log(Rez);
				break;
			case 528: // Log2
				Rez = log2(Rez);
				break;
			case 529: // Log10 Р›РѕРіР°СЂРёС„Рј РїРѕ РѕСЃРЅРѕРІР°РЅРёСЋ 10
				Rez = log10(Rez);
				break;
			}
			break;
		case 530: // Exp
			Rez = exp(Load.toDouble());
			break;
		case 531: // ABS РњРѕРґСѓР»СЊ С‡РёСЃР»Р°
			Rez = fabs(Load.toDouble());
			break;
		case 532: // Round РћРєСЂСѓРіР»РµРЅРёРµ
			Rez = round(Load.toDouble());
			break;
		case 533: // Int Р¦РµР»Р°СЏ С‡Р°СЃС‚СЊ С‡РёСЃР»Р°
			Rez = int(Load.toDouble());
			break;
		case 534: // РЎeil РћРєСЂСѓРіР»РµРЅРёРµ РґРѕ Р±РѕР»СЊС€РµРіРѕ С†РµР»РѕРіРѕ С‡РёСЃР»Р°
			Rez = ceil(Load.toDouble());
			break;
		case 535: // SignReverse РРЅС„РµРІРµСЂСЃРёСЏ Р·РЅР°РєР°
			Rez = -Load.toDouble();
			break;
		case 536: //  Reverse РћР±СЂР°С‚РЅРѕРµ С‡РёСЃР»Рѕ (1/x)
			Rez = -Load.toDouble();

			break;
			// РўСЂРёРіР°РЅРѕРјРµС‚СЂРёС‡РµСЃРєРёРµ РѕРїРµСЂР°С†РёРё
		case 600: // Sin
		case 601: // Cos
		case 602: // tan
		case 603: // ctan
			if (AngleMode == 1 and !Load.isEmpty()) // Р Р°РґРёР°РЅС‹
			{
				Rez = Rez / 360 * 6.283185307179586;
				*Operands.begin() = Rez;
			}
			switch (MK)
			{
			case 600: // Sin
				Rez = sin(Rez);	break;
			case 601: // Cos
				Rez = cos(Rez);	break;
			case 602: // tan
				Rez = tan(Rez);	break;
			case 603: // ctan
				Rez = 1 / tan(Rez);
				break;
				// РћР±СЂР°С‚РЅС‹Рµ С‚СЂРёРіР°РЅРѕРјРµС‚СЂРёС‡РµСЃРєРёРµ РѕРїРµСЂР°С†РёРё
			}
			break;
		case 610: // ASin
		case 611: // ACos
			if (Rez > 1 || Rez < 1)
			{
				Ready = 2; // РљРѕРґ РѕС€РёР±РєРё
				ProgNExec({ ErrProg, MatErrProg }); // Р”РµР»РµРЅРёРµ РЅР° РЅРѕР»СЊ  // РћС€РёР±РєР° РјР°С‚РµРјР°С‚РёС‡РµСЃРєРёС… РІС‹С‡РёСЃР»РµРЅРёР№
				break;
			}
			switch (MK)
			{
			case 610: // Asin
				Rez = asin(Rez); break;
			case 611: // Acos
				Rez = acos(Rez); break;
			}
			break;
		case 612: // Atan
			Rez = atan(Rez); break;
		}
		Ready = 1;
		RezExec(); // Р”РµР№СЃС‚РІРёСЏ РїСЂРё РїРѕР»СѓС‡РµРЅРёРё СЂРµР·СѓР»СЊС‚Р°С‚Р°
		break;

	default:
		CommonMk(MK, Load, Sender);
		break;
	}
	if (PostfixProg != nullptr) ProgExec(PostfixProg); // Р—Р°РїСѓСЃРє РїСЂРµРґРІР°СЂРёС‚РµР»СЊРЅРѕР№ РїСЂРѕРіСЂР°РјРјС‹
}

void StreamFloatALU::RezExec() // Р’С‹РїРѕР»РЅРµРЅРёРµ РїРѕРґРїСЂРѕРіСЂР°РјРј РїСЂРё РїРѕР»СѓС‡РµРЅРёРё СЂРµР·СѓР»СЊС‚Р°С‚Р°
{
	if (Ready == 2) return;
	Ready = 1;
	if (!OutRezBlock)//Р•СЃР»Рё РЅРµ Р·Р°Р±Р»РѕРєРёСЂРѕРІР°РЅР° СЂР°СЃСЃС‹Р»РєР° РњРљ СЃ СЂРµР·СѓР»СЊС‚Р°С‚Р°РјРё РІС‹С‡РёСЃР»РµРЅРёР№
		for (int i = 0; i < ReceiverMk.size(); i++) { // Р Р°СЃСЃС‹Р»РєР° РњРљ СЃ СЂРµР·СѓР»СЊС‚Р°С‚Р°РјРё
			MkExec(ReceiverMk[i], { Cdouble, &Rez }, ReceiverContexts[i]);
		}
	for (auto& i : OutVars) // Р—Р°РїРёСЃР°С‚СЊ СЂРµР·СѓР»СЊС‚Р°С‚ РІ РІС‹С…РѕРґРЅС‹Рµ РїРµСЂРµРјРµРЅРЅС‹Рµ
		i.Write(Rez);
	ProgExec(RezProg);
	if (Rez == 0) ProgExec(ZProg);
	if (Rez != 0) ProgExec(NZProg);
	if (Rez >= 0) ProgExec(BZProg);
	if (Rez <= 0) ProgExec(LZProg);
	if (Rez > 0) ProgExec(BProg);
	if (Rez < 0) ProgExec(LProg);
}

void StreamFloatALU::OperandsClear(long int MK) // РЎР±СЂРѕСЃ РѕРїРµСЂР°РЅРґРѕРІ РїСЂРё РЅР°С‡Р°Р»Рµ РѕР±РѕСЂР°Р±РѕС‚РєРё РЅРѕРІРѕР№ РѕРїРµСЂР°С†РёРё
{
//	if (Ready || OpCode != MK && OpCode != MK - 1) 
	{
		OpCode = MK;
		Ready = 0;
		Operands.clear();
		FOperands.clear();
		RezExtStack.clear();
		OperandsCounter = 0;
	}
}

bool StreamFloatALU::WrongFormatCheck(LoadPoint Load) // РџСЂРѕРІРµСЂРєР° С„РѕСЂРјР°С‚Р° РІС…РѕРґРЅС‹С… РґР°РЅРЅС‹С… (РІРѕР·РІСЂР°С‰Р°РµС‚ true, РµСЃР»Рё РЅРµРїСЂР°РІРёР»СЊРЅС‹Р№ С„РѕСЂРјР°С‚)
{
	if (!Load.isEmpty() && !Load.isDigit()) // РћС€РёР±РєР° С„РѕСЂРјР°С‚Р° РѕРїРµСЂР°РЅРґР°
	{
		Ready = 2; // РљРѕРґ РѕС€РёР±РєРё
		ProgExec(ErrProg); // РќРµРїСЂР°РІРёР»СЊРЅС‹Р№ С„РѕСЂРјР°С‚ РѕРїРµСЂР°РЅРґР°
		ProgExec(WrongFormatErrProg);
		return true;
	}
	return false;
}

StreamFloatALU::StreamFloatALU(void* Dev1) // РљРѕРїРёСЂСѓСЋС‰РёР№ РєРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ
{
	StreamFloatALU* Dev = (StreamFloatALU*)Dev1;
	if (Dev == nullptr || Dev->FUtype != FUtype) return;
	FOperands.resize(Dev->FOperands.size());
	copy(Dev->FOperands.begin(), Dev->FOperands.end(), FOperands.begin());
	Operands.resize(Dev->Operands.size());
	copy(Dev->Operands.begin(), Dev->Operands.end(), Operands.begin());
	RezStack.resize(Dev->RezStack.size());
	copy(Dev->RezStack.begin(), Dev->RezStack.end(), RezStack.begin());
	Operands.resize(Dev->RezExtStack.size());
	copy(Dev->RezExtStack.begin(), Dev->RezExtStack.end(), RezExtStack.begin());
	RezStack.resize(Dev->ReceiverMk.size());
	copy(Dev->ReceiverMk.begin(), Dev->ReceiverMk.end(), ReceiverMk.begin());
	Operands.resize(Dev->ReceiverContexts.size());
	copy(Dev->ReceiverContexts.begin(), Dev->ReceiverContexts.end(), ReceiverContexts.begin());
	Ready = Dev->Ready;
	OutRezBlock = Dev->OutRezBlock;
	Rez = Dev->Rez;
	OperandsCounter = Dev->OperandsCounter;
	OpInd = Dev->OpInd;
	AngleMode = Dev->AngleMode;
	Noperands = Dev->Noperands;
	ZProg = Dev->ZProg;
	NZProg = Dev->NZProg;
	BProg = Dev->BProg;
	BZProg = Dev->BZProg;
	LProg = Dev->LProg;
	LZProg = Dev->LZProg;
	ErrProg = Dev->ErrProg;
	WrongFormatErrProg = Dev->WrongFormatErrProg;
	OveflowErrProg = Dev->OveflowErrProg;
	DivZeroErrProg = Dev->DivZeroErrProg; //  
	MatErrProg = Dev->MatErrProg; // РџСЂРѕРіСЂР°РјРјР° РѕР±СЂР°Р±РѕС‚РєРё РѕС€РёР±РєРё РјР°С‚РµРјР°С‚РёС‡РµСЃРєРѕР№ РѕРїРµСЂР°С†РёРё
	NoOperandErrProg = NoOperandErrProg;// РћС€РёР±РєР° РЅРµС‚ РѕРїРµСЂР°РЅРґР°
	OpIndErrProg = Dev->OpIndErrProg;// РћС€РёР±РєР° РёРЅРґРµРєСЃР° РѕРїРµСЂР°РЅРґР°
	OperationErrProg = Dev->OperationErrProg; // РћС€РёР±РєР° РѕРїРµСЂР°С†РёРё
	RezStackIsEmpyProg = Dev->RezStackIsEmpyProg; // РћС€РёР±РєР° РїСЂРё РїРѕРїС‹С‚РєРµ РёР·РІР»РµС‡РµРЅРёСЏ РёР· РїСѓСЃРєРѕРіРѕ СЃС‚РµРєР° СЂРµР·СѓР»СЊС‚Р°С‚РѕРІ
	RezExtStackIsEmpyProg = Dev->RezExtStackIsEmpyProg; // РћС€РёР±РєР° РїСЂРё РїРѕРїС‹С‚РєРµ РёР·РІР»РµС‡РµРЅРёСЏ РёР· РїСѓСЃРєРѕРіРѕ СЃС‚РµРєР° СЂР°СЃС€РёСЂРµРЅРЅРѕРіРѕ СЂРµР·СѓР»СЊС‚Р°С‚РѕРІ
	OperetionProg = Dev->OperetionProg;// РџСЂРѕРіСЂР°РјРјР° РґР»СЏ РІС‹РїРѕР»РЅРµРЅРёСЏ СЃРїРµС†РёР°Р»СЊРЅРѕР№ РѕРїРµСЂР°С†РёРё
	RezProg = Dev->RezProg; // РџСЂРѕРіСЂР°РјРјР°, Р·Р°РїСѓСЃРєР°РµРјР°СЏ РїРµСЂРµРґ РїРѕР»СѓС‡РµРЅРёРµРј СЂРµР·СѓР»СЊС‚Р°С‚Р°
	PreRezProg = Dev->PreRezProg; // РџСЂРѕРіСЂР°РјРјР°, Р·Р°РїСѓСЃРєР°РµРјР°СЏ РїРµСЂРµРґ РїРѕР»СѓС‡РµРЅРёРµРј СЂРµР·СѓР»СЊС‚Р°С‚Р°
	OpCode = Dev->OpCode;
}

FU* StreamFloatALU::Copy() // РџСЂРѕРіСЂР°РјРјР° РєРѕРїРёСЂРѕРІР°РЅРёСЏ Р¤РЈ
{
	return new StreamFloatALU(Bus,this);
}

FU* StreamFloatALU::TypeCopy() // РЎРѕР·РґР°С‚СЊ Р¤РЈ С‚Р°РєРѕРіРѕ Р¶Рµ С‚РёРїР° (РЅРµ РєРѕРїРёСЂСѓСЏ РєРѕРЅС‚РµРєСЃС‚
{
	return new StreamFloatALU(Bus, nullptr);
}

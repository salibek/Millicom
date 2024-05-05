#include "StreamIntALU.h"
#include <limits>
void StreamIntALU::ProgFU(long int MK, LoadPoint Load, FU* Sender)
{
	if (PrefixProg!=nullptr) ProgExec(PrefixProg); // ������ ��������������� ���������
	if (!Active && MK < 900) return; //��� ���������� ����� ���������� ����������� ����� ��
	if (MK >= FUMkRange && MK < FUMkGlobalAdr && MK >= FUMkGlobalAdr + FUMkRange)
	{

		ProgExec(RoutProg);
		return;
	}
	else
		ProgExec(SelfAdrProg);
	if (MkAbort)
	{
		MkAbort = false;
		return;
	}
	int MKinitial = MK;
	MK %= FUMkRange;
	if (Load.isEmpty()) Load = { Cint,&Rez }; // ���� ������� ��������, �� ��������� �������� �����������
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
		break;
	case 1: // Set ���������� ��������� ����������
		Rez = Load.toInt();
		break;
	case 3: // RoutProg ��������� ��� ������������ ������ �� � ��� ����������� �������
		RoutProg = Load.Point;
		break;
	case 4: // SelfAdrProg ��������� ��� ���������� ������ �� � ��� ����������� �������
		RoutProg = Load.Point;
		break;
	case 5: //Out  ������ ���������
		Load.Write(Rez);
		break;
	case 6: //OutMk ������ �� � �����������
		MkExec(Load, { Cint, &Rez });
		break;
	case 10: //OpCounterOut ������ ������� ����������� ���������
		Load.Write(OperandsCounter);
		break;
	case 11: //OpCounterOutMk ������ �� �� ��������� ����������� ���������
	{
		long int t = OperandsCounter;
		MkExec(Load, { Cint, &t });
	}
	break;
	case 15: //MkAbortSet ���������� ���� ���������� ���������� �� (�� ��������� true)
		MkAbort = Load.toBool(true);
		break;
	case 25: //EarlyCalculiSet ���������� ������ ���������� ���������� (true �� ���������)
		EarlyCalculi = Load.toBool(true);
		break;
	case 30: // OperandsReset ����� ��������� (��� �� ��������, ��� ������, ������������ � ���������� ��������� ���������� ������)
		OperandsCounter = 0;
		for (size_t i = 0; i < FOperands.size(); ++i) {
			FOperands[i] = false;
		}
		break;
	case 40: //ErrProgSet ���������� ��������� ��� ������
		ErrProg = Load.Point;
		break;
	case 41: //WrongFormatErrProgSet ���������� ��������� ��� ������������ ������� ��������
		WrongFormatErrProg = Load.Point;
		break;
	case 42: //OveflowErrProgSet ���������� ��������� ��� ������������
		OveflowErrProg = Load.Point;
		break;
	case 43: //DivZeroErrProgSet ���������� ��������� ��� ������� �� 0
		DivZeroErrProg = Load.Point;
		break;
	case 44: //OpIndErrProgSet ���������� ��������� ��� ������ ������� ��������
		OpIndErrProg = Load.Point;
		break;
	case 45: //RezStackIsEmpyProgSet ���������� ��������� ��� ������ �� ������� ����� �����������
		RezStackIsEmpyProg = Load.Point;
		break;
	case 46: //MatErrProgSet ���������� ��������� ��������� ������ �������������� ��������
		MatErrProg = Load.Point;
		break;
	case 70: // ReadySet ���������� ���� ���������� ���������� (�� ��������� true)
		Ready = Load.toInt(1);
		break;
	case 71: // ReadyOut ������ ���� ���������� ����������    
		Load.Write(Ready);
		break;
	case 72: // ReadyOutMk ������ �� � ������ ���������� ����������   
		MkExec(Load, { Cint, &Ready });
		break;
	case 75: // ReadyExec ������ ��������� �� ����� ���������� �����������     
		if (Ready == 1)
			ProgExec(Load);
		break;
	case 76: // ReadyNotExec   ������ ��������� ��� ���������� ����� ���������� ����������,      
		if (Ready == 0)
			ProgExec(Load);
		break;
	case 77: //ErrExec ��������� ���� ������
		if (Ready == 2)
			ProgExec(Load);
		break;
	case 80: // OutRezBlockSet ���������� ���������� ������ ���������� (��� ������� �������� true)
		OutRezBlock = Load.toBool(true);
		break;
	case 90:// Push �������� � ���� (��� ������� �������� � ���� ���������� Rez)
		if (Load.Point == nullptr)
			RezStack.push_back(Rez);
		else
			RezStack.push_back(Load.toInt());
		break;
	case 91: // Pop ������ �� ����� (��� ��� ������� �������� �������� ���������� � Rez)
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
	case 92: // PopMk ������ �� ����� � ������ �� (��� ��� ������� �������� �������� ���������� � Rez)        
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
			long int temp = RezStack.back();
			RezStack.pop_back();
			MkExec(Load, { Cint, &temp });
		}
	}
	break;
	case 93: // StackCounterOut ������ ���������� ��������� � �����
		Load.Write((long int)RezStack.size());
		break;
	case 94: // StackCounterOut ������ �� � ����������� ��������� � �����
	{
		long int t = RezStack.size();
		MkExec(Load, { Cint,&t });
		break;
	}
	case 95: // StackOut ������ �� ����� (��� ��� ������� �������� �������� ���������� � Rez) 
		if (Load.isEmpty())
			Rez = RezStack.back();
		else
			Load.Write(RezStack.back());
		break;
	case 96: // StackOutMk ������ �� ����� � �� (��� ��� ������� �������� �������� ���������� � Rez)
	{
		if (Load.isEmpty())
			Rez = RezStack.back();
		else {
			auto temp = RezStack.back();
			MkExec(Load, { Cint, &temp });
		}
		break;
	}
	case 97: //StackEmptyExec ���������, ���� ���� ������
		if (!RezStack.size()) ProgExec(Load);
		break;
	case 98: //StackNotEmptyExec ���������, ���� ���� �� ������
		if (RezStack.size()) ProgExec(Load);
		break;
	case 100: //RezExtOut ������ �������� �� ������������ ����� ����������
	case 101: // RezExtOutMk ������ �� �� ��������� �� ������������ ����� ����������
	case 102: // RezExtPop �������� ����������� ��������� (��� ������� �������� ������������ � Rez)
	case 103: // RezExtPopMk �������� �� � ����������� ����������� (��� ������� �������� ������������ � Rez)
		if (!RezExtStack.size())
		{
			ProgExec(RezExtStackIsEmpyProg);
			break;
		}
		switch (MK)
		{
		case 100:
			Load.Write(RezExtStack.back());
			break;
		case 101:
		{
			long int t = RezExtStack.back();
			MkExec(Load, { Cint,&t });
			break;
		}
		case 102:
		{
			Load.Write(RezExtStack.back());
			RezExtStack.pop_back();
			break;
		}
		case 103:
		{
			long int t = RezExtStack.back();
			RezExtStack.pop_back();
			MkExec(Load, { Cint,&t });
			break;
		}
		break;
		}
		break;
	case 105: // RezExtStackIsEmpyProgSet ���������� ��������� ��� ������ ������ �� ������� ����� ������������ ����������
		RezExtStackIsEmpyProg = Load.Point;
		break;

	case 150: // NOperandSet ���������� ���������� ��������� (�� ��������� 2)
		OperandsCounter = Load.toInt(2);
		break;
	case 151: // NOperandAdd ��������� ���������� ��������� (�� ��������� �� 1)
		OperandsCounter += Load.toInt(1);
		break;

	case 160: // ReceiverReset ����� ��������� ����������� ����������
		ReceiverMk.clear();
		ReceiverContexts.clear();
		break;
	case 161: // ReceiverAdd ���������� ������ �� ��������� ���������� (��������������� ����� ���������� ��)
		ReceiverContexts.push_back((FU*)Load.Point);
		break;
	case 162: // ReceiverMkAdd ���������� �� ��� ��������� ���������� 
		if (ReceiverMk.size() == ReceiverContexts.size())
			ReceiverContexts.push_back(nullptr);
		ReceiverMk.push_back(Load.toInt());
		break;
	case 165: // OutVarReset �������� ���������� ��� ������ ����������
		OutVars.clear();
		break;
	case 166: // OutVarSet �������� ���������� ��� ������ ����������
		OutVars.push_back(Load);
		break;
	case 190: // RezProgSet ���������� ������ �� ������������, ����������� ��� ��������� ����������   
		RezProg = Load.Point;
		break;
	case 191: //PreRezProgSet ���������, ����������� ����� ���������� ����������
		PreRezProg = Load.Point;
		break;
	case 200: // Op0Out ������ �������   
	case 205: // Op1Out   
	case 210: // Op2Out   
	case 215: // Op3Out   
	case 220: // Op4Out   
	case 225: // Op5Out   
	case 230: // Op6Out   
	case 235: // Op7Out   
	case 240: // Op8Out   
	case 245: // Op9Out   
		if (Operands.size() < (MK - 200) / 5)
		{
			ProgExec(ErrProg);
			ProgExec(NoOperandErrProg);
			break;
		}
		Load.Write(Operands[(MK - 200) / 5]);
		break;
	case 201: // Op0OutMk  ������ �� � ��������� 0 
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
			long int temp = Operands[(MK - 200) / 5];
			MkExec(Load, { Cint, &temp });
		}
		break;
	case 202: // Op0Set  ���������� ������� 0
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
		{
			long int t = (MK - 200) / 5;
			while (Operands.size() < t)
			{
				Operands.push_back(0);
				FOperands.push_back(false);
			}

			Operands[t] = Load.toInt();
			if (!FOperands[t])
				OperandsCounter++;
			FOperands[t] = true;
		}
		break;
	case 250: //	OpIndSet ���������� ������ ��������
		OpInd = Load.toInt(0);
		break;
	case 251: // OpIndAdd ��������� � ������� �������� (�� ��������� 1)
		OpInd += Load.toInt(1);
		break;
	case 252: // OperandByIndSet ���������� ������� �� �������
		while (Operands.size() <= OpInd)
		{
			Operands.push_back(0);
			FOperands.push_back(false);
		}
		Operands[OpInd] = Load.toInt();
		if (!FOperands[OpInd])
			OperandsCounter++;
		FOperands[OpInd] = true;
		if (OperandsCounter >= Noperands) // ���������� ���������
		{
			ProgExec(OperetionProg);
			RezExec();
		}
		break;
	case 253: // OperandByIndOut ������ ������� �� �������
		if (Operands.size() <= OpInd || OpInd < 0)
		{
			ProgExec(ErrProg);
			ProgExec(OpIndErrProg);
			break;
		}
		Load.Write(Operands[OpInd]);
		break;
	case 254: // OperandByIndOutMk ������ �� � ��������� �� �������
	{
		if (Operands.size() <= OpInd || OpInd < 0)
		{
			ProgExec(ErrProg);
			ProgExec(OpIndErrProg);
			break;
		}
		long int temp = Operands[OpInd];
		MkExec(Load, { Cint, &temp });
		break;
	}
	case 255: // OperandAdd �������� ������� ��� ����������� ��
		Operands.push_back(Load.toInt());
		FOperands.push_back(true);
		OperandsCounter++;
		break;
	case 260: // OperationProgSet ���������� ����������� ��������    
		OperetionProg = Load.Point;
		break;
	case 280: // ZeroProgSet ���������� ��������� ��� == 
		ZProg = Load.Point;
		break;
	case 281: // NotZeroProgSet ���������� ��������� ��� !=
		BZProg = Load.Point;
		break;
	case 282: // LessProgSet   ���������� ��������� ��� <
		LProg = Load.Point;
		break;
	case 283: // BiggerProgSet   ���������� ��������� ��� >
		BProg = Load.Point;
		break;
	case 284: // LessZeroProgSet   ���������� ��������� ��� <=
		LZProg = Load.Point;
		break;
	case 285: // BiggerZProgSet   ���������� ��������� ��� >=
		BZProg = Load.Point;
		break;
	case 290: // ZeroExec   ��������� ��������� ��� ==
		if (Rez == 0)
			ProgExec(Load);
		break;
	case 291: // LessExec    ��������� ��������� ��� <
		if (Rez < 0)
			ProgExec(Load);
		break;
	case 292: // BiggerExec    ��������� ��������� ��� >
		if (Rez > 0)
			ProgExec(Load);
		break;
	case 293: // LessZeroExec    ��������� ��������� ��� <=
		if (Rez <= 0)
			ProgExec(Load);
		break;
	case 294: // BiggerZeroExec    ��������� ��������� ��� >=
		if (Rez >= 0)
			ProgExec(Load);
		break;
	case 296: // NotZeroExec    ��������� ��������� ��� !=
		if (Rez != 0)
			ProgExec(Load);
		break;
	case 300: // ZeroOut ������ ���� ����
		Load.Write(Rez == 0);
		break;
	case 301: // ZeroOutMk ������ �� � ������ ����
	{
		bool temp = Rez == 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 305: // BiggerOut  ������ ���� ������
		Load.Write(Rez > 0);
		break;
	case 306: // BiggerOutMk ������ �� � ������ ������
	{
		bool temp = Rez > 0;
		MkExec(Load, { Cbool, &temp });
	}
	break;
	case 310: // LessOut  ������ ���� ������
		Load.Write(Rez < 0);
		break;
	case 311: // LessOutMk BOutMk ������ �� � ������ ������
	{
		bool temp = Rez < 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 315: // BiggerZeroOut ������ ���� ������ ��� ����
		Load.Write(Rez >= 0);
		break;
	case 316: // BiggerZeroOutMk BOutMk ������ �� � ������ ������ ��� ����
	{
		bool temp = Rez >= 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 320: // LessZeroOut ������ ���� ������ ��� ����
		Load.Write(Rez <= 0);
		break;
	case 321: // LessZeroOutMk BOutMk ������ �� � ������ ������ ��� ����
	{
		bool temp = Rez <= 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 325: // NotZeroOut  ������ ���� �� ����
		Load.Write(Rez != 0);
		break;
	case 326: // NotZeroOutMk ������ �� � ������ �� ����   
	{
		bool temp = Rez != 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}


	case 335: // RoutedMkOut ������ ������������������ ��
		Load.Write(IpForMkAdrOut.atr);
		break;
	case 336: // RoutedMkOutMk ������ �� � ������������������ ��
		MkExec(Load, { Cint, &IpForMkAdrOut.atr });
		break;
	case 337: // RoutedLoadOut  ������ ������������������ �������� ��
		Load.Write(IpForMkAdrOut.atr);
		break;
	case 338: // RoutedLoadOutMk ������ �� � ������������������ ��������� ��
		MkExec(Load, IpForMkAdrOut.Load);
		break;
	case 339: // RoutedIpOut  ������ ������������������ ��
		//Load.Write(IpForMkAdrOut);
		break;
	case 340: // RoutedIpOutMk ������ �� � ������������������ ��
		MkExec(Load, { CIP, &IpForMkAdrOut });
		break;

	case 345: // AdrBufAdd �������� ����� � ����� �������
		AdrBuf.push_back(Load.Point);
		break;
	case 346: // AdrBufClear �������� ����� �������
		AdrBuf.clear();
		break;
	case 347: // AdrBufAllOutMk ������ �� �� ��� ������ �� AdrBuf (�� ����� ��)
		for (auto& i : AdrBuf)
			if (Load.isEmpty())
				MkExec(MK, Load, i);//??? ��������� ������������ ������
			else
				MkExec(Load, Load, i);
		break;

	case 350: // AdrBuf_0_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 0
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[0]); else MkExec(Load, Load, (FU*)AdrBuf[0]);
		break;
	case 351: // AdrBuf_1_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 1
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[1]); else MkExec(Load, Load, (FU*)AdrBuf[1]);
		break;
	case 352: // AdrBuf_2_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 2
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[2]); else MkExec(Load, Load, (FU*)AdrBuf[2]);
		break;
	case 353: // AdrBuf_3_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 3
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[3]); else MkExec(Load, Load, (FU*)AdrBuf[3]);
		break;
	case 354: // AdrBuf_4_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 4
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[4]); else MkExec(Load, Load, (FU*)AdrBuf[4]);
		break;
	case 355: // AdrBuf_5_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 5
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[5]); else MkExec(Load, Load, (FU*)AdrBuf[5]);
		break;
	case 356: // AdrBuf_6_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 6
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[6]); else MkExec(Load, Load, (FU*)AdrBuf[6]);
		break;
	case 357: // AdrBuf_7_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 7
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[7]); else MkExec(Load, Load, (FU*)AdrBuf[7]);
		break;
	case 358: // AdrBuf_8_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 8
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[8]); else MkExec(Load, Load, (FU*)AdrBuf[8]);
		break;
	case 359: // AdrBuf_9_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 9
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[9]); else MkExec(Load, Load, (FU*)AdrBuf[9]);
		break;
	case 360: // AdrBuf_10_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 10
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[10]); else MkExec(Load, Load, (FU*)AdrBuf[10]);
		break;
	case 361: // AdrBuf_11_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 11
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[11]); else MkExec(Load, Load, (FU*)AdrBuf[11]);
		break;
	case 362: // AdrBuf_12_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 12
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[12]); else MkExec(Load, Load, (FU*)AdrBuf[12]);
		break;
	case 363: // AdrBuf_13_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 13
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[13]); else MkExec(Load, Load, (FU*)AdrBuf[13]);
		break;
	case 364: // AdrBuf_14_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 14
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[14]); else MkExec(Load, Load, (FU*)AdrBuf[14]);
		break;
	case 365: // AdrBuf_15_OutMk ������ ��������� �� �� �� � ������� �� AdrBuf � �������� 15
		if (Load.isEmpty()) MkExec(MK, Load, (FU*)AdrBuf[15]); else MkExec(Load, Load, (FU*)AdrBuf[15]);
		break;

//	case 498: // Rand ��������� �������� ����� �� 0 �� Load
	case 499: // RandInt ��������� ���������� ����� �� 0 �� Load (�� ��������� �� 0 �� Rez)
		if (WrongFormatCheck(Load)) break;
		if (Load.isEmpty()) Load = { Cint,&Rez }; // ��� ������� �������� ����� ������� �� �������� ����������
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
			Rez = (int)(rand()) / RAND_MAX * Rez;
		break;
		// �������������� ��������
	case 500: // Add
	case 501: // AddSqr
	case 510: // Mul
		// ���������� ��������
	case 600: // Or
	case 601: // And
	case 602: // Xor
	case 610: // OrBit
	case 611: // AndBit
	case 612: // XorBit
		if (WrongFormatCheck(Load)) break;
		if (Ready || OpCode != MK)
			OperandsClear(MK); // ����� ��������� ��� ������ ���������� ����� ��������

		if (Load.isEmpty()) Load = { Cint,&Rez }; // ��� ������� �������� ����� ������� �� �������� ����������

		if (Load.isEmpty())
			Operands.push_back(Rez); // ���������� �� ������ ����������
		else
			Operands.push_back(Load.toInt()); // ���������� ���������
		FOperands.push_back(true);
		OperandsCounter++;
		if (EarlyCalculi)
		{
			switch (OpCode) {
			case 500: //Add
				Rez += Load.toInt();
				break;
			case 501: //AddSqr
				Rez += Load.toInt() * Load.toInt();
				break;
			case 510: //Mul
				Rez *= Load.toInt();
				break;
			case 600: // Or
				Rez = (bool) bool(Rez) || Load.toBool();
				break;
			case 601: // And
				Rez = (bool) bool(Rez) && Load.toBool();
				break;
			case 602: // Xor
				Rez = (bool) bool(Rez) ^ Load.toBool();
				break;
			case 610: // OrBit
				Rez = (unsigned int)(unsigned int)(Rez) | ((unsigned int)Load.toBool());
				break;
			case 611: // AndBit
				Rez = (unsigned int)(unsigned int)(Rez) | ((unsigned int)Load.toBool());
				break;
			case 612: // XorBit
				Rez = (unsigned int)(unsigned int)(Rez) | ((unsigned int)Load.toBool());
				break;
			}
		}
		if (OperandsCounter == Noperands)
		{ //     ->  
			ProgExec(PreRezProg);// ��������� ����� ���������� ����������
			Rez = Operands[0];
			switch (OpCode) {
			case 500: //Add
				for (int i = 1; i < Noperands; i++)
				{
					if (abs((long int)Rez + Operands[i]) > INT_MAX)
					{
						ProgNExec({ ErrProg, MatErrProg, OveflowErrProg });
						break;
					}
					Rez += Operands[i];
				}
				break;
			case 501: //AddSqr
				Rez *= Rez;
				for (int i = 1; i < Noperands; i++)
				{
					if (abs((long int)Rez + Operands[i] * Operands[i]) > INT_MAX)
					{
						ProgNExec({ ErrProg, MatErrProg, OveflowErrProg });
						break;
					}
					Rez += Operands[i] * Operands[i];
				}
				break;
			case 510: //Mul
				for (int i = 1; i < Noperands; i++)
				{
					if (abs((long int)Rez * Operands[i]) > INT_MAX)
					{
						ProgNExec({ ErrProg, MatErrProg, OveflowErrProg });
						break;
					}
					Rez *= Operands[i];
				}
				break;

			case 600: // And ���������� �
				for (int i = 1; i < Noperands; i++)
					Rez = (bool) Rez && Operands[i];
				break;
			case 601: // Or ���������� ���
				for (int i = 1; i < Noperands; i++)
					Rez = (bool) bool(Rez) || bool(Operands[i]);
				break;
			case 602: // Xor ���������� ����������� ���
				for (int i = 1; i < Noperands; i++)
					Rez =(bool) bool(Rez) ^ bool(Operands[i]);
				break;
			case 610: // AndBit ��������� �
				for (int i = 1; i < Noperands; i++)
					Rez = Rez & Operands[i];
				break;
			case 611: // OrBit ��������� ���
				for (int i = 1; i < Noperands; i++)
					Rez = Rez | Operands[i];
				break;
			case 612: // XorBit ��������� ����������� ���
				for (int i = 1; i < Noperands; i++)
					Rez = (unsigned int)Rez ^ (unsigned int)Operands[i];
				break;
			}
			RezExec(); // �������� ��� ��������� ����������
		}
		break;
	case 505: // Sub1
	case 506: // Sub2
//	case 515: // Div1
//	case 516: // Div2
	case 520: // DivInt1 ������������� �������
	case 521: // DivInt2 ������������� �������
	case 522:	// Rem1 ����� �� �������� ����������� ������� �� �������������� �������
	case 523:	// Rem2 ������� �� �������������� �������
	case 540: // Pow1 ������� (���������)
	case 541: // Pow2 ������� �����
		if (WrongFormatCheck(Load)) break;
		if (Ready || OpCode != MK &&  OpCode != MK - 1 && OpCode-1 != MK )
			OperandsClear(MK); // ����� ��������� ��� ������ ���������� ����� ��������
		if (Load.isEmpty()) Load = { Cint,&Rez }; // ��� ������� �������� ����� ������� �� �������� ����������
		if (MK % 5==0) // ������ ������� (�� ������ 5)
		{
			OpCode = MK;
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
			Operands[0] = Load.toInt();
		}
		else // ������ �������
		{
			if (Operands.size() < 1) // ������� �������� ���
			{
				Operands.resize(1);
				FOperands.push_back(false);
				OpCode = OpCode - OpCode % 5;
			}
			Operands.push_back(Load.toInt(Rez)); // ��������� ������� � ���� ���������
			FOperands.push_back(true);
			OperandsCounter++;
		}
		if (FOperands[0] && OperandsCounter >= Noperands)
		{
			ProgExec(PreRezProg);// ��������� ����� ���������� ����������
			Ready = 1;
			Rez = Operands[0];
			// �������� �������� �� ������
			switch (OpCode) {
			case 505: // Sub
				for (int i = 1; i < Noperands; i++)
				{
					if (abs((long int)Rez  - Operands[i] > INT_MAX))
					{
						ProgNExec({ ErrProg, MatErrProg, OveflowErrProg });
						break;
					}
					Rez -= Operands[i];
				}
				break;
			case 520: // DivInt
				Rez = int(Operands[0]);
				RezExtStack.push_back((int)Rez % int(Operands[1])); // ������� �� ������� ������ � ���� ����������� �����������
				for (int i = 1; i < Noperands; i++)
					if (int(Operands[i]) != 0)
					{
						if (!RezExtStack.size()) // ���������� ������� �� ������� � ����������� ������ ����������
							RezExtStack.push_back(int(Rez) % int(Operands[i]));
						else
							RezExtStack[0] = int(Rez) % int(Operands[i]);
						Rez /= int(Operands[i]);
					}
					else
					{
						Ready = 2; // ��� ������
						ProgExec(DivZeroErrProg);
						ProgExec(MatErrProg);// ������ �������������� ��������
						ProgExec(ErrProg); // ������� �� ����
						break;
					}
				break;
			case 522:	// Rem1 ����� �� �������� ����������� ������� �� �������������� �������
			case 523:	// Rem2 ������� �� �������������� �������

				if (MK == 522)
				{
					if (Operands.size() == 0)
					{
						Operands.push_back(Load.toInt());
						FOperands.push_back(true);
						OperandsCounter = 1;
						Ready = 2;
						break;
					}
					Operands[0] = Load.toInt();
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
					Operands[1] = Load.toInt();
					if (FOperands[1] == false)
						OperandsCounter++;
					FOperands[1] = true;
				}
				if (OperandsCounter == 2)
				{
					if(int(Operands[1]) != 0)
					{
						ProgNExec({ DivZeroErrProg, MatErrProg, ErrProg });
						Ready = 2;
						break;
					}
					//ProgExec(PreRezProg);// ��������� ����� ���������� ����������
					Rez = int(Operands[0]) % int(Operands[1]);
					//RezExec();
				}
				break;
			case 540: // Pow1 ������� (���������)
			{
			//	ProgExec(PreRezProg);// ��������� ����� ���������� ����������
				long int t= pow(Operands[0], Operands[1]);
				if (abs(t) > INT_MAX)
				{
					ProgNExec({ ErrProg, MatErrProg, OveflowErrProg });
					Ready = 2;
					break;
				}
				Rez = t;
				break;
			}
			}
			// --------------------------
			if(Ready==1)RezExec(); // �������� ��� ��������� ����������
		}
		break;
	// �������������� ��������
	case 526: // Sqr
	case 531: // Abs ������ �����
	case 532: // Not ���������� ���
	case 533: // NotBit ��������� ���������� ���
	case 535: // SignReverse ���������� �����
		if (WrongFormatCheck(Load)) break;
		ProgExec(PreRezProg);// ��������� ����� ���������� ����������
		Rez = Load.toInt(Rez); // ��������� � ����������� ��������, ���� �������� �������, �� ��������� Rez
		Operands.clear();
		FOperands.clear();
		Operands.push_back(Rez);
		FOperands.push_back(true);
		switch (MK)
		{
		case 526: // Sqr ������� �����
			Rez *= Rez;
			break;
		case 531: // ABS ������ �����
			Rez = abs(Rez);
			break;
		case 532: // Not ���������� ���
			Rez = !(bool(Rez));
			break;
		case 533: // NotBit ��������� ���������� ���
			Rez = ~(unsigned int)Rez;
			break;
		case 535: // SignReverse ���������� ����� �����
			Rez = -Rez;
			break;
		}
		RezExec(); // �������� ��� ��������� ����������
		break;
	default:
		CommonMk(MK, Load, Sender);
		break;
	}
	if (PostfixProg != nullptr) ProgExec(PostfixProg); // ������ ��������������� ���������
}

void StreamIntALU::RezExec(bool RezExec) // ���������� ����������� ��� ��������� ����������
{
	if (Ready == 2) return;
	Ready = 1;
	if (!OutRezBlock || RezExec)//���� �� ������������� �������� �� � ������������ ����������
	{
		for (int i = 0; i < ReceiverMk.size(); i++) // �������� �� � ������������
			MkExec(ReceiverMk[i], { Cint, &Rez }, ReceiverContexts[i]);
		for (auto& i : OutVars) // �������� ��������� � �������� ����������
			i.Write(Rez);
	}
	ProgExec(RezProg);
	if (Rez == 0) ProgExec(ZProg);
	if (Rez != 0) ProgExec(NZProg);
	if (Rez >= 0) ProgExec(BZProg);
	if (Rez <= 0) ProgExec(LZProg);
	if (Rez > 0) ProgExec(BProg);
	if (Rez < 0) ProgExec(LProg);
}

void StreamIntALU::OperandsClear(long int MK) // ����� ��������� ��� ������ ���������� ����� ��������
{
	//	if (Ready || OpCode != MK && OpCode != MK - 1) 
	{
		OpCode = MK;
		Ready = 0;
		Operands.clear();
		FOperands.clear();
		RezExtStack.clear();
		OperandsCounter = 0;
		if(EarlyCalculi)
			switch (MK)
			{
			case 500: // Add
			case 501: // AddSqr
			case 600: // Or
			case 610: // OrBit
				Rez = 0;
				break;
			case 510: // Mul
			case 601: // And
			case 602: // Xor
			case 611: // AndBit
			case 612: // XorBit
				Rez = 1;
				break;
			}
	}
}

bool StreamIntALU::WrongFormatCheck(LoadPoint Load) // �������� ������� ������� ������ (���������� true, ���� ������������ ������)
{
	if (!Load.isEmpty() && !Load.isDigit()) // ������ ������� ��������
	{
		Ready = 2; // ��� ������
		ProgExec(ErrProg); // ������������ ������ ��������
		ProgExec(WrongFormatErrProg);
		return true;
	}
	return false;
}

StreamIntALU::StreamIntALU(void* Dev1) // ���������� �����������
{
	StreamIntALU* Dev = (StreamIntALU*)Dev1;
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
	MatErrProg = Dev->MatErrProg; // ��������� ��������� ������ �������������� ��������
	NoOperandErrProg = NoOperandErrProg;// ������ ��� ��������
	OpIndErrProg = Dev->OpIndErrProg;// ������ ������� ��������
	OperationErrProg = Dev->OperationErrProg; // ������ ��������
	RezStackIsEmpyProg = Dev->RezStackIsEmpyProg; // ������ ��� ������� ���������� �� ������� ����� �����������
	RezExtStackIsEmpyProg = Dev->RezExtStackIsEmpyProg; // ������ ��� ������� ���������� �� ������� ����� ������������ �����������
	OperetionProg = Dev->OperetionProg;// ��������� ��� ���������� ����������� ��������
	RezProg = Dev->RezProg; // ���������, ����������� ����� ���������� ����������
	PreRezProg = Dev->PreRezProg; // ���������, ����������� ����� ���������� ����������
	OpCode = Dev->OpCode;
	EarlyCalculi = Dev->EarlyCalculi;
}

FU* StreamIntALU::Copy() // ��������� ����������� ��
{
	return new StreamIntALU(Bus, this);
}

FU* StreamIntALU::TypeCopy() // ������� �� ������ �� ���� (�� ������� ��������
{
	return new StreamIntALU(Bus, nullptr);
}

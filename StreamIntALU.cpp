#include "StreamIntALU.h"

void StreamIntALU::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	if (!Active && MK < 900) return; //��� ���������� ����� ���������� ����������� ����� ��
	int MKinitial = MK;
	MK %= FUMkRange;
	switch (MK)
	{
	case 0: //Reset
		Rez = 0;
		OperandsCounter = 0;
		Ready = 0;
		RezStack.clear();
		RezExtStack.clear();
		ReseiverMk.clear();
		ReseiverContext.clear();
		break;
	case 1: // Set ���������� ��������� ����������
		Rez = Load.toInt();
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
		int t = OperandsCounter;
		MkExec(Load, { Cint, &t });
	}
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
			double temp = RezStack.back();
			RezStack.pop_back();
			MkExec(Load, { Cdouble, &temp });
		}
	}
	break;
	case 93: // StackCounterOut ������ ���������� ��������� � �����
		Load.Write((int)RezStack.size());
		break;
	case 94: // StackCounterOut ������ �� � ����������� ��������� � �����
	{
		int t = RezStack.size();
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
			MkExec(Load, { Cdouble, &temp });
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
			double t = RezExtStack.back();
			MkExec(Load, { Cdouble,&t });
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
	case 105: // RezExtStackIsEmpyProgSet ���������� ��������� ��� ������ ������ �� ������� ����� ������������ ����������
		RezExtStackIsEmpyProg = Load.Point;
		break;

	case 150: // NOperandSet ���������� ���������� ��������� (�� ��������� 2)
		OperandsCounter = Load.toInt();
		break;
	case 160: // ReceiverReset ����� ��������� ����������� ����������
		ReseiverMk.clear();
		ReseiverContext.clear();
		break;
	case 161: // ReceiverSet ���������� ������ �� ��������� ���������� (��������������� ����� ���������� ��)
		ReseiverContext.push_back((FU*)Load.Point);
		break;
	case 162: // ReceiverMkSet ���������� �� ��� ��������� ���������� 
		if (ReseiverMk.size() == ReseiverContext.size())
			ReseiverContext.push_back(nullptr);
		ReseiverMk.push_back(Load.toInt());
		break;
	case 190: // RezProgSet ���������� ������ �� ������������, ����������� ��� ��������� ����������   
		RezProg = Load.Point;
		break;
	case 191: //PreRezProgSet ���������, ����������� ����� ���������� ����������


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
			double temp = Operands[(MK - 200) / 5];
			MkExec(Load, { Cdouble, &temp });
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
			int t = (MK - 200) / 5;
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
		Operands[OpInd] = Load.toDouble();
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
		double temp = Operands[OpInd];
		MkExec(Load, { Cdouble, &temp });
		break;
	}
	case 255: // OperandAdd �������� ������� ��� ����������� ��
		Operands.push_back(Load.toDouble());
		FOperands.push_back(true);
		OperandsCounter++;
		break;
	case 260: // OperationProgSet ���������� ����������� ��������    
		OperetionProg = Load.Point;
		break;
	case 280: // ZProgSet ���������� ��������� ��� == 
		ZProg = Load.Point;
		break;
	case 281: // NZProgSet ���������� ��������� ��� !=
		BZProg = Load.Point;
		break;
	case 282: // LProgSet   ���������� ��������� ��� <
		LProg = Load.Point;
		break;
	case 283: // BProgSet   ���������� ��������� ��� >
		BProg = Load.Point;
		break;
	case 284: // LZProgSet   ���������� ��������� ��� <=
		LZProg = Load.Point;
		break;
	case 285: // BZProgSet   ���������� ��������� ��� >=
		BZProg = Load.Point;
		break;
	case 290: // ZExec   ��������� ��������� ��� ==
		if (Rez == 0)
			ProgExec(Load);
		break;
	case 291: // LExec    ��������� ��������� ��� <
		if (Rez < 0)
			ProgExec(Load);
		break;
	case 292: // BExec    ��������� ��������� ��� >
		if (Rez > 0)
			ProgExec(Load);
		break;
	case 293: // LZExec    ��������� ��������� ��� <=
		if (Rez <= 0)
			ProgExec(Load);
		break;
	case 294: // BZExec    ��������� ��������� ��� >=
		if (Rez >= 0)
			ProgExec(Load);
		break;
	case 296: // NZExec    ��������� ��������� ��� !=
		if (Rez != 0)
			ProgExec(Load);
		break;
	case 300: // ZFOut ������ ���� ����
		Load.Write(Rez == 0);
		break;
	case 301: // ZFOutMk ������ �� � ������ ����
	{
		bool temp = Rez == 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 305: // BOut  ������ ���� ������
		Load.Write(Rez > 0);
		break;
	case 306: // BOutMk ������ �� � ������ ������
	{
		bool temp = Rez > 0;
		MkExec(Load, { Cbool, &temp });
	}
	break;
	case 310: // LOut  ������ ���� ������
		Load.Write(Rez < 0);
		break;
	case 311: // LOutMk BOutMk ������ �� � ������ ������
	{
		bool temp = Rez < 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 315: // BZOut ������ ���� ������ ��� ����
		Load.Write(Rez >= 0);
		break;
	case 316: // BZOutMk BOutMk ������ �� � ������ ������ ��� ����
	{
		bool temp = Rez >= 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 320: // LZOut ������ ���� ������ ��� ����
		Load.Write(Rez <= 0);
		break;
	case 321: // LZOutMk BOutMk ������ �� � ������ ������ ��� ����
	{
		bool temp = Rez <= 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	case 325: // NZOut  ������ ���� �� ����
		Load.Write(Rez != 0);
		break;
	case 326: // NZOutMk BOutMk ������ �� � ������ �� ����   
	{
		bool temp = Rez != 0;
		MkExec(Load, { Cbool, &temp });
		break;
	}
	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}

void StreamIntALU::RezExec() // ���������� ����������� ��� ��������� ����������
{
	if (Ready == 2) return;
	Ready = 1;
	if (!OutRezBlock)//���� �� ������������� �������� �� � ������������ ����������
		for (int i = 0; i < ReseiverMk.size(); i++) { // �������� �� � ������������
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

void StreamIntALU::OperandsClear(int MK) // ����� ��������� ��� ������ ���������� ����� ��������
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

void StreamIntALUManager::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	if (!Active && MK < 900) return; //��� ���������� ����� ���������� ����������� ����� ��
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

#include "stdafx.h"
#include "EnumMk.h"
#include "AluGeneral.h"
#include <cmath>

void ALUGeneral::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case E_MK::RESET : // Reset
		ThreadStack.clear();
		ThreadStack.push_back({});
//		ThreadStack.back().Parent = this;
		break;
	case E_MK::PROG:
	case E_MK::PROG_CYCLE:
	case E_MK::PROG_POST:
	case E_MK::PROG_BREAK:
	case E_MK::YES_PROG:
	case E_MK::YES_PROG_CYCLE:
	case E_MK::YES_PROG_POST:
	case E_MK::YES_PROG_BREAK:
	case E_MK::NO_PROG:
	case E_MK::NO_PROG_CYCLE:
	case E_MK::NO_PROG_POST:
	case E_MK::NO_PROG_BREAK:
	{	bool f;
	f = MK <= 4;
	f = f || ThreadStack.back().AluStack.back().getLogic() && (MK == 5 || MK == 6 || MK == 7 || MK == 8);
	f = f || !ThreadStack.back().AluStack.back().getLogic() && (MK == 9 || MK == 10 || MK == 11 || MK == 12);
	if (f)
		//		if(Load.Type>>2!=DIC) // �������� ��� ������ �� ��
	{/* ��������� �� ������ ���� */
	//	ThreadStack.push_back({ {}, (ip*)Load.Point,nullptr });
		ThreadStack.back().AluStack.push_back({ Tint ,(void*) new int(0), 0, 0,"",this});
		ThreadStack.back().CycleFlag = (MK - 2) % 4 == 0 || (MK - 3) % 4 == 0; // Установить флаг цикла
		ThreadStack.back().PostFlag = (MK - 3) % 4 == 0; // Установика флаг цикла с постусловием

		do
			ProgExec(Load.Point);
		while (breakCounter == 0 && ThreadStack.back().CycleFlag);
		if (breakCounter > 0)
		{
			if (ThreadStack.back().CycleFlag) breakCounter--;
			if (breakCounter) ProgStop = true;
		}
		ThreadStack.pop_back();
		if (MK % 4 == 0)
		{
			ProgStop = true; breakCounter = 1;
		} // Выход из программы поисле отработки подпрограммы
	}
	break;
	}
	case E_MK::BREAK:
	{
		ProgStop = true;
		breakCounter = Load.ToInt();
		if (breakCounter == 0) breakCounter = 1;
		break;
	}
	case E_MK::CONTINUE:
		ProgStop = true;
		break;
	case E_MK::CALC:
		if (!ThreadStack.back().PostFlag)
		{
			ProgExec(Load.Point);
		}
		else
		{
			ThreadStack.back().AluStack.back().Clear();
			ThreadStack.back().AluStack.back().accumulatorOld.Type = Cbool;
			ThreadStack.back().AluStack.back().accumulatorOld.Point = new bool(true);
			ThreadStack.back().PostFlag = false;
		}
		for (auto i : ThreadStack.back().Out) // Рассылка результатов вычислений
			ThreadStack.back().AluStack.back().Out(i);
		for (auto i : ThreadStack.back().MkOut)
			MkExec(i, ThreadStack.back().AluStack.back().get());

		break;
	case E_MK::DEEP_OUT:
		*(int*)Load.Point= ThreadStack.size();
		break;
	case E_MK::DEEP_OUT_MK:
		MkExec(*(int*)Load.Point, { Cint,(void*)new int((int)ThreadStack.size()) });
		break;

	case E_MK::GO:
	case E_MK::YES_GO:
	case E_MK::NO_GO:
		if (MK == 40 || MK == 41 && ThreadStack.back().AluStack.back().getLogic()
			|| MK == 42 && !ThreadStack.back().AluStack.back().getLogic())
		{
			ProgExec(Load.Point);
			ProgStop = true;
		}
		break;

	case E_MK::OUT_SET:
		ThreadStack.back().Out.push_back(Load);
		break;
	case E_MK::OUT_MK_SET:
		if((Load.Type>>1)==Dint)
			ThreadStack.back().MkOut.push_back(*(int*)Load.Point);
		break;
	// Set + - * / 
	case E_MK::OUT:
		ThreadStack.back().AluStack.back().Out(Load);
		break;
	case E_MK::OUT_MK:
		MkExec(Load, ThreadStack.back().AluStack.back().get());
		break;
	case 100: 
		ThreadStack.back().AluStack.back().set({ 0,nullptr });//
		break;
	case E_MK::SUM: case E_MK::SUB: case E_MK::DIV: case E_MK::MULT:
	case E_MK::INC: case E_MK::DEC: case E_MK::SET: case E_MK::GET:
	case E_MK::OUT_A: case E_MK::MIN: case E_MK::MAX: case E_MK::COS:
	case E_MK::SIN: case E_MK::TAN: case E_MK::ACOS: case E_MK::ASIN:
	case E_MK::ATAN: case E_MK::MOD: case E_MK::SQRT: case E_MK::POW:
	case E_MK::ABS: case E_MK::CEIL: case E_MK::FLOOR: case E_MK::ROUND:
	case E_MK::LOG: case E_MK::RANDOM: case E_MK::GET_LOGIC: case E_MK::INV:
	case E_MK::AND: case E_MK::OR: case E_MK::DIV_INT: case E_MK::XOR:
	case E_MK::INV_BIT: case E_MK::OR_BIT: case E_MK::AND_BIT: case E_MK::MR_BIT:
	case E_MK::ML_BIT:
	case E_MK::EQ: case E_MK::NotEQ: case E_MK::Bigger: case E_MK::BiggerEQ:
	case E_MK::Smaller: case E_MK::SmallerEQ: case E_MK::Remainder: case E_MK::XOR_BIT:
	case E_MK::Compar3Way:
		if (!ThreadStack.size())
			ThreadStack.push_back({});
		if (ThreadStack.back().AluStack.size() == 0)
		{
			ThreadStack.back().AluStack.push_back({});
			ThreadStack.back().AluStack.back().Parent = this;
		}
		if ((Load.Type >> 1) != DIC) // Выполнение операции
			ThreadStack.back().AluStack.back().calc(MK, Load);
		else // АЛВ более высокого уровня 
		{
			ThreadStack.back().AluStack.push_back({ Tint ,(void*) new int, Load.Type, 0, "",this});
			ProgExec(Load.Point);
			ThreadStack.back().AluStack[ThreadStack.back()
				.AluStack.size()-2].calc(MK, ThreadStack.back().AluStack.back().accumulatorOld);
			ThreadStack.back().AluStack.pop_back();
		}
		break;
	case 109: // CycleLimitSet Установить лимит итераций цикла (чтобы не было зацикливания)
		ThreadStack.back().CycleLimit = Load.ToInt();
	case 110: //RangeStopSet Установить конечное значение диапазона range (может передаваться указатель на множество/массив для перебора range)
		ThreadStack.back().RangeStop = Load.ToInt();
		break;
	case 111: //RangeStepSet Установить шаг для диапазона range
		ThreadStack.back().RangeStep = Load.ToInt();
		break;
	case 112: //RangeProgExec Выполнить программу для цикла range
	{

		if(Load.Type>>1!=DIC)
			if (ThreadStack.back().MkStage == 0)
			{
				ThreadStack.back().RangeStop = Load.ToInt();
				ThreadStack.back().MkStage = 1;
			}
			else if(ThreadStack.back().MkStage == 1)
			{
				if (ThreadStack.back().AluStack.size() == 0)
				{
					ThreadStack.back().AluStack.push_back({});
					ThreadStack.back().AluStack.back().Parent = this;
				}
				ThreadStack.back().AluStack.back().set(Load);
				ThreadStack.back().MkStage = 2;
			}
			else
			{
				ThreadStack.back().RangeStep = Load.ToInt();
				ThreadStack.back().MkStage = 0;
			}
		else
		{ // Реализация цикла
			ThreadStack.back().MkStage = 0;
			for (int i=0;; i++,ThreadStack.back().AluStack.back().add({Tint,&ThreadStack.back().RangeStep}))
			{
				if (ThreadStack.back().CycleLimit != 0 && i > ThreadStack.back().CycleLimit) break;
				if (ThreadStack.back().RangeStep > 0)
				{
					if (ThreadStack.back().AluStack.back().accumulatorOld.ToInt() >= ThreadStack.back().RangeStop)
						break;
				}
				else
					if (ThreadStack.back().AluStack.back().accumulatorOld.ToInt() <= ThreadStack.back().RangeStop)
						break;

				ProgExec(Load.Point);
			}
		}
		break;
		case 115: // MaxProgSet
			if (Load.Type >> 1 == DIC)
				ThreadStack.back().MaxProg = Load.Point;
			break;
		case 117: //"MinProgSet
			if (Load.Type >> 1 == DIC)
				ThreadStack.back().MinProg = Load.Point;
		break;
	}
	case 130: //VarNew Создать и инициализировать новую переменную
		NewVar =Load.Clone();
		NewVar.VarTypeSet(true);
		break;
	case 131: // VarOut Выдать ссылку на новую переменную
		Load.Write(NewVar);
		break;
	case 132: // VarOutMk Выдать MK со ссылкой на новую переменную
		MkExec(Load, NewVar);
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
}
#include "stdafx.h"
#include "gauss.h"
#include "SchedulerEventser.h"
#include < cstdlib >

const int Key = 1, KeyToRight = 3, DownARight = 5, DownA = 7, Cof=9, Zero = 11, SumToLeft = 13, SumToLeftUp=15, Up=17;

void GaussPoint::ProgFU(int MK, LoadPoint Load)
{
	if (Modeling != nullptr && Modeling->ManualMode && Modeling->scheduler != nullptr && !Modeling->SchedulerFlag)
	{
		Modeling->SchedulerFlag = false;
		Modeling->qmk.push_back({ MK, Load });
		double SendAllTime = SendTime;
		((Scheduler*)(Modeling->scheduler))->Scheduling(this, CalcTime + SendAllTime);
		return;
	}
	bool SchedulerFlag = false;
	if (Modeling != nullptr && Modeling->ManualMode && Modeling->SchedulerFlag)
	{
		Modeling->SchedulerFlag = false;
		SchedulerFlag = true;
	}

	switch (MK)
	{
	case 1: // Key
		if (Neighbours[3] == nullptr)
			Neighbours[0]->ProgFU(SumToLeftUp, { Cdouble, new double(0)});
		else
		{
			if (Neighbours[0] != nullptr)
				Neighbours[0]->ProgFU(DownARight, { 0, nullptr });
			Neighbours[3]->ProgFU(Zero, { Cint,&a });
			Neighbours[3]->ProgFU(KeyToRight, { 0, nullptr });
		}
		break;
	case 3://	KeyToRight
		Neighbours[0]->ProgFU(Key, { 0, nullptr });
		break;
	case 5:	//	DownARight
		if(Neighbours[3]!=nullptr)
			Neighbours[3]->ProgFU(DownA, { Cint,&a });
		if (Neighbours[0] != nullptr)
			Neighbours[0]->ProgFU(DownARight, { 0, nullptr });
		break;
	case 7: // DownA
		UpBuf = Load.toDouble();
		if (Neighbours[3] != nullptr)
			Neighbours[3]->ProgFU(DownA, Load);
		break;
	case 9: //Cof
		a = a + UpBuf * Load.toDouble();
		if (Neighbours[0] != nullptr)
			Neighbours[0]->ProgFU(Cof, Load);
		break;
	case 11: // Zero
	{
		if (Neighbours[3] != nullptr)
			Neighbours[3]->ProgFU(Zero, Load);
		double t = -a / UpBuf;
		if (Neighbours[0] != nullptr)
			Neighbours[0]->ProgFU(Cof, { Cdouble, &t });
		break;
	}
	case 13: // SumToLeft
	{
		if (KeyF)
		{
			a = -Load.toDouble() / a; // Вычисление корня
			;// Отсылка результата Менеджеру
			cout << a << endl;
			if (Neighbours[1] != nullptr)
				Neighbours[1]->ProgFU(Up, { Cdouble, &a });
		}
		else
		{
			double t = -a + Load.toDouble();
			if (Neighbours[2] != nullptr)
				Neighbours[2]->ProgFU(SumToLeft, { Cdouble, &t });
		}
		break;
	}
	case 15: // 9.	SumToLeftUp
		if (Neighbours[2] != nullptr)
			Neighbours[2]->ProgFU(SumToLeft, { Cdouble, &a });
		if (Neighbours[1] != nullptr)
			Neighbours[1]->ProgFU(SumToLeftUp, { 0, nullptr });
		break;
	default:
		CommonMk(MK, Load);
		break;
	}

	if (SchedulerFlag)
		((Scheduler*)Modeling->scheduler)->CoreFree();
}

void Gauss::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case 0: // Reset
		break;
	case 1: // ReadAndStart
	{
		if (Load.Type >> 1 == Dstring) // Данные из файла
		{
			ifstream f_in(Load.toStr());
			f_in >> Size;
			Field.resize(Size);
			for (auto& iu : Field)
			{
				iu.resize(Size + 1);
				//iu[N].FreeF = true;
			}
			for (int i = 0; i < Size; i++)
				for (int j = 0; j <= Size; j++)
					f_in >> Field[i][j].a;
		}
		else
		{ // Данные, сгенерированные случайно
			if (Load.Type >> 1 == Dint)
				Size = Load.toInt();
			Field.resize(Size);
			for (auto& iu : Field)
			{
				iu.resize(Size + 1);
			}
			for (int i = 0; i < Size; i++)
				for (int j = 0; j <= Size; j++)
					Field[i][j].a=(Max-Min)*((double)rand()/ RAND_MAX) + Min;
		}
		// Прописать ссылки на соседей
		for (int i = 0; i < Size; i++)
			for (int j = 0; j <= Size; j++)
			{
				Field[i][j].Neighbours[0] = (j == Size) ? nullptr : &Field[i][j + 1];
				Field[i][j].Neighbours[1] = (i == 0) ? nullptr : &Field[i-1][j];
				Field[i][j].Neighbours[2] = (j == 0) ? nullptr : &Field[i][j - 1];
				Field[i][j].Neighbours[3] = (i==Size-1) ? nullptr : &Field[i+1][j];
				if (i == j)Field[i][j].KeyF = true;
			}
		Field[0][0].ProgFU(Key, {0 ,nullptr });
		break;
		case 5: // SizeSet Установить размерность матрицы
			Size = Load.toInt();
			break;
		case 10: //MaxSet Установить максимальную величину коэффициентов для случайной генерации
			Max = Load.toInt();
			break;
		case 15: //MinSet Установить мининмальную величину коэффициентов для случайной генерации
			Min = Load.toInt();
			break;
		case 100: // CalcTimeSet Установить время выполнения операции
			break;
		case 105: // SendTimeSet Установить время пересылки данных
			break;
	}

	default:
		CommonMk(MK, Load);
		break;
	}
}


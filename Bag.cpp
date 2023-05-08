#include "stdafx.h"
#include "Bag.h"
#include <fstream>
#include "SchedulerEventser.h"

void BagPoint::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	MK %= FUMkRange;
	if (Modeling != nullptr && Modeling->ManualMode && Modeling->scheduler != nullptr && !Modeling->SchedulerFlag)
	{
		Modeling->SchedulerFlag = false;
		Modeling->qmk.push_back({ MK, Load });
		double SendAllTime = SendTime*((Down != nullptr) + (Diagonal != nullptr) + (Next != nullptr));
  		((Scheduler*)(Modeling->scheduler))->Scheduling(this, CalcTime+SendAllTime);
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
	case 1: // Calc Вычислить значение
		// Для элементов первой строки, где пока нет ни p_Diag, ни p_Up
		if ((p_Diag < 0) && (p_Up < 0))
		{
			if (w_i >= w_a)
				p = Load.toDouble();
			else
				p = 0;
		}
		else // Остальные случаи
		{
			if ((w_i > w_a) && (p_Diag >= 0))
			{
				p = max(p_Up, p_Diag + Load.toDouble());
			}
			else if ((w_i == w_a) && (p_Diag < 0))
			{
				p = max(p_Up, Load.toDouble());
			}
			else
				p = p_Up;
		}

		if (Down) Down->ProgFU(2, { Cdouble,&p }); // Передача вниз
		if (Diagonal) Diagonal->ProgFU(3, { Cdouble,&p }); // Передача по диагонали
		if (Next) Next->ProgFU(1, Load); // Передача вправо
		break;
	case 2: // Up Принять значение сверху
		p_Up = Load.toDouble();
		break;
	case 3: // Diag Принять значение по диагонали
		p_Diag = Load.toDouble();
		break;
	}
	if (SchedulerFlag)
		((Scheduler*)Modeling->scheduler)->CoreFree();
}

void Bag::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	if (Modeling != nullptr && Modeling->ManualMode && Modeling->scheduler != nullptr && !Modeling->SchedulerFlag)
	{
		Modeling->SchedulerFlag = false;
		Modeling->qmk.push_back({ MK, Load, Sender });
		((Scheduler*)(Modeling->scheduler))->Scheduling(this, SendTime);
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
	case 0: // Reset
		for (auto& i : Field)
			i.resize(0);
		Field.resize(0);
		break;
	case 1: // Start
	{ 
		for (auto& i : Field) // Очистка старых данных
			i.resize(0);
		Field.resize(0);
		W.clear(); P.clear();

		if (!Load.Point)
		{ // Генерация случайной сетки
			W.push_back(0); // Мнимый элемент
			P.push_back(0); // Мнимый элемент
			for (int i = 0; i < NItem; i++)
				{W.push_back(rand() % WMaxOfItem+1); P.push_back(rand() % int(PMaxOfItem)+1);}
			for (int k = 0; k <= NItem; k++)
			{
				Field.push_back({ {} });
				for (int i = 0; i < WMax; i++)
					Field.back().push_back({});
			}
		}
		else
		{
			string str = Load.toStr();
			ifstream in(str);
			if (!in)
				cout << "File error\n";
			in >> WMax;
			Field.push_back({ {} });
			for (int i = 0; i < WMax; i++)
				Field.back().push_back({});
			W.push_back(0); // Мнимый элемент
			P.push_back(0); // Мнимый элемент
			while (!in.eof())
			{
				int w;
				double p;
				in >> w >> p;
				W.push_back(w); P.push_back(p);
				Field.push_back(Field[0]); // Генерация вычислительной сетки
			}
			in.close();
		}

		// Определяем текущий объем и объем предмета (const) для каждого элемента матрицы
		// Чтобы не запутаться, вынес это в отдельный цикл
		for (int i = 1; i < Field.size(); i++)
			for (int j = 1; j < Field[0].size(); j++)
			{
				Field[i][j].w_a = W[i];
				Field[i][j].w_i = j + 0.0; // int to double
				Field[i][j].CalcTime = CalcTime;
				Field[i][j].SendTime = SendTime;
				if (Modeling != nullptr)
				{
					Field[i][j].Modeling = new FUModeling();
					Field[i][j].Modeling->scheduler = (FU*)Modeling->scheduler;
					Field[i][j].Modeling->ManualMode = true;
				}
			}

		// Ссылки
		for (int i = 1; i < Field.size(); i++)
			for (int j = 1; j < Field[0].size()-1; j++)
			{
				Field[i][j].Next = &Field[i][j+1];
				if (i < Field.size() - 1) //Если не последняя строка
				{
					Field[i][j].Down = &Field[i+1][j];

					if (j == Field[0].size() - 2) // Чтобы установить ссылку Down для элементов последнего столбца
					{
						Field[i][j+1].Down = &Field[i + 1][j+1];
					}

					if (j + W[i+1] <= WMax) // Диагональная ссылка
						Field[i][j].Diagonal = &Field[i+1][j+W[i+1]];
				}
			}
		// Запуск вычислений
		for (int i = 1; i < P.size(); i++)
			Field[i][1].ProgFU(1, { Cdouble, &P[i] }); // Запуск вычислений по каждой линии
		break;
	}
	case 10: // RezOut Выдать результат
	case 11: // RezOutMk Выдать МК с результатом
	{
		double Rez = 0;
		for (auto i : Field[Field.size() - 1])
			if (i.p > 0)
				Rez = i.p;
		if (MK == 10)
			Load.Write(Rez);
		else
			MkExec(Load, { Cdouble,&Rez });
	break;
	}
	case 90: //WMaxSet Установить максимальный объем рюкзака
		WMax = Load.toInt();
		break;
	case 91: // WMaxOfItem Установить максимальный размер одной вещи
		WMaxOfItem = Load.toInt();
		break;
	case 92: // PMaxOfItem Установить максимальный размер одной вещи
		PMaxOfItem = Load.toDouble();
		break;
	case 93: //NItemSet Установить количество предметов для укладки и начать вычисление
		NItem = Load.toInt();
		break;
	case 94: //NItemAdd
		NItem += Load.toInt();
		break;
	case 95: //WMaxAdd
		WMax += Load.toInt();
		break;

	case 100: // CalcTimeSet Установить время вычисления результата
		CalcTime = Load.toDouble();
		break;
	case 101: // SendTimeSet Установить время пересылки данных
		SendTime = Load.toDouble();
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
	if (SchedulerFlag)
		((Scheduler*)Modeling->scheduler)->CoreFree();
}
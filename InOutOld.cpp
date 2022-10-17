#include "stdafx.h"
#include "InOut.h"
#include <algorithm>

using namespace std;

void InOut::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case 0: // Reset
		break;
	case 1: //
		break;
	default:
		CommonMk(MK, Load);
	}
}

void AutomatManager::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case 0: // Reset
		ReceiverMk = -1;
		Template->clear();
		delete Template;
		Template = nullptr; // доработать удаление уже имеющейся ИК
		Var.clear();
		IPout = { 0, {0,new double()} };
		IPoutPrev = { 0, {0,new double()} };
		StageInProg.clear();
		StageOutProg.clear();
		StageProgMk = -1;
		break;

	case 3: // RecieverMKSet
		if (TemplAutoClear) // Автоматический сброс ИК сигналов
			Var.clear();
		if (Load.Point == nullptr || Load.Type >> 1 != Dint)
			ReceiverMk = -1;
		else
		{
//			if (StageOutProg.find(ReceiverMk) != StageOutProg.end() && ReceiverMk>=0)
//				ProgExec(StageOutProg[ReceiverMk]);

			ReceiverMk = Load.toInt();
			// Запуск программы для состояния автомата
//			if (StageInProg.find(ReceiverMk) != StageInProg.end())
//				ProgExec(StageInProg[ReceiverMk]);
		}
//		if (TemplAutoClear) ((IC_type)Template)->clear();
		break;
	case 4: // TemplAutoClearSet Установить флаг автоматического сброса шаблона поиска при установке МК для получателя нового сигнала
		TemplAutoClear = Load.toBool(true);
		break;
	case 5: // ReceiverSet Установить ссылку на ФУ-приемника
		Receiver = Load.Point;
		break;
/*
	case 5: // StageProgInAdd Добавить входную программу для состояния
	case 6: // StageProgOutAdd Добавить выходную программу для состояния
		if (StageProgMk < 0){
			if (Load.Type >> 1 == Dint)
				StageProgMk = *(int*)Load.Point;
			}
		else
			{
			if (Load.Type >>= DIC)
				if(MK==5)
					StageInProg[StageProgMk] = (void*)Load.Point;
				else
					StageOutProg[StageProgMk] = (void*)Load.Point;
			StageProgMk = -1;
			}
		break;
	case 7: // StageProgInClear Очистить список входных программ состояний
		StageInProg.clear();
		break;
	case 8: // StageProgOutClear Очистить список выходных программ состояний
		StageOutProg.clear();
		break;
*/
	case 10: // TemplSet Установить шаблон для сигналов (ИК копируется)
		if (Load.Point == nullptr || Load.isIC())
			Template = nullptr;
		else
			Template = (IC_type) ICCopy(Load).Point;
		break;
	case 11: // TemplClear Очистить ИК шаблона
		(*(IC_type)Template).clear();
		break;
	case 12: //TemplOut Выдать шаблон
		Load.Write(Template);
		break;
	case 13: //TemplOutMk выдать МК с указателем на шаблон ИК
		MkExec(Load, {CIC, Template });
		break;

	case 20: // SignalSet Установить сигнал
	{
		ip* temmplUk;
		if (Load.isIP()) // Считывание из ИП
		{
			temmplUk = AtrFind(Template, ((ip*)Load.Point)->atr);
			if (temmplUk != nullptr)
				temmplUk->Load.WriteFromLoad(((ip*)Load.Point)->Load);
		}
		else if (Load.isIC())
		{
			temmplUk = AtrFind(Template, ((IC_type)Load.Point)->begin()->atr);
			if (temmplUk != nullptr)
				temmplUk->Load.WriteFromLoad(((IC_type)Load.Point)->begin()->Load);
		}
		// Выдача ИК сигналов на ФУ-приемник
		if (Receiver == nullptr)
			MkExec(ReceiverMk, { CIC, Template });
		else
			MkExec(ReceiverMk, { CIC, Template }, (FU*)Receiver);
		break;
	}
/*
	case 20: // VarSet Установить сигнал (сигнал приходит от объекта управления)
		if (Load.Point == nullptr || Load.Type >> 1 != DIC)
			Var = nullptr;
		else
			Var = Load.Point;
		break;
	case 21: // VarOut
		if (Load.Point != nullptr)
			if (Load.Type == DIC)
				Load.Point = (void*)Var;
		break;
	case 22: // VarOutMK
		if (Load.Point != nullptr || Load.Type >> 1 == DIC)
			MkExec(Load, { DIC,&Var });
		break;
	case 30: // SignalOut
		if (Load.Point != nullptr)
			if (Load.Type == CIP)
				Load.Point = (void*)&IPout; //??
		break;
	case 31: // SignalOutMK
		if (Load.Point != nullptr || Load.Type >> 1 == Dint)
			MkExec(Load, { CIP,&IPout });
		break;
	case 33: // SignalAtrOut
		if (Load.Point != nullptr)
			if (Load.Type == Dint)
				*(int*)Load.Point = IPout.atr;
		break;
	case 34: // SignalAtrOutMK
		if (Load.Point != nullptr || Load.Type >> 1 == Dint)
			MkExec(*(int*)Load.Point, { Cint,&IPout.atr });
		break;
	case 35: // SignalLoadOut
		if (Load.Point != nullptr)
			if (Load.Type == Dint)
				*(LoadPoint*)Load.Point = IPout.Load;

		break;
	case 36: // SignalLoadOutMK
		if (Load.Point != nullptr || Load.Type >> 1 == Dint)
			MkExec(*(int*)Load.Point, IPout.Load);
		break;
	case 37: // SignalAtrSet
		if (Load.Point != nullptr || Load.Type >> 1 == Dint)
			IPout.atr = Load.toInt(0);
		break;
	case 38: // SignalLoadSet
		if (Load.Point != nullptr)
			IPout.Load = Load;
		break;
	case 50: // SignalSet
	case 51: // Send
	{
		if (Load.Point != nullptr)
			if (Load.Type >> 1 == DIC && (*((IC_type)Load.Point)).size() > 0)
				IPout = (*((IC_type)Load.Point))[0];
		if (Load.Type >> 1 == DIP)
			IPout = *((ip*)Load.Point);
		// Проверить, есть ли атрибут в ИК переменных
		auto ukVar = AtrFind(Var, IPout.atr);
		if (!ukVar)
			;// Ошибка
		else
		{
			// Добавить в ИК шаблона
			auto ukTempl = AtrFind(Template, IPout.atr);
			if (!ukTempl)
				(*(IC_type)Template).push_back(IPout);
			else
				ukTempl->Load = IPout.Load;
		}
		if (MK == 51) // Отправить сигнал на ФУ-состояние
			if (ReceiverMk >= 0)
				MkExec(ReceiverMk, { CIC,Template });
		break;
	}
*/
	default:
		CommonMk(MK, Load);
	}
}

void border::Run(LoadPoint Load) {
	if (!borders.size())return; // Если нет границ
	double val = Load.toDouble();
	if (!borders.size()) return;
	if (Start) // Если первое данное
	{
		OldVal = val;
		Start = false;
		return;
	}
	auto uk = borders.begin();
	for (; uk != borders.end() && *uk<=val, val; uk++);
	if (uk == borders.end())
	{
		if(OldVal<=borders.back())
			Parent.ProgExec(UpProg.back());
	}
	else
	if(OldVal>=*(uk+1))
			Parent.ProgExec(DownProg[distance(borders.begin(),uk)+1]);
		else
			if (uk!=borders.begin() && OldVal <= *(uk-1))
				Parent.ProgExec(UpProg[distance(borders.begin(), uk)-1]);
	OldVal = val;

};
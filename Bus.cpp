#include "stdafx.h"
#include "consts.h"
#include "Bus.h"
#include "FUini.h"
#include <string>
#include <map>
#include <algorithm>
#include "IndFileConvertOld.h"

using namespace std;

void BusFU::FUTypesIni()
{
	FUs.push_back(nullptr);
	FUs.push_back(nullptr);
	FUTypeCorrect = 0;
}

void BusFU::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	ICVect* ipVect; // Указатель на вектор ИК
	if (MK >= FUMkRange*2)
	{
		int FU_num = MK / FUMkRange;
		if(FU_num<FUs.size())
			FUs[FU_num]->ProgFU(MK%FUMkRange, Load, this);
	}
	else
		switch (MK%FUMkRange)
		{
		case 0: // Сброс
	//		for (int i = 2; i < FUs.size(); i++)
		//		delete FUs[i];
		//	FUs.clear();
		//	FUs.push_back(this);
		//	FUs.push_back(this); // Первой ФУ - это сам Bus
			break;
		case 1: // MakeFU Создать ФУ
			FUs.push_back(FUTypes[Load.toInt() - FUTypeCorrect](this, FUTempl));
			FUs.back()->FUMkGloabalRange = FUMkRange * (FUs.size() - 1); // Установить начало глобального диапазона МК
			break;
		case 5: // ProgExec Выполнить программу из ИК
			ProgExec((vector<ip>*)Load.Point);
			break;
		case 10: // FileOldProgExec Выполнить программу из файла
			ipVect = ConvIndOld(*(string*)Load.Point);
			if(ipVect->size()>0) ProgExec( (*ipVect)[0], 0, this,nullptr);
			break;
		case 20: // NFUOut

			Load.Write((int)FUs.size());
			break;
		case 21: // NFUOutMk
		{
			int t= FUs.size();
			MkExec(Load, {Cint,&t});
		}
		case 22: // LastFuContextOut 
			Load.Write(FUs.back());
			break;
		case 23: // LastFuContextOutMk Выдать МК с контекстом последнего созданного ФУ
			MkExec(Load, {TFU, FUs.back()});
			break;
		case 25: // LastFUNameSet Установить имя последнего созданного ФУ
			if (FUs.size() > 1)
				FUs.back()->FUName = Load.toStr();
			break;
		case 40: // LastFUMkRangeOut Выдать начало МК-диапазона последнего ФУ
			Load.Write(FUMkRange*(FUs.size() - 1));
			break;
		case 41: // LastFUMkRangeOutMk Выдать МК с началом МК-диапазона последнего ФУ 
			{
				int t = FUMkRange * (FUs.size() - 1);
				MkExec(Load, { Cint, &t });
			}
			break;
		case 100: // MkExec Выполнить одму МК (в нагрузке ссылка на ИП)
			if (Load.Point!=nullptr && Load.isIP())
				ProgFU(((ip*)Load.Point)->atr, ((ip*)Load.Point)->Load);
			break;
		case 105: //InterpretatorModeSet Установить режим интерпретатора
			InterpretatorMode = Load.toBool(true);
			break;
		case 106: // InterpretatorProgExec Выполнить программу, если режим интерпретатора
			if (InterpretatorMode)
				ProgExec(Load);
			break;
		case 107: // CompilyatorProgExec Выполнить программу, если режим компилятора
			if (!InterpretatorMode)
				ProgExec(Load);
			break;
		case 155:// FUTypeCorrectSet Установить коррекцию номера типа ФУ (для переноса ОА-программы на другую ОА-платформу)
			FUTypeCorrect = *(int *)Load.Point;
			break;
		default:
			CommonMk(MK, Load);
			break;
		}
}
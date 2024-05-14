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

void BusFU::ProgFU(long int MK, LoadPoint Load, FU* Sender)
{
	ICVect* ipVect; // Указатель на вектор ИК
	if (MK >= FUMkRange*2)
	{
		int FU_num = MK / FUMkRange;
		if(FU_num<FUs.size())
			FUs[FU_num]->ProgFU(MK, Load, this);
	}
	else
		switch (MK%FUMkRange)
		{
		case 0: // Сброс
	    	for (int i = 1; i < FUs.size(); i++)
				delete FUs[i];
			FUs.clear();
			FUs.push_back(this);
			FUs.push_back(this); // Первой ФУ - это сам Bus
			break;
		case 1: // MakeFU Создать ФУ
			FUs.push_back(FUTypes.MakeFu(Load.toInt(),this, FUTempl));
			FUs.back()->FUMkGlobalAdr = FUMkRange * (FUs.size() - 1); // Установить начало глобального диапазона МК
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
			long int t= FUs.size();
			MkExec(Load, {Cint,&t});
			break;
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
				long int t = FUMkRange * (FUs.size() - 1);
				MkExec(Load, { Cint, &t });
			}
			break;

		case 45: // FuContextOut Выдать контекст ФУ по его МК
			if (Ind >= FUs.size() || Ind < 0) break;
			Load.Write(FUs[Load.toInt() / FUMkRange]);
			break;
		case 46: // FuContextOutMk Выдать МК с контекстом ФУ по его МК
			if (Ind >= FUs.size() || Ind<0) break;
			MkExec(Load, { TFU, &FUs[Ind] });
			break;
		case 47: // FuContestFormInd1ToInd2OutMk Последовательно выдать МК с контекстами ФУ с индекса 1 до индекса 2
			if (Ind >= FUs.size() || Ind < 0 || Ind2 >= FUs.size() || Ind2 < 0) break;
			for (int i = Ind; i <= Ind2; i++)
				MkExec(Load, { TFU,&FUs[i] });
			break;
		case 48: // FuContestFormInd1ToEndOutMk Последовательно выдать МК с контекстами ФУ с индекса 1 до конца списка ФУ
			if (Ind >= FUs.size() || Ind < 0) break;
			for (int i = Ind; i <= FUs.size(); i++)
				MkExec(Load, { TFU,&FUs[i] });
			break;
		case 50: // IndSet Установить индекс ФУ
			Ind = Load.toInt(0);
			break;
		case 51: // Ind2Set Установить второй индекс ФУ
			Ind2 = Load.toInt(0);
			break;
		case 52: // IndByMkSet Установить индекс ФУ по МК
			Ind = Load.toInt(0) / FUMkRange;
			break;
		case 53: // Ind2ByMkSet Установить второй индекс ФУ по МК
			Ind2 = Load.toInt(0) / FUMkRange;
			break;
		case 54: // IndAdd Увеличить индекс ФУ
			Ind += Load.toInt(1);
			break;
		case 55: // Ind2Add Увеличить второй индекс ФУ
			Ind2 += Load.toInt(1);
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
			FUTypeCorrect = Load.toInt();
			break;
		default:
			CommonMk(MK, Load);
			break;
		}
}

FU* BusFU::Copy() // Программа копирования ФУ
{
	return new BusFU(Bus, this);
}

FU* BusFU::TypeCopy() // Создать ФУ такого же типа (не копируя контекст
{
	return new BusFU(Bus, nullptr);
}
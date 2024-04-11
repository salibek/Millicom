#include "StreamManager.h"

using namespace std;

void StreamManager::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	switch (MK)
	{
	case 0: //Reset
		for (auto& i : Field)
		{
			for (auto& j : i)
				free(j);
			i.clear();
		}
		Field.clear();
		DeviseCounter = 0;
		break;
	case 1: // FieldCreate Создавать поле АЛУ (на входе размерность, по умолчанию 1)
		Field.resize(Load.toInt(1));			
		break;
	case 2: // FieldCreateTempl Создавать поле АЛУ на основе эталона (на входе размерность, эталон находится в векторе поля АЛУ под индексом 0, в нулевой группе)
		if (!Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
	//	for (int i = 1; i < Field.size(); i++) // Убираются все ФУ, кроме первого
	//	{
	//		for(int j=0; j< Field[i].size();j++)
	//			free(Field[i][j]);
	//		Field[i].clear();
	//	}
		{
			int N = Load.toInt();
			for (int i = 1; i <= N; i++)
			{
				Field.push_back({});
				for (auto& j : Field.back())
					switch (Mode)
					{
					case 0: Field.back().push_back(new StreamFloatALU((FU*)j)); break;
					case 1: Field.back().push_back(new StreamIntALU((FU*)j)); break;
					}
			}
			// Коррекция адресов АЛУ
			// ...
		}
		break;
	case 3: // FieldClear Очистить поле АЛУ
		for (int i = 0; i < Field.size(); i++) // Убираются все ФУ, кроме первого
		{
			for (int j = 0; j < Field[i].size(); j++)
				free(Field[i][j]);
			Field[i].clear();
		}
		Field.clear();
		break;
	case 4: // ModeSet Установить режим работы ФУ (0 - дробное АЛУ, 1 - целочисленное АЛУ)
		Mode = Load.toInt();
		break;
	case 5: // IndGroupSet Установить индекс группы АЛУ
		IndGroup = Load.toInt();
		break;
	case 6:	//IndDevSet Установить индекс АЛУ в группе
		Ind = Load.toInt();
		break;
	case 10: // CreateGoup Создать группу АЛУ
		Field.push_back({});
		break;
	case 11: // CreateDev Создать АЛУ в группе (На входе шаблон ФУ)
		if (IndGroup >= Field.size() && IndGroup>=0)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		if (!Load.isEmpty())
			;//Field[IndGroup].push_back();
		else
			switch (Mode)
			{
			case 0: Field[IndGroup].push_back(new StreamFloatALU(this, (FU*)Load.Point)); break;
			case 1: Field[IndGroup].push_back(new StreamIntALU(this, (FU*)Load.Point)); break;
			}
		break;
	case 12: // CreateLastDev Создать АЛУ в последней группе
		if (Field.size() == 0) // Если список пустой, то добавить одно строку
			Field.push_back({});
		switch (Mode)
		{
		case 0: Field.back().push_back(new StreamFloatALU((FU*)this, (FU*)Load.Point)); break;
		case 1: Field.back().push_back(new StreamIntALU((FU*)this, (FU*)Load.Point)); break;
		}
		break;

	case 20: // IndGroupAdd Прибавить к индексу группы (в нагрузке значение, которое прибавляется к индексу
		Ind += Load.toInt();
		break;
	case 21: // IndGroupAdd   Прибавить к индексу группы (в нагрузке значение, которое прибавляется к индексу, 1 по умолчанию) 
		IndGroup += Load.toInt();
		break;
	case 22: // IndDevAdd Прибавить к индексу ФУ в группе группы (в нагрузке значение, которое прибавляется к индексу, 1 по умолчанию)
		Ind += Load.toInt();
		break;
	case 30: // ExecAll Выполнить программу для всех ФУ поля
		for (auto& i : Field)
			for (auto& j : i)
				j->ProgExec(Load);
		break;
	case 31: // ExecGroup Выполнить программу для всех ФУ группы
		if (IndGroup >= Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		for (auto& i : Field[IndGroup])
			i->ProgExec(Load);
		break;
	case 32: // ExecDev Выполнить программу для конкретного ФУ
		if (IndGroup >= Field.size() || Field[IndGroup].size()>=Ind)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Field[IndGroup][Ind]->ProgExec(Load);
		break;

	case 50: //  DevCountOut Выдать количество АЛУ в поле
		Load.Write(DeviseCounter);
		break;
	case 51: //  DevCountOutMk Выдать МК с количеством АЛУ в поле
		MkExec(Load, {Cint,&DeviseCounter});
		break;
	case 52: //  DevGroupCountOut Выдать количество АЛУ в текущей группе
		if (IndGroup >= Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Load.Write(Field[IndGroup].size());
		break;
	case 53: //  DevGroupCountOutMk Выдать количество АЛУ в текущей группе
	{
		if (IndGroup >= Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		int t = Field[IndGroup].size();
		break;
	}
	case 55: //  GroupCountOut Выдать количество групп
		Load.Write(Field.size());
		break;
	case 56: //  GroupCountOutMk Выдать МК с количеством групп
	{
		int t = Field.size();
		break;
	}
	case 70: //  DevOut Выдать контекст текущего АЛУ
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Load.Write((FU*)Field[IndGroup][Ind]);
		break;
	case 71: //  DevOutMk Выдать контекст текущего АЛУ
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{ProgExec(DevNotExistErrProg); 
		break;
		}
		MkExec(Load,{ CFU,Field[IndGroup][Ind] });
		break;
	case 80: //  LastDevOut Выдать контекст последнего созданного АЛУ
		if (!Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Load.Write((FU*) & Field.back().back());
		break;
	case 81: //  LastDevOutMk Выдать МК с контекстом последнего созданного АЛУ
		if (!Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
			MkExec(Load, { CFU,(FU*)&Field.back().back()});
		break;
	case 90: //  LastGroupIndOut Выдать индекс последней созданной группы АЛУ (-1, если поле пустое)
		Load.Write(Field.size()-1);
		break;
	case 91: //  LastGroupIndOutMk Выдать МК с индексом последней созданной группы АЛУ (-1, если поле пустое)
		{
		int t = Field.size() - 1;
		MkExec(Load, { CFU,&t});
		}
		break;
	case 100: //  LastDeviseIndOut Выдать индекс последнего созданного АЛУ (-1, если поле пустое)
		if (!Field.size() || !Field.back().size())
			Load.Write((int)-1);
		else
			Load.Write(Field.back().size()-1);
		break;
	case 101: //  LastDevIndOutMk Выдать МК с индексом последнего созданного АЛУ (-1, если поле пустое)
	{
		int t = -1;
		if (Field.size() && Field.back().size())
			t = Field.back().size() - 1;
		MkExec(Load, { CFU,&t });
	}
		break;

	default:
		CommonMk(MK, Load, Sender);
		break;
	}
}

StreamManager::~StreamManager() // Деструктор
{
	ProgFU(0, {0,nullptr},nullptr);
	Field.clear();
}

FU* StreamManager::Copy() // Программа копирования ФУ
{
	return new StreamManager(Bus, this);
}

FU* StreamManager::TypeCopy() // Создать ФУ такого же типа (не копируя контекст
{
	return new StreamManager(Bus, nullptr);
}


#include "StreamManager.h"
#include "FUIni.h"

using namespace std;

void StreamManager::ProgFU(long int MK, LoadPoint Load, FU* Sender)
{
	switch (MK%FUMkRange)
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
	case 1: // FieldCreate Создавать поле АЛУ (на входе номер типа или указатель на шаблон ФУ)
		for (int i = 1; i <= Counter; i++)
		{
			Field.push_back({});
			if (Load.isEmpty()) continue;
			if (Load.isInt())
			{
				Field.back().push_back(MakeFU.MakeFu(Load.toInt(), Bus));
				break;
			}
			else if (Load.isFU()) // Если указатель на ФУ
			{
				Field.back().push_back(((FU*)Load.Point)->Copy());
				break;
			}
		}
		break;
	case 2: // GroupCreateTempl Создавать группу на основе эталона на входе идекс группы, по умолчанию по индексу GroupInd)
	{
		if (Load.isEmpty() && (IndGroup >= Field.size() && IndGroup >= 0))
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		long int t;
		if (Load.isEmpty())

			for (int i = 1; i <= Counter; i++)
			{
				Field.push_back({});
				if (Load.isInt())
				{
					Field.back().push_back(MakeFU.MakeFu(Load.toInt(), Bus));
					break;
				}
				else if (Load.isFU()) // Если указатель на ФУ
				{
					Field.back().push_back(MakeFU.MakeFu(Load.Point));
					break;
				}
			}
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
	case 5: // IndGroupSet Установить индекс группы АЛУ
		IndGroup = Load.toInt();
		break;
	case 6:	//IndSet Установить индекс АЛУ в группе
		Ind = Load.toInt();
		break;
	case 7: // CounterSet Установить сколько раз необходимо создавать устройства
		Counter = Load.toInt(1);
		break;
	case 4: //Ind2Set Установить второй индекс АЛУ в группе
		Ind2 = Load.toInt();
		break;
	case 9: //Ind2GroupSet Установить второй индекс группы АЛУ
		IndGroup2 = Load.toInt();
		break;
	case 10: // CreateGoup Создать группу АЛУ
		Field.push_back({});
		break;
	case 11: // DevCreate Создать АЛУ в группе (На входе шаблон ФУ)
		if (IndGroup >= Field.size() && IndGroup >= 0)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		if (Load.isEmpty()) break;
		for (int i = Counter; i < 0; i++)
			if (Load.isInt())
			{
				Field[IndGroup].push_back(MakeFU.MakeFu(Load.toInt(), Bus));
				break;
			}
			else if (Load.isFU()) // Если указатель на ФУ
			{
				Field[IndGroup].push_back(MakeFU.MakeFu(Load.Point));
				break;
			}
		break;
	case 12: // DevCreateLast Создать АЛУ в последней группе
		if (Field.size() == 0) // Если список пустой, то добавить одну строку
		{
			Field.push_back({});
			IndGroup = 0;
		}
		if (Load.isEmpty()) break;
		for (int i = Counter; i < 0; i++)
			if (Load.isInt())
			{
				Field.back().push_back(MakeFU.MakeFu(Load.toInt(), Bus));
				break;
			}
			else if (Load.isFU()) // Если указатель на ФУ
			{
				Field.back().push_back(MakeFU.MakeFu(Load.Point));
				break;
			}
		break;
	case 13: // DevAdd Добавить устройство в текущую группу
		if (!Load.isFU()) break;
		if (Field.size() == 0) // Если список пустой, то добавить одну строку
		{
			Field.push_back({});
			IndGroup = 0;
		}
		Field[IndGroup].push_back((FU*)Load.Point);
		break;
	case 14: // DevLastAdd Добавить устойство в последнюю группу
		if (!Load.isFU()) break;
		if (Field.size() == 0) // Если список пустой, то добавить одну строку
		{
			Field.push_back({});
			IndGroup = 0;
		}
		Field.back().push_back((FU*)Load.Point);
		break;
	case 19: // GroupRefCreate Cоздать ссылки на ФУ для группы ФУ
	{
		for (auto& i : Field[IndGroup])
			switch (i->GetFuType())
			{
			case FUStreamFloatALU:
				for (int j = 0; j < ((StreamFloatALU*)i)->ReceiverMk.size(); j++)
				{
					if (((StreamFloatALU*)i)->ReceiverContexts[j] == Bus || ((StreamFloatALU*)i)->ReceiverContexts[j] == nullptr)
					{
						FU* t = nullptr;
						for (auto& k : Field[IndGroup])
							if (k->FUMkGlobalAdr <= 10);
					}
				}
				break;
			case FUStreamIntALU:
				break;
			}
	}
	break;
	case 20: // IndAdd Прибавить к индексу группы (в нагрузке значение, которое прибавляется к индексу
		Ind += Load.toInt();
		break;
	case 21: // IndGroupAdd   Прибавить к индексу группы (в нагрузке значение, которое прибавляется к индексу, 1 по умолчанию) 
		IndGroup += Load.toInt();
		break;
	case 22: // Ind2Add Прибавить ко второму индексу устройства (в нагрузке значение, которое прибавляется к индексу)
		Ind2 += Load.toInt();
		break;
	case 23: // Ind2GroupAdd   Прибавить ко второму индексу группы (в нагрузке значение, которое прибавляется к индексу, 1 по умолчанию) 
		IndGroup2 += Load.toInt();
		break;
	case 24: // MkSet Установить МК для первошго индекса
		Mk1 = Load.toInt();
		break;
	case 25: // Mk2Set Установить МК для второго индекса
		Mk2 = Load.toInt();
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
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Field[IndGroup][Ind]->ProgExec(Load);
		break;
	case 33: // Exec2Group Выполнить программу для всех ФУ группы
		if (IndGroup >= Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		for (auto& i : Field[IndGroup2])
			i->ProgExec(Load);
		break;
	case 34: // Exec2Dev Выполнить программу для конкретного ФУ
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Field[IndGroup2][Ind]->ProgExec(Load);
		break;
	case 35: // MkExec Выполнить МК для ФУ поля
		Field[IndGroup][Ind]->MkExec(Mk1, Load);
		break;
	case 36: // Mk2Exec Выполнить МК для ФУ поля по второму индексу
		Field[IndGroup2][Ind2]->MkExec(Mk1, Load);
		break;
	case 37: // MkBackExec Выполнить МК для последнего ФУ последней группы поля
		Field.back().back()->MkExec(Mk1, Load);
		break;
	case 38: // Mk2BackExec Выполнить МК для ФУ поля по второму индексу
		Field.back().back()->MkExec(Mk2, Load);
		break;
	case 39: // MkAllExec Выполнить МК для всех ФУ поля
		for (auto& i : Field)
			for (auto& j : i)
				j->MkExec(Mk1, Load);
		break;
	case 40: // MkAllGroupExec Выполнить МК для всех ФУ текущей группы
		for (auto& j : Field[IndGroup])
			j->MkExec(Mk1, Load);
		break;
	case 41: // MkAllLastGroupExec Выполнить МК для всех ФУ последней группы
		for (auto& j : Field.back())
			j->MkExec(Mk1, Load);
		break;

	case 50: //  DevCountOut Выдать количество АЛУ в поле
		Load.Write(DeviseCounter);
		break;
	case 51: //  DevCountOutMk Выдать МК с количеством АЛУ в поле
		MkExec(Load, { Cint,&DeviseCounter });
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
		long int t = Field[IndGroup].size();
		MkExec(Load, {Cint,&t});
		break;
	}
	case 55: //  GroupCountOut Выдать количество групп
		Load.Write(Field.size());
		break;
	case 56: //  GroupCountOutMk Выдать МК с количеством групп
	{
		long int t = Field.size();
		MkExec(Load, { Cint,&t });
		break;
	}
	case 70: //  DevOut Выдать контекст текущего АЛУ
	case 71: //  DevOutMk Выдать контекст текущего АЛУ
	case 72: //  Dev2Out Выдать контекст текущего АЛУ
	case 73: //  Dev2OutMk Выдать контекст текущего АЛУ
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		switch (MK%FUMkRange) {
		case 70:
			Load.Write((FU*)Field[IndGroup][Ind]);
			break;
		case 71:
			MkExec(Load, { CFU,Field[IndGroup][Ind] });
			break;
		case 72:
			Load.Write((FU*)Field[IndGroup2][Ind2]);
			break;
		case 73:
			MkExec(Load, { CFU,Field[IndGroup2][Ind2] });
			break;
		}
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
		long int t = Field.size() - 1;
		MkExec(Load, { CFU,&t});
		}
		break;
	case 100: //  LastDevIndOut Выдать индекс последнего созданного АЛУ (-1, если поле пустое)
		if (!Field.size() || !Field.back().size())
			Load.Write((long int)-1);
		else
			Load.Write(Field.back().size()-1);
		break;
	case 101: //  LastDevIndOutMk Выдать МК с индексом последнего созданного АЛУ (-1, если поле пустое)
	{
		long int t = -1;
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
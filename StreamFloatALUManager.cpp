#include "StreamFloatALUManager.h"

using namespace std;

void StreamFloatALUManager::ProgFU(int MK, LoadPoint Load, FU* Sender)
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
	case 1: // FieldCreate создавать поле АЛУ (на входе размерность по умолчанию 1)
		Field.resize(Load.toInt(1));			
		break;
	case 2: // FieldCreateProg создавать поле АЛУ на основе эталона (на входе размерность, эталон находится в векторе поля АЛУ под индексом 0, в нулевой группе)
		if (!Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		for (int i = 0; i < Field.size(); i++) // Убираются все ФУ, кроме первого
		{
			for(int j=0; j< Field[i].size();j++)
				free(Field[i][j]);
			Field[i].clear();
		}
		Field.clear();
		break;
	case 3: // FieldClear Очистить поле АЛУ
		for (int i = 1; i < Field.size(); i++) // Убираются все ФУ, кроме первого
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
	case 6:	//IndDeviceSet Установить индекс АЛУ в группе
		Ind = Load.toInt();
		break;
	case 10: // CreateGoup Создать группу АЛУ
		Field.push_back({});
		break;
	case 11: // CreateDevise Создать АЛУ в группе
		if (IndGroup >= Field.size() || IndGroup<0)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		if(Load.isEmpty())
			Field[IndGroup].push_back(new StreamFloatALU(this,nullptr));
		else
			Field[IndGroup].push_back(new StreamFloatALU((FU*)this, (FU*)Load.Point));
		break;
	case 12: // CreateLastDevise Включить АЛУ в последнюю группу
		if (Load.isEmpty())
			Field.back().push_back(new StreamFloatALU(this, nullptr));
		else
			Field.back().push_back(new StreamFloatALU((FU*)this, (FU*)Load.Point));
		break;
	case 15: // AddDevise Включить устройство в текущую группу
		if (IndGroup >= Field.size() || IndGroup < 0)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Field[IndGroup].push_back((StreamFloatALU*)Load.Point);
		break;
	case 16: // AddLasrDevise Включить устройство в последнюю группу
		if (!Field.size())
			Field.push_back({});
		Field.back().push_back((StreamFloatALU*)Load.Point);
		break;
	case 20: // IndGroupAdd Прибавить к индексу группы (в нагрузке значение, которое прибавляется к индексу
		Ind += Load.toInt();
		break;
	case 21: // IndDeviceAdd Прибавить к индексу ФУ в группе группы (в нагрузке значение, которое прибавляется к индексу, 1 по умолчанию)
		IndGroup += Load.toInt();
		break;
	case 22: // IndGroupAdd Прибавить к индексу группы (в нагрузке значение, которое прибавляется к индексу, 1 по умолчанию)
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
	case 32: // ExecDevise Выполнить программу для конкретного ФУ
		if (IndGroup >= Field.size() || Field[IndGroup].size()>=Ind)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Field[IndGroup][Ind]->ProgExec(Load);
		break;

	case 40: // CreateGoup Создать группу АЛУ
		Field.push_back({});
		IndGroup = Field.size() - 1; // Установить указатель текущей группы на последнюю группу
		break;
	case 41: // CreateDevise Создать АЛУ в группе
		if (IndGroup >= Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		if (Load.isEmpty())
			Field[IndGroup].push_back({});
		else // Создание по образцу
			Field[IndGroup].push_back(new StreamFloatALU(this, (StreamFloatALU*)Load.Point));
		DeviseCounter++;
		break;
	case 42: // CreateLastDevise Создать АЛУ в последней группе
		if (!Field.size())
			Field.push_back({});
		if (Load.isEmpty())
			Field.back().push_back({});
		else // Создание по образцу
			Field.back().push_back(new StreamFloatALU(this, (StreamFloatALU*)Load.Point));
		DeviseCounter++;
		break;

	case 50: //  DeviseCountOut Выдать количество АЛУ в поле
		Load.Write(DeviseCounter);
		break;
	case 51: //  DeviseCountOutMk Выдать количество АЛУ в поле
		MkExec(Load, {Cint,&DeviseCounter});
		break;
	case 52: //  DeviseGroupCountOut Выдать количество АЛУ в текущей группе
		if (IndGroup >= Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Load.Write(Field[IndGroup].size());
		break;
	case 53: //  DeviseGroupCountOutMk Выдать количество АЛУ в текущей группе
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
	case 56: //  GroupCountOutMk Выдать количество групп
	{
		int t = Field.size();
		break;
	}
	case 70: //  DeviseOut Выдать контекст текущего АЛУ
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Load.Write((FU*)Field[IndGroup][Ind]);
		break;
	case 71: //  DeviseOutMk Выдать контекст текущего АЛУ
		if (IndGroup >= Field.size() || Field[IndGroup].size() >= Ind)
		{ProgExec(DevNotExistErrProg); 
		break;
		}
		MkExec(Load,{ CFU,Field[IndGroup][Ind] });
		break;
	case 80: //  LastDeviseOut Выдать контекст последнего созданного АЛУ
		if (!Field.size())
		{
			ProgExec(DevNotExistErrProg);
			break;
		}
		Load.Write((FU*) & Field.back().back());
		break;
	case 81: //  LastDeviseOutMk Выдать контекст последнего созданного АЛУ
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
	case 91: //  LastGroupIndOutMk Выдать индекс последней созданной группы АЛУ
		{
		int t = Field.size() - 1;
		MkExec(Load, { CFU,&t});
		}
		break;
	case 100: //  LastDeviseIndOut Выдать индекс последнего созданного АЛУ
		if (!Field.size() || !Field.back().size())
			Load.Write((int)-1);
		else
			Load.Write(Field.back().size()-1);
		break;
	case 101: //  LastDeviseIndOutMk Выдать индекс последнего созданного АЛУ
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

StreamFloatALUManager::~StreamFloatALUManager() // Деструктор
{
	ProgFU(0, {0,nullptr},nullptr);
	Field.clear();
}

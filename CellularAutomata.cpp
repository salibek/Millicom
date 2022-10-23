#include "CellularAutomat.h"

// Простейший клеточный автомат (устройство для вычисления сеточных функций)
void CellularAutomat::ProgFU(int MK, LoadPoint Load)
{
	// Доделать буфер ИП с лексемами
	if (ReceiveProg != nullptr && Mode == 0) // Запуск программы автоматного режима, т.е. конгда автомат самостоятельно обрабатывает приходящие МК
	{
		int t = Mode; 
		Mode = 0; // Сменить режим работы на нулевой
		ProgExec(ReceiveProg);
		if(!Mode)
			Mode = t; // Восстановить прежний режим работы ФУ
	}
	switch (MK)
	{
	case 0: // Reset
		PlyInd = 0;
		if (AccumCreating)
		{
			delete Alu;
			Alu = nullptr;
			AccumCreating = false;
		}
		Neighbours.resize(0);
		NeighboursMk.resize(0);
		RezReady.resize(0);
		InCounter.resize(0);
		Rez.resize(0);
		Plys.resize(1);
		InComplectF.resize(0);
		PlyCurrent = 0; ParameterInd = -1;
		FiringProg = nullptr; ReceiveProg = nullptr; IndFuOffcet = 0; FUInd = 0;
		break;
	case 20: // NeighbourAdd Добавить ссылку на исполнительное устройство
		Neighbours.push_back((FU*)Load.Point);
		break;
	case 21: // NeighbourMkAdd Добавить МК для соседа (сначала добавляем ссылку, затем МК; если добавляется только МК, то в качестве сслыки автоматически добавлается Bus)
		if (Neighbours.size() == NeighboursMk.size())
			Neighbours.push_back(Bus);
		NeighboursMk.push_back(Load.toInt());
		break;
	case 22: // NeighbourClear Очистить буфера ссылок на соседей и МК для них
		Neighbours.clear();
		NeighboursMk.clear();
		break;
	case 30: // StateSet Установить индекс состояния автомата
		State = Load.toInt();
		break;
	case 31: // StateOut Выдать индекс состояния автомата
		Load.Write(State);
		break;
	case 32: // StateOutMk Выдать МК с индексом состояния автомата
		MkExec(Load, { Cint,&State });
		break;
	case 35: // IndSet Установить индекс
		Ind = Load.toInt();
		break;
	case 36: // IndAdd Прибавить к индексу
		Ind += Load.toInt();
		break;
	case 37: // IndOut Выдать индекс
		Load.Write(Ind);
		break;
	case 38: // IndOutMk Выдать МК с индексом
		MkExec(Load, {Cint, &Ind});
		break;

	case 700: // IndSet Установить индекс
		Ind = Load.toInt();
		break;
	case 701: //IndAdd Увеличить индекс на величину из нагрузки входящей ИП
		Ind += Load.toInt();
		break;
	case 799: // N_InAdd Изменить число входных параметров (по умолчанию +1)
		N_In += Load.toInt(1);
		break;
	case 800: // N_InSet Установить количество входных данных для срабатывания
		N_In = Load.toInt();
		Plys[PlyCurrent].resize(N_In);
		ReceiveProgs.resize(N_In);
		for (auto& i : ReceiveProgs)
			i = nullptr;
		break;
	case 801: // N_PlySet Установить количество слоев для расчета
		Plys.resize(Load.toInt());
		Rez.resize(Load.toInt());
		if (!Neighbours.size())
			for (auto& i : Plys)
				i.resize(Neighbours.size());
		InCounter[PlyCurrent] = Load.toInt();
		break;
	case 805: //RezSet  Установить результат вычислений (если PlyInd<0,  устанавливается значение текущего слоя)
		Rez[PlyInd < 0 ? Rez.size() - 1 : PlyInd] = Load.toDouble();
		break;
	case 806: //RezOut Выдать результат вычислений (если PlyInd<0, выдается значение результат расчета текущего слоя)
		Load.Write(Rez[PlyInd < 0 ? Rez.size() - 1 : PlyInd]);
		break;
	case 807: //RezOutMk Выдать МК с результатом вычислений (если PlyInd<0, выдается значение результат расчета текущего слоя)
	{
		double t = Rez[PlyInd < 0 ? Rez.size() - 1 : 0];
		MkExec(Load, {Cdouble, &t });
		break;
	}
	case 808: // PlyIndSet Установить индекс слоя для расчета
		PlyInd = Load.toInt(0);
		break;
	case 810: //­	IndFuOffcetSet Смещение индекса ФУ (индекс домножается на эту величину и к нему прибавляется индекс слоя)
		IndFuOffcet = Load.toInt();
		break;
	case 811: // ­	ModeSet Режим работы автомата (0 - настройка, 1 - рабочий режим)
		Mode = Load.toInt();
	case 815: //CollectorSet Установить коллектор для сбора результатов
		Collector = Load.Point;
		break;
	case 820: // ­FiringProgSet Установить ссылку на программу вычисления результата при накоплении пакета данных
		FiringProg = Load.Point;
		break;
	case 821: // FiringProgOut Установить ссылку на программу вычисления результата при накоплении пакета данных
		Load.Write(FiringProg);
		break;
	case 822: // FiringProgOutMk Установить ссылку на программу вычисления результата при накоплении пакета данных
		MkExec(Load, {CIC, FiringProg });
		break;
	case 824: // ReceiveProgSet Установить программу, запускаемую при приеме данных по каналу Ind
		ReceiveProgs[Ind] = Load.Point;
		break;
	case 825: // ReceiveAllProgSet Установить программу, запускаемую при приеме данных
		ReceiveProg = Load.Point;
		break;
	case 826: // ReceiveProgOut Установить ссылку на программу вычисления результата при накоплении пакета данных
		Load.Write(ReceiveProg);
		break;
	case 827: // ­ReceiveProgOutMk Установить ссылку на программу вычисления результата при накоплении пакета данных
		MkExec(Load, { CIC, ReceiveProg });
		break;
	case 830: // ­AutoSendSet Установить флаг автоматической пересылки результатов вычисления
		AutoSend = Load.toBool();
		break;
	case 834: // RezReadySet
		RezReady[PlyCurrent] = Load.toBool();
	case 835: // ­	RezReadyOut 
		Load.Write(RezReady[PlyCurrent]);
		break;
	case 836: // ­	RezReadyOutMk
	{
		bool t = RezReady[PlyCurrent];
		MkExec(Load, { Cbool, &t });
		break;
	}
	case 840: // VarAdd добавить локальную переменную
		Vars.push_back(Load.toDouble());
		break;
	case 841: // VarClear очистить список локальных переменных
		Vars.clear();
		break;
	case 842: // VarIndexSet Установить индекс параметра (если -1, то выбирается последний добавленный параметр)
	case 843: // VarOut Выдать локальную переменную
		if (VarInd < 0)
			Load.Write(Vars.back());
		else
			Load.Write(Vars[VarInd]);
		break;
	case 844: // VarOutMk Выдать МК с локальной переменной
		if (VarInd < 0)
			MkExec(Load, { Cdouble,&Vars.back() });
		else
			MkExec(Load, { Cdouble,&Vars[VarInd] });
		break;
	case 848: // VarSet Установить значение в список параметров (если VarInd=-1, то уставливается последний параметр)
		if (VarInd < 0)
			Vars[Vars.size() - 1] = Load.toDouble();
		else
			Vars[VarInd] = Load.toDouble();
		break;
	case 845: // ­ParameterIndSet Индекс текущего параметра (при -1 выжается последний параметр)
		ParameterInd = Load.toInt();
		break;
	case 846: // ­ParameterClear Сбросить список параметров
		ParameterInd = -1;
		parameters.clear();
		break;
	case 850: // ParameterAdd Установить параметр для расчетов (при -1 добавляется новый параметр)
		if (ParameterInd < 0)
			parameters.push_back(Load);
		else
			if(ParameterInd< parameters.size())
				parameters[ParameterInd]=Load;
		break;
	case 851: // ParametrOut
		if (ParameterInd < 0)
			Load.WriteFromLoad(parameters.back());
		else
			if(ParameterInd< parameters.size())
				parameters.back().WriteFromLoad(Load);
		break;
	case 852: // ParametrOutMk
		if (ParameterInd < 0)
			MkExec(Load, parameters.back());
		else
			if (ParameterInd < parameters.size())
				MkExec(Load, parameters[ParameterInd]);
		break;
	case 853: // ParameterAdd Добавить новый параметр
		parameters.push_back(Load.Clone());
		break;
	case 854: // ParametersClear Очистать список параметров
		parameters.clear();
		break;
	case 855: // ­InComplectFSet
		Load.Write(InComplectF[PlyCurrent]);
		break;
	case 856: // ­InComplectFOut
		Load.Write(InComplectF[PlyCurrent]);
		break;
	case 857: // ­InComplectFOutMk
		MkExec(Load, { Cbool,&InComplectF });
		break;
	case 860: //ParametersCountOut
		Load.Write(parameters.size());
		break;
	case 861: //ParametersCountOutMk
	{
		int t = parameters.size();
		MkExec(Load, { Cint,&t });
		break;
	}
	case 863: // FUIndSet Установить индекс ФУ-автомата
		FUInd = Load.toInt();
		break;
	case 865: // ­	ManagerSet Установить ссылку на менеджера автомата
		Manager = Load.Point;
		break;
	case 870: // Настройка/рабочий режим
		Mode = Load.toInt();
		break;
	case 880: // RezSendToAll Выдать результат вычислений соседям
		for (int i = 0; i < Neighbours.size(); i++)
			Neighbours[i]->ProgFU(NeighboursMk[i], {Cdouble,&Rez[PlyCurrent]});
		break;
	case 882: // InCounterSet Установить счетчик входных данных (Если PlyInd<0, то устанавливается для текущего уровня)
		if(PlyInd<0)
			InCounter[PlyCurrent] = Load.toInt();
		else
			InCounter[PlyInd] = Load.toInt();
		break;
	case 883: // InCounterOut Выдать знаение счетчика входных данных
		Load.Write(InCounter);
		break;
	case 884: // InCounterOutMk Выдать МК со значением счетчика входных данных
		MkExec(Load, { Cint,&InCounter });
		break;
	case 885: //PlyNSet Установить количество в
		PlyN = Load.toInt();
		break;
	case 887: //  PlyCurrentOut Выдать индекс текущего слоя расчета
		Load.Write(PlyCurrent);
		break;
	case 888: //  PlyCurrentOutMk Выдать МК с индексом текущего слоя расчета
		MkExec(Load, { Cint, &PlyCurrent });
		break;
	case 895: // SendToManager Переслать МК или несколько МК менеджеру
		if(Load.Type>>1==DIC)
			for(auto i: *(IC_type)Load.Point)
				MkExec(i.atr,i.Load,(FU*)Manager);
		else if(Load.Type >> 1 == DIP)
			MkExec(((ip*)Load.Point)->atr, ((ip*)Load.Point)->Load, (FU*)Manager);
		break;
	case 896: // SendToCollector Переслать МК или несколько МК коллектору
		if (Load.isIC())
			for (auto i : *(IC_type)Load.Point)
				MkExec(i.atr, i.Load, (FU*)Collector);
		else if (Load.Type >> 1 == DIP)
			MkExec(((ip*)Load.Point)->atr, ((ip*)Load.Point)->Load, (FU*)Collector);
		break;

	case 897: //Synk Синхронизация вычислений (по этому сигналу ФУ выдает результат вычислений соседям, если не установлен AutoSend)
		if (RezReady[PlyCurrent])
		{
			for (int i=0;i< Neighbours.size(); i++)
				MkExec(NeighboursMk[i]+IndFuOffcet, {Cdouble,&Rez[PlyCurrent]}, Neighbours[i]);
			PlyCurrent = (PlyCurrent + 1) % Neighbours.size();
		}
		break;
	
	case 3: // InOut Выдать полученное от соседа значение по индексу
		Load.Write(Plys[PlyInd][Ind]);
		break;
	case 4: // InOutMk Выдать соседу МК со значением от соседа по индексу
		MkExec(Load, Plys[PlyInd][Ind]);
		break;
	case 5: // NeighbourMkSet Установить МК для соседа по индексу
		NeighboursMk[Ind] = Load.toInt(); // Установить МК для соседа
		break;
	case 6: // NeighbourSet Установить ссылку на соседа по индексу
		Neighbours[Ind] = (CellularAutomat*)Load.Point;
		break;
	case 7: // ParametrSet Установить параметр по индексу
		parameters[Ind].WriteFromLoad(Load);
		break;
	case 8: // ParametrOut Выдать МК с параметром по индексу
		MkExec(Load, parameters[Ind]);
		break;
	case 9: // ParametrOutMk Установить ссылку на соседа по индексу
		Neighbours[Ind] = (FU*)Load.Point;
		break;
	case 10: // Cend Выдать МК со значением для соседа по индексу
		if (Ind > Plys[PlyInd].size()) break;
		Neighbours[PlyInd][Ind].ProgFU(NeighboursMk[Ind], Load);
		break;

	
	case 100: //In_0_Set Принять значение от 0-го соседа
	case 150: //In_0_Out Выдать значение, полученное от 0-го соседа
	case 200: //In_0_OutMk Выдать МК соз значением, полученным от 0-го соседа
	case 250: //NeighbourMk_0_Set Установить МК для 0-го соседа
	case 300: //Neighbour_0_Set Установить ссылку на 0-го соседа
	case 350: //Parametr_0_Set Установить 0-й параметр
	case 400: //Parametr_0_Out Выдать 0-й параметр
	case 450: //Parametr_0_OutMk Выдать  МК с 0-м параметром
	case 500: //SendTo_0 Выдать для соседа 0
	case 550: //AccumToNeighbour_0_Out Выдать значение аккумулятора соседу 0

	case 101:   //In_1_Set Принять значение от 1-го соседа
	case 151: //In_1_Out Выдать значение, полученное от 1-го соседа
	case 201: //In_1_OutMk Выдать МК соз значением, полученным от 1-го соседа
	case 251: //NeighbourMk_1_Set Установить МК для 1-го соседа
	case 301: //Neighbour_1_Set Установить ссылку на 1-го соседа
	case 351: //Parametr_1_Set Установить 1-й параметр
	case 401: //Parametr_1_Out Выдать 1-й параметр
	case 451: //Parametr_1_OutMk Выдать  МК с 1-м параметром
	case 501: //SendTo_1 Выдать для соседа 1
	case 551: //AccumToNeighbour_1_Out Выдать значение аккумулятора соседу 1

	case 102:   //In_2_Set Принять значение от 2-го соседа
	case 152: //In_2_Out Выдать значение, полученное от 2-го соседа
	case 202: //In_2_OutMk Выдать МК соз значением, полученным от 2-го соседа
	case 252: //NeighbourMk_2_Set Установить МК для 2-го соседа
	case 302: //Neighbour_2_Set Установить ссылку на 2-го соседа
	case 352: //Parametr_2_Set Установить 2-й параметр
	case 402: //Parametr_2_Out Выдать 2-м параметром
	case 452: //Parametr_2_OutMk Выдать  МК с 1-й параметр
	case 502: //SendTo_1 Выдать для соседа 2
	case 552: //AccumToNeighbour_2_Out Выдать значение аккумулятора соседу 2

	case 103:   //In_3_Set Принять значение от 3-го соседа
	case 153: //In_3_Out Выдать значение, полученное от 3-го соседа
	case 203: //In_3_OutMk Выдать МК соз значением, полученным от 3-го соседа
	case 253: //NeighbourMk_3_Set Установить МК для 3-го соседа
	case 303: //Neighbour_3_Set Установить ссылку на 3-го соседа
	case 353: //Parametr_3_Set Установить 3-й параметр
	case 403: //Parametr_3_Out Выдать 3-й параметр
	case 453: //Parametr_3_OutMk Выдать МК с 3-м параметром
	case 503: //SendTo_3 Выдать для соседа 3
	case 553: //AccumToNeighbour_3_Out Выдать значение аккумулятора соседу 3

	case 104:   //In4_Set Принять значение от 4-го соседа
	case 154: //In4_Out Выдать значение, полученное от 4-го соседа
	case 204: //In4_OutMk Выдать МК соз значением, полученным от 4-го соседа
	case 254: //NeighbourMk_4_Set Установить МК для 4-го соседа
	case 304: //Neighbour_4_Set Установить ссылку на 4-го соседа
	case 354: //Parametr_4_Set Установить 4-й параметр
	case 404: //Parametr_4_Out Выдать 4-й параметр
	case 454: //Parametr_4_OutMk Выдать  МК с 4-м параметром
	case 504: //ToNeighbour_4_Send Выдать для соседа 4
	case 554: //AccumToNeighbour_4_Out Выдать значение аккумулятора соседу 4

	case 105:   //In5_Set Принять значение от 5-го соседа
	case 155: //In5_Out Выдать значение, полученное от 5-го соседа
	case 205: //In5_OutMk Выдать МК соз значением, полученным от 5-го соседа
	case 255: //NeighbourMk_5_Set Установить МК для 5-го соседа
	case 305: //Neighbour_5_Set Установить ссылку на 5-го соседа
	case 355: //Parametr_5_Set Установить 5-й параметр
	case 405: //Parametr_5_Out Выдать 5-й параметр
	case 455: //Parametr_5_OutMk Выдать  МК с 5-й параметр
	case 505: //SendTo_5 Выдать для соседа 5
	case 555: //AccumToNeighbour_5_Out Выдать значение аккумулятора соседу 5

	case 106:   //In6_Set Принять значение от 6-го соседа
	case 156: //In6_Out Выдать значение, полученное от 6-го соседа
	case 206: //In6_OutMk Выдать МК соз значением, полученным от 6-го соседа
	case 256: //NeighbourMk_6_Set Установить МК для 6-го соседа
	case 306: //Neighbour_6_Set Установить ссылку на 6-го соседа
	case 356: //Parametr_6_Set Установить 6-й параметр
	case 406: //Parametr_6_Out Выдать 6-й параметр
	case 456: //Parametr_6_OutMk Выдать  МК с 6-й параметр
	case 506: //SendTo_6 Выдать для соседа 6
	case 556: //AccumToNeighbour_6_Out Выдать значение аккумулятора соседу 6

	case 107:   //In7_Set Принять значение от 7-го соседа
	case 157: //In7_Out Выдать значение, полученное от 7-го соседа
	case 207: //In7_OutMk Выдать МК соз значением, полученным от 7-го соседа
	case 257: //NeighbourMk_7_Set Установить МК для 7-го соседа
	case 307: //Neighbour_7_Set Установить ссылку на 7-го соседа
	case 357: //Parametr_7_Set Установить 7-й параметр
	case 407: //Parametr_7_Out Выдать 7-й параметр
	case 457: //Parametr_7_OutMk Выдать  МК с 7-й параметр
	case 507: //SendTo_7 Выдать для соседа 7
	case 557: //AccumToNeighbour_7_Out Выдать значение аккумулятора соседу 7

	case 108:   //In8_Set Принять значение от 8-го соседа
	case 158: //In8_Out Выдать значение, полученное от 8-го соседа
	case 208: //In8_OutMk Выдать МК соз значением, полученным от 8-го соседа
	case 258: //NeighbourMk_8_Set Установить МК для 8-го соседа
	case 308: //Neighbour_8_Set Установить ссылку на 8-го соседа
	case 358: //Parametr_8_Set Установить 8-й параметр
	case 408: //Parametr_8_Out Выдать 8-й параметр
	case 458: //Parametr_8_OutMk Выдать  МК с 8-й параметр
	case 508: //SendTo_8 Выдать для соседа 8
	case 558: //AccumToNeighbour_8_Out Выдать значение аккумулятора соседу 8

	case 109:   //In9_Set Принять значение от 9-го соседа
	case 159: //In9_Out Выдать значение, полученное от 9-го соседа
	case 209: //In9_OutMk Выдать МК соз значением, полученным от 9-го соседа
	case 259: //NeighbourMk_9_Set Установить МК для 9-го соседа
	case 309: //Neighbour_9_Set Установить ссылку на 9-го соседа
	case 359: //Parametr_9_Set Установить 9-й параметр
	case 409: //Parametr_9_Out Выдать 9-й параметр
	case 459: //Parametr_9_OutMk Выдать  МК с 9-й параметр
	case 509: //SendTo_9 Выдать для соседа 9
	case 559: //AccumToNeighbour_9_Out Выдать значение аккумулятора соседу 9

	case 110:  //In10_Set Принять значение от 10-го соседа
	case 160: //In10_Out Выдать значение, полученное от 10-го соседа
	case 210: //In10_OutMk Выдать МК соз значением, полученным от 10-го соседа
	case 260: //NeighbourMk_10_Set Установить МК для 10-го соседа
	case 310: //Neighbour_10_Set Установить ссылку на 10-го соседа
	case 360: //Parametr_10_Set Установить 10-й параметр
	case 410: //Parametr_10_Out Выдать 10-й параметр
	case 460: //Parametr_10_OutMk Выдать   МК с 10-й параметр
	case 510: //SendTo_10 Выдать для соседа 10
	case 560: //AccumToNeighbour_10_Out Выдать значение аккумулятора соседу 10

	case 111:  //In11_Set Принять значение от 11-го соседа
	case 161: //In11_Out Выдать значение, полученное от 11-го соседа
	case 211: //In11_OutMk Выдать МК соз значением, полученным от 11-го соседа
	case 261: //NeighbourMk_11_Set Установить МК для 11-го соседа
	case 311: //Neighbour_11_Set Установить ссылку на 11-го соседа
	case 361: //Parametr_11_Set Установить 11-й параметр
	case 411: //Parametr_11_Out Выдать 11-й параметр
	case 461: //Parametr_11_OutMk Выдать  МК 11-й параметр
	case 511: //SendTo_11 Выдать для соседа 11
	case 561: //AccumToNeighbour_11_Out Выдать значение аккумулятора соседу 11

	case 112:  //In12_Set Принять значение от 12-го соседа
	case 162: //In12_Out Выдать значение, полученное от 12-го соседа
	case 212: //In12_OutMk Выдать МК соз значением, полученным от 12-го соседа
	case 262: //Neighbour_12_Set Установить ссылку на 12-го соседа
	case 312: //NeighbourMk_12_Set Установить МК для 12-го соседа
	case 362: //Parametr_12_Set Установить 12-й параметр
	case 412: //Parametr_12_Out Выдать 12-й параметр
	case 462: //Parametr_12_OutMk Выдать  МК 12-й параметр
	case 512: //TSendTo_12 Выдать для соседа 12
	case 562: //AccumToNeighbour_12_Out Выдать значение аккумулятора соседу 12

	case 113:  //In13_Set Принять значение от 13-го соседа
	case 163: //In13_Out Выдать значение, полученное от 13-го соседа
	case 213: //In13_OutMk Выдать МК соз значением, полученным от 13-го соседа
	case 263: //Neighbour_13_Set Установить МК для 13-го соседа
	case 313: //NeighbourMk_13_Set Установить ссылку на 13-го соседа
	case 363: //Parametr_13_Set Установить 13-й параметр
	case 413: //Parametr_13_Out Выдать 13-й параметр
	case 463: //Parametr_13_OutMk Выдать  МК 13-й параметр
	case 513: //SendTo_13 Выдать для соседа 13
	case 563: //AccumToNeighbour_13_Out Выдать значение аккумулятора соседу 13

	case 114:  //In14_Set Принять значение от 14-го соседа
	case 164: //In14_Out Выдать значение, полученное от 14-го соседа
	case 214: //In14_OutMk Выдать МК соз значением, полученным от 14-го соседа
	case 264: //NeighbourMk_14_Set Установить МК для 14-го соседа
	case 314: //Neighbour_14_Set Установить ссылку на 14-го соседа
	case 364: //Parametr_14_Set Установить 14-й параметр
	case 414: //Parametr_14_Out Выдать 14-й параметр
	case 464: //Parametr_14_OutMk Выдать МК 14-й параметр
	case 514: //SendTo_14 Выдать для соседа 14
	case 564: //AccumToNeighbour_14_Out Выдать значение аккумулятора соседу 14

	case 115:  //In15_Set Принять значение от 15-го соседа
	case 165: //In15_Out Выдать значение, полученное от 15-го соседа
	case 215: //In15_OutMk Выдать МК соз значением, полученным от 15-го соседа
	case 265: //Neighbour_15_Set Установить МК для 15-го соседа
	case 315: //NeighbourMk_15_Set Установить ссылку на 15-го соседа
	case 365: //Parametr_15_Set Установить 15-й параметр
	case 415: //Parametr_15_Out Выдать 15-й параметр
	case 465: //Parametr_15_OutMk Выдать МК 15-й параметр
	case 515: //SendTo_15 Выдать для соседа 15
	case 565: //AccumToNeighbour_15_Out Выдать значение аккумулятора соседу 15

		MK-=100;
		switch (MK / 50) {
		case 1: // Выдать полученное от соседа значение
			if (MK % 50 > Plys[PlyInd].size()) break;
			Load.Write(Plys[PlyInd][MK % 50]);
			break;
		case 2: // Выдать соседу МК со значением от соседа
			if (MK % 50 > Plys[PlyInd].size()) break;
			MkExec(Load, Plys[PlyInd][MK % 50]);
			break;
		case 3: // Установить МК для соседа
			if (MK % 50 > Neighbours.size()) break;
			NeighboursMk[MK % 50] = Load.toInt(); // Установить МК для соседа
			break;
		case 4: // Установить ссылку на соседа
			if (MK % 50 > Neighbours.size()) break;
			Neighbours[MK % 50] = (CellularAutomat*)Load.Point;
			break;
		case 5: // Установить параметр
			if (MK % 50 > parameters.size()) break;
			parameters[MK % 50].WriteFromLoad(Load);
			break;
		case 6: // Выдать МК с параметром
			if (MK % 50 > parameters.size()) break;
			MkExec(Load, parameters[MK % 50]);
			break;
		case 7: // Установить ссылку на соседа
			if (MK % 50 > Neighbours.size()) break;
			Neighbours[MK % 50] = (FU*)Load.Point;
			break;
		case 8: // Send_N Выдать значение для соседа с индексом
			if (MK % 50 > Neighbours.size()) break;
			Neighbours[PlyInd][MK % 50].ProgFU(NeighboursMk[MK % 50], Load);
			break;
		case 9: //AccumToNeighbour_N_Out Выдать значение аккумулятора соседу
			Neighbours[PlyInd][MK % 50].ProgFU(NeighboursMk[MK % 50], Accum);
			break;
		case 0: // Приять значение от соседа
		{
			if (Plys[PlyCurrent][MK % 50].Point == nullptr)
				InCounter[PlyCurrent]++;
			Plys[PlyCurrent][MK % 50] = Load.Clone();
			InCounter[PlyCurrent]++;
			ProgExec(ReceiveProg); // Запуск программы по приему данных от соседа
			if (InCounter[PlyCurrent] == Neighbours.size()) // Набрался полный комплект
			{
				InComplectF[PlyCurrent][MK % 50] = true;
				ProgExec(FiringProg);
				RezReady[PlyCurrent] = true;
				if (AutoSend) ProgFU(890, { 0, nullptr });
				PlyCurrent++;
				PlyCurrent %= PlyN;
				InCounter[PlyCurrent] = 0;
				RezReady[PlyCurrent] = false;
				for (int i = 0; i < Neighbours.size(); i++)
					InComplectF[PlyCurrent][i] = false;
			}
		}
		}
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
}

void CellularAutomatManager::ProgFU(int MK, LoadPoint Load)
{
	// Доделать буфер ИП с лексемами
	switch (MK)
	{
	case 0: // Reset
		NetType = 0;
		break;
	case 1:// NetTypeSet Установить тип сетки
		NetType = Load.toInt();
		break;
	case 3: // DimSet Добавить измерение
		Dim = Load.toInt();
		break;

	case 10: // IniAutmataProgSet Установить программу инициализации автомата
		iniAutmataProg = Load.Point;
		break;
	case 11: // CollectorSet Утановить ссылку на коллектор
		Collector = Load.Point;
		break;
	case 15: // FiringProgSet Установить программу вычисления результата
		FiringProg = Load.Point;
		break;
	case 16: // ReceiveProgSet Установить программу, запускаемую при приходе одного значения от соседа
		ReceiveProg = Load.Point;
		break;
	case 17: // InCounterSet Установить счетчик входных данных
		InCounter = Load.toInt();
		break;
	case 28: // Ind1Add Увеличить значение первого счетчика
		Ind1 += Load.toInt();
		break;
	case 29: //Ind2Add Увеличить значение второго счетчика
		Ind2 += Load.toInt();
		break;
	case 30: // Ind1Set Установить первый индекс ФУ-автомата
		Ind1 = Load.toInt();
		break;
	case 31: // Ind2Set Установить второй индекс ФУ-автомата
		Ind2 = Load.toInt();
		break;
	case 32: // Step1Set Установить шаг автоматической инкрементации для 1-го индекса
		Step1 = Load.toInt();
		break;
	case 33: // Step2Set Установить шаг автоматической инкрементации для 2-го индекса
		Step2 = Load.toInt();
		break;
	case 35: // Context1Out Выдать контекст первого ФУ-автомата
		if (Ind1 >= Net.size() || Ind1 < 0) break;
		Load.Write((FU*)&Net[Ind1]);
		break;
	case 36: // Context2OutMk Выдать МК с контекстот первого ФУ-автомата
		if (Ind1 >= Net.size() || Ind1 < 0) break;
		MkExec(Load, { TFU, &Net[Ind1] });
		break;
	case 37: // Context2Out Выдать контекст второго ФУ-автомата
		if (Ind2 >= Net.size() || Ind2 < 0) break;
		Load.Write((FU*)&Net[Ind2]);
		break;
	case 38: // Context2OutMk Выдать МК с контекстом второго ФУ-автомата
		if (Ind2 >= Net.size() || Ind2 < 0) break;
		MkExec(Load, { TFU, &Net[Ind2] });
		break;
	case 40: // ContextAddMkSet Установить МК для добавления контекста соседа исполнительного ФУ
		ContextAddMk = Load.toInt();
		break;
	case 41: // MkAppendMkSet Установить МК для добавления МК для соседа исполнительного ФУ
		MkAddMk = Load.toInt();
		break;
	case 50: // Prog1Exec Выполнить программу для первого ФУ-автомата
		if (Ind1 >= Net.size() || Ind1 < 0) break;
		Net[Ind1].ProgExec(Load);
		Ind1 += Step1;
		break;
	case 51: // Prog2Exec Выполнить программу для второго ФУ-автомата
		if (Ind2 >= Net.size() || Ind2 < 0) break;
		Net[Ind1].ProgExec(Load);
		Ind2 += Step2;
		break;
	case 60: // Neitborder1Append Добавить соседа для ФУ с индексом 1 (на входе ссылка на соседе, если nil, то соседом делается ФУ и Ind2)
		if (Ind1 >= Net.size() || Ind1<0) break;
		if (Load.Point == nullptr)
		{
			if(Ind2 >= Net.size() || Ind2 < 0)
				Net[Ind1].Neighbours.push_back(Bus);
			else
				Net[Ind1].Neighbours.push_back(&Net[Ind2]);
			Net[Ind2].N_In++;
			Net[Ind2].Plys[Net[Ind2].PlyCurrent].push_back({0, nullptr});
			Net[Ind2].ReceiveProgs.push_back(nullptr);
			Ind1 += Step1;
			Ind2 += Step2;
		}
		else
		{
			Net[Ind1].Neighbours.push_back((FU*)Load.Point);
			Ind1 += Step1;
		}
		break;
	case 61: // Neitborder2Append Добавить соседа для ФУ с индексом 2 (на входе ссылка на соседе, если nil, то соседом делается ФУ и Ind1)
		if (Ind2 >= Net.size() || Ind2 < 0) break;
		if (Load.Point == nullptr)
		{
			if (Ind1 >= Net.size() || Ind1 < 0)
				Net[Ind2].Neighbours.push_back(nullptr);
			else
				Net[Ind2].Neighbours.push_back(&Net[Ind1]);
			Net[Ind1].N_In++; // Инкремент количества входных параметров
			Net[Ind1].Plys[Net[Ind1].PlyCurrent].push_back({ 0, nullptr });
			Net[Ind1].ReceiveProgs.push_back(nullptr);

			Ind1 += Step1;
			Ind2 += Step2;
		}
		else
		{
			Net[Ind2].Neighbours.push_back((FU*)Load.Point);
			Ind2 += Step2;
		}
		break;
	case 62: // Neitborder1MkAppend Добавить МК для соседа для ФУ с индексом 1
		if (Ind1 >= Net.size() || Ind1 < 0) break;
		Net[Ind1].NeighboursMk.push_back(Load.toInt());
		if (Net[Ind1].Neighbours.size() < Net[Ind1].NeighboursMk.size())
			Net[Ind1].Neighbours.push_back(Bus);
		break;
	case 63: // Neitborder2MkAppend Добавить МК для соседа для ФУ с индексом 2
		if (Ind2 >= Net.size() || Ind2 < 0) break;
		Net[Ind2].NeighboursMk.push_back(Load.toInt());
		if (Net[Ind2].Neighbours.size() < Net[Ind2].NeighboursMk.size())
			Net[Ind2].Neighbours.push_back(Bus);
		break;
	case 65: // Neitborder1Out Выдать ссылку на контекст ФУ с индексом 1
		Load.Write((FU*)&Net[Ind1]);
		Ind1 += Step1;
		break;
	case 66: // Neitborder2Out  Выдать ссылку на контекст ФУ с индексом 2
		Load.Write((FU*)&Net[Ind2]);
		Ind2 += Step2;
		break; 
	case 67: // Neitborder1OutMk  Выдать МК со ссылкой на контекст ФУ с индексом 1
		MkExec(Load, {TFU,&Net[Ind1] });
		Ind1 += Step1;
		break;
	case 68: // Neitborder2OutMk Выдать МК со ссылкой на контекст ФУ с индексом 2
		MkExec(Load, { TFU,&Net[Ind2] });
		Ind2 += Step2;
		break;

	case 200: //NetGenerate Генерация сетки (На вход может подаваться количество ФУ в сетке)
	{
		if (Load.Point != nullptr)
			Dim = Load.toInt();
		Net.resize(Dim); // Создать поле автоматов
		for (auto& i : Net)
			i.Bus = Bus;
		int k = 0;
		for (auto& i : Net)
		{
			i.Manager = this; // Установить ссылку на менеджера
			i.Collector = Collector; // Установит ссылку на коллектор
			i.ProgExec(iniAutmataProg); // Запуск программы инициализации каждого автомата
			i.ProgFU(FUIndSetMk, {Cint,&k}); // Установить индекс ФУ
			k++;
		}
	}
		break;
	case 201: // NetClear Очистить сетку
		Net.clear();
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
}
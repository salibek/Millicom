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
	case 700: // IndSet Установить индекс
		Ind = Load.toInt();
		break;
	case 701: //IndAdd Увеличить индекс на величину из нагрузки входящей ИП
		Ind += Load.toInt();
		break;
	case 800: // N_NeighbourSet Установить количество соседей
		Neighbours.resize(Load.toInt());
		NeighboursMk.resize(Load.toInt());
		if (!Plys.size())
		{
			for (auto& i : Plys)
				i.resize(Load.toInt());
		}
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
		PlyInd = Load.toInt(-1);
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
	case 825: // ReceiveProgSet Установить программу, запускаемую при приеме данных
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
	case 850: // ParameterSet Установить параметр (при -1 добавляется новый параметр)
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
	case 10: // ToNeighbourOutMk Выдать значение для соседа с индексом по индексу
		Neighbours[PlyInd][Ind].ProgFU(NeighboursMk[MK % 50], Load);
		break;

	
	case 100: //In_0_Set Принять значение от 0-го соседа
	case 150: //In_0_Out Выдать значение, полученное от 0-го соседа
	case 200: //In_1_OutMk Выдать МК соз значением, полученным от 0-го соседа
	case 250: //NeighbourMk_0_Set Установить МК для 0-го соседа
	case 300: //Neighbour_0_Set Установить ссылку на 0-го соседа
	case 350: //Parametr_0_Set Установить 0-й параметр
	case 400: //Parametr_0_Out Выдать 0-й параметр
	case 450: //Parametr_0_OutMk Выдать  МК с 0-м параметром
	case 500: //ToNeighbour_1_OutMk Выдать МК для соседа

	case 101:   //In_1_Set Принять значение от 1-го соседа
	case 151: //In_1_Out Выдать значение, полученное от 1-го соседа
	case 201: //In_1_OutMk Выдать МК соз значением, полученным от 1-го соседа
	case 251: //NeighbourMk_1_Set Установить МК для 1-го соседа
	case 301: //Neighbour_1_Set Установить ссылку на 1-го соседа
	case 351: //Parametr_1_Set Установить 1-й параметр
	case 401: //Parametr_1_Out Выдать 1-й параметр
	case 451: //Parametr_1_OutMk Выдать  МК с 1-м параметром
	case 501: //ToNeighbour_1_OutMk Выдать МК для соседа

	case 102:   //In_2_Set Принять значение от 2-го соседа
	case 152: //In_2_Out Выдать значение, полученное от 2-го соседа
	case 202: //In_2_OutMk Выдать МК соз значением, полученным от 2-го соседа
	case 252: //NeighbourMk_2_Set Установить МК для 2-го соседа
	case 302: //Neighbour_2_Set Установить ссылку на 2-го соседа
	case 352: //Parametr_2_Set Установить 2-й параметр
	case 402: //Parametr_2_Out Выдать 2-м параметром
	case 452: //Parametr_2_OutMk Выдать  МК с 1-й параметр
	case 502: //ToNeighbour_1_OutMk Выдать МК для соседа

	case 103:   //In_3_Set Принять значение от 3-го соседа
	case 153: //In_3_Out Выдать значение, полученное от 3-го соседа
	case 203: //In_3_OutMk Выдать МК соз значением, полученным от 3-го соседа
	case 253: //NeighbourMk_3_Set Установить МК для 3-го соседа
	case 303: //Neighbour_3_Set Установить ссылку на 3-го соседа
	case 353: //Parametr_3_Set Установить 3-й параметр
	case 403: //Parametr_3_Out Выдать 3-й параметр
	case 453: //Parametr_3_OutMk Выдать МК с 3-м параметром
	case 503: //ToNeighbour_3_OutMk Выдать МК для соседа

	case 104:   //In4_Set Принять значение от 4-го соседа
	case 154: //In4_Out Выдать значение, полученное от 4-го соседа
	case 204: //In4_OutMk Выдать МК соз значением, полученным от 4-го соседа
	case 254: //NeighbourMk_4_Set Установить МК для 4-го соседа
	case 304: //Neighbour_4_Set Установить ссылку на 4-го соседа
	case 354: //Parametr_4_Set Установить 4-й параметр
	case 404: //Parametr_4_Out Выдать 4-й параметр
	case 454: //Parametr_4_OutMk Выдать  МК с 4-м параметром
	case 504: //ToNeighbour_4_OutMk Выдать МК для соседа

	case 105:   //In5_Set Принять значение от 5-го соседа
	case 155: //In5_Out Выдать значение, полученное от 5-го соседа
	case 205: //In5_OutMk Выдать МК соз значением, полученным от 5-го соседа
	case 255: //NeighbourMk_5_Set Установить МК для 5-го соседа
	case 305: //Neighbour_5_Set Установить ссылку на 5-го соседа
	case 355: //Parametr_5_Set Установить 5-й параметр
	case 405: //Parametr_5_Out Выдать 5-й параметр
	case 455: //Parametr_5_OutMk Выдать  МК с 5-й параметр
	case 505: //ToNeighbour_5_OutMk Выдать МК для соседа

	case 106:   //In6_Set Принять значение от 6-го соседа
	case 156: //In6_Out Выдать значение, полученное от 6-го соседа
	case 206: //In6_OutMk Выдать МК соз значением, полученным от 6-го соседа
	case 256: //NeighbourMk_6_Set Установить МК для 6-го соседа
	case 306: //Neighbour_6_Set Установить ссылку на 6-го соседа
	case 356: //Parametr_6_Set Установить 6-й параметр
	case 406: //Parametr_6_Out Выдать 6-й параметр
	case 456: //Parametr_6_OutMk Выдать  МК с 6-й параметр
	case 506: //ToNeighbour_6_OutMk Выдать МК для соседа

	case 107:   //In7_Set Принять значение от 7-го соседа
	case 157: //In7_Out Выдать значение, полученное от 7-го соседа
	case 207: //In7_OutMk Выдать МК соз значением, полученным от 7-го соседа
	case 257: //NeighbourMk_7_Set Установить МК для 7-го соседа
	case 307: //Neighbour_7_Set Установить ссылку на 7-го соседа
	case 357: //Parametr_7_Set Установить 7-й параметр
	case 407: //Parametr_7_Out Выдать 7-й параметр
	case 457: //Parametr_7_OutMk Выдать  МК с 7-й параметр
	case 507: //ToNeighbour_7_OutMk Выдать МК для соседа

	case 108:   //In8_Set Принять значение от 8-го соседа
	case 158: //In8_Out Выдать значение, полученное от 8-го соседа
	case 208: //In8_OutMk Выдать МК соз значением, полученным от 8-го соседа
	case 258: //NeighbourMk_8_Set Установить МК для 8-го соседа
	case 308: //Neighbour_8_Set Установить ссылку на 8-го соседа
	case 358: //Parametr_8_Set Установить 8-й параметр
	case 408: //Parametr_8_Out Выдать 8-й параметр
	case 458: //Parametr_8_OutMk Выдать  МК с 8-й параметр
	case 508: //ToNeighbour_8_OutMk Выдать МК для соседа

	case 109:   //In9_Set Принять значение от 9-го соседа
	case 159: //In9_Out Выдать значение, полученное от 9-го соседа
	case 209: //In9_OutMk Выдать МК соз значением, полученным от 9-го соседа
	case 259: //NeighbourMk_9_Set Установить МК для 9-го соседа
	case 309: //Neighbour_9_Set Установить ссылку на 9-го соседа
	case 359: //Parametr_9_Set Установить 9-й параметр
	case 409: //Parametr_9_Out Выдать 9-й параметр
	case 459: //Parametr_9_OutMk Выдать  МК с 9-й параметр
	case 509: //ToNeighbour_9_OutMk Выдать МК для соседа

	case 110:  //In10_Set Принять значение от 10-го соседа
	case 160: //In10_Out Выдать значение, полученное от 10-го соседа
	case 210: //In10_OutMk Выдать МК соз значением, полученным от 10-го соседа
	case 260: //NeighbourMk_10_Set Установить МК для 10-го соседа
	case 310: //Neighbour_10_Set Установить ссылку на 10-го соседа
	case 360: //Parametr_10_Set Установить 10-й параметр
	case 410: //Parametr_10_Out Выдать 10-й параметр
	case 460: //Parametr_10_OutMk Выдать   МК с 10-й параметр
	case 510: //ToNeighbour_10_OutMk Выдать МК для соседа

	case 111:  //In11_Set Принять значение от 11-го соседа
	case 161: //In11_Out Выдать значение, полученное от 11-го соседа
	case 211: //In11_OutMk Выдать МК соз значением, полученным от 11-го соседа
	case 261: //NeighbourMk_11_Set Установить МК для 11-го соседа
	case 311: //Neighbour_11_Set Установить ссылку на 11-го соседа
	case 361: //Parametr_11_Set Установить 11-й параметр
	case 411: //Parametr_11_Out Выдать 11-й параметр
	case 461: //Parametr_11_OutMk Выдать  МК 11-й параметр
	case 511: //ToNeighbour_11_OutMk Выдать МК для соседа

	case 112:  //In12_Set Принять значение от 12-го соседа
	case 162: //In12_Out Выдать значение, полученное от 12-го соседа
	case 212: //In12_OutMk Выдать МК соз значением, полученным от 12-го соседа
	case 262: //Neighbour_12_Set Установить ссылку на 12-го соседа
	case 312: //NeighbourMk_12_Set Установить МК для 12-го соседа
	case 362: //Parametr_12_Set Установить 12-й параметр
	case 412: //Parametr_12_Out Выдать 12-й параметр
	case 462: //Parametr_12_OutMk Выдать  МК 12-й параметр
	case 512: //ToNeighbour_12_OutMk Выдать МК для соседа

	case 113:  //In13_Set Принять значение от 13-го соседа
	case 163: //In13_Out Выдать значение, полученное от 13-го соседа
	case 213: //In13_OutMk Выдать МК соз значением, полученным от 13-го соседа
	case 263: //Neighbour_13_Set Установить МК для 13-го соседа
	case 313: //NeighbourMk_13_Set Установить ссылку на 13-го соседа
	case 363: //Parametr_13_Set Установить 13-й параметр
	case 413: //Parametr_13_Out Выдать 13-й параметр
	case 463: //Parametr_13_OutMk Выдать  МК 13-й параметр
	case 513: //ToNeighbour_13_OutMk Выдать МК для соседа

	case 114:  //In14_Set Принять значение от 14-го соседа
	case 164: //In14_Out Выдать значение, полученное от 14-го соседа
	case 214: //In14_OutMk Выдать МК соз значением, полученным от 14-го соседа
	case 264: //NeighbourMk_14_Set Установить МК для 14-го соседа
	case 314: //Neighbour_14_Set Установить ссылку на 14-го соседа
	case 364: //Parametr_14_Set Установить 14-й параметр
	case 414: //Parametr_14_Out Выдать 14-й параметр
	case 464: //Parametr_14_OutMk Выдать МК 14-й параметр
	case 514: //ToNeighbour_14_OutMk Выдать МК для соседа

	case 115:  //In15_Set Принять значение от 15-го соседа
	case 165: //In15_Out Выдать значение, полученное от 15-го соседа
	case 215: //In15_OutMk Выдать МК соз значением, полученным от 15-го соседа
	case 265: //Neighbour_15_Set Установить МК для 15-го соседа
	case 315: //NeighbourMk_15_Set Установить ссылку на 15-го соседа
	case 365: //Parametr_15_Set Установить 15-й параметр
	case 415: //Parametr_15_Out Выдать 15-й параметр
	case 465: //Parametr_15_OutMk Выдать МК 15-й параметр
	case 515: //ToNeighbour_15_OutMk Выдать МК для соседа

		switch (MK / 50) {
		case 3: // Выдать полученное от соседа значение
			Load.Write(Plys[PlyInd][MK % 50]);
			break;
		case 4: // Выдать соседу МК со значением от соседа
			MkExec(Load, Plys[PlyInd][MK % 50]);
			break;
		case 5: // Установить МК для соседа
			NeighboursMk[MK % 50] = Load.toInt(); // Установить МК для соседа
			break;
		case 6: // Установить ссылку на соседа
			Neighbours[MK % 50] = (CellularAutomat*)Load.Point;
			break;
		case 7: // Установить параметр
			parameters[MK % 50].WriteFromLoad(Load);
			break;
		case 8: // Выдать МК с параметром
			MkExec(Load, parameters[MK % 50]);
			break;
		case 9: // Установить ссылку на соседа
			Neighbours[MK % 50] = (FU*)Load.Point;
			break;
		case 10: // ToNeighbour_N_OutMk Выдать значение для соседа с индексом
			Neighbours[PlyInd][MK % 50].ProgFU(NeighboursMk[MK % 50], Load);
			break;
		case 2: // Приять значение от соседа
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
	case 3: // DimAdd Добавить измерение
		Dim.push_back(Load.toInt());
		break;
	case 5: // DimClear Очистить вектора размерностей
		Dim.clear();
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
	case 16: // ReceiveProgSet Установить программу вычисления результата
		ReceiveProg = Load.Point;
		break;
	case 17: // InCounterSet Установить счетчик входных данных
		InCounter = Load.toInt();
		break;
	case 30: // Ind1Set Установить первый индекс ФУ-автомата
		Ind1 = Load.toInt();
		break;
	case 31: // Ind2Set Установить второй индекс ФУ-автомата
		Ind2 = Load.toInt();
		break;
	case 35: // Ind1ContextOut Выдать контекст первого ФУ-автомата
		Load.Write((FU*)&Net[Ind1]);
		break;
	case 36: // Ind1ContextOutMk Выдать МК с контекстот первого ФУ-автомата
		MkExec(Load, { TFU, &Net[Ind1] });
		break;
	case 37: // Ind2ContextOut Выдать контекст второго ФУ-автомата
		Load.Write((FU*)&Net[Ind2]);
		break;
	case 38: // Ind2ContextOutMk Выдать МК с контекстом второго ФУ-автомата
		MkExec(Load, { TFU, &Net[Ind2] });
		break;
	case 50: // Ind1ProgExec Выполнить программу для первого ФУ-автомата
		Net[Ind1].ProgExec(Load);
	case 51: // Ind2ProgExec Выполнить программу для второго ФУ-автомата
		Net[Ind1].ProgExec(Load);
	case 60: // Neitborder1Set Установить соседа для ФУ с индексом 1
		Net[Ind1].Neighbours.push_back((FU*)Load.Point);
	case 61: // Neitborder2Set Установить соседа для ФУ с индексом 2
		Net[Ind2].Neighbours.push_back((FU*)Load.Point);
	case 65: // Neitborder1Out Выдать ссылку на контекст ФУ с индексом 1
		Load.Write((FU*)&Net[Ind1]);
		break;
	case 66: // Neitborder2Out  Выдать ссылку на контекст ФУ с индексом 2
		Load.Write((FU*)&Net[Ind2]);
		break; 
	case 67: // Neitborder1OutMk  Выдать МК со ссылкой на контекст ФУ с индексом 1
		MkExec(Load, {TFU,&Net[Ind1] });
		break;
	case 68: // Neitborder2OutMk Выдать МК со ссылкой на контекст ФУ с индексом 2
		MkExec(Load, { TFU,&Net[Ind2] });
		break;
	case 70: //PlyNSet Установить количество слоев
		PlyN = Load.toInt(1);

	case 100: //NetGenerate Генерация сетки
	{
		int N = 0;
		for (auto i : Dim)
			N += i;
		Net.resize(N); // Создать поле автоматов
		int k = 0;
		for (auto& i : Net)
		{
			i.Manager = this; // Установить ссылку на менеджера
			i.Collector = Collector; // Установит ссылку на коллектор
			i.ProgExec(iniAutmataProg); // Запуск программы инициализации каждого автомата
			i.FUInd = k; // Установить индекс автомата
			i.FiringProg = FiringProg; // Установить программу вычисления результата
			i.InCounter[0] = InCounter;
			i.Plys[0].resize(InCounter);
			k++;
		}
	}
		break;
	case 101: // NetClear Очистить сетку
		Net.clear();
		Dim.clear();
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
}
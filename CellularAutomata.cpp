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
		Plys.resize(0);
		InComplectF.resize(0);
		PlyCurrent = 0; ParameterInd = -1;
		CalcProg = nullptr; ReceiveProg = nullptr; IndFuOffcet = 0; FUInd = 0;
		break;
	case 700: // N_NeighbourSet Установить количество соседей
		Neighbours.resize(Load.ToInt());
		NeighboursMk.resize(Load.ToInt());
		if (!Plys.size())
		{
			for (auto& i : Plys)
				i.resize(Load.ToInt());
		}
		break;
	case 701: // N_PlySet Установить количество слоев для расчета
		Plys.resize(Load.ToInt());
		Rez.resize(Load.ToInt());
		if (!Neighbours.size())
			for (auto& i : Plys)
				i.resize(Neighbours.size());
		break;
	case 705: //RezSet  Установить результат вычислений (если PlyInd<0, выдается значение результат расчета текущего слоя)
		Rez[PlyInd < 0 ? Rez.size() - 1 : PlyInd] = Load.ToDouble();
		break;
	case 706: //RezOut Выдать результат вычислений (если PlyInd<0, выдается значение результат расчета текущего слоя)
		Load.Write(Rez[PlyInd < 0 ? Rez.size() - 1 : PlyInd]);
		break;
	case 707: //RezOutMk Выдать МК с результатом вычислений (если PlyInd<0, выдается значение результат расчета текущего слоя)
	{
		double t = Rez[PlyInd < 0 ? Rez.size() - 1 : PlyInd];
		MkExec(Load, {Cdouble, &t });
		break;
	}
	case 708: // PlyIndSet Установить индекс слоя для расчета
		PlyInd = Load.ToInt(-1);
		break;
	case 710: //­	IndFuOffcetSet Смещение индекса ФУ (индекс домножается на эту величину и к нему прибавляется индекс слоя)
		IndFuOffcet = Load.ToInt();
		break;
	case 711: // ­	ModeSet Режим работы автомата (0 - настройка, 1 - рабочий режим)
		Mode = Load.ToInt();
	case 715: //CollectorSet 
		CollectorFU = Load.Point;
		break;
	case 720: // ­CalcProgSet Установить ссылку на программу вычисления результата при накоплении пакета данных
		CalcProg = Load.Point;
		break;
	case 721: // ­CalcProgOut Установить ссылку на программу вычисления результата при накоплении пакета данных
		Load.Write(CalcProg);
		break;
	case 722: // ­CalcProgOutMk Установить ссылку на программу вычисления результата при накоплении пакета данных
		MkExec(Load, {CIC, CalcProg });
		break;
	case 725: // ReceiveProgSet Установить программу, запускаемую при приеме данных
		ReceiveProg = Load.Point;
		break;
	case 726: // ReceiveProgOut Установить ссылку на программу вычисления результата при накоплении пакета данных
		Load.Write(ReceiveProg);
		break;
	case 727: // ­ReceiveProgOutMk Установить ссылку на программу вычисления результата при накоплении пакета данных
		MkExec(Load, { CIC, ReceiveProg });
		break;
	case 730: // ­AutoSendSet Установить флаг автоматической пересылки результатов вычисления
		AutoSend = Load.ToBool();
		break;
	case 735: // ­	RezReadyOut 
		Load.Write(RezReady);
		break;
	case 736: // ­	RezReady OutMk
		MkExec(Load,{Cbool, &RezReady});
		break;
	case 740: // VarAdd добавить локальную переменную
		Vars.push_back(Load.ToDouble());
		break;
	case 741: // VarClear очистить список локальных переменных
		Vars.clear();
		break;
	case 742: // VarIndexSet Установить индекс параметра (если -1, то выбирается последний добавленный параметр)
	case 743: // VarOut Выдать локальную переменную
		if (VarInd < 0)
			Load.Write(Vars.back());
		else
			Load.Write(Vars[VarInd]);
		break;
	case 744: // VarOutMk Выдать МК с локальной переменной
		if (VarInd < 0)
			MkExec(Load, { Cdouble,&Vars.back() });
		else
			MkExec(Load, { Cdouble,&Vars[VarInd] });
		break;
	case 748: // VarSet Установить значение в список параметров (если VarInd=-1, то уставливается последний параметр)
		if (VarInd < 0)
			Vars[Vars.size() - 1] = Load.ToDouble();
		else
			Vars[VarInd] = Load.ToDouble();
		break;
	case 745: // ­ParameterIndSet Индекс текущего параметра (при -1 выжается последний параметр)
		ParameterInd = Load.ToInt();
		break;
	case 746: // ­ParameterClear Сбросить список параметров
		ParameterInd = -1;
		parameters.clear();
		break;
	case 750: // ParameterSet Установить параметр (при -1 добавляется новый параметр)
		if (ParameterInd < 0)
			parameters.push_back(Load);
		else
			if(ParameterInd< parameters.size())
				parameters[ParameterInd]=Load;
		break;
	case 751: // ParametrOut
		if (ParameterInd < 0)
			Load.WriteFromLoad(parameters.back());
		else
			if(ParameterInd< parameters.size())
				parameters.back().WriteFromLoad(Load);
		break;
	case 752: // ParametrOutMk
		if (ParameterInd < 0)
			MkExec(Load, parameters.back());
		else
			if (ParameterInd < parameters.size())
				MkExec(Load, parameters[ParameterInd]);
		break;
	case 753: // ParameterAdd Добавить новый параметр
		parameters.push_back(Load.Clone());
		break;
	case 755: // ­InComplectFSet
		Load.Write(InComplectF[PlyCurrent]);
		break;
	case 756: // ­InComplectFOut
		Load.Write(InComplectF[PlyCurrent]);
		break;
	case 757: // ­InComplectFOutMk
		MkExec(Load, { Cbool,&InComplectF });
		break;
	case 760: //ParametersCountOut
		Load.Write(parameters.size());
		break;
	case 761: //ParametersCountOutMk
	{
		int t = parameters.size();
		MkExec(Load, { Cint,&t });
		break;
	}
	case 763: // FUIndSet Установить индекс ФУ-автомата
		FUInd = Load.ToInt();
		break;
	case 765: // ­	ManagerSet Установить ссылку на менеджера автомата
		Manager = Load.Point;
		break;
	case 770: // Настройка/рабочий режим
		Mode = Load.ToInt();
		break;
	case 780: // RezSend Выдать результат вычислений соседям
		for (int i = 0; i < Neighbours.size(); i++)
			MkExec(NeighboursMk[i], {Cdouble,&Rez[PlyCurrent]});
		break;
	case 785: // CalcPrigSet Установить программы вычисления резульатата
		CalcProg = Load.Point;
		break;
	case 790: // InCounterSet Установить счетчик входных данных (Если PlyInd<0, то устанавливается для текущего уровня)
		if(PlyInd<0)
			InCounter[PlyCurrent] = Load.ToInt();
		else
			InCounter[PlyInd] = Load.ToInt();
		break;
	case 791: // InCounterOut Выдать знаение счетчика входных данных
		Load.Write(InCounter);
		break;
	case 792: // InCounterOutMk Выдать МК со значением счетчика входных данных
		MkExec(Load, { Cint,&InCounter });
		break;
	case 795: // RezOut Выдать результат вычислений (если PlyInd<0, то выдается результат рекущего слоя)
		if(PlyInd>=0)
			Load.Write(Rez[PlyInd]);
		else
			Load.Write(Rez[PlyCurrent]);
		break;
	case 796: // RezOutMk
		if (PlyInd >= 0)
			MkExec(Load, {Cdouble, &Rez[PlyInd] });
		else
			MkExec(Load, { Cdouble, &Rez[PlyCurrent] });
		break;
	case 797: //  PlyCurrentOut Выдать индекс текущего слоя расчета
		Load.Write(PlyCurrent);
		break;
	case 798: //  PlyCurrentOutMk Выдать МК с индексом текущего слоя расчета
		MkExec(Load, { Cint, &PlyCurrent });
		break;
	case 800: // FUIndSet Установить индекс автомата
		FUInd = Load.ToInt();
		break;
	case 801: // FUIndSet Выдать индекс автомата
		Load.Write(FUInd);
		break; 
	case 802: // FUIndSet Выдать МК с индексом автомата
		MkExec(Load, {Cint,&FUInd});
		break;


	case 885: // SendToManager Переслать МК или несколько МК менеджеру
		if(Load.Type>>1==DIC)
			for(auto i: *(IC_type)Load.Point)
				MkExec(i.atr,i.Load,(FU*)Manager);
		else if(Load.Type >> 1 == DIP)
			MkExec(((ip*)Load.Point)->atr, ((ip*)Load.Point)->Load, (FU*)Manager);
		break;
	case 886: // SendToCollector Переслать МК или несколько МК коллектору
		if (Load.Type >> 1 == DIC)
			for (auto i : *(IC_type)Load.Point)
				MkExec(i.atr, i.Load, (FU*)CollectorFU);
		else if (Load.Type >> 1 == DIP)
			MkExec(((ip*)Load.Point)->atr, ((ip*)Load.Point)->Load, (FU*)CollectorFU);
		break;

	case 890: //Synk Синхронизация вычислений (по этому сигналу ФУ выдает результат вычислений соседям, если не установлен AutoSend)
		if (RezReady[PlyCurrent])
		{
			for (int i=0;i< Neighbours.size(); i++)
				MkExec(NeighboursMk[i]+IndFuOffcet, {Cdouble,&Rez[PlyCurrent]}, Neighbours[i]);
			PlyCurrent = (PlyCurrent + 1) % Neighbours.size();
		}
		break;
	case 1:   //In_1_Set Принять значение от 1-го соседа
	case 101: //In_1_Out Выдать значение, полученное от 1-го соседа
	case 201: //In_1_OutMk Выдать МК соз значением, полученным от 1-го соседа
	case 301: //NeighbourMk_1_Set Установить МК для 1-го соседа
	case 401: //Parametr_1_Set Установить 1-й параметр
	case 501: //Parametr_1_Out Выдать 1-й параметр
	case 601: //Parametr_1_OutMk Выдать  МК с 1-м параметром
	case 2:   //In_2_Set Принять значение от 2-го соседа
	case 102: //In_2_Out Выдать значение, полученное от 2-го соседа
	case 202: //In_2_OutMk Выдать МК соз значением, полученным от 2-го соседа
	case 302: //NeighbourMk_2_Set Установить МК для 2-го соседа
	case 402: //Parametr_2_Set Установить 2-й параметр
	case 502: //Parametr_2_Out Выдать 2-м параметром
	case 602: //Parametr_2_OutMk Выдать  МК с 1-й параметр
	case 3:   //In3_Set Принять значение от 3-го соседа
	case 103: //In3_Out Выдать значение, полученное от 3-го соседа
	case 203: //In3_OutMk Выдать МК соз значением, полученным от 3-го соседа
	case 303: //NeighbourMk_3_Set Установить МК для 3-го соседа
	case 403: //Parametr_2_Set Установить 3-й параметр
	case 503: //Parametr_2_Out Выдать 3-й параметр
	case 603: //Parametr_2_OutMk Выдать МК с 3-м параметром
	case 4:   //In4_Set Принять значение от 4-го соседа
	case 104: //In4_Out Выдать значение, полученное от 4-го соседа
	case 204: //In4_OutMk Выдать МК соз значением, полученным от 4-го соседа
	case 304: //NeighbourMk_4_Set Установить МК для 4-го соседа
	case 404: //Parametr_3_Set Установить 4-й параметр
	case 504: //Parametr_3_Out Выдать 4-й параметр
	case 604: //Parametr_3_OutMk Выдать  МК с 4-м параметром
	case 5:   //In5_Set Принять значение от 5-го соседа
	case 105: //In5_Out Выдать значение, полученное от 5-го соседа
	case 205: //In5_OutMk Выдать МК соз значением, полученным от 5-го соседа
	case 305: //NeighbourMk_5_Set Установить МК для 5-го соседа
	case 405: //Parametr_4_Set Установить 5-й параметр
	case 505: //Parametr_4_Out Выдать 5-й параметр
	case 605: //Parametr_4_OutMk Выдать  МК с 5-й параметр
	case 6:   //In6_Set Принять значение от 6-го соседа
	case 106: //In6_Out Выдать значение, полученное от 6-го соседа
	case 206: //In6_OutMk Выдать МК соз значением, полученным от 6-го соседа
	case 306: //NeighbourMk_6_Set Установить МК для 6-го соседа
	case 406: //Parametr_5_Set Установить 6-й параметр
	case 506: //Parametr_5_Out Выдать 6-й параметр
	case 606: //Parametr_5_OutMk Выдать  МК с 6-й параметр
	case 7:   //In7_Set Принять значение от 7-го соседа
	case 107: //In7_Out Выдать значение, полученное от 7-го соседа
	case 207: //In7_OutMk Выдать МК соз значением, полученным от 7-го соседа
	case 307: //NeighbourMk_7_Set Установить МК для 7-го соседа
	case 407: //Parametr_6_Set Установить 7-й параметр
	case 507: //Parametr_6_Out Выдать 7-й параметр
	case 607: //Parametr_6_OutMk Выдать  МК с 7-й параметр
	case 8:   //In8_Set Принять значение от 8-го соседа
	case 108: //In8_Out Выдать значение, полученное от 8-го соседа
	case 208: //In8_OutMk Выдать МК соз значением, полученным от 8-го соседа
	case 308: //NeighbourMk_8_Set Установить МК для 8-го соседа
	case 408: //Parametr_7_Set Установить 8-й параметр
	case 508: //Parametr_7_Out Выдать 8-й параметр
	case 608: //Parametr_7_OutMk Выдать  МК с 8-й параметр
	case 9:   //In9_Set Принять значение от 9-го соседа
	case 109: //In9_Out Выдать значение, полученное от 9-го соседа
	case 209: //In9_OutMk Выдать МК соз значением, полученным от 9-го соседа
	case 309: //NeighbourMk_9_Set Установить МК для 9-го соседа
	case 409: //Parametr_8_Set Установить 9-й параметр
	case 509: //Parametr_8_Out Выдать 9-й параметр
	case 609: //Parametr_8_OutMk Выдать  МК с 9-й параметр
	case 10:  //In10_Set Принять значение от 10-го соседа
	case 110: //In10_Out Выдать значение, полученное от 10-го соседа
	case 210: //In10_OutMk Выдать МК соз значением, полученным от 10-го соседа
	case 310: //NeighbourMk_10_Set Установить МК для 10-го соседа
	case 410: //Parametr_9_Set Установить 10-й параметр
	case 510: //Parametr_9_Out Выдать 10-й параметр
	case 610: //Parametr_9_OutMk Выдать   МК с 10-й параметр
	case 11:  //In11_Set Принять значение от 11-го соседа
	case 111: //In11_Out Выдать значение, полученное от 11-го соседа
	case 211: //In11_OutMk Выдать МК соз значением, полученным от 11-го соседа
	case 311: //NeighbourMk_11_Set Установить МК для 11-го соседа
	case 411: //Parametr_10_Set Установить 11-й параметр
	case 511: //Parametr_10_Out Выдать 11-й параметр
	case 611: //Parametr_10_OutMk Выдать  МК 11-й параметр
	case 12:  //In12_Set Принять значение от 12-го соседа
	case 112: //In12_Out Выдать значение, полученное от 12-го соседа
	case 212: //In12_OutMk Выдать МК соз значением, полученным от 12-го соседа
	case 312: //NeighbourMk_12_Set Установить МК для 12-го соседа
	case 412: //Parametr_11_Set Установить 12-й параметр
	case 512: //Parametr_11_Out Выдать 12-й параметр
	case 612: //Parametr_11_OutMk Выдать  МК 12-й параметр
	case 13:  //In13_Set Принять значение от 13-го соседа
	case 113: //In13_Out Выдать значение, полученное от 13-го соседа
	case 213: //In13_OutMk Выдать МК соз значением, полученным от 13-го соседа
	case 313: //NeighbourMk_13_Set Установить МК для 13-го соседа
	case 413: //Parametr_12_Set Установить 13-й параметр
	case 513: //Parametr_12_Out Выдать 13-й параметр
	case 613: //Parametr_12_OutMk Выдать  МК 13-й параметр
	case 14:  //In14_Set Принять значение от 14-го соседа
	case 114: //In14_Out Выдать значение, полученное от 14-го соседа
	case 214: //In14_OutMk Выдать МК соз значением, полученным от 14-го соседа
	case 314: //NeighbourMk_14_Set Установить МК для 14-го соседа
	case 414: //Parametr_13_Set Установить 14-й параметр
	case 514: //Parametr_13_Out Выдать 14-й параметр
	case 614: //Parametr_13_OutMk Выдать МК 14-й параметр
	case 15:  //In15_Set Принять значение от 15-го соседа
	case 115: //In15_Out Выдать значение, полученное от 15-го соседа
	case 215: //In15_OutMk Выдать МК соз значением, полученным от 15-го соседа
	case 315: //NeighbourMk_15_Set Установить МК для 15-го соседа
	case 415: //Parametr_14_Set Установить 15-й параметр
	case 515: //Parametr_14_Out Выдать 15-й параметр
	case 615: //Parametr_14_OutMk Выдать МК 15-й параметр
	case 16:  //In16_Set Принять значение от 16-го соседа
	case 116: //In16_Out Выдать значение, 16-му соседу
	case 216: //In16_OutMk Выдать МК соз значением, полученным от 16-го соседа
	case 316: //NeighbourMk_16_Set Установить МК для 16-го соседа
	case 416: //Parametr_15_Set Установить 16-й параметр
	case 516: //Parametr_15_Out Выдать 16-й параметр
	case 616: //Parametr_15_OutMk Выдать МК 16-й параметр
		if (MK / 100 == 1) // Выдать значение соседу
			Load.Write(Plys[PlyInd][MK % 100]);
		else if (MK / 100 == 2) // Выдать соседу МК со значением
			MkExec(Load, Plys[PlyInd][MK % 100]); 
		else if (MK / 100 == 3) // Установить ссылку на соседа
			Neighbours[MK % 100] = (CellularAutomat*)Load.Point;
		else if (MK / 100 == 4) // Установить МК для соседа
			NeighboursMk[MK % 100] = Load.ToInt(); // Установить МК для соседа
		else if (MK / 100 == 4) // Установить параметр
			parameters[MK % 100].WriteFromLoad(Load);
		else if (MK / 100 == 6) // Выдать МК с параметром
			MkExec(Load, parameters[MK % 100]);
		else if (MK / 100 == 0)// Приять значение от соседа
		{
			if(Plys[PlyCurrent][MK%100].Point==nullptr)
				InCounter[PlyCurrent]++;
			Plys[PlyCurrent][MK % 100] = Load.Clone();
			if (InCounter[PlyCurrent] == Neighbours.size()) // Набрался полный комплект
			{
				InComplectF[PlyCurrent][MK % 100] = true;
				RezReady[MK % 100] = true;
				ProgExec(CalcProg);
				RezReady[PlyCurrent] = true;
				if(AutoSend) ProgFU(890, { 0, nullptr });
				PlyCurrent++;
				InCounter[PlyCurrent] = 0;
				RezReady[PlyCurrent] = false;
				for(int i=0;i<Neighbours.size();i++)
					InComplectF[PlyCurrent][i]= false;
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
		NetType = Load.ToInt();
		break;
	case 3: // DimAdd Добавить измерение
		Dim.push_back(Load.ToInt());
	case 5: // DimClear Очистить вектора размерностей
		Dim.clear();
	case 10: // IniAutmataProgSet Установить программу инициализации автомата
		iniAutmataProg = Load.Point;
	default:
		CommonMk(MK, Load);
		break;
	}
}
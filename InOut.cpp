#include "stdafx.h"
#include "InOut.h"
#include <algorithm>
using namespace std;

void InOut::ProgFU(long int MK, LoadPoint Load, FU* Sender)
{
	MK %= FUMkRange;
	switch (MK)
	{
	case 0:  // Reset
		Channels.clear();
		Ind = -1; // не установлен индекс
		AutoInc = 0;
		CurrentCh = -1;
		break;
	case 1:  // IndSet установка индекса текущего канала
		Ind = Load.toInt();
		if (Ind < 0 || Ind >= Channels.size()) { ProgExec(OutOfChRange); }
		break;
	case 2: // IndOut
		Load.Write(Ind);
		break;
	case 3: // IndOutMk
		MkExec(Load, { Cint, &Ind });
		break;
	case 4:  // IndAdd  Прибавление к индексу значение из нагрузки (использовать, например, для инкремента индекса)
		Ind += Load.toInt();
		break;
	case 5:  // AutoIncSet // Величина автоинкрементации при любых действиях с каналами
		AutoInc = Load.toInt();
		break;

	case 6:  // OutOfChRangeErrProgSet // Установить программу обработки ошибки выхода за пределеы диапазона индексов канала
		OutOfChRange = Load.Point;
		break;
	case 7:  // IndLastSet Установить последний индекс канала в качестве текущего канала
		Ind = Channels.size() - 1;
		break;
	case 9: // NameIndSet Установить индекс по имени канала
	case 10: // NameSet Установить имя канала
		Channels[Ind]->Name = Load.toStr();
		break;
	case 11: // NameSetOut Выдать имя текущего канала
		Load.Write(Channels[Ind]->Name);
		break;
	case 12: // NameSetOutMk Выдать МК с именем текущего канала
		MkExec(Load, {Cstring, &Channels[Ind]->Name });
		break;
	case 15:  // ChCountOut Выдать количество каналов
		Load.Write(Channels.size());
		break;
	case 16:  // ChCountOutMk Выдать МК с количеством каналов;
	{
		long int t = Channels.size();
		MkExec(Load, {Cint, &t});
		break;
	}
	case 17:  // ChLastOut Вывод индекса последнего канала
		Load.Write(Channels.size() - 1);
		break;
	case 18:  // ChLastOutMk Вывод МК с индексом последнего канала
	{
		long int t = Channels.size() - 1;
		MkExec(Load, { Cint, &t });
		break;
	}
	case 20:  // ChClone Клонирование канала с индексом Ind
	{
		Channels.push_back(new Channel(*Channels[Ind]));
		Channels.back()->Name=Load.toStr();
		break;
	}
	case 21:  // ChCreate Создать новый канал (На входе может быть имя канала)
		Channels.push_back(new Channel({}));
		if (Load.isStr(Load.Type)) Channels.back()->Name = Load.toStr();//
		Ind = Channels.size() - 1;
		break;
	case 22:  // ChDelete // Удалить канал с индексом Ind
		Channels.erase(Channels.begin() + Ind);
		break;

	case 30:  // ChInAdrAdd Добавить адрес записи входного сигнала
		Channels[Ind]->InAdrBuf.push_back(Load);
		break;
	case 31:  // ChInAdrReset Очистить буфер адресов для записи входного сигнала
		Channels[Ind]->InAdrBuf.clear();
		break;
	case 32:  // ChInMkAdd Добавить милликоманду для рассылки входного сигнала
		Channels[Ind]->InMks.push_back(Load.toInt());
		break;
	case 33:  // ChInAtrSet Установить атрибут сигнала
		Channels[Ind]->InSignalAtr = Load.toInt();
		break;
	case 34:  // ChInMkAtrAdd  Добавить МК и атрибут для рассылки (FUName.ChMkAtrAdd=Mk=Atr)
		if (ChMkAtrCount == 0) 
		{
			 ChMkAtrCount++;
			 Channels[Ind]->InSignalAtr = Load.toInt();
		}
		else 
		{
			Channels[Ind]->InMks.push_back(Load.toInt());
			ChMkAtrCount = 0;
		}
		break;
	case 35:  // ChOutAdrAdd Добавить адрес для записи выходного значения
		Channels[Ind]->InAdrBuf.push_back(Load); // Добавить адрес для выходного сигнала канала
		break;
	case 36:  // ChOutAdrReset Очистить буфер адресов для записи входного сигнала
		Channels[Ind]->InAdrBuf.clear();
		break;
	case 37:  // ChOutMkAdd Добавить МК для выходного сигнала
		Channels[Ind]->InMks.push_back(Load.toInt());
		break;
	case 38:  // ChOutAtrSet Добавить атрибут для выходного сигнала
		Channels[Ind]->InSignalAtr = Load.toInt();
		break;
	case 39:  // ChOutMkAtrAdd Добавить МК и атрибут для выходного сигнала (FUName.ChOutMkAtrAdd=2=4)
		if (ChMkAtrCount == 0) 
		{
			Channels[Ind]->InMks.push_back(Load.toInt());
			ChMkAtrCount++;
		}
		else 
		{
			Channels[Ind]->InSignalAtr = Load.toInt();
			ChMkAtrCount = 0;
		}
		break;
	// OUT сделать	
	case 40: // ChOutMkClear Очистить буфер МК для выходного сигнала
		Channels[Ind]->InMks.clear();
		break;

	case 50:  // ChReceiveProgSet Установить программу реакции на входной сигнал
		Channels[Ind]->InProg = Load.Point;
		Ind += AutoInc;
		break;
	case 51:  // ChSendProgSet Установить программу реакции на входной сигнал
		Channels[Ind]->OutProg = Load.Point;
		Ind += AutoInc;
		break;
	case 53:  // ChInPollProgSet Установить программу опроса входного сигнала
		Channels[Ind]->InPollProg = Load.Point;
		Ind += AutoInc;
		break;
	case 54:  // ChOutPollProgSet Установить программы опроса выходного сигнала
		Channels[Ind]->OutPollProg = Load.Point;
		Ind += AutoInc;
		break;
/*
	case 55:  // ChValSet Установить значение текущего канала
		Channels[Ind]->PrevSignals.push_back(Channels[Ind]->Signal);
		if (Channels[Ind]->PrevSignals.size() >= ArchSignalVolume)
		{
			Channels[Ind]->PrevSignals.erase(Channels[Ind]->PrevSignals.begin());
		}
		Channels[Ind]->Signal = Load.toDouble();
		
		for (auto i : Channels[Ind]->InAdrBuf)
		{
			i.WriteFromLoad(Load);
		}
		for (auto i : Channels[Ind]->InMks)
		{
			MkExec(i, Load);
		}
		ProgExec(Channels[Ind]->InProg);
		if (Channels[Ind]->Borders != nullptr) 
		{
			Channels[Ind]->Borders->Run(Load);
		}
		Ind += AutoInc;
		break;		

	case 56:  // ChValOut Выдать значение текущего канала
		Load.Write(Channels[Ind]->Signal);
		Ind += AutoInc;
		break;
	case 57:  // ChValOutMk Выдать МК со значением текущего канала
		MkExec(Load, {Cdouble, &Channels[Ind]->Signal});
		Ind += AutoInc;
		break;
*/
	case 58:  // ChDiffValOut Выдать разницу двух последних значений текущего канала
		if (Channels[Ind]->PrevSignals.size()) 
		{
			Load.Write(Channels[Ind]->Signal - Channels[Ind]->PrevSignals.back());
		}
		Ind += AutoInc;
		break;
	case 59:  // ChDiffValOutMk Выдать МК с разницей двух последних значений текущего канала
		if (Channels[Ind]->PrevSignals.size()) 
		{
			double t = Channels[Ind]->Signal - Channels[Ind]->PrevSignals.back();
			MkExec(Load, {Cdouble, &t});
		}
		Ind += AutoInc;
		break;

	case 60:  // BorderSet Установить значение порога
		if (Channels[Ind]->Borders == nullptr)
		{
			Channels[Ind]->Borders = new border;
		}
		Channels[Ind]->Borders->borders.push_back(Load.toDouble());
		Channels[Ind]->Borders->UpProg.push_back(nullptr);
		Channels[Ind]->Borders->DownProg.push_back(nullptr);
		// что-то с Parent

	case 61:  // BorderUpProgSet Установить программу реакции на проход порога снизу вверх
		if (Channels[Ind]->Borders == nullptr) { break; }
		Channels[Ind]->Borders->UpProg.back()=Load.Point;
		break;
	case 62:  // BorderDownProgSet Установить программу реакции на проход порога свезху вниз
		if (Channels[Ind]->Borders == nullptr) { break; }
		Channels[Ind]->Borders->DownProg.back()=Load.Point;
		break;
	case 63:  // BorderReset Сброс порогов
		Channels[Ind]->Borders->Reset();
		Channels[Ind]->Borders->Start = true; // Установить флаг начала измерений
		Ind += AutoInc;
		break;
	case 64:  // BorderActiveSet Установить флаг активность порогов (true по умолчанию)
		Channels[Ind]->Borders->Active = Load.toBool(true);
		Ind += AutoInc;
		break;
	case 65:  // ChPollStepSet // Установить период опроса канала
		Channels[Ind]->Period = Load.toDouble();
		Ind += AutoInc;
		break;

	case 99: //SignalOut Выдать сигнал
	case 100: //SignalOut_0
	case 101:
	case 102:
	case 103:
	case 104:
	case 105:
	case 106:
	case 107:
	case 108:
	case 109:
	case 110:
	case 111:
	case 112:
	case 113:
	case 114:
	case 115: //SignalOut_15
	{
		long int t=Ind;
		if (MK != 99) t = MK - 100; // Вычисление индекса канала
		if (t < 0 || t >= Channels.size()) { ProgExec(OutOfChRange); break; }
		Load.Write(Channels[t]->Signal);
	}
	break;

	case 149: //SignalOutMk Выдать MK с сигналом
	case 150: //SignalOutMk_0
	case 151:
	case 152:
	case 153:
	case 154:
	case 155:
	case 156:
	case 157:
	case 158:
	case 159:
	case 160:
	case 161:
	case 162:
	case 163:
	case 164:
	case 165: //SignalOut_15
	{
		long int t = 149;
		if (MK != 149) t = MK - 150; // Вычисление индекса канала
		if (t < 0 || t >= Channels.size()) { ProgExec(OutOfChRange); break; }
		MkExec(Load, { Cdouble, &Channels[t]->Signal });
	}
	break;

	case 199: //SignalReceive Установить сигнал
	case 200: //SignalReceive_0
	case 201:
	case 202:
	case 203:
	case 204:
	case 205:
	case 206:
	case 207:
	case 208:
	case 209:
	case 210:
	case 211:
	case 212:
	case 213:
	case 214:
	case 215: //SignalReceive_15
	{
		long int t = Ind;
		if (MK != 199) t = MK - 200; // Вычисление индекса канала
		if (t < 0 || t >= Channels.size()) { ProgExec(OutOfChRange); break; }
		LastReceiveChInd = t;
		Channels[t]->PrevSignals.push_back(Channels[t]->Signal);
		if (Channels[t]->PrevSignals.size() >= Channels[t]->SignalArchSize)
			Channels[t]->PrevSignals.erase(Channels[t]->PrevSignals.begin());
		Channels[t]->Signal = Load.toDouble();
		for (auto i : Channels[t]->InAdrBuf)
			i.Write(Channels[t]->Signal);
		for (auto i : Channels[t]->InMks)
			if (Channels[t]->InAtr == 0)
				MkExec(i,Load);
			else
			{
				ip ipt = { Channels[t]->InAtr,Load};
				MkExec(i, Load);
			}
		ProgExec(Channels[t]->ReceiveProg);
		if (Channels[t]->Borders != 0) // Запуск программ по границам
			Channels[t]->Borders->Run(Load);
	}
	break;

	case 249: //SignalSend Выдать MK Cсигналом
	case 250: //SignalSend_0
	case 251:
	case 252:
	case 253:
	case 254:
	case 255:
	case 256:
	case 257:
	case 258:
	case 259:
	case 260:
	case 261:
	case 262:
	case 263:
	case 264:
	case 265: //SignalSend_15
	{
		long int t = Ind;
		if (MK != 249) t = MK - 250; // Вычисление индекса канала
		if (t < 0 || t >= Channels.size()) { ProgExec(OutOfChRange); break; }
		for (auto i : Channels[t]->OutAdrBuf)
			i.Write(Channels[t]->Signal);
		for (auto i : Channels[t]->OutMkBuf)
			if (Channels[t]->OutAtr == 0)
				MkExec(i, Load);
			else
			{
				ip ipt = { Channels[t]->OutAtr,Load };
				MkExec(i, Load);
			}
// ....
	}
	case 299: //ReceiveProgSet Установить программу при приходе входного сигнала
	case 300: //ReceiveProgSet_0
	case 301:
	case 302:
	case 303:
	case 304:
	case 305:
	case 306:
	case 307:
	case 308:
	case 309:
	case 310:
	case 311:
	case 312:
	case 313:
	case 314:
	case 315: //ReceiveProgSet_15
	{
		long int t = Ind;
		if (MK != 199) t = MK - 200; // Вычисление индекса канала
		if (t < 0 || t >= Channels.size()) { ProgExec(OutOfChRange); break; }
		Channels[t]->ReceiveProg = Load.Point;
	}
		break;

	default:
		CommonMk(MK, Load);
	}
}


void AutomatManager::ProgFU(long int MK, LoadPoint Load, FU* Sender)
{
	switch (MK)
	{
	case 0: // Reset
		ReceiverMk = -1;
		if(Template!=nullptr)
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

	case 3: // RecieverMKSet Установить милликоманду для приемника сигнала
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
			Template = (IC_type)ICCopy(Load).Point;
		break;
	case 11: // TemplClear Очистить ИК шаблона
		(*(IC_type)Template).clear();
		break;
	case 12: //TemplOut Выдать шаблон
		Load.Write(Template);
		break;
	case 13: //TemplOutMk выдать МК с указателем на шаблон ИК
		MkExec(Load, { CIC, Template });
		break;

	case 20: // SignalSet Установить сигнал
	{
		if (Template == nullptr) // Выдача каждого сигнала напрямую потребителю
		{
			if(Receiver==nullptr)
				MkExec(ReceiverMk, Load);
			else
				MkExec(ReceiverMk, Load, (FU*)Receiver);
			break;
		}
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
		CommonMk(MK, Load, Sender);
	}
}

void border::Run(LoadPoint Load) {
	if (!borders.size()) return; 
	double val = Load.toDouble();
	if (!borders.size()) return;
	if (Start)
	{
		OldVal = val;
		Start = false;
		return;
	}
	auto uk = borders.begin();
	for (; uk != borders.end() && *uk <= val, val; uk++);
	if (uk == borders.end())
	{
		if (OldVal <= borders.back())
			Parent.ProgExec(UpProg.back());
	}
	else if (OldVal >= *(uk+1))
			Parent.ProgExec(DownProg[distance(borders.begin(),uk)+1]);
		else if (uk != borders.begin() && OldVal <= *(uk-1))
				Parent.ProgExec(UpProg[distance(borders.begin(), uk)-1]);
	OldVal = val;
};

FU* InOut::Copy() // Программа копирования ФУ
{
	return new InOut(Bus, this);
}

FU* InOut::TypeCopy() // Создать ФУ такого же типа (не копируя контекст
{
	return new InOut(Bus, nullptr);
}

FU* AutomatManager::Copy() // Программа копирования ФУ
{
	return new AutomatManager(Bus, this);
}

FU* AutomatManager::TypeCopy() // Создать ФУ такого же типа (не копируя контекст
{
	return new AutomatManager(Bus, nullptr);
}

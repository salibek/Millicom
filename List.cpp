// ФУ Список
#include "stdafx.h"
#include "List.h"

using namespace std;

void List::ProgFU(int MK, LoadPoint Load)
{
	ProgExec(PrefixProg); // Предварительная программа, вызываемая при приходе любой МК
	switch (MK)
	{
	case 0: // Reset
		Searcher.Clear();
		Stack.clear();// Сделать потом очистку списка
		Stack.push_back({});
		break;
	case 1:	// Set
		Stack.back().ListHead = (IC_type)Load.Point;
		break;
	case 2:// Out Выдать ссылку на список
		if (Load.Type >> 1 == Cvoid)
			*(void**)Load.Point = Stack.back().ListHead;
		break;
	case 3:// OutMk Выдать МК со ссылкой на список
		MkExec(Load, { TIC,(void*)Stack.back().ListHead });
		break;
	case 5:	// MultiLineModeSet 
		MultiLineMode = Load.ToInt(1);
		break;
	case 6: // MultyListModeSet Установить режим многоуровневого списка для поиска (true по умолчанию)
		MultyListMode = Load.ToBool(true);
		break;
	case 7: // MkModeSet Режим выполнения всех МК в ИК-шаблоне (МК-ой считается любой атрибут, индекс которого больше 0) При пустой нагрузке режим устанавливается
		Searcher.MkMode = Load.ToBool(true);
		break;
	case 8:// EmptyProgExec Выполнить программу, если список пуст
	case 9:// FullExec Выполнить программу, если список не пуст
	case 14: // OneLineExec Выполнить программу, если в списке только одна строка
	case 13: // MoreOneLineExec Выполнить программу, если в списке больше одной строка
		if (MK==8 && (Stack.back().ListHead == nullptr || Stack.back().ListHead->size() == 0)||\
			Stack.back().ListHead != nullptr && Stack.back().ListHead->size() != 0 &&\
			(MK==9 || MK == 14 && Stack.back().ListHead->size() == 1 ||\
				MK == 13 && Stack.back().ListHead->size() >1 ) )
			if (Load.Point != nullptr && Load.Type >> 1 == DIC)
				ProgExec(Load);
			else
				ProgExec(Prog);
		else if(Load.Point == nullptr)
			ProgExec(ElseProg);
		break;
	case 10: // SuccessLineProgSet Установить указатель на программу, выполняемую при удачном поиска в линии списка
		Searcher.SuccessProg = (IC_type)Load.Point;
		break;
	case 11: // SuccessAfterLineProgSet Установить указатель на программу, выполняемую при удачном поиска в линии списка после обоработки программ линии
		Searcher.SuccessAfterProg = (IC_type)Load.Point;
		break;
	case 12: // SuссessProgSet Установить указатель на программу, выполняемую в случае удачного поиска во всем  списке
		SuссessProg = Load.Point;
		break;
	case 15: // FailLineProgSet Установить указатель на программу, выполняемую в случае неудачного поиска в линии списка
		Searcher.FailProg = (IC_type)Load.Point;
		break;
	case 16: // FailAfterLineProgSet Установить указатель на программу, выполняемую в случае неудачного поиска в линии списка после обоработки программ линии
		Searcher.FailAfterProg = (IC_type)Load.Point;
		break;
	case 17: // FailProgSet Установить указатель на программу, выполняемую в случае неудачного поиска во всем  списке
		FailProg = Load.Point;
		break;
	case 18: //  Установить указатель на программу, выполняемую в случае удачного поиска в текущей линии
		SuссessLineProg = Load.Point;
		break;
	case 20: // ProgAtrSet Установить атрибут линии списка по умолчанию
		if (Load.Point != nullptr && Load.Type >> 1 == Dint)
			Searcher.Prog_atr = Load.ToInt();
		break;
	case 21: //LineAtrDef Установить атрибут марки списка по умолчанию
		LineAtr = Load.ToInt();
		break;
	case 22: //SubAtrDef Установить атрибут подсписка по умолчанию
		SubAtr = Load.ToInt();
		break;
	case 25: // BackOut Вылать входной объект для поиска
		if (Load.Type == TPPoint)
			(*(LoadPoint*)Load.Point) = Searcher.Obj;
		break;
	case 30: // BackOutMk Вылаить МК со входным объектом для поиска
		MkExec(Load, Searcher.Obj.Clone());
		break;
	case 31: // CopyBackOutMk Вылаить МК со входным объектом для поиска
		MkExec(Load, Searcher.Obj.Clone());
		break;

	case 35: // LoadBackOut Выдать нагрузку входного объекта для поиска
		if (Searcher.Obj.Type >> 1 == DIC)
			Load.Point = ((IC_type)(Searcher.Obj.Point))->begin()->Load.Point;
		else if (Searcher.Obj.Type >> 1 == DIP)
			Load.Point = ((ip*)(Searcher.Obj.Point))->Load.Point;
		break;
	case 36: // LoadBackOutMk Выдать МК с нагрузкой входного объекта для поиска
		if (Load.Type >> 1 == Dint)
			if(Searcher.Obj.Type>>1==DIC)
				MkExec(*((int*)Load.Point), ((IC_type)(Searcher.Obj.Point))->begin()->Load);
			else if(Searcher.Obj.Type >> 1 == DIP)
				MkExec(*((int*)Load.Point), ((ip*)(Searcher.Obj.Point))->Load);
		break;

	case 100: // RezOut Выдача результата сравнения
		if (Load.Type == Tbool)
			Load.Write(Searcher.Rez);
		break;
	case 101: // RezOutMk Выдача МК с результатом сравнения
		MkExec(*(int*)(Load.Point), { Cbool , &Searcher.Rez });
		break;
	case 105: // MkAtrAdd Добавить МК
		if (Load.Type >> 1 == Dint)
			Searcher.MkAtrAdd(*(int*)Load.Point);
		break;
	case 106: // MkAtrClear Очистить список МК
		Searcher.MkAtrClear();
		break;

	case 120: // MarkAtrSet Установить атрибут текущей строки
		if (Stack.back().LineUk != nullptr)
			Stack.back().LineUk->atr = Load.ToInt();
		break;
	case 121: // LastMarkAtrSet Установить атрибут последней строки
		if (Stack.back().ListHead != nullptr)
			(Stack.back().ListHead->end() - 1)->atr = Load.ToInt();
		break;
	case 122: //MarkAtrOut Выдать маркер текущей строки
		if (Stack.back().LineUk != nullptr)
			Load.Write(Stack.back().LineUk->atr);
		break;
	case 123: //MarkAtrOutMk Выдать МК с маркером текущей строки
		if (Stack.back().LineUk != nullptr)
			MkExec(Load, { Cint, &Stack.back().LineUk->atr });
		break;
	case 124: //LastMarkAtrOut Выдать маркер последней строки
		if(Stack.back().ListHead!=nullptr && !Stack.back().ListHead->size())
			Load.Write((Stack.back().ListHead->end()-1)->atr);
		break;
	case 129: //LastMarkAtrOutMk Выдать МК с маркером последней строки
		if (Stack.back().ListHead != nullptr && !Stack.back().ListHead->size())
			MkExec(Load, { Cint, &(Stack.back().ListHead->end() - 1)->atr });
		break;
	case 125: //  MarkAtrInc Инкремент атрибута текущей строки
		if (Stack.back().LineUk != nullptr)
			Stack.back().LineUk->atr++;
		break;
	case 126: // LastMarkAtrInc Инкремент атрибута последней строки
		if (Stack.back().ListHead != nullptr)
			(Stack.back().ListHead->end() - 1)->atr++;
		break;
	case 127: //  MarkAtrDec Декримент атрибута текущей строки
		if (Stack.back().LineUk != nullptr)
			Stack.back().LineUk->atr--;
		break;
	case 128: // LastMarkAtrDec Декримент атрибута последней строки
		if (Stack.back().ListHead != nullptr)
			(Stack.back().ListHead->end() - 1)->atr--;
		break;
	case 130: // MarkAtrEQExec Запуск программы при нуле/равенстве атрибута текущей строки или равестве c нагрузкой МК
		if (Stack.back().ListHead == nullptr || Stack.back().ListHead->size() == 0 || Stack.back().LineUk == nullptr) break;
		if (Load.Type >> 1 == DIC) {
			if (!Stack.back().LineUk->atr)
				ProgExec(Load);
		}
		else if(Stack.back().LineUk->atr == Load.ToInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 131: // LastMarkEQExec Запуск программы при нуле/равенстве атрибута последней строки или равестве c нагрузкой МК
		if (Stack.back().ListHead == nullptr || Stack.back().ListHead->size() == 0) break;
		if (Load.Type >> 1 == DIC) {
			if (!(Stack.back().ListHead->end() - 1)->atr )
				ProgExec(Load);
		}
		else if ((Stack.back().ListHead->end() - 1)->atr == Load.ToInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 133: // MarkAtrNEQExec Запуск программы при не нуле/неравенстве атрибута текущей строки
		if (Stack.back().ListHead == nullptr || Stack.back().ListHead->size() == 0 || Stack.back().LineUk==nullptr) break;
		if (Load.Type >> 1 == DIC) {
			if (Stack.back().LineUk->atr)
				ProgExec(Load);
		}
		else if (Stack.back().LineUk->atr != Load.ToInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 134: // LastMarkAtrNEQExec Запуск программы при не нуле/неравенстве атрибута последней строки
		if (Stack.back().ListHead == nullptr || Stack.back().ListHead->size() == 0) break;
		if (Load.Type >> 1 == DIC) {
			if ((Stack.back().ListHead->end() - 1)->atr)
				ProgExec(Load);
		}
		else if ((Stack.back().ListHead->end() - 1)->atr != Load.ToInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 135: // MarkAtrBiggerExec Запуск программы при атрибуте текущей строки больше (больше 0)
		if (Stack.back().ListHead == nullptr || Stack.back().ListHead->size() == 0 || Stack.back().LineUk == nullptr) break;
		if (Load.Type >> 1 == DIC) {
			if (Stack.back().LineUk->atr>0)
				ProgExec(Load);
		}
		else if (Stack.back().LineUk->atr > Load.ToInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 136: // LastMarkAtrBiggerExec Запуск программы при атрибуте последней строки больше (больше 0)
		if (Stack.back().ListHead == nullptr || Stack.back().ListHead->size() == 0) break;
		if (Load.Type >> 1 == DIC) {
			if (!(Stack.back().ListHead->end() - 1)->atr>0)
				ProgExec(Load);
		}
		else if ((Stack.back().ListHead->end() - 1)->atr > Load.ToInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 137: // MarkAtrBiggerEQExec Запуск программы при атрибуте текущей строки больше или райным 0
		if (Stack.back().ListHead == nullptr || Stack.back().ListHead->size() == 0 || Stack.back().LineUk == nullptr) break;
		if (Load.Type >> 1 == DIC) {
			if (Stack.back().LineUk->atr>=0)
				ProgExec(Load);
		}
		else if (Stack.back().LineUk->atr >= Load.ToInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 138: // LastMarkAtrBiggerEQExec Запуск программы при атрибуте последней строки больше или равным 0
		if (Stack.back().ListHead == nullptr || Stack.back().ListHead->size() == 0) break;
		if (Load.Type >> 1 == DIC) {
			if ((Stack.back().ListHead->end() - 1)->atr>=0)
				ProgExec(Load);
		}
		else if ((Stack.back().ListHead->end() - 1)->atr >= Load.ToInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 139: // MarkAtrLessExec Запуск программы при атрибуте текущей строки меньше 0
		if (Stack.back().ListHead == nullptr || Stack.back().ListHead->size() == 0 || Stack.back().LineUk == nullptr) break;
		if (Load.Type >> 1 == DIC) {
			if (Stack.back().LineUk->atr<0)
				ProgExec(Load);
		}
		else if (Stack.back().LineUk->atr < Load.ToInt()) 
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 140: // LastMarkAtrLessExec Запуск программы при атрибуте последней строки меньше 0
		if (Stack.back().ListHead == nullptr || Stack.back().ListHead->size() == 0) break;
		if (Load.Type >> 1 == DIC) {
			if ((Stack.back().ListHead->end() - 1)->atr <0)
				ProgExec(Load);
		}
		else if ((Stack.back().ListHead->end() - 1)->atr < Load.ToInt()) 
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 141: // MarkAtrLessEQExec Запуск программы при атрибуте текущей строки меньше или равным 0
		if (Stack.back().ListHead == nullptr || Stack.back().ListHead->size() == 0 || Stack.back().LineUk == nullptr) break;
		if (Load.Type >> 1 == DIC) {
			if (Stack.back().LineUk->atr <= 0)
				ProgExec(Load);
		}
		else if (Stack.back().LineUk->atr <= Load.ToInt()) 
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 142: // LastMarkAtrLessEQExec Запуск программы при атрибуте последней строки меньше или равным 0
		if (Stack.back().ListHead == nullptr || Stack.back().ListHead->size() == 0) break;
		if (Load.Type >> 1 == DIC) {
			if ((Stack.back().ListHead->end() - 1)->atr<=0)
				ProgExec(Load);
		}
		else if ((Stack.back().ListHead->end() - 1)->atr <= Load.ToInt()) 
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;

	case 150: //LastOut Выдать ссылку на последнюю линию списка
	case 151: //LastPop Выдать ссылку на последнюю линию списка и удалить из списка
	case 152: //LastDel Выдать ссылку на последнюю линию списка и удалить из ИК
		if (Load.Type == Tvoid)
			*(void**)Load.Point = (void*)Stack.back().ListHead->back().Load.Point;
		if (MK == 152) ICDel((void*)Stack.back().ListHead->back().Load.Point);
		if (MK == 151 || MK == 152) 
			Stack.back().ListHead->pop_back();
		break;
	case 153: //LastIpOutMk Выдать ссылку на последнюю ИП последней линии
		MkExec(Load, { CIP,(void*)&((IC_type)Stack.back().ListHead->back().Load.Point)->back() });
		break;
	case 154: // LastIpCopyOutMk Выдать МК со ссылкой на последнюю ИП последней линии
		MkExec(Load, { CIC,((IC_type)Stack.back().ListHead->back().Load.Point)->back().Сlone() });
		break;
	case 155: //LastOutMK Выдать ссылку на последнюю линию списка
	case 156: //LastPopMK Выдать ссылку на последнюю линию списка и удалить из списка
	case 157: //LastDelMk Выдать ссылку на последнюю линию списка и удалить из ИК
		if (Stack.back().ListHead == nullptr || !Stack.back().ListHead->size()) break;
		MkExec(Load, Stack.back().ListHead->back().Load);
		if (MK == 157) ICDel((void*)Stack.back().ListHead->back().Load.Point);
		if (MK == 156 || MK == 157) 
			Stack.back().ListHead->pop_back();
		break;
	case 158: // LastLoadOut Выдать нагрузку последней ИП последней линии
		Load.Write(((IC_type)(Stack.back().ListHead->back().Load.Point))->back().Load);
//		*(LoadPoint*)Load.Point = ((IC_type)(Stack.back().ListHead->back().Load.Point))->back().Load;
		break;
	case 169: // LastLoadOutMk Выдать МК с нагрузкой последней ИП последней линии
		MkExec(Load, ((IC_type)(Stack.back().ListHead->back().Load.Point))->back().Load);
		break;
	case 145: //LineIpOutMk Выдать ссылку на последнюю ИП текущей линии
		if (Stack.back().ListHead == nullptr || !Stack.back().ListHead->size() || Stack.back().LineUk==nullptr) break;
		MkExec(Load, { CIP,&((IC_type)Stack.back().LineUk->Load.Point)->back() });
		break;
	case 146: // LineIcCopyOutMk Выдать МК со ссылкой на последнюю ИП текущей линии
//		MkExec(Load, { CIC,((IC_type)Stack.back().ListHead->back().Load.Point)->back().СloneToIC() });
		break;
	case 147: // LastIpOutMk Выдать ссылку на последнюю ИП текущей линии
		MkExec(Load, { CIP,&((IC_type)Stack.back().ListHead->back().Load.Point)->back() });
		break;
	case 163: // LastCopyAddPrevLoadSet
	case 159: // LastCopyAddPrevLoadSetLoadMov Добавить копию линии перенести нагрузку в на новую строку и добавить в нагрзуку предыдущей строки ссылку на новую строку 
	{
		LoadPoint t = { 0, nullptr };
		if (Stack.back().ListHead != nullptr && Stack.back().ListHead->size() && Stack.back().ListHead->back().Load.Type>>1==DIC)\
			t = ((IC_type)Stack.back().ListHead->back().Load.Point)->back().Load;
		if (Stack.back().ListHead == nullptr) Stack.back().ListHead = new vector<ip>;
		if (Load.Point == nullptr)
			Stack.back().ListHead->push_back({ LineAtr, TIC, new vector<ip> });
		else
			Stack.back().ListHead->push_back({ LineAtr, TIC, ICCopy(Load) });
		if (Stack.back().ListHead->size() > 1 && Stack.back().ListHead->back().Load.Point!=nullptr && Stack.back().ListHead->back().Load.Type>>1==DIC)
			((IC_type)Stack.back().ListHead->at(Stack.back().ListHead->size() - 2).Load.Point)->back().Load = Stack.back().ListHead->back().Load;
		if (MK == 159)
			((IC_type)Stack.back().ListHead->back().Load.Point)->back().Load = t;
		break;
	}
	case 160: // LineAdd Добавить новую строку всписок
		if (Stack.back().ListHead == nullptr) Stack.back().ListHead = new vector<ip>;
		if (Stack.back().ListHead == nullptr)
			Stack.back().ListHead = new vector<ip>;
		if(Load.Point!=nullptr)
			Stack.back().ListHead->push_back({ LineAtr, Load });
		break;
	case 161: // LineCopyAdd Добавить копию строки
		if (Stack.back().ListHead == nullptr) Stack.back().ListHead = new vector<ip>;
		if (Load.Point != nullptr)
			Stack.back().ListHead->push_back({ LineAtr, TIC, ICCopy(Load) });
		break;
//	case 163: //  LineCopyAddPrevLoadSet Добавить линию в список и поместить ссылку на нее в нагрузку последней ИП последней строки
//	 	if (Stack.back().ListHead == nullptr) Stack.back().ListHead = new vector<ip>;
//		if(Load.Point==nullptr)
//			Stack.back().ListHead->push_back({ LineAtr, TIC, new vector<ip> });
//		else
//			Stack.back().ListHead->push_back({ LineAtr, TIC, ICCopy(Load) });
//		if (Stack.back().ListHead->size() > 1)
//			((IC_type)Stack.back().ListHead->at(Stack.back().ListHead->size() - 2).Load.Point)->back().Load = Stack.back().ListHead->back().Load;
//		break;
	case 162: // LineCopyTreeAdd Добавить копию ОА-графа
		if (Stack.back().ListHead == nullptr) Stack.back().ListHead = new vector<ip>;
		// ....
		break;
	case 164: // LineLoadOutMk Выдать МК с нагрузкой последней ИП текущей линии
		MkExec(Load, ((IC_type)(Stack.back().LineUk->Load.Point))->back().Load);
		break;
	case 165: // LinePopMk Исключить линию списка
		if (Stack.back().ListHead == nullptr || Stack.back().ListHead->size() == 0) break;
		MkExec(Load, { TIC , (void*)Stack.back().ListHead->back().Load.Point });
		Stack.back().ListHead->pop_back();
		break;
	case 166: // LineDelMk Выдать МК со ссылкой на ИК и удалить ИК
	{
		if (Stack.back().ListHead == nullptr || Stack.back().ListHead->size() == 0) break;
		LoadPoint t = Stack.back().ListHead->back().Load;
		Stack.back().ListHead->pop_back();
		MkExec(Load, t);
		ICDel(Stack.back().ListHead->back().Load.Point);
		break;
	}
	case 167: // LineIpCut Удалить последние ИП из текущей линии (в нагрузке количество удаляемых ИП, по умолчанию 1)
	case 168: // LineVarIpCut" Hint = "Удалить последние ИП c пемеменными из текущей линии (в нагрузке количество удаляемых ИП, по умолчанию 1)" }
		if (Stack.back().LineUk != nullptr && Stack.back().LineUk->Load.Type>>1==DIC && Stack.back().LineUk->Load.Point!=nullptr)
		{
			int n;
			if (Load.Point == nullptr || Load.Type >> 1 != Dint)
				n = 1;
			else
				n = *(int*)Load.Point;
			while (n-- && ((IC_type)Stack.back().LineUk->Load.Point)->size() != 0)
			{
				((IC_type)Stack.back().LineUk->Load.Point)->resize(((IC_type)Stack.back().LineUk->Load.Point)->size() - 1);
			}
		}
		break;
	case 170: // LastAttach Конкатенация ИК к последней линии списка
	case 171: // LastCopyAttach Конкатенация копии ИК к последней линии списка
	case 172: // LastCopyAttachLoadMove Добавить ИК и переписать последнюю нагрузку
	{
		if (Stack.back().ListHead == nullptr)
		{
			Stack.back().ListHead = new vector<ip>;
			Stack.back().ListHead->push_back({ LineAtr,{ CIC,new vector<ip>} });
		}
		if (Stack.back().ListHead != nullptr && Load.Point != nullptr)
			if (Load.Type >> 1 == DIP || Load.Type >> 1 == DIC)
			{
				if (Load.Type >> 1 == DIP)
				{
					((IC_type)Stack.back().ListHead->back().Load.Point)->push_back(*(ip*)Load.Point);
					if (MK == 170 && Load.Type == CIP) { delete (ip*)Load.Point; Load.Point = nullptr; Load.Type = 0; }
				}
				else if (Load.Type >> 1 == DIC)
				{
					for (auto i : *(IC_type)Load.Point)
						((IC_type)Stack.back().ListHead->back().Load.Point)->push_back(i);
					if (MK == 170 && Load.Type == CIP) { delete (ip*)Load.Point; Load.Point = nullptr; Load.Type = 0; }
				}
			}
			else
			{
				if (Load.Type >> 1 == DIC)
				{
					copy(((IC_type)Load.Point)->begin(), ((IC_type)Load.Point)->end(), inserter(*((IC_type)Stack.back().ListHead->back().Load.Point), ((IC_type)Stack.back().ListHead->back().Load.Point)->end()));
					if (MK == 170 && Load.Type == CIP) { ICDel(Load.Point); Load.Point = nullptr; Load.Type = 0; };
				}
			}
		break;
	}
	case 173: // LastCopyGrahpAttach
		// ....
		break;
	case 175: // LineAttach Конкатенация ИК к текущей линии списка
	case 176: // LineCopyAttach Конкатенация копии ИК к текущей линии списка
		if (Stack.back().LineUk != nullptr && Load.Point != nullptr)
			if (Load.Type >> 1 == DIP)
			{
				((IC_type)Stack.back().LineUk->Load.Point)->push_back(*(ip*)Load.Point);
				if (MK == 175 && Load.Type == CIP) { delete (ip*)Load.Point; Load.Point = nullptr; Load.Type = 0; }
			}
			else
			{
				if (Load.Type >> 1 == DIC)
				{
					copy(((IC_type)Load.Point)->begin(), ((IC_type)Load.Point)->end(), inserter(*((IC_type)Stack.back().LineUk->Load.Point), ((IC_type)Stack.back().LineUk->Load.Point)->end()));
					if (MK == 175 && Load.Type == CIP) { ICDel(Load.Point); Load.Point = nullptr; Load.Type = 0; };
				}
			}
		break;
	case 178: // LineCopyAttach Конкатенация копии ИК к текущей линии списка
		if (Stack.back().LineUk != nullptr && Load.Point != nullptr)
			if (Load.Type >> 1 == DIP)
			{
				((IC_type)Stack.back().LineUk->Load.Point)->push_back(*(ip*)Load.Point);
				if (MK == 175 && Load.Type == CIP) { delete (ip*)Load.Point; Load.Point = nullptr; Load.Type = 0; }
			}
			else
			{
				if (Load.Type >> 1 == DIC)
				{
					copy(((IC_type)Load.Point)->begin(), ((IC_type)Load.Point)->end(), inserter(*((IC_type)Stack.back().LineUk->Load.Point), ((IC_type)Stack.back().LineUk->Load.Point)->end()));
					if (MK == 175 && Load.Type == CIP) { ICDel(Load.Point); Load.Point = nullptr; Load.Type = 0; };
				}
			}
		break;
	case 179: // LineCopyGrahpAttach
			  // ....
		break;
	case 185: // LastLoadSet Записать адрес в нагрузку последней ИП последней строки
	case 195: // LineLoadSet Записать адрес в нагрузку последней ИП последней строки
	case 186: // LastLoadCopySet Записать адрес в нагрузку последней ИП последней строки
	case 196: // LineLoadCopySet Записать адрес в нагрузку последней ИП последней строки
	case 187: // LastPointLoadCopyGraphSet
	case 197: // LinePointLoadCopyGraphSet
	case 189: // LastAtrSet Установить атрибут у последней ИК последней линии 
	case 199: // LineAtrSet Установить атрибут у последней ИК последней линии 
	{	IC_type t;
		if (Stack.back().ListHead == nullptr || Stack.back().ListHead->back().Load.Point == nullptr || Stack.back().ListHead->back().Load.Type >> 1 != DIC) break;
		if (((IC_type)Stack.back().ListHead->back().Load.Point)->size() == 0) break;
		if (MK < 190)
			t = ((IC_type)Stack.back().ListHead->back().Load.Point);
		else if (Stack.back().LineUk != nullptr && Stack.back().LineUk->Load.Point != nullptr && Stack.back().LineUk->Load.Type >> 1 != DIC)
			break;
		else
			t = (IC_type)Stack.back().LineUk->Load.Point;
		switch (MK)
			{
			case 185:
			case 195:
				t->back().Load = Load;	break;
			case 186:
			case 196:
				if (Load.Type >> 1 != DIC)
					t->back().Load.Copy(Load);
				else
				{
					t->back().Load.Type = Load.Type;
					t->back().Load.Point = ICCopy(Load);
				}
				break;
			case 189:
			case 199:
				if (Load.Type >> 1 == Dint)
					t->back().atr = *(int*)Load.Point;
				break;
				//	case 177: // LastPointLoadCopyGraphSet
						// ...
						//break;
				}

		break;
	}
	case 180: // LineVarTypeSet // Установить тип 'переменная' в последней ИП текущей линии
		if (Stack.back().ListHead == nullptr || !Stack.back().ListHead->size() || \
			Stack.back().LineUk == nullptr || Stack.back().LineUk->Load.Point == nullptr || \
			Stack.back().LineUk->Load.Type>>1!=DIC || !((IC_type)Stack.back().LineUk->Load.Point)->size())
			break;
		((IC_type)Stack.back().LineUk->Load.Point)->back().Load.VarTypeSet(Load.ToBool(true));
		break;
	case 182: // LastVarTypeSet // Установить тип 'переменная' в последней ИП последней линии
		if (Stack.back().ListHead == nullptr || !Stack.back().ListHead->size() || \
			Stack.back().ListHead->back().Load.Type >> 1 != DIC || !((IC_type)Stack.back().ListHead->back().Load.Point)->size())
			break;
		((IC_type)Stack.back().ListHead->back().Load.Point)->back().Load.VarTypeSet(Load.ToBool(true));
		break;
	case 200: // LineToLast Установить текущую строку на последнюю строку
		if(Stack.back().ListHead!=nullptr && Stack.back().ListHead->size()>0 && Stack.back().ListHead->back().Load.Point!=nullptr && Stack.back().ListHead->back().Load.Type>>1==DIC)
			Stack.back().LineUk = &Stack.back().ListHead->back();
		break;
	case 205: // LinePush Положить текущую линию в стек
		if (Stack.back().LineNum >= 0 && Stack.back().LineNum < Stack.back().ListHead->size() > 0) 
			LineStack.push_back(Stack.back().LineNum);
		break;
	case 206: // LineLastPush Положить последнюю линию в стек
		if (Stack.back().ListHead->size() > 0) LineStack.push_back(Stack.back().ListHead->size()-1);
		break;
	case 207: // LinePop Взять текущую линию из стека
		if (LineStack.size() && Stack.back().ListHead->size()> LineStack.back() && LineStack.back()>=0)
		{
			Stack.back().LineNum = LineStack.back();
			LineStack.pop_back();
			Stack.back().LineUk = Stack.back().ListHead->begin()._Ptr+ Stack.back().LineNum;
		}
		break;
	case 220: // FindOr Поиск ИЛИ
	case 221: // FindOrLastLine Поиск ИЛИ в последней строке
	case 226: // FindAnd Поиск И                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             227: // FindAndLastLine Поиск И в последней строке
	case 230: // FindXor Поиск XOR
	case 231: // FindXorLastLine Поиск XOR в последней строке
	case 236: // FindAndSource Поиск И в источнике
	case 237: // FindAndSourceLastLine Поиск И в источнике в последней строке
	{

		Stack.back().LineCount = 0; // счетчик совпадений
		Stack.back().LineNumOld = Stack.back().LineNum;
		Stack.back().LineNum = 0; // Номер первой совпадающей линии
		Stack.back().LineUk = nullptr; // Ссылка на найденную строку
		int LineNumFirst = -1;

		if (Stack.back().ListHead == nullptr)
		{
			Searcher.Template = { 0,nullptr };
			Searcher.FindOr({ 0,nullptr });
			ProgExec(FailProg);
			break;
		}
		auto i = Stack.back().ListHead->begin();
		if (MK % 2 != 0)
			i = Stack.back().ListHead->end() - 1;
		for (; i != Stack.back().ListHead->end(); i++)
		{
			if (i->atr != LineAtr) 
				continue;
			if (MultyListMode && AtrSearch(i->Load.Point,LineAtr)) // Попытка перехода на другой уровень списка
				{
					Stack.push_back(Stack.back()); // Добавить контекст в стек контекстов
					Stack.back().ListHead = (IC_type)i->Load.Point;
					ProgFU(MK, Load); // Поиск на следующем уровне
					IC_type t = (Stack.end() - 1)->ListHead;
					*(Stack.end() - 1) = Stack.back();
					Stack.pop_back(); // Убрать из стека контекстов
					Stack.back().ListHead = t;
					if (Stack.back().LineCount && !MultiLineMode) // Если поиск до первого совпадения, то выход
						break;
				}

			//Stack.back().LineUk = i._Ptr;
			Searcher.Template = i->Load;
			switch (MK)
			{
			case 220: // FindOr		
				Searcher.FindOr(Load);
				break;
			case 226: // FindAnd 
				Searcher.FindAnd(Load);
				break;
			case 230: // FindXor
				Searcher.FindXor(Load);
				break;
			case 235: // FindAndSource
				Searcher.FindAndSource(Load);
				break;
			}
			if (Searcher.Rez)
			{
				Stack.back().LineCount++;
				if (Stack.back().LineCount==1)
				{
					if (MK % 2 == 1)
						LineNumFirst = Stack.back().ListHead->size() - 1;
					else
						LineNumFirst = Stack.back().LineNum;

				}
				if (MultiLineMode == 0) break;
			}
			Stack.back().LineNum++;
		}
		if (Stack.back().LineCount)
		{
			Stack.back().LineUk = &Stack.back().ListHead->at(LineNumFirst);
			Stack.back().LineNum = LineNumFirst;
			ProgExec(SuссessProg);
		}
		else
		{
			Stack.back().LineNum == -1;
			Stack.back().LineUk = nullptr;
			ProgExec(FailProg);
		}
	}
	if (Stack.back().LineNum > Stack.back().LineNumOld)  ProgExec(BibberProg);
	if (Stack.back().LineNum < Stack.back().LineNumOld)  ProgExec(LessProg);
	if (Stack.back().LineNum >= Stack.back().LineNumOld) ProgExec(BibberEQProg);
	if (Stack.back().LineNum < Stack.back().LineNumOld)  ProgExec(LessEQProg);
	if (Stack.back().LineNum == Stack.back().LineNumOld) ProgExec(EQProg);
	///Stack.back().LineUk == nullptr;
	break;

	// Операции с подсписками
	case 238: // ToRoot Переход к корневому уровню списка
		while (Stack.size() > 1)
			Stack.pop_back(); // Возврат к корневому уровню
		break;
	case 239: // SubCopyAdd
		Stack.back().ListHead->push_back({ Load.ToInt(ListLine), TIC, new ICVect });
		Stack.push_back({ (vector<ip>*)Stack.back().ListHead->back().Load.Point, nullptr, -1, -1, 0 });
		Stack.back().ListHead->push_back({ LineAtr, Load.Copy()});
		break;
	case 240: // SubAdd Добавить новый уровень в список и спуститься на него (в нагрзуке атрибут подсписка, если nil атрибут по умолчанию)
		Stack.back().ListHead->push_back({ Load.ToInt(LineAtr), TIC, new ICVect });
		Stack.push_back({ (vector<ip>*) Stack.back().ListHead->back().Load.Point,nullptr,-1,-1,0 });
		break;
	case 241: // SubRootAdd Добавить новый уровень в корневой список и спуститься на него оставить в стеке уровней ссылку на предыдущую ИК  (в нагрзуке атрибут подсписка, если nil атрибут по умолчанию)
	case 242: // SubRootCopyAdd
		Stack.back().StopSearch = true;
		while (Stack.size() > 1)
			Stack.pop_back(); // Возврат к корневому уровню

		if (Stack.back().ListHead == nullptr)
			Stack.back().ListHead = new vector<ip>;
		Stack.back().ListHead->push_back({LineAtr,TIC, new vector<ip>});
		Stack.push_back({ (vector<ip>*) Stack.back().ListHead->back().Load.Point, nullptr, -1, -1, 0 });
		if(Load.isIC())
			if(MK==241)
				Stack.back().ListHead->push_back({ LineAtr, Load.Type, Load.Point });
			else
				Stack.back().ListHead->push_back({ LineAtr, Load.Copy()});
		break;
	
	case 250: // SubUp Перейти на уровень выше
		Stack.pop_back();
		break;
	case 251: // SubUpDel Перейти на уровень выше и уничтожить текущий уровень
		ICDel(Stack.back().ListHead->back().Load.Point);
		Stack.pop_back();
		break;
	case 252: // SubUpDelGraph Перейти на уровень выше и уничтожить текущий уровень с подсписками
		//GraphDel(Stack.back().ListHead->back().Load.Point);
		Stack.pop_back();
		break;
	case 253: // SubDeepOut Выдать номер текущего уровня
		Load.Write(Stack.size());
		break;
	case 254: // SubDeepOutMk Выдать МК с номером текущего уровня
	{
		int t = Stack.size();
		MkExec(MK, {Cint,&t});
		break;
	}
	case 255: // SubDown Перейти на уровень ниже (по умолчанию переход осуществляется на текущую найденную строку
		if (Load.Point == nullptr)
			Stack.push_back({ (IC_type)Stack.back().LineUk->Load.Point,nullptr,-1,-1,0,false});
		else if(Load.isIC())
			Stack.push_back({ (IC_type)Load.Point,nullptr,-1,-1,0,false });
		break;

	case 400: // LineOutMk Выдать МК с найденной линией
			MkExec(Load, Stack.back().LineUk->Load);
		break;
	case 401: // LineOut Выдать найденную линиию
			Load.Write(Stack.back().LineUk->Load.Point);
		break;

		// Программы по срезультатам сравнения номеров строк
	case 450: // EqProgExec
		if(Load.Point==nullptr)
			if (Stack.back().LineNumOld == Stack.back().LineNum) ProgExec(Load.Point);
		break;
	case 451: // BiggerExec
		if (Stack.back().LineNumOld < Stack.back().LineNum)
			if (Load.Point != 0)
				ProgExec(Load);
			else
				ProgExec(Prog);
		break;
	case 452: // SmallerExec
		if (Stack.back().LineNumOld > Stack.back().LineNum)
			if (Load.Point != 0)
				ProgExec(Load);
			else
				ProgExec(Prog);
			break;
	case 453: // BigerEqExec
		if (Stack.back().LineNumOld <= Stack.back().LineNum)
			if (Load.Point != 0)
				ProgExec(Load);
			else
				ProgExec(Prog);
		break;
	case 454: // SmallerEqExec
		if (Stack.back().LineNumOld >= Stack.back().LineNum)
			if (Load.Point != 0)
				ProgExec(Load);
			else
				ProgExec(Prog);
		break;
	case 455: // DifferenceOut Выдать различие старой найденной линии и новой
		Load.Write(Stack.back().LineNum - Stack.back().LineNumOld);
		break;
	case 456: // DifferenceOutMk Выдать МК с различием старой найденной линии и новой
	{
		int temp = Stack.back().LineNum - Stack.back().LineNumOld;
		MkExec(Load, { Cint,&temp });
		break; }
	case 459: // Stack.back().LineNumSet Установить номер найденной строки (нужно для организации операций сравнения)
		Stack.back().LineNum = Load.ToInt();
		break;
	case 460: //Stack.back().LineCountOut Выдать количество найденных строк
		Load.Write(Stack.back().LineCount);
		break;
	case 461: //Stack.back().LineCountOutMk Выдать МК с количеством найденных строк
		MkExec(Load, { Cint, &Stack.back().LineCount });
		break;

	case 470: // LessProgSet
		LessProg = Load.Point;
		break;
	case 471: // BiggerProgSet
		BibberProg = Load.Point;
		break;
	case 472: // EQProgSet
		EQProg = Load.Point;
		break;
	case 473: // LessEQProgSet
		LessEQProg = Load.Point;
		break;
	case 474: // BiggerEQProgSet
		BibberEQProg = Load.Point;
		break;
	// Программы по анализу строк
	case 500: // LineEmptyExec Выполнить подпрограмму, если текущая строка пустая
		if (Stack.back().ListHead != nullptr && Stack.back().LineUk != nullptr && ((IC_type)Stack.back().LineUk->Load.Point)->size()==0)
			ProgExec(Load);
		break;
	case 501: //LineFullExec  Выполнить подпрограмму, если текущая строка не пустая
		if (Stack.back().ListHead != nullptr && Stack.back().LineUk != nullptr && ((IC_type)Stack.back().LineUk->Load.Point)->size() != 0)
			ProgExec(Load);
		break;
	case 505: // LastEmptyExec  Выполнить подпрограмму, если последняя строка пустая
		if (Stack.back().ListHead != nullptr && Stack.back().ListHead->size() != 0 && ((IC_type)Stack.back().ListHead->back().Load.Point)->size() == 0)
			ProgExec(Load);
		break;
	case 506: //LastFullExec Выполнить подпрограмму, если последняя строка не пустая
		if (Stack.back().ListHead != nullptr && Stack.back().ListHead->size() != 0 && ((IC_type)Stack.back().ListHead->back().Load.Point)->size() != 0)
			ProgExec(Load);
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
	if (Stack.size() == 1)
		ProgExec(PostfixProg); // Выполнить финальную программу
}

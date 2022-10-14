// ФУ Список
#include "stdafx.h"
#include "List.h"

using namespace std;

void List::ProgFU(int MK, LoadPoint Load)
{
	if (PrefixProg != nullptr)
	{
		IC_type t = PrefixProg;
		PrefixProg = nullptr;
		ProgExec(t); // Предварительная программа, вызываемая при приходе любой МК
		PrefixProg = t;
	}
	switch (MK)
	{
	case 0: // Reset
		Searcher.Clear();
		ListHead.clear();// Сделать потом очистку списка
		ListHead.push_back(nullptr);
		break;
	case 1:	// Set
		ListHead.back() = (IC_type)Load.Point;
		break;
	case 2:// Out Выдать ссылку на список
		if (Load.Type >> 1 == Cvoid)
			*(void**)Load.Point = ListHead.back();
		break;
	case 3:// OutMk Выдать МК со ссылкой на список
		if (!ListHead.size())
			MkExec(Load, { 0,nullptr });
		else
			MkExec(Load, { TIC,(void*)ListHead.back() });
		break;
	case 4: // RootOutMk Выдать МК с указателем на корневой список
		if(!ListHead.size())
			MkExec(Load, { 0,nullptr });
		else
			MkExec(Load, { TIC,(void*)*ListHead.begin() });
		break;
	case 5:	// MultiLineModeSet
		MultiLineMode = Load.toInt(1);
		break;
	case 6: // MultyListModeSet Установить режим многоуровневого списка для поиска (true по умолчанию)
		MultyListMode = Load.toBool(true);
		break;
	case 7: // MkModeSet Режим выполнения всех МК в ИК-шаблоне (МК-ой считается любой атрибут, индекс которого больше 0) При пустой нагрузке режим устанавливается
		Searcher.MkMode = Load.toBool(true);
		break;
	case 26: // CalcModeSet Установить режим управления поиском с помощью арифметико-логичекого устройства (true по умолчанию)
		Searcher.CalcMode = Load.toBool(true);
		break;
	case 27: // CalcMkSet Установить МК для вычисления АЛВ
		Searcher._CalcMk=Load.toInt();
		break;
	case 8:// EmptyProgExec Выполнить программу, если список пуст
	case 9:// FullExec Выполнить программу, если список не пуст
	case 14: // OneLineExec Выполнить программу, если в списке только одна строка
	case 13: // MoreOneLineExec Выполнить программу, если в списке больше одной строка
	case 19: // EmptyOrOneLineExec Выполнить программу, если в списке меньше двух строк
		if (MK == 8 && (ListHead.back() == nullptr || ListHead.back()->size() == 0) ||
			MK == 9 && ListHead.size() != 0 && ListHead.back() != nullptr && ListHead.back()->size() > 0 ||
			MK == 14 && ListHead.size() != 0 && ListHead.back() != nullptr && ListHead.back()->size() == 1 ||
			MK == 13 && ListHead.size() != 0 && ListHead.back() != nullptr && ListHead.back()->size() > 1 ||
			MK == 19 && (ListHead.back() == nullptr || ListHead.back()->size() <= 1)
			)
			if (Load.isIC())
				ProgExec(Load);
			else
				ProgExec(Prog);
		else if (Load.Point == nullptr)
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
	case 23: // SuссessExec Выполнить программу, если перед этим был удачный поиск
		if (Searcher.Rez)
			ProgExec(Load);
		break;
	case 24: // FailExec Выполнить программу, если перед этим был удачный поиск
		if (!Searcher.Rez)
			ProgExec(Load);
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
			Searcher.Prog_atr = Load.toInt();
		break;
	case 21: //LineAtrDef Установить атрибут марки списка по умолчанию
		LineAtr = Load.toInt();
		break;
	case 22: //SubAtrDef Установить атрибут подсписка по умолчанию
		SubAtr = Load.toInt();
		break;
	case 25: // BackOut Вылать входной объект для поиска
		if (Load.Type == TPPoint)
			(*(LoadPoint*)Load.Point) = Searcher.Obj;
		break;
	case 30: // BackOutMk Вылаить МК со входным объектом для поиска
		MkExec(Load, Searcher.Obj);
		break;
	case 31: // BackOut Вылать входной объект для поиска
		if (Load.Type == TPPoint)
			(*(LoadPoint*)Load.Point) = Searcher.Obj.Clone();
		break;
	case 32: // CopyBackOutMk Вылаить МК со входным объектом для поиска
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
			if (Searcher.Obj.Type >> 1 == DIC)
				MkExec(*((int*)Load.Point), ((IC_type)(Searcher.Obj.Point))->begin()->Load);
			else if (Searcher.Obj.Type >> 1 == DIP)
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
		if (LineUk != nullptr)
			LineUk->atr = Load.toInt();
		break;
	case 121: // LastMarkAtrSet Установить атрибут последней строки
		if (ListHead.back() != nullptr)
			(ListHead.back()->end() - 1)->atr = Load.toInt();
		break;
	case 122: //MarkAtrOut Выдать маркер текущей строки
		if (LineUk != nullptr)
			Load.Write(LineUk->atr);
		break;
	case 123: //MarkAtrOutMk Выдать МК с маркером текущей строки
		if (LineUk != nullptr)
			MkExec(Load, { Cint, &LineUk->atr });
		break;
	case 124: //LastMarkAtrOut Выдать маркер последней строки
		if (ListHead.back() != nullptr && !ListHead.back()->size())
			Load.Write((ListHead.back()->end() - 1)->atr);
		break;
	case 129: //LastMarkAtrOutMk Выдать МК с маркером последней строки
		if (ListHead.back() != nullptr && !ListHead.back()->size())
			MkExec(Load, { Cint, &(ListHead.back()->end() - 1)->atr });
		break;
	case 125: //  MarkAtrInc Инкремент атрибута текущей строки
		if (LineUk != nullptr)
			LineUk->atr++;
		break;
	case 126: // LastMarkAtrInc Инкремент атрибута последней строки
		if (ListHead.back() != nullptr)
			(ListHead.back()->end() - 1)->atr++;
		break;
	case 127: //  MarkAtrDec Декримент атрибута текущей строки
		if (LineUk != nullptr)
			LineUk->atr--;
		break;
	case 128: // LastMarkAtrDec Декримент атрибута последней строки
		if (ListHead.back() != nullptr)
			(ListHead.back()->end() - 1)->atr--;
		break;
	case 130: // MarkAtrEQExec Запуск программы при нуле/равенстве атрибута текущей строки или равестве c нагрузкой МК
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0 || LineUk == nullptr) break;
		if (Load.Type >> 1 == DIC) {
			if (!LineUk->atr)
				ProgExec(Load);
		}
		else if (LineUk->atr == Load.toInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 131: // LastMarkEQExec Запуск программы при нуле/равенстве атрибута последней строки или равестве c нагрузкой МК
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0) break;
		if (Load.Type >> 1 == DIC) {
			if (!(ListHead.back()->end() - 1)->atr)
				ProgExec(Load);
		}
		else if ((ListHead.back()->end() - 1)->atr == Load.toInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 133: // MarkAtrNEQExec Запуск программы при не нуле/неравенстве атрибута текущей строки
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0 || LineUk == nullptr) break;
		if (Load.Type >> 1 == DIC) {
			if (LineUk->atr)
				ProgExec(Load);
		}
		else if (LineUk->atr != Load.toInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 134: // LastMarkAtrNEQExec Запуск программы при не нуле/неравенстве атрибута последней строки
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0) break;
		if (Load.Type >> 1 == DIC) {
			if ((ListHead.back()->end() - 1)->atr)
				ProgExec(Load);
		}
		else if ((ListHead.back()->end() - 1)->atr != Load.toInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 135: // MarkAtrBiggerExec Запуск программы при атрибуте текущей строки больше (больше 0)
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0 || LineUk == nullptr) break;
		if (Load.Type >> 1 == DIC) {
			if (LineUk->atr > 0)
				ProgExec(Load);
		}
		else if (LineUk->atr > Load.toInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 136: // LastMarkAtrBiggerExec Запуск программы при атрибуте последней строки больше (больше 0)
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0) break;
		if (Load.Type >> 1 == DIC) {
			if (!(ListHead.back()->end() - 1)->atr > 0)
				ProgExec(Load);
		}
		else if ((ListHead.back()->end() - 1)->atr > Load.toInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 137: // MarkAtrBiggerEQExec Запуск программы при атрибуте текущей строки больше или райным 0
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0 || LineUk == nullptr) break;
		if (Load.Type >> 1 == DIC) {
			if (LineUk->atr >= 0)
				ProgExec(Load);
		}
		else if (LineUk->atr >= Load.toInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 138: // LastMarkAtrBiggerEQExec Запуск программы при атрибуте последней строки больше или равным 0
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0) break;
		if (Load.Type >> 1 == DIC) {
			if ((ListHead.back()->end() - 1)->atr >= 0)
				ProgExec(Load);
		}
		else if ((ListHead.back()->end() - 1)->atr >= Load.toInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 139: // MarkAtrLessExec Запуск программы при атрибуте текущей строки меньше 0
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0 || LineUk == nullptr) break;
		if (Load.Type >> 1 == DIC) {
			if (LineUk->atr < 0)
				ProgExec(Load);
		}
		else if (LineUk->atr < Load.toInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 140: // LastMarkAtrLessExec Запуск программы при атрибуте последней строки меньше 0
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0) break;
		if (Load.Type >> 1 == DIC) {
			if ((ListHead.back()->end() - 1)->atr < 0)
				ProgExec(Load);
		}
		else if ((ListHead.back()->end() - 1)->atr < Load.toInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 141: // MarkAtrLessEQExec Запуск программы при атрибуте текущей строки меньше или равным 0
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0 || LineUk == nullptr) break;
		if (Load.Type >> 1 == DIC) {
			if (LineUk->atr <= 0)
				ProgExec(Load);
		}
		else if (LineUk->atr <= Load.toInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;
	case 142: // LastMarkAtrLessEQExec Запуск программы при атрибуте последней строки меньше или равным 0
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0) break;
		if (Load.Type >> 1 == DIC) {
			if ((ListHead.back()->end() - 1)->atr <= 0)
				ProgExec(Load);
		}
		else if ((ListHead.back()->end() - 1)->atr <= Load.toInt())
			ProgExec(Prog);
		else
			ProgExec(ElseProg);
		break;

	case 150: //LastOut Выдать ссылку на последнюю линию списка
	case 151: //LastPop Выдать ссылку на последнюю линию списка и удалить из списка
	case 152: //LastDel Выдать ссылку на последнюю линию списка и удалить из ИК
		if (Load.Type == Tvoid)
			*(void**)Load.Point = (void*)ListHead.back()->back().Load.Point;
		if (MK == 152) ICDel((void*)ListHead.back()->back().Load.Point);
		if (MK == 151 || MK == 152)
			ListHead.back()->pop_back();
		break;
	case 153: //LastIpOutMk Выдать ссылку на последнюю ИП последней линии
		MkExec(Load, { CIP,(void*)&((IC_type)ListHead.back()->back().Load.Point)->back() });
		break;
	case 154: // LastIpCopyOutMk Выдать МК со ссылкой на последнюю ИП последней линии
		MkExec(Load, { TIC,((IC_type)ListHead.back()->back().Load.Point)->back().Сlone() });
		break;
	case 155: //LastOutMK Выдать ссылку на последнюю линию списка
	case 156: //LastPopMK Выдать ссылку на последнюю линию списка и удалить из списка
	case 157: //LastDelMk Выдать ссылку на последнюю линию списка и удалить из ИК
		if (ListHead.back() == nullptr || !ListHead.back()->size()) break;
		MkExec(Load, ListHead.back()->back().Load);
		if (MK == 157) ICDel((void*)ListHead.back()->back().Load.Point);
		if (MK == 156 || MK == 157)
			ListHead.back()->pop_back();
		break;
	case 158: // LastLoadOut Выдать нагрузку последней ИП последней линии
		Load.Write(((IC_type)(ListHead.back()->back().Load.Point))->back().Load);
		//		*(LoadPoint*)Load.Point = ((IC_type)(ListHead.back()->back().Load.Point))->back().Load;
		break;
	case 169: // LastLoadOutMk Выдать МК с нагрузкой последней ИП последней линии
		MkExec(Load, ((IC_type)(ListHead.back()->back().Load.Point))->back().Load);
		break;
	case 188: // LastLoadRefOut Выдать ссылку на Load последней ИП последней строки списка
		// Load.Write(&(IC_type)(ListHead.back()->back().Load.Point))->back().Load);
		break;
	case 198: // LastLoadRefOutMk Выдать МК со ссылкой на Load последней ИП последней строки списка
		MkExec(Load, { TLoad, &((IC_type)(ListHead.back()->back().Load.Point))->back().Load });
		break;
	case 145: //LineIpOutMk Выдать ссылку на последнюю ИП текущей линии
		if (ListHead.back() == nullptr || !ListHead.back()->size() || LineUk == nullptr) break;
		MkExec(Load, { CIP,&((IC_type)LineUk->Load.Point)->back() });
		break;
	case 146: // LineIcCopyOutMk Выдать МК со ссылкой на последнюю ИП текущей линии
		//		MkExec(Load, { CIC,((IC_type)ListHead.back()->back().Load.Point)->back().СloneToIC() });
		break;
	case 147: // LastIpOutMk Выдать ссылку на последнюю ИП текущей линии
		MkExec(Load, { CIP,&((IC_type)ListHead.back()->back().Load.Point)->back() });
		break;
	case 192: // LineCopyAddPrevSet Добавить копию строки и установить ссылку на нее в предыдущую линию
	{
		if (ListHead.back()->size()==0) // Если список пустой, добавить линию
			ListHead.back()->push_back({ LineAtr, TIC, nullptr });
		ListHead.back()->push_back({ LineAtr,Load.Copy()});// ->push_back(LineAtr, TIC, new vector<ip>);
		if (Load.Point == nullptr)
			ListHead.back()->back().Load = { TIC,new vector<ip> };
		ListHead.back()->at(ListHead.back()->size() - 2).Load = ListHead.back()->back().Load;
		break;
	}
	case 163: // LineCopyAddPrevLoadSet
	case 159: // LastCopyAddPrevLoadSetLoadMov Добавить копию линии перенести нагрузку в на новую строку и добавить в нагрзуку предыдущей строки ссылку на новую строку 
	{
		LoadPoint t = { 0, nullptr };
		if (ListHead.back() != nullptr && ListHead.back()->size() && ListHead.back()->back().Load.Type >> 1 == DIC)
			t = ((IC_type)ListHead.back()->back().Load.Point)->back().Load;
		if (ListHead.back() == nullptr) 
			ListHead.back() = new vector<ip>;
		if (Load.Point == nullptr)
			ListHead.back()->push_back({ LineAtr, TIC, new vector<ip> });
		else
			ListHead.back()->push_back({ LineAtr, TIC, ICCopy(Load).Point });
		if (ListHead.back()->size() > 1 && ListHead.back()->back().Load.Point != nullptr && ListHead.back()->back().Load.Type >> 1 == DIC)
			((IC_type)ListHead.back()->at(ListHead.back()->size() - 2).Load.Point)->back().Load = ListHead.back()->back().Load;
		else if (ListHead.back()->size() == 1) // Добавление в пустой список
		{
			ListHead.back()->insert(ListHead.back()->begin(), {LineAtr, ListHead.back()->back().Load});
		}
		if (MK == 159)
			((IC_type)ListHead.back()->back().Load.Point)->back().Load = t;
		break;
	}
	case 160: // LineAdd Добавить новую строку всписок
		if (ListHead.back() == nullptr)
			ListHead.back() = new vector<ip>;
	    ListHead.back()->push_back({ LineAtr, Load });
		break;
	case 161: // LineCopyAdd Добавить копию строки
		if (ListHead.back() == nullptr) ListHead.back() = new vector<ip>;
		if (Load.Point != nullptr)
			ListHead.back()->push_back({ LineAtr, TIC, ICCopy(Load).Point });
		else
			ListHead.back()->push_back({ LineAtr, TIC, new vector<ip> }); // Создание пустой строки
		break;
		//	case 163: //  LineCopyAddPrevLoadSet Добавить линию в список и поместить ссылку на нее в нагрузку последней ИП последней строки
		//	 	if (ListHead.back() == nullptr) ListHead.back() = new vector<ip>;
		//		if(Load.Point==nullptr)
		//			ListHead.back()->push_back({ LineAtr, TIC, new vector<ip> });
		//		else
		//			ListHead.back()->push_back({ LineAtr, TIC, ICCopy(Load) });
		//		if (ListHead.back()->size() > 1)
		//			((IC_type)ListHead.back()->at(ListHead.back()->size() - 2).Load.Point)->back().Load = ListHead.back()->back().Load;
		//		break;
	case 162: // LineCopyTreeAdd Добавить копию ОА-графа
		if (ListHead.back() == nullptr) ListHead.back() = new vector<ip>;
		// ....
		break;
	case 164: // LineLoadOutMk Выдать МК с нагрузкой последней ИП текущей линии
		MkExec(Load, ((IC_type)(LineUk->Load.Point))->back().Load);
		break;
	case 165: // LinePopMk Исключить линию списка
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0) break;
		MkExec(Load, { TIC , (void*)ListHead.back()->back().Load.Point });
		ListHead.back()->pop_back();
		break;
	case 166: // LineDelMk Выдать МК со ссылкой на ИК и удалить ИК
	{
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0) break;
		LoadPoint t = ListHead.back()->back().Load;
		ListHead.back()->pop_back();
		MkExec(Load, t);
		ICDel(ListHead.back()->back().Load.Point);
		break;
	}
	case 167: // LineIpCut Удалить последние ИП из текущей линии (в нагрузке количество удаляемых ИП, по умолчанию 1)
	case 168: // LineVarIpCut" Hint = "Удалить последние ИП c пемеменными из текущей линии (в нагрузке количество удаляемых ИП, по умолчанию 1)" }
		if (LineUk != nullptr && LineUk->Load.Type >> 1 == DIC && LineUk->Load.Point != nullptr)
		{
			int n;
			if (Load.Point == nullptr || Load.Type >> 1 != Dint)
				n = 1;
			else
				n = *(int*)Load.Point;
			while (n-- && ((IC_type)LineUk->Load.Point)->size() != 0)
			{
				((IC_type)LineUk->Load.Point)->resize(((IC_type)LineUk->Load.Point)->size() - 1);
			}
		}
		break;
	case 174: // LastConstVarTypeSet Установить тип константа/переменная для последней нагрузки (по умолчанию переменная)
	if (ListHead.size() && ListHead.back() != nullptr)
	{
		((IC_type)ListHead.back()->back().Load.Point)->back().Load.Type |= 1;
		if (!Load.toBool(false))
			((IC_type)ListHead.back()->back().Load.Point)->back().Load.Type -= 1;
	break;
	}
	case 170: // LastAttach Конкатенация ИК к последней линии списка
	case 171: // LastCopyAttach Конкатенация копии ИК к последней линии списка
	case 172: // LastCopyAttachLoadMove Добавить ИК и переписать последнюю нагрузку
	{
		if (ListHead.back() == nullptr)
		{
			ListHead.back() = new vector<ip>;
			ListHead.back()->push_back({ LineAtr,{ TIC,new vector<ip>} });

		}
		if (!ListHead.back()->size())
			ListHead.back()->push_back( {LineAtr, TIC, new vector<ip> }); // Добавить пустую строку в список
		if (Load.Point == nullptr)
			((IC_type)ListHead.back()->back().Load.Point)->push_back({ 0,0, nullptr }); // Добавить пустую ИП, если Load==nil
		else if (Load.isIP() || Load.isIC())
		{
			if (Load.isIP())
			{
				((IC_type)ListHead.back()->back().Load.Point)->push_back(((ip*)Load.Point)->Copy());
				if (MK == 170 && Load.Type == CIP) { delete (ip*)Load.Point; Load.Point = nullptr; Load.Type = 0; }
			}
			else if (Load.isIC())
			{
				for (auto i : *(IC_type)Load.Point)
					((IC_type)ListHead.back()->back().Load.Point)->push_back(i);
				if (MK == 170 && Load.Type == CIP) { delete (ip*)Load.Point; Load.Point = nullptr; Load.Type = 0; }
			}
		}
		else
		{
			if (Load.isIC())
			{
				copy(((IC_type)Load.Point)->begin(), ((IC_type)Load.Point)->end(), inserter(*((IC_type)ListHead.back()->back().Load.Point), ((IC_type)ListHead.back()->back().Load.Point)->end()));
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
		if (LineUk != nullptr && Load.Point != nullptr)
			if (Load.Type >> 1 == DIP)
			{
				((IC_type)LineUk->Load.Point)->push_back(*(ip*)Load.Point);
				if (MK == 175 && Load.Type == CIP) { delete (ip*)Load.Point; Load.Point = nullptr; Load.Type = 0; }
			}
			else
			{
				if (Load.Type >> 1 == DIC)
				{
					copy(((IC_type)Load.Point)->begin(), ((IC_type)Load.Point)->end(), inserter(*((IC_type)LineUk->Load.Point), ((IC_type)LineUk->Load.Point)->end()));
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
	if (ListHead.back() == nullptr || ListHead.back()->back().Load.Point == nullptr || ListHead.back()->back().Load.Type >> 1 != DIC) break;
	if (((IC_type)ListHead.back()->back().Load.Point)->size() == 0) break;
	if (MK < 190)
		t = ((IC_type)ListHead.back()->back().Load.Point);
	else if (LineUk != nullptr && LineUk->Load.Point != nullptr && LineUk->Load.Type >> 1 != DIC)
		break;
	else
		t = (IC_type)LineUk->Load.Point;
	switch (MK)
	{
	case 185:
	case 195:
		t->back().Load = Load.Clone();	break;
	case 186:
	case 196:
		if (Load.Type >> 1 != DIC)
			t->back().Load.Copy(Load);
		else
		{
			t->back().Load.Type = Load.Type;
			t->back().Load.Point = ICCopy(Load).Point;
		}
		break;
	case 189:
	case 199:
		//if (Load.Type >> 1 == Dint)
		t->back().atr = Load.toInt();
		break;
		//	case 177: // LastPointLoadCopyGraphSet
				// ...
				//break;
	}

	break;
	}
	case 180: // LineVarTypeSet // Установить тип 'переменная' в последней ИП текущей линии
		if (ListHead.back() == nullptr || !ListHead.back()->size() || \
			LineUk == nullptr || LineUk->Load.Point == nullptr || \
			LineUk->Load.Type >> 1 != DIC || !((IC_type)LineUk->Load.Point)->size())
			break;
		((IC_type)LineUk->Load.Point)->back().Load.VarTypeSet(Load.toBool(true));
		break;
	case 182: // LastVarTypeSet // Установить тип 'переменная' в последней ИП последней линии
		if (ListHead.back() == nullptr || !ListHead.back()->size() || \
			ListHead.back()->back().Load.Type >> 1 != DIC || !((IC_type)ListHead.back()->back().Load.Point)->size())
			break;
		((IC_type)ListHead.back()->back().Load.Point)->back().Load.VarTypeSet(Load.toBool(true));
		break;
	case 190: // LastLoadIcEmptySet Установить ссылку на пустую ИК в нагрузке последней ИП последней строки
		if (ListHead.back() != nullptr && !ListHead.size() && ((IC_type)ListHead.back()->back().Load.isIC()))
			((IC_type)ListHead.back()->back().Load.Point)->back().Load = { TIC, new vector<ip> };
		break;
	case 191: // LineLoadIcEmptySet Установить ссылку на пустую ИК в нагрузке последней ИП текущей строки
		if (LineUk != nullptr && ((IC_type)LineUk->Load.isIC()))
			((IC_type)LineUk->Load.Point)->back().Load = {TIC, new vector<ip>};
		break;
	case 200: // LineToLast Установить текущую строку на последнюю строку
		if (ListHead.back() != nullptr && ListHead.back()->size() > 0 && ListHead.back()->back().Load.Point != nullptr && ListHead.back()->back().Load.Type >> 1 == DIC)
			LineUk = &ListHead.back()->back();
		break;
	case 205: // LinePush Положить текущую линию в стек
		if (LineNum >= 0 && LineNum < ListHead.back()->size() > 0)
			LineStack.push_back(LineNum);
		break;
	case 206: // LineLastPush Положить последнюю линию в стек
		if (ListHead.back()->size() > 0) LineStack.push_back(ListHead.back()->size() - 1);
		break;
	case 207: // LinePop Взять текущую линию из стека
		if (LineStack.size() && ListHead.back()->size() > LineStack.back() && LineStack.back() >= 0)
		{
			LineNum = LineStack.back();
			LineStack.pop_back();
			LineUk = ListHead.back()->begin()._Ptr + LineNum;
		}
		break;

	case 210: // LastAtrAttach Прикрепить к последней строке ИП с атрибутом из нагрзуки
		if (ListHead.back() == nullptr)
			ListHead.back() = new vector<ip>;
		if (!ListHead.back()->size())
			ListHead.back()->push_back({ LineAtr, TIC, new vector<ip> });
		((IC_type)ListHead.back()->back().Load.Point)->push_back({ Load.toInt() ,0, nullptr});
		break;
	case 211: // LineAtrAttach Прикрепить к текущей строке ИП с атрибутом из нагрзуки
		if (LineUk == nullptr) break;
		if (LineUk->Load.Point == 0)
			LineUk->Load = { TIC, new vector<ip> };
		((IC_type)LineUk->Load.Point)->push_back({ Load.toInt() ,0, nullptr });
		break;
	case 215: // LastLoadBackSet Записать нагрзуку последней ИП последний строки в предыдущие ИП (на входе количество обратных записей)
	{
		if (!ListHead.back() || !ListHead.back()->size() || !ListHead.back()->back().Load.isIC() ||
			((IC_type)ListHead.back()->back().Load.Point)->size()<2) break;
		auto uk = ((IC_type)ListHead.back()->back().Load.Point)->rbegin() + 1;
		//auto uk2 = ((IC_type)ListHead.back()->back().Load.Point)->end() - 2;
		int i = Load.toInt();
		for (; i > 0 && uk != ((IC_type)ListHead.back()->back().Load.Point)->rend(); i--, uk++)
			uk->Load = ((IC_type)ListHead.back()->back().Load.Point)->back().Load.Clone();
		break;
	}
	case 216: // LineLoadBackSet Записать нагрзуку последней ИП некущей строки в предыдущие ИП (на входе количество обратных записей)
	{
		if (LineUk == nullptr || ((IC_type)LineUk->Load.Point)->size() < 2) break;
		auto uk = ((IC_type)LineUk->Load.Point)->end() - 2;
		for (int i = Load.toInt(); i > 0 && uk != ((IC_type)LineUk->Load.Point)->begin() - 1; i++, uk--)
			uk->Load = ((IC_type)LineUk->Load.Point)->back().Load.Clone();
		break;
	}
	case 220: //FindOrLast
		//if(ListHead!=nullptr)
		Searcher.Template = { 0,nullptr };
		if(ListHead.size() && ListHead.back() != nullptr && ListHead.back()->size())
			Searcher.Template = ListHead.back()->back().Load;
		Searcher.FindOr(Load);
		break;
	case 221: //FindOrLine
		if (LineUk == nullptr) Searcher.Template = { 0,nullptr };
		else Searcher.Template = LineUk->Load;
		Searcher.FindOr(Load);
		break;
	case 222: //FindAndLast
		Searcher.Template = { 0,nullptr };
		if (ListHead.size() && ListHead.back() != nullptr && ListHead.back()->size())
			Searcher.Template = ListHead.back()->back().Load;
		Searcher.FindAnd(Load);
		break;
	case 223: //FindAndLine
		if (LineUk == nullptr) Searcher.Template = { 0,nullptr };
		else Searcher.Template = LineUk->Load;
		Searcher.FindAnd(Load);
		break;
	case 224: //FindXorLast
		Searcher.Template = { 0,nullptr };
		if (ListHead.size() && ListHead.back() != nullptr && ListHead.back()->size())
			Searcher.Template = ListHead.back()->back().Load;
		Searcher.FindXor(Load);
		break;
	case 225: //FindXorLine
		if (LineUk == nullptr) Searcher.Template = { 0,nullptr };
		else Searcher.Template = LineUk->Load;
		Searcher.FindXor(Load); // Заглушка !!!
		break;
	case 226: //FindAndSourceLast
		Searcher.Template = { 0,nullptr };
		if (ListHead.size() && ListHead.back() != nullptr && ListHead.back()->size())
			Searcher.Template = ListHead.back()->back().Load;
		Searcher.FindAndSource(Load);
		break;
	case 227: //FindAndSourceLine
		Searcher.Template = LineUk->Load;
		Searcher.FindAndSource(Load); // Заглушка !!!
		break;

	case 228: // FindOr Поиск ИЛИ
	case 229: // FindAnd Поиск И
	case 230: // FindXor Поиск XOR
	case 231: // FindAndSource Поиск И в источнике
	{
		if (!ListHead.size()) break;
		if (DeepStartSearch == 0)
		{
			DeepStartSearch = ListHead.size();
			LineCount = 0; // счетчик совпадений
			LineNumOld = LineNum;
			LineUk = nullptr; // Ссылка на найденную строку
			LineNum = -1;
		}
		if (ListHead.back() == nullptr)
		{
			Searcher.Template = { 0,nullptr };
			Searcher.FindOr({ 0,nullptr });
			ProgExec(FailProg);
			break;
		}
		auto i = ListHead.back()->begin();
//		if (MK % 2 != 0)
//			i = ListHead.back()->end() - 1;
		for (int k = 1; i != ListHead.back()->end(); i++, k++)
		{
			if (i->atr != LineAtr)
				continue;
			if (MultyListMode && AtrSearch(i->Load.Point, LineAtr)) // Попытка перехода на другой уровень списка
			{
				ListHead.push_back((IC_type)i->Load.Point); // Добавить контекст в стек контекстов
				ListHead.back() = (IC_type)i->Load.Point;
				ProgFU(MK, Load); // Поиск на следующем уровне

				ListHead.pop_back(); // Убрать из стека контекстов
				if (LineCount && !MultiLineMode) // Если поиск до первого совпадения, то выход
					break;
			}

			Searcher.Template = i->Load;
			switch (MK)
			{
			case 228: // FindOr		
				Searcher.FindOr(Load);
				break;
			case 229: // FindAnd 
				Searcher.FindAnd(Load);
				break;
			case 230: // FindXor
				Searcher.FindXor(Load);
				break;
			case 231: // FindAndSource

				Searcher.FindAndSource(Load);
				break;
			}
			if (Searcher.Rez)
			{
				LineCount++;
				if (LineCount == 1)
				{
					if (MK % 2 == 1)
						LineNum = ListHead.back()->size() - 1;
					else
					{
						LineNum = k;// Запомнить номер первой строки (k- счетчик номеров строк в том числе и в иерархическом списке)
						LineUk = i._Ptr; // Запомнить найденную строку
					}

				}
				if (MultiLineMode == 0) break;
			}
		}
		if (LineCount && ListHead.size() == DeepStartSearch)
		{
			//LineUk = &ListHead.back()->at(LineNumFirst);
			ProgExec(SuссessProg);
		}
		else
		{
			//LineUk = nullptr;
			ProgExec(FailProg);
		}

		if (ListHead.size() == DeepStartSearch)
		{
			if (LineNum > LineNumOld)  ProgExec(BibberProg);
			if (LineNum < LineNumOld)  ProgExec(LessProg);
			if (LineNum >= LineNumOld) ProgExec(BibberEQProg);
			if (LineNum < LineNumOld)  ProgExec(LessEQProg);
			if (LineNum == LineNumOld) ProgExec(EQProg);
			DeepStartSearch = 0;
		}
	}
	break;

	// Операции с подсписками
	case 238: // ToRoot Переход к корневому уровню списка
		while (ListHead.size() > 1)
			ListHead.pop_back(); // Возврат к корневому уровню
		break;
	case 240: // Push SubAdd Добавить ИК на новый уровень списока и спуститься на него
	case 241: // PushCopy SubCopyAdd Добавить копию ИК на новый уровень списка и спуститься на него
	case 242: // SubOACopy SubOACopyAdd Добавить копию ОА-графа на новый уровень списка и спуститься на него
	case 243: // PushTied SubRootAdd Добавить ИК на новый уровень списока и спуститься на него оставить в стеке уровней ссылку на нов. список
	case 244: // PushTiedCopy SubRootCopyAdd Добавить копию ИК на новый уровень списка и спуститься на него оставить в стеке уровней ссылку на нов. список
	case 245: // PushTiedOACopy SubRootOACopyAdd Добавить копию ОА-графа на новый уровень списка и спуститься на него оставить в стеке уровней ссылку на нов. список
	case 246: // PushTiedLine Добавить ИК на новый уровень списока и спуститься на него оставить в стеке уровней ссылку на нов. список
	case 247: // PushTiedLineCopy Добавить копию ИК на новый уровень списка и спуститься на него оставить в стеке уровней ссылку на нов. список
	case 248: // PushTiedLineOACopy Добавить копию ОА-графа на новый уровень списка и спуститься на него оставить в стеке уровней ссылку на нов. список
		ListHead.push_back(new vector<ip>);
		if (Load.isIC())
			if (MK == 240 || MK == 243 || MK == 246)
			{
	//			ListHead.back() = new vector<ip>;
				ListHead.back()->push_back({ LineAtr, Load });
			}
			else if (MK == 241 || MK == 244 || MK == 247)
			{
		//		ListHead.back() = new vector<ip>;
				ListHead.back()->push_back({ LineAtr, Load.Copy() });
			}
			else
				; // Копия ОА-графа
		if (MK >= 243) //Установить ссылку с предыдущего уровня
		{
		//	if (ListHead.back() == nullptr)
		//		ListHead.back()=new vector<ip>;
			if (ListHead.size() > 1 && (*(ListHead.end() - 2))->size() > 0 && (*(ListHead.end() - 2))->at(0).Load.isIC())
				if(MK==243 || MK==244 || MK==245)
					((IC_type)((*(ListHead.end() - 2))->back().Load.Point))->back().Load = { TIC, ListHead.back() };
				else
				{
					if(!ListHead.back()->size())
						ListHead.back()->push_back({ LineAtr,TIC, new vector<ip> });
					((IC_type)((*(ListHead.end() - 2))->back().Load.Point))->back().Load = ListHead.back()->back().Load;
				}
		}
//		if (ListHead.back()->size() == 0)
//		{
//			delete ListHead.back();
//			ListHead.back() = nullptr;
//		}
		break;
	
	case 260: // Pop Вытолкнуть подсписок
	case 261: // PopMk Вытолкнуть подсписок и выдать МК с указателем на него
	case 262: // PopMkDel Вытолкнуть подсписок и выдать МК с указателем на него и удалить нагрузку
		{
			void* t = ListHead.back();
			if (ListHead.size() > 1)
				ListHead.pop_back();
			if (MK == 260)
				Load.Write(t);
			else
				MkExec(Load, { CIP,t });
			if (MK == 262)
			{
				ICDel((IC_type)t);
				//delete (IC_type)t;
			}
		}
		break;
	case 265: // ZeroDeepExec Выполнить, если нет подсписков
		if (ListHead.size() == 0)
			ProgExec(Load);
		break;
	case 266: // OneDeepExec Выполнить, если на первом уровне подсписка
		if (ListHead.size() == 1)
			ProgExec(Load);
		break;
	case 267: // MoreOneDeepExec Выполнить, если более, чем на первом уровне подсписка
		if (ListHead.size() >1)
			ProgExec(Load);
		break;
	case 268: //
		break;
	case 250: // SubUp Перейти на уровень выше (на входе количество уровней наверх, по умолчанию 1)
		{
			int t = Load.toInt(1);
			for (; ListHead.size() > 1 && t > 0; ListHead.pop_back(), t--);
			break;
	}
	case 251: // SubUpDel Перейти на уровень выше и уничтожить текущий уровень
		ICDel(ListHead.back()->back().Load.Point);
		ListHead.pop_back();
		break;
	case 252: // SubUpDelGraph Перейти на уровень выше и уничтожить текущий уровень с подсписками
		//GraphDel(ListHead.back()->back().Load.Point);
		ListHead.pop_back();
		break;
	case 253: // SubDeepOut Выдать номер текущего уровня
		Load.Write(ListHead.size());
		break;
	case 254: // SubDeepOutMk Выдать МК с номером текущего уровня
	{
		int t = ListHead.size();
		MkExec(MK, {Cint,&t});
		break;
	}
	case 255: // SubDown Перейти на уровень ниже (переход осуществляется на текущей найденной строке) в нагрузке атрибут указателя на подсписок (по умолчанию атрибут строки)
			  // Если в нагрузке IC, то переход текущего списка на этот указатель
		if (Load.Point == nullptr)
		{
			if (LineUk == nullptr) break;
			ip* t = AtrFind(LineUk->Load.Point, LineAtr);
			if (t == nullptr || !t->Load.isIC()) break;
			ListHead.push_back((IC_type)t->Load.Point);
		}
		else if (Load.isIC())
			ListHead.push_back((IC_type)Load.Point);
		else if (Load.Type >> 1 == Dint || Load.Type >> 1 == Dmk)
		{
			if (LineUk == nullptr) break;
			ip* t = AtrFind(LineUk->Load.Point,Load.toInt());
			if (t == nullptr || !t->Load.isIC()) break;
			ListHead.push_back((IC_type)t->Load.Point);
		}
		break;

	case 400: // LineOutMk Выдать МК с найденной линией
		if(LineUk==nullptr)
			MkExec(Load, {0,nullptr});
		else
			MkExec(Load, LineUk->Load);
		break;
	case 401: // LineOut Выдать найденную линиию
		if (LineUk == nullptr)
			Load.Write((void*)nullptr);
		else
			Load.Write(LineUk->Load.Point);
		break;
	case 415: // IpOut Выдать найденную ИП
		Load.Write(Searcher.IPRezPoint);
		break;
	case 416: // IpOutMk Выдать МК с найденной ИП
		MkExec(Load,{CIP, Searcher.IPRezPoint});
		break;
	case 417: // LoadOut Выдать нагрузку найденной ИП
		Load.Write(Searcher.IPRezPoint->Load);
		break;
	case 418: // LoadOutMk Выдать МК с нагрузкой найденной ИП
		MkExec(Load, Searcher.IPRezPoint->Load);
		break;
	case 419: // AtrOut Выдать атрибут наденной ИП
		Load.Write(Searcher.IPRezPoint->atr);
		break;
	case 420: // AtrOutMk Выдать МК с атрибутом найденной ИП
		MkExec(Load, { Cint, &Searcher.IPRezPoint->atr });
		break;
	case 425: // LastIpPopMk
		if (ListHead.back() != nullptr && ListHead.back()->size() > 0)
		{
			ip t = ((IC_type)ListHead.back()->back().Load.Point)->back();
			((IC_type)ListHead.back()->back().Load.Point)->back().Load.Clear();
			((IC_type)ListHead.back()->back().Load.Point)->pop_back();
			MkExec(Load, { TIP, &t });
		}

	case 405: // IpReceivedOut Выдать найденную в источнике ИП
		Load.Write(Searcher.IPTemplRezPoint);
		break;
	case 406: // IpReceivedOutMk Выдать МК с найденной в источнике ИП
		MkExec(Load, { CIP, Searcher.IPTemplRezPoint });
		break;
	case 407: // LoadReceivedOut Выдать нагрузку найденной в источнике ИП
		Load.Write(Searcher.IPTemplRezPoint->Load);
		break;
	case 408: // LoadReceivedOutMk Выдать МК с нагрузкой найденной в источнике ИП
		MkExec(Load, Searcher.IPTemplRezPoint->Load);
		break;
	case 409: // AtrReceivedOut Выдать атрибут наденной в источнике ИП
		Load.Write(Searcher.IPTemplRezPoint->atr);
		break;
	case 410: // AtrReceivedOutMk Выдать МК с атрибутом найденной в источнике ИП
		MkExec(Load, { Cint, &Searcher.IPTemplRezPoint->atr });
		break;

		// Программы по срезультатам сравнения номеров строк
	case 450: // EqProgExec
		if(Load.Point==nullptr)
			if (LineNumOld == LineNum) ProgExec(Load.Point);
		break;
	case 451: // BiggerExec
		if (LineNumOld < LineNum)
			if (Load.Point != 0)
				ProgExec(Load);
			else
				ProgExec(Prog);
		break;
	case 452: // SmallerExec
		if (LineNumOld > LineNum)
			if (Load.Point != 0)
				ProgExec(Load);
			else
				ProgExec(Prog);
			break;
	case 453: // BigerEqExec
		if (LineNumOld <= LineNum)
			if (Load.Point != 0)
				ProgExec(Load);
			else
				ProgExec(Prog);
		break;
	case 454: // SmallerEqExec
		if (LineNumOld >= LineNum)
			if (Load.Point != 0)
				ProgExec(Load);
			else
				ProgExec(Prog);
		break;
	case 455: // DifferenceOut Выдать различие старой найденной линии и новой
		Load.Write(LineNum - LineNumOld);
		break;
	case 456: // DifferenceOutMk Выдать МК с различием старой найденной линии и новой
	{
		int temp = LineNum - LineNumOld;
		MkExec(Load, { Cint,&temp });
		break; }
	case 459: // LineNumSet Установить номер найденной строки (нужно для организации операций сравнения)
		LineNum = Load.toInt();
		break;
	case 460: //Stack.back().LineCountOut Выдать количество найденных строк
		Load.Write(LineCount);
		break;
	case 461: //Stack.back().LineCountOutMk Выдать МК с количеством найденных строк
		MkExec(Load, { Cint, &LineCount });
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
		if (ListHead.back() != nullptr && LineUk != nullptr && ((IC_type)LineUk->Load.Point)->size()==0)
			ProgExec(Load);
		break;
	case 501: //LineFullExec  Выполнить подпрограмму, если текущая строка не пустая
		if (ListHead.back() != nullptr && LineUk != nullptr && ((IC_type)LineUk->Load.Point)->size() != 0)
			ProgExec(Load);
		break;
	case 505: // LastEmptyExec  Выполнить подпрограмму, если последняя строка пустая
		if (ListHead.back() != nullptr && ListHead.back()->size() != 0 && ((IC_type)ListHead.back()->back().Load.Point)->size() == 0)
			ProgExec(Load);
		break;
	case 506: //LastFullExec Выполнить подпрограмму, если последняя строка не пустая
		if (ListHead.back() != nullptr && ListHead.back()->size() != 0 && ((IC_type)ListHead.back()->back().Load.Point)->size() != 0)
			ProgExec(Load);
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
	if (ListHead.size() == 1 && PostfixProg!=nullptr)
		ProgExec(PostfixProg); // Выполнить финальную программу
}

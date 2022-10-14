// �� ������
#include "stdafx.h"
#include "List.h"

using namespace std;

void List::ProgFU(int MK, LoadPoint Load)
{
	if (PrefixProg != nullptr)
	{
		IC_type t = PrefixProg;
		PrefixProg = nullptr;
		ProgExec(t); // ��������������� ���������, ���������� ��� ������� ����� ��
		PrefixProg = t;
	}
	switch (MK)
	{
	case 0: // Reset
		Searcher.Clear();
		ListHead.clear();// ������� ����� ������� ������
		ListHead.push_back(nullptr);
		break;
	case 1:	// Set
		ListHead.back() = (IC_type)Load.Point;
		break;
	case 2:// Out ������ ������ �� ������
		if (Load.Type >> 1 == Cvoid)
			*(void**)Load.Point = ListHead.back();
		break;
	case 3:// OutMk ������ �� �� ������� �� ������
		if (!ListHead.size())
			MkExec(Load, { 0,nullptr });
		else
			MkExec(Load, { TIC,(void*)ListHead.back() });
		break;
	case 4: // RootOutMk ������ �� � ���������� �� �������� ������
		if(!ListHead.size())
			MkExec(Load, { 0,nullptr });
		else
			MkExec(Load, { TIC,(void*)*ListHead.begin() });
		break;
	case 5:	// MultiLineModeSet
		MultiLineMode = Load.toInt(1);
		break;
	case 6: // MultyListModeSet ���������� ����� ��������������� ������ ��� ������ (true �� ���������)
		MultyListMode = Load.toBool(true);
		break;
	case 7: // MkModeSet ����� ���������� ���� �� � ��-������� (��-�� ��������� ����� �������, ������ �������� ������ 0) ��� ������ �������� ����� ���������������
		Searcher.MkMode = Load.toBool(true);
		break;
	case 26: // CalcModeSet ���������� ����� ���������� ������� � ������� ����������-���������� ���������� (true �� ���������)
		Searcher.CalcMode = Load.toBool(true);
		break;
	case 27: // CalcMkSet ���������� �� ��� ���������� ���
		Searcher._CalcMk=Load.toInt();
		break;
	case 8:// EmptyProgExec ��������� ���������, ���� ������ ����
	case 9:// FullExec ��������� ���������, ���� ������ �� ����
	case 14: // OneLineExec ��������� ���������, ���� � ������ ������ ���� ������
	case 13: // MoreOneLineExec ��������� ���������, ���� � ������ ������ ����� ������
	case 19: // EmptyOrOneLineExec ��������� ���������, ���� � ������ ������ ���� �����
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
	case 10: // SuccessLineProgSet ���������� ��������� �� ���������, ����������� ��� ������� ������ � ����� ������
		Searcher.SuccessProg = (IC_type)Load.Point;
		break;
	case 11: // SuccessAfterLineProgSet ���������� ��������� �� ���������, ����������� ��� ������� ������ � ����� ������ ����� ���������� �������� �����
		Searcher.SuccessAfterProg = (IC_type)Load.Point;
		break;
	case 12: // Su��essProgSet ���������� ��������� �� ���������, ����������� � ������ �������� ������ �� ����  ������
		Su��essProg = Load.Point;
		break;
	case 23: // Su��essExec ��������� ���������, ���� ����� ���� ��� ������� �����
		if (Searcher.Rez)
			ProgExec(Load);
		break;
	case 24: // FailExec ��������� ���������, ���� ����� ���� ��� ������� �����
		if (!Searcher.Rez)
			ProgExec(Load);
		break;
	case 15: // FailLineProgSet ���������� ��������� �� ���������, ����������� � ������ ���������� ������ � ����� ������
		Searcher.FailProg = (IC_type)Load.Point;
		break;
	case 16: // FailAfterLineProgSet ���������� ��������� �� ���������, ����������� � ������ ���������� ������ � ����� ������ ����� ���������� �������� �����
		Searcher.FailAfterProg = (IC_type)Load.Point;
		break;
	case 17: // FailProgSet ���������� ��������� �� ���������, ����������� � ������ ���������� ������ �� ����  ������
		FailProg = Load.Point;
		break;
	case 18: //  ���������� ��������� �� ���������, ����������� � ������ �������� ������ � ������� �����
		Su��essLineProg = Load.Point;
		break;
	case 20: // ProgAtrSet ���������� ������� ����� ������ �� ���������
		if (Load.Point != nullptr && Load.Type >> 1 == Dint)
			Searcher.Prog_atr = Load.toInt();
		break;
	case 21: //LineAtrDef ���������� ������� ����� ������ �� ���������
		LineAtr = Load.toInt();
		break;
	case 22: //SubAtrDef ���������� ������� ��������� �� ���������
		SubAtr = Load.toInt();
		break;
	case 25: // BackOut ������ ������� ������ ��� ������
		if (Load.Type == TPPoint)
			(*(LoadPoint*)Load.Point) = Searcher.Obj;
		break;
	case 30: // BackOutMk ������� �� �� ������� �������� ��� ������
		MkExec(Load, Searcher.Obj);
		break;
	case 31: // BackOut ������ ������� ������ ��� ������
		if (Load.Type == TPPoint)
			(*(LoadPoint*)Load.Point) = Searcher.Obj.Clone();
		break;
	case 32: // CopyBackOutMk ������� �� �� ������� �������� ��� ������
		MkExec(Load, Searcher.Obj.Clone());
		break;

	case 35: // LoadBackOut ������ �������� �������� ������� ��� ������
		if (Searcher.Obj.Type >> 1 == DIC)
			Load.Point = ((IC_type)(Searcher.Obj.Point))->begin()->Load.Point;
		else if (Searcher.Obj.Type >> 1 == DIP)
			Load.Point = ((ip*)(Searcher.Obj.Point))->Load.Point;
		break;
	case 36: // LoadBackOutMk ������ �� � ��������� �������� ������� ��� ������
		if (Load.Type >> 1 == Dint)
			if (Searcher.Obj.Type >> 1 == DIC)
				MkExec(*((int*)Load.Point), ((IC_type)(Searcher.Obj.Point))->begin()->Load);
			else if (Searcher.Obj.Type >> 1 == DIP)
				MkExec(*((int*)Load.Point), ((ip*)(Searcher.Obj.Point))->Load);
		break;

	case 100: // RezOut ������ ���������� ���������
		if (Load.Type == Tbool)
			Load.Write(Searcher.Rez);
		break;
	case 101: // RezOutMk ������ �� � ����������� ���������
		MkExec(*(int*)(Load.Point), { Cbool , &Searcher.Rez });
		break;
	case 105: // MkAtrAdd �������� ��
		if (Load.Type >> 1 == Dint)
			Searcher.MkAtrAdd(*(int*)Load.Point);
		break;
	case 106: // MkAtrClear �������� ������ ��
		Searcher.MkAtrClear();
		break;

	case 120: // MarkAtrSet ���������� ������� ������� ������
		if (LineUk != nullptr)
			LineUk->atr = Load.toInt();
		break;
	case 121: // LastMarkAtrSet ���������� ������� ��������� ������
		if (ListHead.back() != nullptr)
			(ListHead.back()->end() - 1)->atr = Load.toInt();
		break;
	case 122: //MarkAtrOut ������ ������ ������� ������
		if (LineUk != nullptr)
			Load.Write(LineUk->atr);
		break;
	case 123: //MarkAtrOutMk ������ �� � �������� ������� ������
		if (LineUk != nullptr)
			MkExec(Load, { Cint, &LineUk->atr });
		break;
	case 124: //LastMarkAtrOut ������ ������ ��������� ������
		if (ListHead.back() != nullptr && !ListHead.back()->size())
			Load.Write((ListHead.back()->end() - 1)->atr);
		break;
	case 129: //LastMarkAtrOutMk ������ �� � �������� ��������� ������
		if (ListHead.back() != nullptr && !ListHead.back()->size())
			MkExec(Load, { Cint, &(ListHead.back()->end() - 1)->atr });
		break;
	case 125: //  MarkAtrInc ��������� �������� ������� ������
		if (LineUk != nullptr)
			LineUk->atr++;
		break;
	case 126: // LastMarkAtrInc ��������� �������� ��������� ������
		if (ListHead.back() != nullptr)
			(ListHead.back()->end() - 1)->atr++;
		break;
	case 127: //  MarkAtrDec ��������� �������� ������� ������
		if (LineUk != nullptr)
			LineUk->atr--;
		break;
	case 128: // LastMarkAtrDec ��������� �������� ��������� ������
		if (ListHead.back() != nullptr)
			(ListHead.back()->end() - 1)->atr--;
		break;
	case 130: // MarkAtrEQExec ������ ��������� ��� ����/��������� �������� ������� ������ ��� �������� c ��������� ��
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
	case 131: // LastMarkEQExec ������ ��������� ��� ����/��������� �������� ��������� ������ ��� �������� c ��������� ��
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
	case 133: // MarkAtrNEQExec ������ ��������� ��� �� ����/����������� �������� ������� ������
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
	case 134: // LastMarkAtrNEQExec ������ ��������� ��� �� ����/����������� �������� ��������� ������
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
	case 135: // MarkAtrBiggerExec ������ ��������� ��� �������� ������� ������ ������ (������ 0)
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
	case 136: // LastMarkAtrBiggerExec ������ ��������� ��� �������� ��������� ������ ������ (������ 0)
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
	case 137: // MarkAtrBiggerEQExec ������ ��������� ��� �������� ������� ������ ������ ��� ������ 0
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
	case 138: // LastMarkAtrBiggerEQExec ������ ��������� ��� �������� ��������� ������ ������ ��� ������ 0
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
	case 139: // MarkAtrLessExec ������ ��������� ��� �������� ������� ������ ������ 0
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
	case 140: // LastMarkAtrLessExec ������ ��������� ��� �������� ��������� ������ ������ 0
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
	case 141: // MarkAtrLessEQExec ������ ��������� ��� �������� ������� ������ ������ ��� ������ 0
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
	case 142: // LastMarkAtrLessEQExec ������ ��������� ��� �������� ��������� ������ ������ ��� ������ 0
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

	case 150: //LastOut ������ ������ �� ��������� ����� ������
	case 151: //LastPop ������ ������ �� ��������� ����� ������ � ������� �� ������
	case 152: //LastDel ������ ������ �� ��������� ����� ������ � ������� �� ��
		if (Load.Type == Tvoid)
			*(void**)Load.Point = (void*)ListHead.back()->back().Load.Point;
		if (MK == 152) ICDel((void*)ListHead.back()->back().Load.Point);
		if (MK == 151 || MK == 152)
			ListHead.back()->pop_back();
		break;
	case 153: //LastIpOutMk ������ ������ �� ��������� �� ��������� �����
		MkExec(Load, { CIP,(void*)&((IC_type)ListHead.back()->back().Load.Point)->back() });
		break;
	case 154: // LastIpCopyOutMk ������ �� �� ������� �� ��������� �� ��������� �����
		MkExec(Load, { TIC,((IC_type)ListHead.back()->back().Load.Point)->back().�lone() });
		break;
	case 155: //LastOutMK ������ ������ �� ��������� ����� ������
	case 156: //LastPopMK ������ ������ �� ��������� ����� ������ � ������� �� ������
	case 157: //LastDelMk ������ ������ �� ��������� ����� ������ � ������� �� ��
		if (ListHead.back() == nullptr || !ListHead.back()->size()) break;
		MkExec(Load, ListHead.back()->back().Load);
		if (MK == 157) ICDel((void*)ListHead.back()->back().Load.Point);
		if (MK == 156 || MK == 157)
			ListHead.back()->pop_back();
		break;
	case 158: // LastLoadOut ������ �������� ��������� �� ��������� �����
		Load.Write(((IC_type)(ListHead.back()->back().Load.Point))->back().Load);
		//		*(LoadPoint*)Load.Point = ((IC_type)(ListHead.back()->back().Load.Point))->back().Load;
		break;
	case 169: // LastLoadOutMk ������ �� � ��������� ��������� �� ��������� �����
		MkExec(Load, ((IC_type)(ListHead.back()->back().Load.Point))->back().Load);
		break;
	case 188: // LastLoadRefOut ������ ������ �� Load ��������� �� ��������� ������ ������
		// Load.Write(&(IC_type)(ListHead.back()->back().Load.Point))->back().Load);
		break;
	case 198: // LastLoadRefOutMk ������ �� �� ������� �� Load ��������� �� ��������� ������ ������
		MkExec(Load, { TLoad, &((IC_type)(ListHead.back()->back().Load.Point))->back().Load });
		break;
	case 145: //LineIpOutMk ������ ������ �� ��������� �� ������� �����
		if (ListHead.back() == nullptr || !ListHead.back()->size() || LineUk == nullptr) break;
		MkExec(Load, { CIP,&((IC_type)LineUk->Load.Point)->back() });
		break;
	case 146: // LineIcCopyOutMk ������ �� �� ������� �� ��������� �� ������� �����
		//		MkExec(Load, { CIC,((IC_type)ListHead.back()->back().Load.Point)->back().�loneToIC() });
		break;
	case 147: // LastIpOutMk ������ ������ �� ��������� �� ������� �����
		MkExec(Load, { CIP,&((IC_type)ListHead.back()->back().Load.Point)->back() });
		break;
	case 192: // LineCopyAddPrevSet �������� ����� ������ � ���������� ������ �� ��� � ���������� �����
	{
		if (ListHead.back()->size()==0) // ���� ������ ������, �������� �����
			ListHead.back()->push_back({ LineAtr, TIC, nullptr });
		ListHead.back()->push_back({ LineAtr,Load.Copy()});// ->push_back(LineAtr, TIC, new vector<ip>);
		if (Load.Point == nullptr)
			ListHead.back()->back().Load = { TIC,new vector<ip> };
		ListHead.back()->at(ListHead.back()->size() - 2).Load = ListHead.back()->back().Load;
		break;
	}
	case 163: // LineCopyAddPrevLoadSet
	case 159: // LastCopyAddPrevLoadSetLoadMov �������� ����� ����� ��������� �������� � �� ����� ������ � �������� � �������� ���������� ������ ������ �� ����� ������ 
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
		else if (ListHead.back()->size() == 1) // ���������� � ������ ������
		{
			ListHead.back()->insert(ListHead.back()->begin(), {LineAtr, ListHead.back()->back().Load});
		}
		if (MK == 159)
			((IC_type)ListHead.back()->back().Load.Point)->back().Load = t;
		break;
	}
	case 160: // LineAdd �������� ����� ������ �������
		if (ListHead.back() == nullptr)
			ListHead.back() = new vector<ip>;
	    ListHead.back()->push_back({ LineAtr, Load });
		break;
	case 161: // LineCopyAdd �������� ����� ������
		if (ListHead.back() == nullptr) ListHead.back() = new vector<ip>;
		if (Load.Point != nullptr)
			ListHead.back()->push_back({ LineAtr, TIC, ICCopy(Load).Point });
		else
			ListHead.back()->push_back({ LineAtr, TIC, new vector<ip> }); // �������� ������ ������
		break;
		//	case 163: //  LineCopyAddPrevLoadSet �������� ����� � ������ � ��������� ������ �� ��� � �������� ��������� �� ��������� ������
		//	 	if (ListHead.back() == nullptr) ListHead.back() = new vector<ip>;
		//		if(Load.Point==nullptr)
		//			ListHead.back()->push_back({ LineAtr, TIC, new vector<ip> });
		//		else
		//			ListHead.back()->push_back({ LineAtr, TIC, ICCopy(Load) });
		//		if (ListHead.back()->size() > 1)
		//			((IC_type)ListHead.back()->at(ListHead.back()->size() - 2).Load.Point)->back().Load = ListHead.back()->back().Load;
		//		break;
	case 162: // LineCopyTreeAdd �������� ����� ��-�����
		if (ListHead.back() == nullptr) ListHead.back() = new vector<ip>;
		// ....
		break;
	case 164: // LineLoadOutMk ������ �� � ��������� ��������� �� ������� �����
		MkExec(Load, ((IC_type)(LineUk->Load.Point))->back().Load);
		break;
	case 165: // LinePopMk ��������� ����� ������
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0) break;
		MkExec(Load, { TIC , (void*)ListHead.back()->back().Load.Point });
		ListHead.back()->pop_back();
		break;
	case 166: // LineDelMk ������ �� �� ������� �� �� � ������� ��
	{
		if (ListHead.back() == nullptr || ListHead.back()->size() == 0) break;
		LoadPoint t = ListHead.back()->back().Load;
		ListHead.back()->pop_back();
		MkExec(Load, t);
		ICDel(ListHead.back()->back().Load.Point);
		break;
	}
	case 167: // LineIpCut ������� ��������� �� �� ������� ����� (� �������� ���������� ��������� ��, �� ��������� 1)
	case 168: // LineVarIpCut" Hint = "������� ��������� �� c ����������� �� ������� ����� (� �������� ���������� ��������� ��, �� ��������� 1)" }
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
	case 174: // LastConstVarTypeSet ���������� ��� ���������/���������� ��� ��������� �������� (�� ��������� ����������)
	if (ListHead.size() && ListHead.back() != nullptr)
	{
		((IC_type)ListHead.back()->back().Load.Point)->back().Load.Type |= 1;
		if (!Load.toBool(false))
			((IC_type)ListHead.back()->back().Load.Point)->back().Load.Type -= 1;
	break;
	}
	case 170: // LastAttach ������������ �� � ��������� ����� ������
	case 171: // LastCopyAttach ������������ ����� �� � ��������� ����� ������
	case 172: // LastCopyAttachLoadMove �������� �� � ���������� ��������� ��������
	{
		if (ListHead.back() == nullptr)
		{
			ListHead.back() = new vector<ip>;
			ListHead.back()->push_back({ LineAtr,{ TIC,new vector<ip>} });

		}
		if (!ListHead.back()->size())
			ListHead.back()->push_back( {LineAtr, TIC, new vector<ip> }); // �������� ������ ������ � ������
		if (Load.Point == nullptr)
			((IC_type)ListHead.back()->back().Load.Point)->push_back({ 0,0, nullptr }); // �������� ������ ��, ���� Load==nil
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
	case 175: // LineAttach ������������ �� � ������� ����� ������
	case 176: // LineCopyAttach ������������ ����� �� � ������� ����� ������
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

	case 185: // LastLoadSet �������� ����� � �������� ��������� �� ��������� ������
	case 195: // LineLoadSet �������� ����� � �������� ��������� �� ��������� ������
	case 186: // LastLoadCopySet �������� ����� � �������� ��������� �� ��������� ������
	case 196: // LineLoadCopySet �������� ����� � �������� ��������� �� ��������� ������
	case 187: // LastPointLoadCopyGraphSet
	case 197: // LinePointLoadCopyGraphSet
	case 189: // LastAtrSet ���������� ������� � ��������� �� ��������� ����� 
	case 199: // LineAtrSet ���������� ������� � ��������� �� ��������� ����� 
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
	case 180: // LineVarTypeSet // ���������� ��� '����������' � ��������� �� ������� �����
		if (ListHead.back() == nullptr || !ListHead.back()->size() || \
			LineUk == nullptr || LineUk->Load.Point == nullptr || \
			LineUk->Load.Type >> 1 != DIC || !((IC_type)LineUk->Load.Point)->size())
			break;
		((IC_type)LineUk->Load.Point)->back().Load.VarTypeSet(Load.toBool(true));
		break;
	case 182: // LastVarTypeSet // ���������� ��� '����������' � ��������� �� ��������� �����
		if (ListHead.back() == nullptr || !ListHead.back()->size() || \
			ListHead.back()->back().Load.Type >> 1 != DIC || !((IC_type)ListHead.back()->back().Load.Point)->size())
			break;
		((IC_type)ListHead.back()->back().Load.Point)->back().Load.VarTypeSet(Load.toBool(true));
		break;
	case 190: // LastLoadIcEmptySet ���������� ������ �� ������ �� � �������� ��������� �� ��������� ������
		if (ListHead.back() != nullptr && !ListHead.size() && ((IC_type)ListHead.back()->back().Load.isIC()))
			((IC_type)ListHead.back()->back().Load.Point)->back().Load = { TIC, new vector<ip> };
		break;
	case 191: // LineLoadIcEmptySet ���������� ������ �� ������ �� � �������� ��������� �� ������� ������
		if (LineUk != nullptr && ((IC_type)LineUk->Load.isIC()))
			((IC_type)LineUk->Load.Point)->back().Load = {TIC, new vector<ip>};
		break;
	case 200: // LineToLast ���������� ������� ������ �� ��������� ������
		if (ListHead.back() != nullptr && ListHead.back()->size() > 0 && ListHead.back()->back().Load.Point != nullptr && ListHead.back()->back().Load.Type >> 1 == DIC)
			LineUk = &ListHead.back()->back();
		break;
	case 205: // LinePush �������� ������� ����� � ����
		if (LineNum >= 0 && LineNum < ListHead.back()->size() > 0)
			LineStack.push_back(LineNum);
		break;
	case 206: // LineLastPush �������� ��������� ����� � ����
		if (ListHead.back()->size() > 0) LineStack.push_back(ListHead.back()->size() - 1);
		break;
	case 207: // LinePop ����� ������� ����� �� �����
		if (LineStack.size() && ListHead.back()->size() > LineStack.back() && LineStack.back() >= 0)
		{
			LineNum = LineStack.back();
			LineStack.pop_back();
			LineUk = ListHead.back()->begin()._Ptr + LineNum;
		}
		break;

	case 210: // LastAtrAttach ���������� � ��������� ������ �� � ��������� �� ��������
		if (ListHead.back() == nullptr)
			ListHead.back() = new vector<ip>;
		if (!ListHead.back()->size())
			ListHead.back()->push_back({ LineAtr, TIC, new vector<ip> });
		((IC_type)ListHead.back()->back().Load.Point)->push_back({ Load.toInt() ,0, nullptr});
		break;
	case 211: // LineAtrAttach ���������� � ������� ������ �� � ��������� �� ��������
		if (LineUk == nullptr) break;
		if (LineUk->Load.Point == 0)
			LineUk->Load = { TIC, new vector<ip> };
		((IC_type)LineUk->Load.Point)->push_back({ Load.toInt() ,0, nullptr });
		break;
	case 215: // LastLoadBackSet �������� �������� ��������� �� ��������� ������ � ���������� �� (�� ����� ���������� �������� �������)
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
	case 216: // LineLoadBackSet �������� �������� ��������� �� ������� ������ � ���������� �� (�� ����� ���������� �������� �������)
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
		Searcher.FindXor(Load); // �������� !!!
		break;
	case 226: //FindAndSourceLast
		Searcher.Template = { 0,nullptr };
		if (ListHead.size() && ListHead.back() != nullptr && ListHead.back()->size())
			Searcher.Template = ListHead.back()->back().Load;
		Searcher.FindAndSource(Load);
		break;
	case 227: //FindAndSourceLine
		Searcher.Template = LineUk->Load;
		Searcher.FindAndSource(Load); // �������� !!!
		break;

	case 228: // FindOr ����� ���
	case 229: // FindAnd ����� �
	case 230: // FindXor ����� XOR
	case 231: // FindAndSource ����� � � ���������
	{
		if (!ListHead.size()) break;
		if (DeepStartSearch == 0)
		{
			DeepStartSearch = ListHead.size();
			LineCount = 0; // ������� ����������
			LineNumOld = LineNum;
			LineUk = nullptr; // ������ �� ��������� ������
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
			if (MultyListMode && AtrSearch(i->Load.Point, LineAtr)) // ������� �������� �� ������ ������� ������
			{
				ListHead.push_back((IC_type)i->Load.Point); // �������� �������� � ���� ����������
				ListHead.back() = (IC_type)i->Load.Point;
				ProgFU(MK, Load); // ����� �� ��������� ������

				ListHead.pop_back(); // ������ �� ����� ����������
				if (LineCount && !MultiLineMode) // ���� ����� �� ������� ����������, �� �����
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
						LineNum = k;// ��������� ����� ������ ������ (k- ������� ������� ����� � ��� ����� � � ������������� ������)
						LineUk = i._Ptr; // ��������� ��������� ������
					}

				}
				if (MultiLineMode == 0) break;
			}
		}
		if (LineCount && ListHead.size() == DeepStartSearch)
		{
			//LineUk = &ListHead.back()->at(LineNumFirst);
			ProgExec(Su��essProg);
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

	// �������� � �����������
	case 238: // ToRoot ������� � ��������� ������ ������
		while (ListHead.size() > 1)
			ListHead.pop_back(); // ������� � ��������� ������
		break;
	case 240: // Push SubAdd �������� �� �� ����� ������� ������� � ���������� �� ����
	case 241: // PushCopy SubCopyAdd �������� ����� �� �� ����� ������� ������ � ���������� �� ����
	case 242: // SubOACopy SubOACopyAdd �������� ����� ��-����� �� ����� ������� ������ � ���������� �� ����
	case 243: // PushTied SubRootAdd �������� �� �� ����� ������� ������� � ���������� �� ���� �������� � ����� ������� ������ �� ���. ������
	case 244: // PushTiedCopy SubRootCopyAdd �������� ����� �� �� ����� ������� ������ � ���������� �� ���� �������� � ����� ������� ������ �� ���. ������
	case 245: // PushTiedOACopy SubRootOACopyAdd �������� ����� ��-����� �� ����� ������� ������ � ���������� �� ���� �������� � ����� ������� ������ �� ���. ������
	case 246: // PushTiedLine �������� �� �� ����� ������� ������� � ���������� �� ���� �������� � ����� ������� ������ �� ���. ������
	case 247: // PushTiedLineCopy �������� ����� �� �� ����� ������� ������ � ���������� �� ���� �������� � ����� ������� ������ �� ���. ������
	case 248: // PushTiedLineOACopy �������� ����� ��-����� �� ����� ������� ������ � ���������� �� ���� �������� � ����� ������� ������ �� ���. ������
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
				; // ����� ��-�����
		if (MK >= 243) //���������� ������ � ����������� ������
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
	
	case 260: // Pop ���������� ���������
	case 261: // PopMk ���������� ��������� � ������ �� � ���������� �� ����
	case 262: // PopMkDel ���������� ��������� � ������ �� � ���������� �� ���� � ������� ��������
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
	case 265: // ZeroDeepExec ���������, ���� ��� ����������
		if (ListHead.size() == 0)
			ProgExec(Load);
		break;
	case 266: // OneDeepExec ���������, ���� �� ������ ������ ���������
		if (ListHead.size() == 1)
			ProgExec(Load);
		break;
	case 267: // MoreOneDeepExec ���������, ���� �����, ��� �� ������ ������ ���������
		if (ListHead.size() >1)
			ProgExec(Load);
		break;
	case 268: //
		break;
	case 250: // SubUp ������� �� ������� ���� (�� ����� ���������� ������� ������, �� ��������� 1)
		{
			int t = Load.toInt(1);
			for (; ListHead.size() > 1 && t > 0; ListHead.pop_back(), t--);
			break;
	}
	case 251: // SubUpDel ������� �� ������� ���� � ���������� ������� �������
		ICDel(ListHead.back()->back().Load.Point);
		ListHead.pop_back();
		break;
	case 252: // SubUpDelGraph ������� �� ������� ���� � ���������� ������� ������� � �����������
		//GraphDel(ListHead.back()->back().Load.Point);
		ListHead.pop_back();
		break;
	case 253: // SubDeepOut ������ ����� �������� ������
		Load.Write(ListHead.size());
		break;
	case 254: // SubDeepOutMk ������ �� � ������� �������� ������
	{
		int t = ListHead.size();
		MkExec(MK, {Cint,&t});
		break;
	}
	case 255: // SubDown ������� �� ������� ���� (������� �������������� �� ������� ��������� ������) � �������� ������� ��������� �� ��������� (�� ��������� ������� ������)
			  // ���� � �������� IC, �� ������� �������� ������ �� ���� ���������
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

	case 400: // LineOutMk ������ �� � ��������� ������
		if(LineUk==nullptr)
			MkExec(Load, {0,nullptr});
		else
			MkExec(Load, LineUk->Load);
		break;
	case 401: // LineOut ������ ��������� ������
		if (LineUk == nullptr)
			Load.Write((void*)nullptr);
		else
			Load.Write(LineUk->Load.Point);
		break;
	case 415: // IpOut ������ ��������� ��
		Load.Write(Searcher.IPRezPoint);
		break;
	case 416: // IpOutMk ������ �� � ��������� ��
		MkExec(Load,{CIP, Searcher.IPRezPoint});
		break;
	case 417: // LoadOut ������ �������� ��������� ��
		Load.Write(Searcher.IPRezPoint->Load);
		break;
	case 418: // LoadOutMk ������ �� � ��������� ��������� ��
		MkExec(Load, Searcher.IPRezPoint->Load);
		break;
	case 419: // AtrOut ������ ������� �������� ��
		Load.Write(Searcher.IPRezPoint->atr);
		break;
	case 420: // AtrOutMk ������ �� � ��������� ��������� ��
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

	case 405: // IpReceivedOut ������ ��������� � ��������� ��
		Load.Write(Searcher.IPTemplRezPoint);
		break;
	case 406: // IpReceivedOutMk ������ �� � ��������� � ��������� ��
		MkExec(Load, { CIP, Searcher.IPTemplRezPoint });
		break;
	case 407: // LoadReceivedOut ������ �������� ��������� � ��������� ��
		Load.Write(Searcher.IPTemplRezPoint->Load);
		break;
	case 408: // LoadReceivedOutMk ������ �� � ��������� ��������� � ��������� ��
		MkExec(Load, Searcher.IPTemplRezPoint->Load);
		break;
	case 409: // AtrReceivedOut ������ ������� �������� � ��������� ��
		Load.Write(Searcher.IPTemplRezPoint->atr);
		break;
	case 410: // AtrReceivedOutMk ������ �� � ��������� ��������� � ��������� ��
		MkExec(Load, { Cint, &Searcher.IPTemplRezPoint->atr });
		break;

		// ��������� �� ������������ ��������� ������� �����
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
	case 455: // DifferenceOut ������ �������� ������ ��������� ����� � �����
		Load.Write(LineNum - LineNumOld);
		break;
	case 456: // DifferenceOutMk ������ �� � ��������� ������ ��������� ����� � �����
	{
		int temp = LineNum - LineNumOld;
		MkExec(Load, { Cint,&temp });
		break; }
	case 459: // LineNumSet ���������� ����� ��������� ������ (����� ��� ����������� �������� ���������)
		LineNum = Load.toInt();
		break;
	case 460: //Stack.back().LineCountOut ������ ���������� ��������� �����
		Load.Write(LineCount);
		break;
	case 461: //Stack.back().LineCountOutMk ������ �� � ����������� ��������� �����
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
	// ��������� �� ������� �����
	case 500: // LineEmptyExec ��������� ������������, ���� ������� ������ ������
		if (ListHead.back() != nullptr && LineUk != nullptr && ((IC_type)LineUk->Load.Point)->size()==0)
			ProgExec(Load);
		break;
	case 501: //LineFullExec  ��������� ������������, ���� ������� ������ �� ������
		if (ListHead.back() != nullptr && LineUk != nullptr && ((IC_type)LineUk->Load.Point)->size() != 0)
			ProgExec(Load);
		break;
	case 505: // LastEmptyExec  ��������� ������������, ���� ��������� ������ ������
		if (ListHead.back() != nullptr && ListHead.back()->size() != 0 && ((IC_type)ListHead.back()->back().Load.Point)->size() == 0)
			ProgExec(Load);
		break;
	case 506: //LastFullExec ��������� ������������, ���� ��������� ������ �� ������
		if (ListHead.back() != nullptr && ListHead.back()->size() != 0 && ((IC_type)ListHead.back()->back().Load.Point)->size() != 0)
			ProgExec(Load);
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
	if (ListHead.size() == 1 && PostfixProg!=nullptr)
		ProgExec(PostfixProg); // ��������� ��������� ���������
}

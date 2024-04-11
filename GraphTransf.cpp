#include "stdafx.h"
#include "GraphTransf.h"
#include <algorithm>
#include <vector>

using namespace std;

void GraphTransf::ProgFU(int MK, LoadPoint Load, FU* Sender)
{
	switch (MK)
	{
		case 0:// �����
			if (LTable != nullptr)
			{
				LTable->Clear();
				LTable->HostPointSet(nullptr);
			}
			break;
	////////////////////////// ������� ������������ ////////////////////////////
		case 1:// TableSet ���������� ������ �� ������� �������������� �������
			LTable = (LocatTable*)Load.Point;
			//Load.print();
			break;
		case 2: // Clear �������� �������
			LTable->Clear();
			break;
		case 5: // AddLine �������� �����
			LTable->AddLine(Load.Point);
			break;
		case 10: // AddToLastLine
			LTable->AddToLastLine(Load.Point);
			break;
		case 15: // HostPointSet
			LTable->HostPointSet(Load.Point);
			break;
		case 17: // PointSet
			LTable->AddPointToHost(Load.Point);
			break;
		case 18: // 
			LTable->Index(Load.Point);
			break;
		case 19: //
			LTable->DelLastLine();
			break;
		case 20: //
			LTable->DelLineWithPoint(Load.Point);
			break;
	////////////////////////// ����������� ��� ������������ �� ////////////////////////////
		case 45: // NavigatorPush �������� �������
			if (Load.Type >> 1 == DIC) {
				navigator.CapsPush((IC_type)Load.Point, PBuf);
			}
			break;
		case 46: // NavigatorPop ������� �������
			if (navigator.size() > 0)
				navigator.CapsPop();
			break;
		case 47: // OpenrandSet ���������� �������
			Operands.clear();
			Operands.push_back((IC_type)Load.Point);
			break;
		case 48: // OpenrandAdd �������� �������
			Operands.push_back((IC_type)Load.Point);
			break;
		case 49: // OperandPop ������� ��������� �������
			Operands.pop_back();
			break;
	////////////////////////// ������������ �� ////////////////////////////
		case 50: // ConcatFromIPToIP ��������������� �� �� �������� (�� ����� 2 �� ��� ����, ����� ����� �� � ��������� � � ���������)
		case 51: // MoveFromIPToIP ��������������� �� �� �������� � ������� �������� �� (�� ����� 2 �� ��� ����, ����� ����� �� � ��������� � � ���������)
		case 52: // ConcatFromIPDelIP ��������������� �� �� �������� � ������� �� (�� ����� 2 �� ��� ����, ����� ����� �� � ��������� � � ���������)
		{
			if (!Load.isIC() || ICLen(Load.Point) < 2 || Operands.size() < 2) break;
			vector<void*>* Op2 = LTable->GetLine(Operands.back());
			vector<void*>* Op1 = LTable->GetLine(*(Operands.end()-2));
			if (Op1 == nullptr || Op2 == nullptr) break;
			auto ip1 = IPSearch(*(Op1->begin() + 1), *(((IC_type)Load.Point)->begin()));
			if (ip1 == ((IC_type)*(Op1->begin() + 1))->end() ) break;
			for (auto i = Op2->begin() + 1; i != Op2->end(); i++)
			{
				auto ip2 = IPSearch(*i, *(((IC_type)Load.Point)->begin() + 1));
				if (ip2 == ((IC_type) * (Op2->begin() + 1))->end()) continue;
				if (ICLen(Load.Point) == 2)
					ICCopyConcat(ip1->Load.Point, ip2->Load.Point);
				else
					IPAdd(ip1->Load, { (((IC_type)Load.Point)->begin() +2)->atr , ICCopy(ip2->Load) });
				if (MK == 51 || MK==52) ICDel(ip2->Load); // �������� �������� ��
				if (MK == 52)
					((IC_type) * (Op2->begin() + 1))->erase(ip2);
			}
			break;
		}

		case 54: // ConcatCopy ��������������� ����� �� � ������ ���������
			if (Load.isIC() && Operands.size())
			{
				vector<void*>* Op2 = LTable->GetLine(Load.Point);
				vector<void*>* Op1 = LTable->GetLine(Operands.back());
				if (Op1 == nullptr || Op2 == nullptr) break;
				for(auto i=Op1->begin()+1; i!=Op1->end();i++)
					for (auto j = Op2->begin() + 1; j != Op2->end(); j++)
						ICCopyConcat((IC_type)*i, (IC_type)*j);
				//			if (Load.Type >> 1 == DIC){
				//				for (auto i = ((IC_type)Load.Point)->begin()._Ptr; i != ((IC_type)Load.Point)->end()._Ptr; i++) {
				//					navigator.CurrentPos()->Ic->push_back(*i);
				//				}
				//			}
				//if (MK == 50) delete (IC_type)Load.Point;
			}
			break;
		case 55: // ConcatOfAccum ��������������� ��
		case 56: // ConcatCopyOfAccum ��������������� ����� ��
			if (Load.Type >> 1 == DIC)
				copy(Operands.back()->begin(), Operands.back()->end(), inserter(*Operands.back(), ((IC_type)Load.Point)->end()));
			if (MK == 55) { delete Operands.back(); Operands.pop_back();}
			for (auto i = Operands.begin()._Ptr; i < Operands.end()._Ptr; i++) {
				cout << i << endl;
			}
			break;
	////////////////////////// ������ ������ � ����� ////////////////////////////
		case 60: // PointSet ������ ������ �� � �����
		case 61: // PointAdd ���������� ������ �� � �����
		{
		vector<void*>* row = LTable->GetRow(Load.Point);
		if (row != nullptr && row->size() > 1) {
			for (auto i = ((IC_type)row->at(1))->begin(); i != ((IC_type)row->at(1))->end(); i++)
				if (IPCmp((ip*)i._Ptr, (ip*)Load.Point)) {
					navigator.currentPos->clear();
					navigator.CapsPush(((IC_type)row->at(1)), PBuf);
					navigator.IpPush(i._Ptr);
					break;
				}
		}
		break;
		}
	////////////////////////// ������ �������� ////////////////////////////
		case 65: // VarSet ������ �������� � ������ ������ �� �������� ��
//			bufIp->Load = Load;
			break;
		case 66: // LoadSet ������ ������ �� ��������� ��������
			for (int i = 0; i < navigator.CurrentPos()->Ic->size(); i++) {
				if (IPCmp(&navigator.CurrentPos()->IpList->at(0), &navigator.CurrentPos()->Ic->at(i))) {
					navigator.CurrentPos()->Ic->at(i).Load = Load;
					break;
				}
			}
			break;
		case 67: // AtrSet ������ ������ � �������
			for (int i = 0; i < navigator.CurrentPos()->Ic->size(); i++) {
				if (IPCmp(&navigator.CurrentPos()->IpList->at(0), &navigator.CurrentPos()->Ic->at(i))) {
					navigator.CurrentPos()->Ic->at(i).atr = *(int*)Load.Point;
					break;
				}
			}
			break;
		case 68: // IpSet ������ ������ � ��
			for (int i = 0; i < navigator.CurrentPos()->Ic->size(); i++) {
				if (IPCmp(&navigator.CurrentPos()->IpList->at(0), &navigator.CurrentPos()->Ic->at(i))) {
					if (Load.Type >> 1 == DIP)
						navigator.CurrentPos()->Ic->at(i) = (ip&)Load.Point;
					else if (Load.Type >> 1 == DIC)
						navigator.CurrentPos()->Ic->at(i) = *((IC_type)Load.Point)->begin();
					break;
				}
			}
			break;
	////////////////////////// �������� ////////////////////////////
		case 70: // DelIp  �������� ��������� �� (����� ��������� ���������� ���������� ������ �� ��, ��� ��������� ��)
		{
			if (!Operands.size() || !(Load.isIC() || Load.isIP())) break;
			vector<void*>* Op1 = LTable->GetLine(Operands.back());
			vector<void*>* Op2 = LTable->GetLine(Load.Point);
			if (Op1 == nullptr || Op2 == nullptr) break;
			for (auto i = Op1->begin() + 1; i != Op1->end(); i++)
				for(auto j=(*(IC_type) *i).begin(); j != (*(IC_type) *i).end(); j++)
					if((void*)j._Ptr==*(Op2->begin()+1))
					{
						(*(IC_type)*i).erase(j);
						break;
					}
			/*
			vector<void*>* row = LTable->GetRow(Load.Point);
			if (row != nullptr && row->size() > 1) {
				ip* IP;
				if (Load.isIP())
					IP = (ip*)Load.Point;
				else if (Load.isIC())
					IP = ((IC_type)Load.Point)->begin()._Ptr;
				else
					return;

				for (auto i = ((IC_type)row->at(1))->begin(); i != ((IC_type)row->at(1))->end(); i++)
					if (IPCmp((ip*)i._Ptr, IP)) {
						delIC.IC = new vector<ip*>();
						delIC.IC->push_back((ip*)i._Ptr);
						((IC_type)row->at(1))->erase(i);
						break;
					}
			}
			for (int i = navigator.currentPos->size()-1; i >= 0; i--) {
				if (navigator.currentPos->at(i).type == PDel) {
					//row = LTable->GetRow(Load.Point);
					delIC.IC = new vector<ip*>();
					delIC.IC->push_back(&navigator.currentPos->at(i).IpList->at(0));
					for (int j = 0; j < navigator.currentPos->at(i).Ic->size(); j++) {
						if (IPCmp(&navigator.currentPos->at(i).IpList->at(0), &navigator.currentPos->at(i).Ic->at(j))) {
							navigator.currentPos->at(i).Ic->erase(navigator.currentPos->at(i).Ic->begin() + j);
						}
					}
				}
			navigator.currentPos->erase(navigator.currentPos->begin() + i);
			}
			*/
			break;
		}
		case 71: // DelIpAdd �������� �� ��� ��������� ��������
		{
			vector<void*>* row = LTable->GetRow(Load.Point);
			if (row != nullptr && row->size() > 1) {
				for (auto i = ((IC_type)row->at(1))->begin(); i != ((IC_type)row->at(1))->end(); i++)
					if (IPCmp((ip*)i._Ptr, (ip*)Load.Point)) {
						navigator.CapsPush((IC_type)row->at(1), PDel);
						navigator.IpPush((ip*)i._Ptr);
						break;
					}
			}
			break;
		}
	////////////////////////// ������� ////////////////////////////
		case 75: // InsertBefore �������� �� �� �������
		case 76: // InsertAfter �������� �� ����� �������
			if (navigator.CurrentPos()->IpList == nullptr) {
				navigator.CurrentPos()->Ic->insert((MK == 75) ? navigator.CurrentPos()->Ic->begin() : navigator.CurrentPos()->Ic->end(), (ip&)Load.Point);
			}
			else {
				for (int i = 0; i < navigator.CurrentPos()->Ic->size(); i++) {
					if (IPCmp(&navigator.CurrentPos()->Ic->at(i), &navigator.CurrentPos()->IpList->back())) {
		
						navigator.CurrentPos()->Ic->insert(navigator.CurrentPos()->Ic->begin() + ((MK == 75) ? i : i + 1), *(ip*)Load.Point);
						break;
					}
				}
			}
			break;
	////////////////////////// ��������� ////////////////////////////
		case 80: // GoToIp ������� ��: ������� � ��
		{
			vector<void*>* row = LTable->GetRow(Load.Point);
			if (row != nullptr && row->size() > 1) {
				for (auto i = ((IC_type)row->at(1))->begin(); i != ((IC_type)row->at(1))->end(); i++)
					if (IPCmp((ip*)i._Ptr, (ip*)Load.Point)) {
						if (navigator.CurrentPos() == nullptr) {
							navigator.CapsPush((IC_type)row->at(1), PNav);
						}
						navigator.IpPush((ip*)i._Ptr);
						break;
					}
			}
			break;
		}
		case 81: // GoToIc ������� ��: ������� � ��
		{
			vector<void*>* row = LTable->GetRow(Load.Point);
			if (row != nullptr && row->size() > 1) {
				navigator.CapsPush((IC_type)row->at(1), PNav);
				break;
			}
			break;
		}
		case 82: // GoToBackIp ������� ��: ������ �� ���� �������
			navigator.IpPop();
			break;
		case 83: // GoToBackIc ������� ��: ������ �� ���� �������
			navigator.CapsPop();
			break;
		case 85: // GotoAtr ������� �� ��, ����� ������� ���������� � �������� �� � ��������� �� �������� �� (����� ������������� � ��, �� ������� ��������� �������)
		{
		//	Searcher.Template = navigator.back();
		//	if(Searcher.FindAnd({Cint, Load.toInt(),nullptr}));
		}
			break;
		default:
			CommonMk(MK, Load, Sender);
	}
}

FU* GraphTransf::Copy() // ��������� ����������� ��
{
	return new GraphTransf(Bus, this);
}

FU* GraphTransf::TypeCopy() // ������� �� ������ �� ���� (�� ������� ��������
{
	return new GraphTransf(Bus, nullptr);
}

#include "stdafx.h"
#include "GraphTransf.h"
#include <algorithm>
#include <vector>

using namespace std;

void GraphTransf::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
		case 0:// Сброс
			if (LTable != nullptr)
			{
				LTable->Clear();
				LTable->HostPointSet(nullptr);
			}
			break;
	////////////////////////// Таблица соответствий ////////////////////////////
		case 1:// Установить ссылку на таблицу преобразования адресов
			LTable = (LocatTable*)Load.Point;
			//Load.print();
			break;
		case 2: // Clear Очистить таблицу
			LTable->Clear();
			break;
		case 5: // AddLine Добавить линию
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
	////////////////////////// аккумулятор для конкатенации ИК ////////////////////////////
		case 45: // AccumSet Установить указатель в аккумулятор
			if (Load.Type >> 1 == DIC) {
				navigator.CapsPush((IC_type)Load.Point, PBuf);
			}
			break;
		case 46: // AccumPop Удалить указатель из аккумулятора
			if (navigator.size() > 0)
				navigator.CapsPop();
			break;
	////////////////////////// Конкатенация ИК ////////////////////////////
		case 50: // Concat Конкатенировать ИК
		case 51: // ConcatCopy Конкатенировать копию ИК
			if (Load.Type >> 1 == DIC){
				for (auto i = ((IC_type)Load.Point)->begin()._Ptr; i != ((IC_type)Load.Point)->end()._Ptr; i++) {
					navigator.CurrentPos()->Ic->push_back(*i);
				}
			}
			if(MK==50) delete (IC_type)Load.Point;
			break;
		case 55: // ConcatOfAccum Конкатенировать ИК
		case 56: // ConcatCopyOfAccum Конкатенировать копию ИК
			if (Load.Type >> 1 == DIC)
				copy(Accum.back()->begin(), Accum.back()->end(), inserter(*Accum.back(), ((IC_type)Load.Point)->end()));
			if (MK == 55) { delete Accum.back(); Accum.pop_back();}
			for (auto i = Accum.begin()._Ptr; i < Accum.end()._Ptr; i++) {
				cout << i << endl;
			}
			break;
	////////////////////////// Запись адреса в буфер ////////////////////////////
		case 60: // PointSet запись адреса ИП в буфер
		case 61: // PointAdd добавление адреса ИП в буфер
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
	////////////////////////// Запись значений ////////////////////////////
		case 65: // VarSet запись значения в ячейку памяти из нагрузки ИП
//			bufIp->Load = Load;
			break;
		case 66: // LoadSet запись данных по указателю нагрузки
			for (int i = 0; i < navigator.CurrentPos()->Ic->size(); i++) {
				if (IPCmp(&navigator.CurrentPos()->IpList->at(0), &navigator.CurrentPos()->Ic->at(i))) {
					navigator.CurrentPos()->Ic->at(i).Load = Load;
					break;
				}
			}
			break;
		case 67: // AtrSet запись данных в атрибут
			for (int i = 0; i < navigator.CurrentPos()->Ic->size(); i++) {
				if (IPCmp(&navigator.CurrentPos()->IpList->at(0), &navigator.CurrentPos()->Ic->at(i))) {
					navigator.CurrentPos()->Ic->at(i).atr = *(int*)Load.Point;
					break;
				}
			}
			break;
		case 68: // IpSet запись данных в ИП
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
	////////////////////////// Удаление ////////////////////////////
		case 70: // DelIp  удаление Одиночной ИП (если в буфере для удаления присутствуют адреса ИП, эти ИП тоже удаляются)
		{
			vector<void*>* row = LTable->GetRow(Load.Point);
			if (row != nullptr && row->size() > 1) {
				for (auto i = ((IC_type)row->at(1))->begin(); i != ((IC_type)row->at(1))->end(); i++)
					if (IPCmp((ip*)i._Ptr, (ip*)Load.Point)) {
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
			break;
		}
		case 71: // DelIpAdd Добавить ИП для пакетного удаления
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
	////////////////////////// Вставка ////////////////////////////
		case 75: // InsertBefore Вставить ИП до бегунка
		case 76: // InsertAfter Вставить ИП после бегунка
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
	////////////////////////// Навигация ////////////////////////////
		case 80: // GoToIp Бегунок ИП: Перейти к ИП
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
		case 81: // GoToIc Бегунок ИК: Перейти к ИК
		{
			vector<void*>* row = LTable->GetRow(Load.Point);
			if (row != nullptr && row->size() > 1) {
				navigator.CapsPush((IC_type)row->at(1), PNav);
				break;
			}
			break;
		}
		case 82: // GoToBackIp Бегунок ИП: отойти на одну позицию
			navigator.IpPop();
			break;
		case 83: // GoToBackIc Бегунок ИК: отойти на одну позицию
			navigator.CapsPop();
			break;
	}
}

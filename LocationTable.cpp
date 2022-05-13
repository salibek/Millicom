// “аблица соотвестви€ адресов
#include "stdafx.h"
#include "LocationTable.h"

LocatTable::LocatTable() {  // конструктор класса, вызываетс€ при создании объекта таблицы
	ind = 0;
	Table = new vector<vector<void*>>();
}

vector<void*>* LocatTable::GetRow(void* Point)
{
	for (auto i = Table->begin(); i != Table->end(); i++) {
		for (auto j = i._Ptr->begin(); j != i._Ptr->end(); j++) {
			if (isIPinIC(Point, *j._Ptr))
				return i._Ptr;
		}
	}
	return nullptr;
}

void* LocatTable::EqLoacat(void* Point, int index) // ¬озвращает адрес, соответствующий запрошенномую, index - индекс указател€ в строке, при index<0 используетс€ поле класса ind
{
	int _index = (index < 0) ? ind : index;
	for (int _i = 0; _i < Table->size(); _i++) {
		if (Table->at(_i).at(_index) == Point)
			return Table->at(_i).at(1);
	}
	return nullptr;
}
int LocatTable::Index(void* Point) // ¬озвращаетс€ индекс найденного указател€ (если указатель не найден, возвращаетс€ -1)
{
	for (int _i = 0; _i < Table->size(); _i++) {
		for (int _j = 0; _j < Table->at(_i).size(); _j++) {
			if (Table->at(_i).at(_j) == Point) return _j;
		}
	}
	return -1;
}
void LocatTable::AddLine(void* Point) // ƒобавл€ет новую линию в таблицу соотвитстви€ адресов и записывает в нее адрес
{
	if (Table == nullptr) {
		Table = new vector<vector<void*>>();
	}
	vector<void*> _line;
	Table->push_back(_line);
	Table->back().push_back(Point);
	indVect.push_back(0);
}

void LocatTable::AddToLastLine(void* Point) // ƒобавл€ет адрес в последнюю линию таблицы соотвитстви€ адресов
{
	if (LineCount() > 0) {
		Table->back().push_back(Point);
	}
}

bool LocatTable::AddPoint(void* Point, void* HostPoint) // »щетс€ лини€ с адресом HostPoint и в нее добавл€етс€ адрес Point
{
	for (int _i = 0; _i < Table->size(); _i++) {
		for (int _j = 0; _j < Table->at(_i).size(); _j++)
		if (Table->at(_i).at(_j) == HostPoint) {
			Table->at(_i).push_back(Point);
			return true;
		}
	}
	return false;
}

void LocatTable::Clear() // —брос указател€ на таблицу таблицы
{
	ind = 0;
	indVect.clear();
	Table = nullptr;
}

void LocatTable::TableClear() // ќчищение таблицы
{
	ind = 0;
	indVect.clear();
	Table->clear();
}


bool LocatTable::Empty() // ѕроверка на пустоту таблицы
{
	return Table == nullptr || LineCount() == 0;
}
int  LocatTable::LineCount() // ¬озвращает количество линий в таблице
{
	return Table->size();
}
void LocatTable::DelLastLine(int Count) // ”дал€ет Count последних строк таблицы 
{
	for (int _i = LineCount(), _j = 0; _j < Count; _i--, _j++) {
		Table[_i-1].clear();
		indVect[_i-1] = -1;
	}
}

bool LocatTable::DelLineWithPoint(void* Point) // ”даление строки, в которой записан адрес Point, возвращает true, если удаление состо€лось
{
	for (int _i = 0; _i < LineCount(); _i++) {
		for (int _j = 0; _j < Table->at(_i).size(); _j++) {
			if (Table->at(_i).at(_j) == Point) {
				Table->at(_i).clear();
				indVect[_i] = -1;
				return true;
			}
		}
	}
	return false;
}

bool LocatTable::DelCopy(void* Point) //удаление копии (столбца строки, в котором записан адрес копии)
{
	for (int _i = 0; _i < Table->size(); _i++) {
		for (int _j = 0; _j < Table->at(_i).size(); _j++)
			if (Table->at(_i).at(_j) == Point) {
				delete Table->at(_i).at(_j);
				return true;
			}
	}
	return false;
}

// “аблица соотвестви€ адресов
#pragma once
#include "Consts.h"
class LocatTable
{
private:
	vector<vector<void*>>* Table; // “аблица соответстви€ адресов
	int ind = -1; // »ндекс адреса в таблице адресов (при -1 используетс€ вектора индексов)
	vector<int> indVect; // »ндекс адреса в таблице адресов
	void *HostPoint = nullptr; // Ѕуфер дл€ указател€
public:
	LocatTable(); // конструктор класса, вызываетс€ при создании объекта таблицы
	void HostPointSet(void* Point) { HostPoint=Point;}; // ”становить значение в буфер дл€ указател€
	void* EqLoacat(void* Point, int index=-1); // ¬озвращает адрес, соответствующий запрошенномую, index - индекс указател€ в строке, при index<0 используетс€ поле класса ind
	int Index(void* Point); // ¬озвращаетс€ индекс найденного указател€ (если указатель не найден, возвращаетс€ -1)
	void AddLine(void* Point); // ƒобавл€ет новую линию в таблицу соответстви€ адресов и записывает в нее адрес
	void AddToLastLine(void* Point); // ƒобавл€ет адрес в последнюю линию таблицы соотвитстви€ адресов
	bool AddPoint(void* Point, void* HostPoint); // »щетс€ лини€ с адресом HostPoint и в нее добавл€етс€ адрес Point
	void AddPointToHost(void* Point) { AddPoint(Point, HostPoint); }; // »щетс€ лини€ с адресом HostPoint (поле класса) и в нее добавл€етс€ адрес Point
	void Clear(); // —брос указател€ таблицы соотвестви€ адресов
	void TableClear(); // ќчистка таблицы соотвестви€ адресов
	vector<void*>* GetRow(void* Point);

	bool Empty(); // ѕроверка на пустоту таблицы
	int LineCount(); // ¬озвращает количество линий в таблице
	void DelLastLine(int Count=1); // ”дал€ет Count последних строк таблицы 
	bool DelLineWithPoint(void* Point); // ”даление строки, в которой записан адрес Point, возвращает true, если удаление состо€лось
	bool DelCopy(void* Point); //удаление копии (столбца строки, в котором записан адрес копии)
};
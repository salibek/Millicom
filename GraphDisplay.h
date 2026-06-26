#pragma once

#include "Consts.h"
#include "List.h"
#include <string>

class Fl_Tree;
class Fl_Tree_Item;
class Fl_Widget;

struct SearchState
{
	std::vector<Fl_Tree_Item*> results;
	int currentIndex = -1;
};

struct GraphStats
{
	size_t vertexCount = 0;
	size_t pairCount = 0;
	size_t arrayCount = 0;
	size_t maxDepth = 0;
};


class GraphDisplay : public FU
{
public:
	IC_type GraphUk = nullptr; // Указатель на ОА-граф
private:
	bool Extend = false; // Флаг разворачивания всех узлов дерева
	unordered_map<int, Fl_Tree_Item*> treeIcMap;
	SearchState searchState;
	GraphStats graphStats;
	LoadMnemoToStr mnemoToStr; // Преобразователь нагрузки в мнемонику и использованием таблицы лексем 

public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст
	GraphDisplay(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; FUtype = 1; };
	GraphDisplay() : FU() { GraphDisplay(nullptr, nullptr); };

	string GenerateDot();

	static ICVect TraverseGraph(IC_type graphUk, GraphStats& stats);

private:
	void DisplayGraphWindow();
	Fl_Tree* CreateFlTree();

};
// ФУ Список
#pragma once
#include "Consts.h"
#include "Search.h"

class List : public FU
{
public:
	void ProgFU(int MK, LoadPoint Load) override;
	vector<IC_type> ListHead; // Ссылка на голову списка (вектор, т.к. список может быть многоуровневым)

	ip* LineUk = nullptr; // Указатель на найленную строку списка
	int LineNum = -1, LineNumOld = -1; // Номер первой совпадающей линии, номер предыдущей совпадающей линии
	int LineCount = 0; // Количество найденных строк
	bool StopSearch = false; // Флаг прекращения поиска в текущей иерархии

	int MultiLineMode = 0; // 0 - поиск только первого совпадения, 1 - поиск всех совпадений
	int MultiLavelSearch = 0; // Режим поиска на нескольких уровнях списка
	// 0 - на текущем уровне, 1 - на всех уровнях, 2- только в текущей иерархии
	int ReceiverMk = 0; // Мк для выдаваемой лексемы
	int* ReceiverMkUk = &ReceiverMk; // Указатель на Мк для выдаваемой лексемы
//	int LastIpOffcet = 0; // Смещение относительно последней ИП капсулы для записи
	// Выделить в контекст
/*	vector<ip>* ListHead = nullptr; // Указатель на начало списка
	ip* LineUk=nullptr; // Указатель на найленную строку списка
	int LineNum = -1, LineNumOld = -1; // Номер первой совпадающей линии, номер предыдущей совпадающей линии
	int MultiLineMode = 0; // 0 - поиск только первого совпадения, 1 - поиск всех совпадений
	int ReceiverMk = 0; // Мк для выдаваемой лексемы
	int *ReceiverMkUk = &ReceiverMk; // Указатель на Мк для выдаваемой лексемы
	int LineCount = 0; // Количество найденных строк
*/	// -----
	bool MultyLineMode = false, MultyListMode = false, BackSearchMode = false; //  Режимы поиска нескольки линий, иерархического списка и поиска в обратном направлении
	bool BackListSerch = false; // Поиск вверх по списку (по иерархии)
	int LineAtr = SubObj; // Атрибут линии списка
	set<int> SubListAtrs = { LineAtr };
	void* LessProg = nullptr, * BibberProg = nullptr, *EQProg = nullptr, *LessEQProg = nullptr, *BibberEQProg = nullptr;
	vector<int> LineStack; // Стек номеров линий
	Search Searcher; // Устройство поиска
	void *FailLineProg = nullptr; // Программа, выполняемая в случае неудачного поиска в текущей линии списка
	void *FailProg = nullptr; // Программа, выполняемаЯ в случае неудачного поиска во всем  списке
	void* SuссessLineProg = nullptr, * SuссessProg = nullptr;
	vector<ip> *DefProg = nullptr;
	int Mode = 0; // Режима работы списка 0 - список на основе ИК, 1 - список на основе хеш-таблицы

	List(FU* BusContext, FU* Templ) : FU(BusContext) { ProgFU(0, { 0, nullptr }); Bus = BusContext; Searcher.MainFU = this; FUtype = 5; };
	List() : FU() { List(nullptr, nullptr); };
	int HashAtr = 0; // Атрибут для хеширования
	unordered_map<string, IC_type> Hash; // Хеш-таблица
	ip HashLineUk; // Псевдосписок для хеш-режима
	vector<ip> HashListBack = {}; // Превдоконец списка для режима хеширования
private:
	int DeepStartSearch = 0; // Стартовый уровень для пописка
};
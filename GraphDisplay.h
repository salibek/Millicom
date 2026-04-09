#pragma once
#include "Consts.h"
#include <string>

class GraphDisplay : public FU
{
private:
	void* GraphUk = nullptr; // Указатель не ОА-граф
	bool Extend = false; // Флаг разборачивания всех узлов дерева
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст
	GraphDisplay(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; FUtype = 1; };
	GraphDisplay() : FU() { GraphDisplay(nullptr, nullptr); };
};
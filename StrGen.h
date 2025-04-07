#pragma once
#include "Consts.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
class StrGen : public FU {
private:
	long int LineCount = 0;
	void *Receiver = nullptr;
	long int ReceiverMK = 0;
	bool work = true;
	string Filename;
	long int TimeStart = 0, TimeLong = 0;// Время начала компиляции, время конца компиляции
	long int str_count = 0; // счетчик считанных строк
	vector<string> str_buf; // Буфер строк
	long int str_buf_size=3;// Размер буфера строк
	long int str_bufCount = 0; // Индекс текущей строки в буфере
	void *StartProg = nullptr, *FinProg = nullptr, *StopProg = nullptr; // Ссылки на программы, аапускаемые при начале, оокончании и прерывании генерации строк
	string finStr = ""; // Строка, выдаваемая при окончании геренации строк
	void* FileNotExistErrProg = nullptr;
	string IncludeStr = "#include"; // Строка с ключевым словом для включения модуля в программу
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст
	StrGen(FU* BusContext, FU* Templ) : FU(BusContext) { ProgFU(0, { 0, nullptr }); Bus = BusContext; FUtype = 2; };
	StrGen(): FU() { StrGen(nullptr, nullptr); };
};

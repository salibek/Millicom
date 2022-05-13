#pragma once
#include "Consts.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
class StrGenContext : public FU {
private:
	int LineCount = 0;
	void *Receiver = nullptr;
	int ReceiverMK = 0;
	bool work = true;
	ifstream Source;
	string Filename;
	int TimeStart = 0, TimeLong = 0;// Время начала компиляции, время конца компиляции
	int str_count = 0; // счетчик считанных строк
	vector<string> str_buf; // Буфер строк
	int str_buf_size;// Размер буфера строк
	int str_bufCount = 0; // Индекс текущей строки в буфере
	void *StartProg = nullptr, *FinProg = nullptr, *StopProg = nullptr; // Ссылки на программы, аапускаемые при начале, оокончании и прерывании генерации строк
	string finStr = ""; // Строка, выдаваемая при окончании геренации строк
public:
	void ProgFU(int MK, LoadPoint Load) override;
	StrGenContext(FU *BusContext, FU *Templ) : FU(BusContext) { ProgFU(0, { 0, nullptr }); Bus = BusContext;};
	StrGenContext(): FU() { StrGenContext(nullptr, nullptr); };
};

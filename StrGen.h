#pragma once
#include "Consts.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
class StrGen : public FU {
private:
	int LineCount = 0;
	void *Receiver = nullptr;
	int ReceiverMK = 0;
	bool work = true;
	ifstream Source;
	string Filename;
	int TimeStart = 0, TimeLong = 0;// ����� ������ ����������, ����� ����� ����������
	int str_count = 0; // ������� ��������� �����
	vector<string> str_buf; // ����� �����
	int str_buf_size;// ������ ������ �����
	int str_bufCount = 0; // ������ ������� ������ � ������
	void *StartProg = nullptr, *FinProg = nullptr, *StopProg = nullptr; // ������ �� ���������, ����������� ��� ������, ���������� � ���������� ��������� �����
	string finStr = ""; // ������, ���������� ��� ��������� ��������� �����
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	StrGen(FU* BusContext, FU* Templ) : FU(BusContext) { ProgFU(0, { 0, nullptr }); Bus = BusContext; FUtype = 2; };
	StrGen(): FU() { StrGen(nullptr, nullptr); };
};

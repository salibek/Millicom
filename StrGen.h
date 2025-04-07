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
	long int TimeStart = 0, TimeLong = 0;// ����� ������ ����������, ����� ����� ����������
	long int str_count = 0; // ������� ��������� �����
	vector<string> str_buf; // ����� �����
	long int str_buf_size=3;// ������ ������ �����
	long int str_bufCount = 0; // ������ ������� ������ � ������
	void *StartProg = nullptr, *FinProg = nullptr, *StopProg = nullptr; // ������ �� ���������, ����������� ��� ������, ���������� � ���������� ��������� �����
	string finStr = ""; // ������, ���������� ��� ��������� ��������� �����
	void* FileNotExistErrProg = nullptr;
	string IncludeStr = "#include"; // ������ � �������� ������ ��� ��������� ������ � ���������
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������
	StrGen(FU* BusContext, FU* Templ) : FU(BusContext) { ProgFU(0, { 0, nullptr }); Bus = BusContext; FUtype = 2; };
	StrGen(): FU() { StrGen(nullptr, nullptr); };
};

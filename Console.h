// �� �������
#pragma once
#include "Consts.h"
#include <fstream>
#include <string>
#include <map>

class MkRef { // ������� �� � ������ ��� ������ ����������
public:
	long int Mk = -1;
	LoadPoint Ref = {0,nullptr};
};

class Console : public FU
{
private:
	map<long int, string> AtrMnemo; // �������� ��������
	string prefix="";
	string filename = "";
	ofstream fout; // ??
	ofstream fin; // ??
	FILE* streamOut=nullptr, *streamIn=nullptr; // �������/�������� �����
	string Sep = " ", VectSep=",", End = "\n"; // ������-����������� � ������ � ����� ������
	string quote = ""; // ������ ������� ��� ������ ������
	string FloatPoint = "."; // ����������� ������� �����
	string ArrayBracketStart = "[", ArrayBracketFin = "]"; // ��������� � �������� ������ ��� ������ �������
	vector<string> TrueVar = { "true","True" }; // ������ �������� "������"
	vector<string> FalseVar = { "false","False" }; // ������ �������� "����"
//	vector<LoadPoint> *NewVect; // ��������� �� ������
	void* NoVarToOutProg = nullptr; // ����������� ������� �� ������ "��� ���������� ��� ������"
	void* WrongFormatProg = nullptr; // ����������� ������� �� ������ "������������ ������"
	void* WrongVectFormatProg = nullptr; // ����������� ������� �� ������ "������������ ������"
	LoadPoint Var = {0,nullptr};
	void* WrongInterFormat = nullptr; // ������������ ��������� ������ '������������ ������ ��������� ������'
	vector<MkRef> VarOutBuf; // ������� �� � ���������� ��� ������ ����������
	set<string> True = { "true", "True" }, False = {"false", "False"}; // ��������� �������� true false
	string inStr; // ��������� ��������� ������
	void* InputFormatErrProg = nullptr; // ��������� ������ ������� ������� ������
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������
	Console(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; FUtype = 1; };
	Console() : FU() { Console(nullptr, nullptr); };
private:
};

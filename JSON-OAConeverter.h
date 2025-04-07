// ��������� �� JSON � OA � �������

#pragma once
#include "Consts.h"
#include "Search.h"
#include <string>
#include <fstream>

using namespace std;
class JSON_OAConeverter : public FU {
private:
	IC_type MnemoCaps = nullptr;  // ��������� �� ������� � �����������
	IC_type MnemoTable = nullptr; // ��������� �� ������� ����������
	LoadPoint Graph ={ 0, nullptr}; // ��������� �� ��-����
//	ofstream FOut; // ���� ��� ������ ����������� ���������
//	ifstream FIn; // ���� ��� ����� json
	string FOutName, FInName; // ����� ��������� � �������� ������
	IC_type FOpenErrProg = nullptr; // ��������� ��� ������ �������� �����
	void Recurs(LoadPoint Load, ofstream& F, string Tab=""); // ����������� ����� �������� � JSON 
	Search Searcher; // ���������� ������
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr);
	void FOutNameSet(string Fname) { FOutName = Fname; };
	void FInNameSet(string Fname) { FInName = Fname; };
	void ToJson(LoadPoint Graph, string FileName); // �������������� �� �� � JSON
	LoadPoint FromJSON(string FileName); // �������������� �� JSPN � ��
	void MnemoCapsSet(LoadPoint LP) 
	{ MnemoCaps = (IC_type) LP.Point; };
	void MnemoTableSet(LoadPoint LP) { MnemoTable = (IC_type)LP.Point; };
};
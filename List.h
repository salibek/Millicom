// �� ������
#pragma once
#include "Consts.h"
#include "Search.h"

class List : public FU
{
public:
	void ProgFU(int MK, LoadPoint Load) override;
	vector<IC_type> ListHead; // ������ �� ������ ������ (������, �.�. ������ ����� ���� ��������������)

	ip* LineUk = nullptr; // ��������� �� ��������� ������ ������
	int LineNum = -1, LineNumOld = -1; // ����� ������ ����������� �����, ����� ���������� ����������� �����
	int LineCount = 0; // ���������� ��������� �����
	bool StopSearch = false; // ���� ����������� ������ � ������� ��������

	int MultiLineMode = 0; // 0 - ����� ������ ������� ����������, 1 - ����� ���� ����������
	int MultiLavelSearch = 0; // ����� ������ �� ���������� ������� ������
	// 0 - �� ������� ������, 1 - �� ���� �������, 2- ������ � ������� ��������
	int ReceiverMk = 0; // �� ��� ���������� �������
	int* ReceiverMkUk = &ReceiverMk; // ��������� �� �� ��� ���������� �������
//	int LastIpOffcet = 0; // �������� ������������ ��������� �� ������� ��� ������
	// �������� � ��������
/*	vector<ip>* ListHead = nullptr; // ��������� �� ������ ������
	ip* LineUk=nullptr; // ��������� �� ��������� ������ ������
	int LineNum = -1, LineNumOld = -1; // ����� ������ ����������� �����, ����� ���������� ����������� �����
	int MultiLineMode = 0; // 0 - ����� ������ ������� ����������, 1 - ����� ���� ����������
	int ReceiverMk = 0; // �� ��� ���������� �������
	int *ReceiverMkUk = &ReceiverMk; // ��������� �� �� ��� ���������� �������
	int LineCount = 0; // ���������� ��������� �����
*/	// -----
	bool MultyLineMode = false, MultyListMode = false, BackSearchMode = false; //  ������ ������ ��������� �����, �������������� ������ � ������ � �������� �����������
	bool BackListSerch = false; // ����� ����� �� ������ (�� ��������)
	int LineAtr = SubObj; // ������� ����� ������
	set<int> SubListAtrs = { LineAtr };
	void* LessProg = nullptr, * BibberProg = nullptr, *EQProg = nullptr, *LessEQProg = nullptr, *BibberEQProg = nullptr;
	vector<int> LineStack; // ���� ������� �����
	Search Searcher; // ���������� ������
	void *FailLineProg = nullptr; // ���������, ����������� � ������ ���������� ������ � ������� ����� ������
	void *FailProg = nullptr; // ���������, ����������� � ������ ���������� ������ �� ����  ������
	void* Su��essLineProg = nullptr, * Su��essProg = nullptr;
	vector<ip> *DefProg = nullptr;
	int Mode = 0; // ������ ������ ������ 0 - ������ �� ������ ��, 1 - ������ �� ������ ���-�������

	List(FU* BusContext, FU* Templ) : FU(BusContext) { ProgFU(0, { 0, nullptr }); Bus = BusContext; Searcher.MainFU = this; FUtype = 5; };
	List() : FU() { List(nullptr, nullptr); };
	int HashAtr = 0; // ������� ��� �����������
	unordered_map<string, IC_type> Hash; // ���-�������
	ip HashLineUk; // ������������ ��� ���-������
	vector<ip> HashListBack = {}; // ����������� ������ ��� ������ �����������
private:
	int DeepStartSearch = 0; // ��������� ������� ��� �������
};
// �� ������
#pragma once
#include "Consts.h"
#include "Search.h"
#include "JSON-OAConeverter.h"

class List : public FU
{
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender=nullptr) override;
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������
	vector<IC_type> ListHead; // ������ �� ������ ������ (������, �.�. ������ ����� ���� ��������������)

	ip* LineUk = nullptr; // ��������� �� ��������� ������ ������
	long int LineNum = -1, LineNumOld = -1; // ����� ������ ����������� �����, ����� ���������� ����������� �����
	long int LineCount = 0; // ���������� ��������� �����
	bool StopSearch = false; // ���� ����������� ������ � ������� ��������

	long int MultiLineMode = 0; // 0 - ����� ������ ������� ����������, 1 - ����� ���� ����������
	long int MultiLavelSearch = 0; // ����� ������ �� ���������� ������� ������
	// 0 - �� ������� ������, 1 - �� ���� �������, 2- ������ � ������� ��������
	long int ReceiverMk = 0; // �� ��� ���������� �������
	long int* ReceiverMkUk = &ReceiverMk; // ��������� �� �� ��� ���������� �������
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
	long int LineAtr = SubObj; // ������� ����� ������
	set<int> SubListAtrs = { LineAtr };
	void* LessProg = nullptr, * BibberProg = nullptr, *EQProg = nullptr, *LessEQProg = nullptr, *BibberEQProg = nullptr;
	vector<int> LineStack; // ���� ������� �����
	Search Searcher; // ���������� ������
	void *FailLineProg = nullptr; // ���������, ����������� � ������ ���������� ������ � ������� ����� ������
	void *FailProg = nullptr; // ���������, ����������� � ������ ���������� ������ �� ����  ������
	void* Su��essLineProg = nullptr, * Su��essProg = nullptr;
	vector<ip> *DefProg = nullptr;
	long int Mode = 0; // ������ ������ ������ 0 - ������ �� ������ ��, 1 - ������ �� ������ ���-�������

	List(FU* BusContext, FU* Templ) : FU(BusContext) { ProgFU(0, { 0, nullptr }); Bus = BusContext; Searcher.MainFU = this; FUtype = 5; };
	List(){ List(nullptr, nullptr); };
	long int HashAtr = 0; // ������� ��� �����������
	unordered_map<string, IC_type> Hash; // ���-�������
	ip HashLineUk; // ������������ ��� ���-������
	vector<ip> HashListBack = {}; // ����������� ������ ��� ������ �����������
	LoadPoint CopyAdrCorrectOriginal = {}, CopyAdrCorrectCopy = {}, AdrToCorrect = {}; // �� � �� ����� ��� ��������� �������
private:
	long int DeepStartSearch = 0; // ��������� ������� ��� �������
	JSON_OAConeverter JsonConv; // ��������� � JSON
};
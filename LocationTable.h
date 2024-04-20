// ������� ����������� �������
#pragma once
#include "Consts.h"
class LocatTable
{
private:
	vector<vector<void*>>* Table; // ������� ������������ �������
	long int ind = -1; // ������ ������ � ������� ������� (��� -1 ������������ ������� ��������)
	vector<int> indVect; // ������ ������ � ������� �������
	void *HostPoint = nullptr; // ����� ��� ���������
public:
	LocatTable(); // ����������� ������, ���������� ��� �������� ������� �������
	void HostPointSet(void* Point) { HostPoint=Point;}; // ���������� �������� � ����� ��� ���������
	void* EqLoacat(void* Point, int index=-1); // ���������� �����, ��������������� �������������, index - ������ ��������� � ������, ��� index<0 ������������ ���� ������ ind
	long int Index(void* Point); // ������������ ������ ���������� ��������� (���� ��������� �� ������, ������������ -1)
	void AddLine(void* Point); // ��������� ����� ����� � ������� ������������ ������� � ���������� � ��� �����
	void AddToLastLine(void* Point); // ��������� ����� � ��������� ����� ������� ������������ �������
	bool AddPoint(void* Point, void* HostPoint); // ������ ����� � ������� HostPoint � � ��� ����������� ����� Point
	void AddPointToHost(void* Point) { AddPoint(Point, HostPoint); }; // ������ ����� � ������� HostPoint (���� ������) � � ��� ����������� ����� Point
	void Clear(); // ����� ��������� ������� ����������� �������
	void TableClear(); // ������� ������� ����������� �������
	vector<void*>* GetRow(void* Point);
	vector<void*>* GetLine(void* Point); // ������ ������ �� ����� ������, ��� ������������ ����������
	bool Empty(); // �������� �� ������� �������
	long int LineCount(); // ���������� ���������� ����� � �������
	void DelLastLine(long int Count=1); // ������� Count ��������� ����� ������� 
	bool DelLineWithPoint(void* Point); // �������� ������, � ������� ������� ����� Point, ���������� true, ���� �������� ����������
	bool DelCopy(void* Point); //�������� ����� (������� ������, � ������� ������� ����� �����)
};
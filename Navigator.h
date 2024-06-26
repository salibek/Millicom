#pragma once
#include "consts.h"

const int PNav = 0, PDel = 1, PBuf = 2;

struct Position
{
	long int type;
	vector<ip>* Ic; // ������� �������������� �������
	vector<ip>* IpList; // ������� �������������� ���� � ������� �������������� �������
};

class Navigator
{
public:
	vector<Position>* currentPos; //������� ������� �������
	void CapsPush(vector<ip>* uk, int _type); // ������� �� ����� ������� �������� ��
	void CapsPop();  // ������ �� ���� ������� �������� ��
	void IpPush(ip* uk);   // ������� �� ����� ������� �������� �� � ������� ������� ������� ��
	void IpPop();	 // ������ �� ���� ������� �������� �� � ������� ������� ������� ��
	void CapsSet(void* uk);  // ������ ����� ������� ������� ��
	void IpSet(void* uk);    // ������ ����� ������� ������� �� � ������� ������� ������� ��
	void IpLoadGoTo(); // ������� (������ �������� ��������� ������� ��) �� ��, ��������� �� ������� ��������� � �������� ��, �� ������� ��������� ������� ��
	void IpLoadPushGoTo(); //������� �� ��, ��������� �� ������� ��������� � �������� ��, �� ������� ��������� ������� �� (� ����������� �������� ��������� ������� ��)
	long int size(); //������� ������ �������
	Position* CurrentPos();
private:
};
#pragma once
#include "Consts.h"
#include "LocationTable.h"
#include "Search.h"
#include "Navigator.h"

class GraphTransf : public FU
{
private:
	LocatTable *LTable=nullptr; // ������� �������������� �������
	deletedIC delIC = { nullptr, nullptr }; //����� ��������� ��
	Search Searcher;  //���������
	Navigator navigator = {nullptr}; //�������
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������
	vector<IC_type> Operands; // ����������� ��� �������� (������ �������)
public:
	GraphTransf(FU *BusContext, FU *Templ) : FU(BusContext) {
		FUtype = 6;
		Bus = BusContext; 
		LTable = new LocatTable(); 
		navigator.currentPos = new vector<Position>(); 
		ProgFU(0, { 0,nullptr }); 
	};
	GraphTransf(){GraphTransf(nullptr, nullptr);};
};
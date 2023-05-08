#pragma once
#include <vector>
#include "Consts.h"
#include "ALU.h"

// ���������� ��������� ������� (���������� ��� ���������� �������� �������)
class CellularAutomat : public FU {
//private:
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	CellularAutomat(FU* BusContext, FU* Templ) : FU(BusContext) { FUtype = 18; Bus = BusContext; ProgFU(0, { 0,nullptr }); };
	CellularAutomat() : FU() { Bus = nullptr; ProgFU(0, { 0,nullptr });};
	void* Manager = nullptr;// ������ �� ���������
	vector<FU*> Neighbours;//������ �� �������
	vector<int> NeighboursMk;// �� ��� �������
	vector<double> Rez; //������ ����������� ����������
	vector<vector<LoadPoint>> Plys = { { {0,nullptr} } }; // ������ ������� ������, ���������� �� �������
	vector<LoadPoint> parameters; // ������ ����������
	vector<void*>ReceiveProgs; // ��������� ��� ������� ��������� ������ �� ��������� ������� ������
//	vector<bool> parametersF; // ������ ������ ����������� ������� ����������
	vector<double> Vars; // ������ ��������� ���������� (������������ ��� �������� ������������� ������ ����������)
	bool AutoSend = false; // ���� �������������� ��������� ����������� ����������
	vector<bool> RezReady; // ���� ���������� ���������� (��� ������� ���� ��������)
	vector<bool> InComplectF; // ���� ����������� ���� ������� �����
	vector<int> InCounter = {0}; // ������� ���������� ����������� ������ (����� ���������� ���������� ������������)
	int PlyInd = 0; //������ ����
	int PlyCurrent = 0; //������ �������� ����
	int ParameterInd = 0;// ������ �������� ���������
	int PlyN = 1; // ������������ ���������� ����� ��� �������
	int Ind = 0; // ������ (���������� ��������� �������� Neighbours, NeighboursMk, parameters � �.�.
	void* FiringProg = nullptr; // ��������� �� ���������, ��� ���������� ���������� ��� ������� ���� �������� ������
	void* ReceiveProg = nullptr; // ��������� �� ���������, ����������� ��� ������� ������ �� ������ ������
	int Mode = 1; // ����� ������ �������� (0 - ���������, 1 - ������� �����)
	void* Collector = nullptr; // �	������ �� �������� �� ��� ������ ����������
	int IndFuOffcet = 0; // �	�������� ������� �� (������ ����������� �� ��� �������� � � ���� ������������ ������ ����)
//	int FUInd = 0; // ������ ��-��������
	int VarInd = -1; // ������ ���������� ��� ��������������� ��������� ����������
	int N_In = 0; // ���������� ������� ������ ��� ������������
	int State = 0; // ��������� ��������
	int MkToSet = -1; // ������������ �� ��������� ��� ��������� ???
	double ReceiveTime=0, CalcTime=0, SendTime=0, OtherMkTime=0; // ����� �������� ������, ����������, ������ ������
	double TransferTime = 0; // ����� �������� ������ �� ������ �������� �������
};

// �������� ����������� ���������� �������� (���������� ��� ���������� �������� �������)
class CellularAutomatManager : public FU {
	//private:
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	int NetType = 0;// ��� ���������� ����� 0 - �� ������, 1- ����������, 2 - �����������, 3 ��������������
	CellularAutomatManager(FU* BusContext, FU* Templ) : FU(BusContext) { FUtype = 19; Bus = BusContext; ProgFU(0, { 0,nullptr });};
	CellularAutomatManager() : FU() { Bus = nullptr; };
	vector<CellularAutomat> Net; //������ �������� ���������
	int Dim=0; // ����������� ���� ��������� (���������� �� ��� ������� ���������)
	void* iniAutmataProg = nullptr; // ��������� ������������� ���������
	void* Collector = nullptr; // ������ �� ���������
//	void* ActivationProg = nullptr; // ��������� ��������� �� ��� ������� ���� ����������� ������
//	void* ReceiveProg = nullptr; // ��������� ��������� ��� ��������� ��������� ������ �� �������
//	void* FiringProg = nullptr; // ��������� ���������� ����������
	int Ind1 = -1, Ind2 = -1; // ������� ������� � ������� ��-��������
	int Step1 = 0, Step2 = 0; // ��� �������������� �������������
//	int InCounter = 0; // ���������� ���������� �������� ������
	int PlyN = 1; // ������������ ���������� ����� ��� �������
	int ContextAddMk=20; // ��� �� ���������� �� ��� ��������������� ��
	int MkAddMk=21; // ��� �� ��� ���������� �� ������ ��������������� ��
	int AutomataMk1 = -1, AutomataMk2 = -1; // ������� �� ��� �������� �� ��������� 1, 2
};
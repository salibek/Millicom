#pragma once
#include <vector>
#include "Consts.h"
#include "ALU.h"
#include "Router.h"
#include "Gateway.h"

// ���������� ��������� ������� (���������� ��� ���������� �������� �������)
class CellularAutomata : public FU {
//private:
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������
	CellularAutomata(FU* BusContext, FU* Templ) : FU(BusContext) { FUtype = 18; Bus = BusContext; ProgFU(0, { 0,nullptr }); };
	CellularAutomata() : FU() { CellularAutomata(nullptr, nullptr); };
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
	ipSender Received; // ���������� ������
	int PriorityMk = -1; // ������������ �� ��� �������� ������� (��� -1 ���������, ��� ������������ �� ���
	bool OutBlock = false; // ���� ���������� ������ ������
};

// �������� ����������� ���������� �������� (���������� ��� ���������� �������� �������)
class CellularAutomatManager : public FU {
	//private:
public:
	void DistrebuteModelGenerate(); // ��������� ������ �������������� ��
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	int NetType = 0;// ��� ���������� ����� 0 - �� ������, 1- ����������, 2 - �����������, 3 ��������������
	CellularAutomatManager(FU* BusContext, FU* Templ) : FU(BusContext) { FUtype = 19; Bus = BusContext; ProgFU(0, { 0,nullptr });};
	CellularAutomatManager() : FU() { Bus = nullptr; };
	vector<CellularAutomata> Net; //������ �������� ���������
	int Dim = 0; // ����������� ���� ��������� (���������� �� ��� ������� ���������)
	bool NilRefMode = false; // ����� ���������� ������� ������, ���� ������ ��������������� �� �������������� ������� �������������� �����
	void* Collector = nullptr; // ������ �� ���������

	vector <Router> Routers; // ������ �������� ��� ������������� �������������� �������������� �����
	vector <Gateway> Gateways; // ������ ������ ��� ������������� �������������� �������������� �����
	vector <int> SectorDim; // ������� �������� ��� ������������� �������������� ��
	vector <Scheduler> Schedullers; // ������ ������������� ����������
	Eventser* NetEventser = nullptr; // ���������� �������

	void* iniAutmataProg = nullptr; // ��������� ������������� ���������

	int Ind1 = -1, Ind2 = -1; // ������� ������� � ������� ��-��������
	int Step1 = 0, Step2 = 0; // ��� �������������� �������������
	int PlyN = 1; // ������������ ���������� ����� ��� �������
	int ContextAddMk=20; // ��� �� ���������� �� ��� ��������������� ��
	int MkAddMk=21; // ��� �� ��� ���������� �� ������ ��������������� ��
	int AutomataMk1 = -1, AutomataMk2 = -1; // ������� �� ��� �������� �� ��������� 1, 2
	int GatewayMk = -1, RouterMk = -1; // �� ��� ����� � ��������������
	vector<int> NetDim; // ������ � ������������� ������������� �����
};
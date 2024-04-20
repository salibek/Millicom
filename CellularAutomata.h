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
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
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
	long int PlyInd = 0; //������ ����
	long int PlyCurrent = 0; //������ �������� ����
	long int ParameterInd = 0;// ������ �������� ���������
	long int PlyN = 1; // ������������ ���������� ����� ��� �������
	long int Ind = 0; // ������ (���������� ��������� �������� Neighbours, NeighboursMk, parameters � �.�.
	void* FiringProg = nullptr; // ��������� �� ���������, ��� ���������� ���������� ��� ������� ���� �������� ������
	void* ReceiveProg = nullptr; // ��������� �� ���������, ����������� ��� ������� ������ �� ������ ������
	long int Mode = 1; // ����� ������ �������� (0 - ���������, 1 - ������� �����)
	void* Collector = nullptr; // �	������ �� �������� �� ��� ������ ����������
	long int IndFuOffcet = 0; // �	�������� ������� �� (������ ����������� �� ��� �������� � � ���� ������������ ������ ����)
//	int FUInd = 0; // ������ ��-��������
	long int VarInd = -1; // ������ ���������� ��� ��������������� ��������� ����������
	long int N_In = 0; // ���������� ������� ������ ��� ������������
	long int State = 0; // ��������� ��������
	long int MkToSet = -1; // ������������ �� ��������� ��� ��������� ???
	double ReceiveTime=0, CalcTime=0, SendTime=0, OtherMkTime=0; // ����� �������� ������, ����������, ������ ������
	double TransferTime = 0; // ����� �������� ������ �� ������ �������� �������
	ipSender Received; // ���������� ������
	long int PriorityMk = -1; // ������������ �� ��� �������� ������� (��� -1 ���������, ��� ������������ �� ���
	bool OutBlock = false; // ���� ���������� ������ ������
};

// �������� ����������� ���������� �������� (���������� ��� ���������� �������� �������)
class CellularAutomatManager : public FU {
	//private:
public:
	void DistrebuteModelGenerate(); // ��������� ������ �������������� ��
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	long int NetType = 0;// ��� ���������� ����� 0 - �� ������, 1- ����������, 2 - �����������, 3 ��������������
	CellularAutomatManager(FU* BusContext, FU* Templ) : FU(BusContext) { FUtype = 19; Bus = BusContext; ProgFU(0, { 0,nullptr });};
	CellularAutomatManager() : FU() { Bus = nullptr; };
	vector<CellularAutomata> Net; //������ �������� ���������
	long int Dim = 0; // ����������� ���� ��������� (���������� �� ��� ������� ���������)
	bool NilRefMode = false; // ����� ���������� ������� ������, ���� ������ ��������������� �� �������������� ������� �������������� �����
	void* Collector = nullptr; // ������ �� ���������

	vector <Router> Routers; // ������ �������� ��� ������������� �������������� �������������� �����
	vector <Gateway> Gateways; // ������ ������ ��� ������������� �������������� �������������� �����
	vector <long int> SectorDim; // ������� �������� ��� ������������� �������������� ��
	vector <Scheduler> Schedullers; // ������ ������������� ����������
	Eventser* NetEventser = nullptr; // ���������� �������

	void* iniAutmataProg = nullptr; // ��������� ������������� ���������

	long int Ind1 = -1, Ind2 = -1; // ������� ������� � ������� ��-��������
	long int Step1 = 0, Step2 = 0; // ��� �������������� �������������
	long int PlyN = 1; // ������������ ���������� ����� ��� �������
	long int ContextAddMk=20; // ��� �� ���������� �� ��� ��������������� ��
	long int MkAddMk=21; // ��� �� ��� ���������� �� ������ ��������������� ��
	long int AutomataMk1 = -1, AutomataMk2 = -1; // ������� �� ��� �������� �� ��������� 1, 2
	long int GatewayMk = -1, RouterMk = -1; // �� ��� ����� � ��������������
	vector<long int> NetDim; // ������ � ������������� ������������� �����
};
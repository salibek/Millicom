#pragma once
#include <vector>
#include "Consts.h"
#include "ALU.h"

// ���������� ��������� ������� (���������� ��� ���������� �������� �������)
class CellularAutomat : public FU {
//private:
public:
	void ProgFU(int MK, LoadPoint Load) override;
	CellularAutomat(FU* BusContext, FU* Templ) : FU(BusContext) { FUtype = 18; Bus = BusContext; };
	CellularAutomat() : FU() { Bus = nullptr; };
	void* Manager = nullptr;// ������ �� ���������
	vector<FU*> Neighbours;//������ �� �������
	vector<int> NeighboursMk;// �� ��� �������
	vector<double> Rez; //������ ����������� ����������
	vector<vector<LoadPoint>> Plys = { { {0,nullptr} } }; // ������ ������� ������, ���������� �� �������
	vector<LoadPoint> parameters; // ������ ����������
//	vector<bool> parametersF; // ������ ������ ����������� ������� ����������
	vector<double> Vars; // ������ ��������� ���������� (������������ ��� �������� ������������� ������ ����������)
	bool AutoSend = false; // ���� �������������� ��������� ����������� ����������
	vector<bool> RezReady; // ���� ���������� ���������� (��� ������� ���� ��������)
	vector<vector<bool>> InComplectF; // ���� ����������� ���� ������� �����
	vector<int> InCounter = {0}; // ������� ���������� ����������� ������ (����� ���������� ���������� ������������)
	int PlyInd = -1; //������ ����
	int PlyCurrent = 0; //������ �������� ����
	int ParameterInd = -1;// ������ �������� ���������
	int PlyN = 1; // ������������ ���������� ����� ��� �������
	int Ind = 0; // ������
	void* FiringProg = nullptr; // ��������� �� ���������, ��� ���������� ���������� ��� ������� ���� �������� ������
	void* ReceiveProg = nullptr; // ��������� �� ���������, ����������� ��� ������� ��, ���� ��������� ����������, �� �� ��������� �������� �� 
	int Mode = 1; // ����� ������ �������� (0 - ���������, 1 - ������� �����)
	void* Collector = nullptr; // �	������ �� �������� �� ��� ������ ����������
	int IndFuOffcet = 0; // �	�������� ������� �� (������ ����������� �� ��� �������� � � ���� ������������ ������ ����)
//	int FUInd = 0; // ������ ��-��������
	int VarInd = -1; // ������ ���������� ��� ��������������� ��������� ����������
	void* ActivationProg = nullptr; // ��������� ��������� �� ��� ������� ���� ����������� ������
	int N_In = 0; // ���������� ������� ������ ��� ������������

};

// �������� ����������� ���������� �������� (���������� ��� ���������� �������� �������)
class CellularAutomatManager : public FU {
	//private:
public:
	void ProgFU(int MK, LoadPoint Load) override;
	int NetType = 0;// ��� ���������� ����� 0 - �� ������, 1- ����������, 2 - �����������, 3 ��������������
	CellularAutomatManager(FU* BusContext, FU* Templ) : FU(BusContext) { FUtype = 19; Bus = BusContext; };
	CellularAutomatManager() : FU() { Bus = nullptr; };
	vector<CellularAutomat> Net; //������ �������� ���������
	int Dim; // ����������� ���� ��������� (���������� �� ��� ������� ���������)
	void* iniAutmataProg = nullptr; // ��������� ������������� ���������
	void* Collector = nullptr; // ������ �� ���������
	void* ActivationProg = nullptr; // ��������� ��������� �� ��� ������� ���� ����������� ������
	void* ReceiveProg = nullptr; // ��������� ��������� ��� ��������� ��������� ������ �� �������
	void* FiringProg = nullptr; // ��������� ���������� ����������
	int Ind1 = -1, Ind2 = -1; // ������� ������� � ������� ��-��������
	int InCounter = 0; // ���������� ���������� �������� ������
	int PlyN = 1; // ������������ ���������� ����� ��� �������
	int ContextAddMk=20; // ��� �� ���������� �� ��� ��������������� ��
	int MkAddMk=21; // ��� �� ��� ���������� �� ������ ��������������� ��
};
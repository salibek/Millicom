#pragma once
#include "Consts.h"
#include <vector>
#include <map>
#include <set>
#include <fstream>

class MeanShiftPoint : public FU {
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender=nullptr) override;
	long int ID = 0; // ������������� ��-�����������
	long int NDim = 2; // ���������� ��������� � ������� ������������
	double Mass = 1; // ��� �����
	MeanShiftPoint(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; };
	MeanShiftPoint() { MeanShiftPoint(nullptr, nullptr); };
	vector<double> Coodinate; // ���������� ����� � ������� �����������
	long int NV=0; // ���������� ����� � ��������� ����������� �����?
	void* Manager = nullptr; // ��������� �� ���������
	vector<int> eps; // ����������� ������ �������� ��� ������ ����� �� ��������� ����������� �����
					 // � ��������� �������� ������� �� 2, �.�. ��������������� �������, ����� ���� ������� �������� � ���������
	vector<int> eps_t; // ������� ������ �������� ��� ������ ����� �� ��������� ����������� �����
					 // � ��������� �������� ������� �� 2, �.�. ��������������� �������, ����� ���� ������� �������� � ���������
	vector<MeanShiftPoint**>refXY; // ��������� �� �������� ������ �� �� � �������� �������������� ������ �� ����
	set<MeanShiftPoint*> N; // ��������� ����������� �����
	vector<int> IdXY; // ������� ��-����������� �� ���� ���� ���������
	double  dist(vector<double>& a, vector<double>& b); // ���������� ���������� ����� ����� �������
private:
//	vector < vector<MeanShiftPoint*>>* VXY; // ��������� �� �����, ������������� �� ���������� X � Y
//											// ������ ��������� - ��� ��������� ����� � �������
//	vector<vector<MeanShiftPoint*>::iterator> vxy;  //��������� �� �������� �� � �������� VX, VY  
//													// ������ ��������� - ��� ��������� ����� � �������
	multimap <double,MeanShiftPoint*> Nd; // ��������� ����������� ����� (������� - ���������� �� �����)
	multimap <double, MeanShiftPoint*> N_Alph; // ����������� ����� � �����, ������������� �� ���� �� ������� �����
	long int Nneighbourhood = 14; // �������� ����������� (�.�. ����� �����, ��������� ������ ����� �������)
	double Lmax = -1;
};

class MeanShiftCluster : public FU { // ������� ��� ������ ��������� ������������
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender=nullptr) override;
	void Migration(); // ����� ������������ �����
	void* Manager = nullptr; // ��������� �� ���������
	vector<double> Center; // ���������� ������ ��������
	long int ID = 0; // ������������� ��-�������
	long int NDim = 2; // ���������� ��������� � ������� ������������
	long int CenterPhase = 0; // ���� ������ ��������� ����� ��� �������
	MeanShiftPoint* CenterFU = nullptr; // �� �������� ������� � ������
	double R = 1; // ������ �������
	void MoveToPoint(MeanShiftPoint* CenterStart); // ����� ��������� ������� � ��������� �������
	int MassCenter(vector<double>& CenterNew, MeanShiftPoint* Point, set<MeanShiftPoint*>& Pass); // ���������� ������ ���� ������� (������������ ���������� �����)
	// �� ����� ������ �� �� ��� ���������� � ������ ���������� ������
	vector<vector<double>> MigrationHistory; // ������� ����������� ������ ������� (� ������� �������� ������ ������� �� ������ ����)
	double Eps = 0.0001; // ����������� � ����������� ��� ��������
	long int NExUFCall = 0; // ���������� ������� �� ��� ����������� �� ����� ������
	double Mass = 0; // ����������� ����� ��������
	long int NPoints = 0; // ���������� ����� � ��������
};

class MeanShift : public FU {
public:
	vector <vector<MeanShiftPoint*>> VXY; // ��������� �� �����, ������������� �� ���������� X � Y � �.�.
											// ������ ��������� - ��� ��������� ����� � �������
	void ProgFU(long int MK, LoadPoint Load, FU* Sender=nullptr) override;
	FU* Copy() override; // ��������� ����������� ��
	FU* TypeCopy() override; // ������� �� ������ �� ���� (�� ������� ��������
	MeanShift(FU* BusContext, FU* Templ) : FU(BusContext) { FUtype = 14; ProbMaxMin = { {0,1},{0,1} }; eps = { 15,15 }; eps.resize(2);  Bus = BusContext; };
	MeanShift() : FU() { ProbMaxMin = { {0,1},{0,1} }; eps = { 16,16 }; Bus = nullptr; };
private:
	long int NDim = 2; // ���������� ��������� � ������� ������������
	long int NV=15; // ��������� ���������� ����������� ����� ��� ������ ���������� ��������� ����� ������ ����
	vector<int> eps; // ���������� ������������� ����� �� ����
	vector<vector<double>> ProbMaxMin; // ������������ � ����������� ���������� �� ���� ������������� ��� ��������� �������������� �����
	long int NProb = 10; // ���������� ������������ ��������� �����
	long int ProbPhase = 0, EpsFaze=0; // ���� ���������� ���������� ��� ��������� ��������� ��������� ����� � ���� ���������� ������������ ���������� ����� �� ���� ��� ���������� �������������� �����

	void* NVPointErrProg = nullptr;  // ������������ ������ ��������� �� ������ ��� ���������� ���������� ����� 
								 //� ������� ��� ����������� ��������� ����� ��� ���������� ����� 
	vector<MeanShiftCluster> Clusters; // ������ �������� ��� ������
	long int ClusterPhase = 0; // ���� ������ ���������� � ������� (���������� � ������)
	long int ClusterNetPhaze = 0; // ���� ������ ���������� � �������
	vector<double> ClusterNetParameters; // ��������� ����� ��������
	long int ClusterID = 0; // ������ �������� �������
	double ClusterEps = 0.0001; // ����������� � ����������� ��� ��������
	void  NetGen(); // ��������� �����
	void FileRead(LoadPoint Load); // ���������� ����� ������������ �� �����
	void PointsGen(); // �������� ��������� ����� ������������
	void Start(); // ������ �������������
};
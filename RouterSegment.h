#pragma once
#include "Consts.h"

class SegChannel : FU // ����� ����������� �����
{
public:
	void* Router; // ������ �� 
	int Id = -1; // ������������� (������) ������ 
	//SegChannel(void* parent, FU* Templ = nullptr) { Bus = (FU*)parent; ProgFU(0, { 0, nullptr }); };
	void ProgFU(int MK, LoadPoint Load) override;
};

class RouterSegment : FU // ���������� ������
{
	vector<SegChannel> InChannels; // ������ ������� �������
	vector<FU*> OutCannels; // �������� ������, ���������� ������ �� ��-���������
	int DelayTime = 0; // ����� ��������
	int MKCount = 0; // ���������� ���������� ��
	int MaxMKQueue = 0; // ������������ ����� �������
	double AverageMKQueue = 0; // ������� ����� �������
	double CurrentTime = 0; // ������� ����� �������������
public:
	RouterSegment(void* parent, FU* Templ = nullptr) { FUtype = 21; Bus = (FU*)parent; ProgFU(0, { 0, nullptr }); };
	void ProgFU(int MK, LoadPoint Load) override;
	void Routing(int MK, LoadPoint Load, int ChInd);
};
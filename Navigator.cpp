#include "stdafx.h"
#include "Navigator.h"

using namespace std;

void Navigator::CapsPush(vector<ip>* uk, int _type)
{

	Position _pos;
	_pos.type = _type;
	_pos.Ic = uk;
	_pos.IpList = new vector<ip>();
	currentPos->push_back(_pos);
}

void Navigator::CapsPop()
{
	currentPos->pop_back();
}

void Navigator::IpPush(ip* uk)
{
	if (currentPos->size() > 0){
		currentPos->back().IpList->push_back(*uk);
	}
}

Position* Navigator::CurrentPos()
{
	if (currentPos->size() > 0) {
		return &currentPos->at(currentPos->size() - 1);
	}
	return nullptr;
}

void Navigator::IpPop()
{
	if (currentPos->size() > 0) {
		currentPos->back().IpList->pop_back();
	}
}

void Navigator::CapsSet(void* uk)
{
	if (currentPos->size() > 0) {
		currentPos->back().Ic = (vector<ip>*)uk;
		currentPos->back().IpList->clear();
	}
}

void Navigator::IpSet(void* uk)
{
	if (currentPos->size() > 0) {
		if (currentPos->back().IpList->size() > 0) {
			currentPos->back().IpList->pop_back();
			currentPos->back().IpList->push_back((ip&)uk);
		}
	}
}

void Navigator::IpLoadGoTo()
{
	if (currentPos->size() > 0) {
		currentPos->back().Ic = (vector<ip>*)currentPos->back().IpList->back().Load.Point;
		currentPos->back().IpList->clear();
	}
}

void Navigator::IpLoadPushGoTo()
{
	if (currentPos->size() > 0) {
		Position pos;
		pos.Ic = (vector<ip>*)currentPos->back().IpList->back().Load.Point;
		pos.IpList = new vector<ip>();
		currentPos->push_back(pos);
	}
}

int Navigator::size() {
	return currentPos->size();
}
#include "RouterSegment.h"

void SegChannel::ProgFU(int MK, LoadPoint Load) // Программа работы входного канала сегментного роутера
{
	(*(RouterSegment*)Router).Routing(MK, Load, Id);
}

void RouterSegment::Routing(int MK, LoadPoint Load, int ChInd)
{
	OutCannels[ChInd]->ProgFU(MK, Load);
}

void RouterSegment::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case 0: // Reset
		DelayTime = 0;
		MKCount = 0;
		MaxMKQueue = 0;
		AverageMKQueue = 0;
		CurrentTime = 0;
		InChannels.clear();
		OutCannels.clear();
		break;
	case 3: // ChClear Очистить список каналов
		InChannels.clear();
		OutCannels.clear();
		break;
	case 5: // ChInSet Установить входной канал
		InChannels.push_back({});
		InChannels.back().Id = InChannels.size() - 1;
		InChannels.back().Router = this;
		OutCannels.push_back(nullptr);
		break;
	case 6: // ChOutSet Установить выходной канал
		InChannels.push_back({});
		InChannels.back().Id = InChannels.size() - 1;
		InChannels.back().Router = nullptr;
		OutCannels.push_back((FU*)Load.Point);
		break;
	case 7: // ChOutAdd Добавить выход для текущего канала
		OutCannels.back()=((FU*)Load.Point);
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
}
#include "stdafx.h"
#include "GraphDisplay.h"

void GraphDisplay::ProgFU(long int MK, LoadPoint Load, FU* Sender)
{
	MK %= FUMkRange; // Оставить только свои МК
	switch (MK)
	{
	case 0: // Reset
		GraphUk = nullptr;
		break;
	case 1: // Set Установить указатель на ОА граф
		GraphUk = Load.Point;
		break;
	case 5: // ExtendSet Установить флаг разворачивания всех ветвей иерархческого списка
		Extend = Load.toBool();
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
}

FU* GraphDisplay::Copy() // Программа копирования ФУ
{
	return new GraphDisplay(Bus, this);
}

FU* GraphDisplay::TypeCopy() // Создать ФУ такого же типа (не копируя контекст
{
	return new GraphDisplay(Bus, nullptr);
}

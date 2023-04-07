// ФУ Шлюз
#pragma once

#include "FUini.h"

class Gateway : public FU
{
	FU* Router = nullptr; // Ссылка на роутер
	FU* GatewayFriend = nullptr; //Ссылка на парный шлюз
	double TranslTime = 0; // Время передачи единицы данных
	double TransError = 0; // Вероятность ошибки передачи
	void* RoutingProg = nullptr; //
	void* OverflowProg = nullptr; //
	void* MkOverflowProg = nullptr; //
	int Mode = 0; // 0 - дуплекс, 1 - симплекс от шлюза, 2 - симплекс к шлюзу, 3 - полудуплекс

	int Indata=0, OutData = 0; // Количество переденных данных к шлюзу, и от шлюза

public:
	void ProgFU(int MK, LoadPoint Load) override;
	Gateway(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
	};
};
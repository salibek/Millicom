#pragma once
#include "Consts.h"
#include "StreamFloatALU.h"
#include "StreamIntALU.h"

using namespace std;

class StreamManager : public FU
{
private:
	vector<vector<FU*> > Field; // Поле АЛУ
	int Ind = 0, IndGroup=0;
	int Mode = 0; //  Режим работы ФУ (0 - дробное АЛУ, 1 - целочисленное АЛУ)
	int DeviseCounter = 0;
	void* DevNotExistErrProg = nullptr; // Подпрограмма ошибки 'Нет существует устройства'
	vector<StreamFloatALU> Group; // Вектор группы ФУ для копирования
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	StreamManager(FU* BusContext, FU* Templ) 
	{
		Bus = BusContext;
		FUtype = 23;
		ProgFU(0, { 0,nullptr },nullptr);
	};
	StreamManager() { StreamManager(nullptr, nullptr); };
	~StreamManager();
};

#pragma once
#include "Consts.h"
#include <map>

class AutomatManager : public FU
{
private:
	bool TemplAutoClear = true; // Флаг автоматического сброса шаблона поиска при установке МК для получателя нового сигнала
	int ReceiverMk = -1; // МК для текущего ФУ-состояния
	void* Template = nullptr;// Ссылка на эталонную ИК ic *
	void* Var = nullptr; // Ссылка на ИК переменных ic *
	ip IPout = { 0, {0,new double(0)} };// ИП сигнала(сигнал и его атрибут) ip
	ip IPoutPrev = { 0, {0,new double(0)} };// ИП предыдущего сигнала(сигнал и его атрибут) ip
	map<int, void*> StageInProg; // Программы на состояниях автомата (запуск программы по атрибуту для receiver-а)
	map<int, void*> StageOutProg; // Программы на состояниях автомата (запуск программы по атрибуту для receiver-а)
	int StageProgMk = -1; // МК для установки программы для состояния
public:
	void ProgFU(int MK, LoadPoint Load) override;
	AutomatManager(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; ProgFU(0, { 0,0 }); };
	AutomatManager() : FU() { Bus = nullptr; };
};

class border
{
public:
	FU Parent = nullptr;
	bool Start = true; // Флаг стартового состояния
	double OldVal = 0;
	vector<double> borders;
	vector<void*> UpProg;
	vector<void*> DownProg;
	void Run(LoadPoint Load);
	void Reset() { Start = true; borders.clear(); UpProg.clear(); DownProg.clear(); };
	int* MK; // Ссылка на МК для потребителя
	border(FU Pr) { Parent = Pr; };
	border() { Parent = nullptr; };
};

class Channel
{
public:
	string Name; //Имя канала
	int Ind = -1; // индекс канала
	int Mode = 0; // Режим работы ФУ: 0 - вход, 1 - выход, 2 - дуплекс ...
	// Входная функция преобразования сигнала???
	// Выходная функция преобразования сигнала???
	int Atr = 0, Mk = -1; // Атрибут и МК для выходного сигнала
	double Tact = 0; // Период опроса
	double PrevSignal = 0, Signal = 0; // Предыдущий и текущий сигнал (лучше очередь значений)
	double Sensit = 0; // Чувствительность
	border Border; // Границы
	bool Active = true;
	ip OutIP;
	int* CurrentCh = nullptr;// Ссылка на канал, к которому пришли данные
	FU Parent=nullptr;
	Channel(FU Pr) { Parent = Pr;};
};

class InOut : public FU
{
private:
	string prefix;
	vector <Channel*> Channels; // Вектор каналов
	int Ind = -1; // Номер текущего канала
	int AutoInc = 0; // Автоинкремент
	int Nch = 0; // Количество каналов
public:
	int CurrentCh = -1; // Номер текущего канала (т.е. канала, на который пришли данные)
	void ProgFU(int MK, LoadPoint Load) override;
	InOut(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; ProgFU(0, { 0,0 }); };
	InOut() : FU() { Bus = nullptr; };
};
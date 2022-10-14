#pragma once
#include <map>
#include "Consts.h"
#include "ALU.h"


class AutomatManager : public FU
{
private:
	bool TemplAutoClear = true;  // флаг автоматическогог сброса шаблона поиска при установке МК для получателя
	int ReceiverMk = -1;  // МК для текущего ФУ-состояния
	void *Receiver = nullptr;  // Указатель для текущего ФУ-состояния
	IC_type Template = nullptr;  // ссылка на эталонную ИК ic*
	vector<ip> Var;  // ИК переменных ic*
	ip IPout = { 0, {0,new double(0)} };  // ИП сиграла (сигнал и его атрибут) ip
	ip IPoutPrev = { 0, {0,new double(0)} };  // ИП предыдущего сигнала (сигнал и его атрибут) ip
	map<int, void*> StageInProg;  // программы на состояних автомата (запуск программы по атрибуту для reciever-a)
	map<int, void*> StageOutProg;  // программы на состояних автомата (запуск программы по атрибуту для reciever-a)
	int StageProgMk = -1;  // МК для установки программы для состояния
public:
	void ProgFU(int MK, LoadPoint Load) override;
	AutomatManager(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; ProgFU(0, { 0,0 }); };
	AutomatManager() : FU() { Bus = nullptr; };
};

class border
{
public:
	FU Parent = nullptr;
	bool Start = true;  // флаг стартового состояния
	double OldVal = 0;
	vector<double> borders;
	vector<void*> UpProg;
	vector<void*> DownProg;
	void Run(LoadPoint Load);
	void Reset() { Start = true; borders.clear(); UpProg.clear(); DownProg.clear(); };
	int* MK;  // ссылка на МК для потребителя
	border(FU Pr) { Parent = Pr; };
	border() { Parent = nullptr; };
};

class Channel
{
public:
    Channel(FU* Pr) { ParentFU = Pr; };
	FU* Parent = nullptr;
	string Name;  // имя канала
    int* CurrentCh = nullptr;  // ссылка на канал, к которому пришли данные
    int SignalOutMk = -1;  // МК выходного сигнала (для моделирования)
    int SignalOutAttr = 0;  // атрибут выходного сигнала (для моделирования)
	int Ind = -1;  // инедекс канала
	int Mode = 0;  // режим (вход/выход/дуплекс)
    bool Active = true;  // актив/неактив отдельного канала
	double Tact = 0;  // период выдачи сигнала (для цифровых сигналов или импульсов)
    double Period = 0;  // период опроса
	double Signal = 0;  // текущее значение сигнала
	double Sensit = 0;  // чувствительность
    border* Borders=nullptr;  // таблица зон сигнала
//    ip OutIP;  // 
    FU* ParentFU = nullptr;  // ссылка на ФУ ввода-вывода
    FU* Archiver = nullptr;  // ссылка на ФУ-архиватор
    ALU* ParentALU = nullptr;  // ссылка на АЛУ (множество АЛУ)
    vector<void*> ALUs;  // стек стандартных функций преобразования сигнала
    vector<int> InMks;  // буфер МК для входного сигнала
    vector<LoadPoint> InAdrBuf;  // буфер адресов для входного сигнала
    vector<double> PrevSignals;  // буфер предыдущих значений сигнала
    void* InProg = nullptr;  // ссылка на программу обработки входного сигнала
    void* OutProg = nullptr;  // ссылка на программу обработки выходного сигнала
};

class InOut : public FU
{
private:
	string prefix;
	vector <Channel*> Channels;  // вектор каналов
	int Ind = -1;  // номер текущего канала
	int AutoInc = 0;  // автоинкремент
	int Nch = 0; // количество каналов
public:
	int CurrentCh = -1;  // ??? номер текущего канала, на который пришли данные
	void ProgFU(int MK, LoadPoint Load) override;
	InOut(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; ProgFU(0, { 0,0 }); };
	InOut() : FU() { Bus = nullptr; };
};
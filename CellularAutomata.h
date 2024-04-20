#pragma once
#include <vector>
#include "Consts.h"
#include "ALU.h"
#include "Router.h"
#include "Gateway.h"

// Простейший клеточный автомат (устройство для вычисления сеточных функций)
class CellularAutomata : public FU {
//private:
public:
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст
	CellularAutomata(FU* BusContext, FU* Templ) : FU(BusContext) { FUtype = 18; Bus = BusContext; ProgFU(0, { 0,nullptr }); };
	CellularAutomata() : FU() { CellularAutomata(nullptr, nullptr); };
	void* Manager = nullptr;// Ссылка на менеджера
	vector<FU*> Neighbours;//Ссылки на соседей
	vector<int> NeighboursMk;// МК для соседей
	vector<double> Rez; //вектор результатов вычислений
	vector<vector<LoadPoint>> Plys = { { {0,nullptr} } }; // Вектор входных данных, полученных от соседей
	vector<LoadPoint> parameters; // Список параметров
	vector<void*>ReceiveProgs; // Программы при приходе одиночных данных на отдельные входные каналы
//	vector<bool> parametersF; // Список флагов поступления входных параметров
	vector<double> Vars; // Вектор локальных переменных (используется для хранения промежуточных данных вычислений)
	bool AutoSend = false; // Флаг автоматической пересылки результатов вычисления
	vector<bool> RezReady; // Флаг готовности результата (для каждого слоя расчетов)
	vector<bool> InComplectF; // Флаг поступления всех входных даных
	vector<int> InCounter = {0}; // Счетчик количества поступивших данных (после вычисления резульатов сбрасывается)
	long int PlyInd = 0; //индекс слоя
	long int PlyCurrent = 0; //индекс текушего слоя
	long int ParameterInd = 0;// Индекс текущего параметра
	long int PlyN = 1; // Максимальное количество слоев для расчета
	long int Ind = 0; // Индекс (индексацая различных векторов Neighbours, NeighboursMk, parameters и т.д.
	void* FiringProg = nullptr; // Указатель на программу, для вычисления результата при приходе всех входящих данных
	void* ReceiveProg = nullptr; // Указатель на программу, запускаемую при приходе данных от любого соседа
	long int Mode = 1; // Режим работы автомата (0 - настройка, 1 - рабочий режим)
	void* Collector = nullptr; // ­	Ссылка на контекст ФУ для вывода информации
	long int IndFuOffcet = 0; // ­	Смещение индекса ФУ (индекс домножается на эту величину и к нему прибавляется индекс слоя)
//	int FUInd = 0; // Индекс ФУ-автомата
	long int VarInd = -1; // Индекс выдаваемой или устанавливаемой локальной переменной
	long int N_In = 0; // Количество входных данных для срабатывания
	long int State = 0; // Состояние автомата
	long int MkToSet = -1; // Милликоманда по умолчанию для установки ???
	double ReceiveTime=0, CalcTime=0, SendTime=0, OtherMkTime=0; // Время задержки чтения, вычисления, записи данных
	double TransferTime = 0; // Време передачи данных от одного автомата другому
	ipSender Received; // Полученные данные
	long int PriorityMk = -1; // Приоритетная МК для отправки соседям (при -1 считается, что приоритетной МК нет
	bool OutBlock = false; // Флаг блокировки выдачи данных
};

// Менеджер простейшего клеточного автомата (устройство для вычисления сеточных функций)
class CellularAutomatManager : public FU {
	//private:
public:
	void DistrebuteModelGenerate(); // Генерация модели распределенное ВС
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	long int NetType = 0;// Тип автоматной сетки 0 - не задано, 1- квадратная, 2 - треугольная, 3 гексагональная
	CellularAutomatManager(FU* BusContext, FU* Templ) : FU(BusContext) { FUtype = 19; Bus = BusContext; ProgFU(0, { 0,nullptr });};
	CellularAutomatManager() : FU() { Bus = nullptr; };
	vector<CellularAutomata> Net; //вектор сеточных автоматов
	long int Dim = 0; // Размерность поля автоматов (количество ФУ для каждого измерения)
	bool NilRefMode = false; // Режим добавления нулевых ссылок, если ссылка устанавливается на несуществующий элемент вычислительной сетки
	void* Collector = nullptr; // Ссылка на коллектор

	vector <Router> Routers; // Вектор роутеров для моделирования распределенной вычислительной сетки
	vector <Gateway> Gateways; // Вектор шлюзов для моделирования распределенной вычислительной сетки
	vector <long int> SectorDim; // Размеры областей для моделирования распределенной ВС
	vector <Scheduler> Schedullers; // Вектор планировщиков вычислений
	Eventser* NetEventser = nullptr; // Контроллер событий

	void* iniAutmataProg = nullptr; // Программа инициализации автоматов

	long int Ind1 = -1, Ind2 = -1; // Индексы первого и второго ФУ-автомата
	long int Step1 = 0, Step2 = 0; // Шаг автоматической инсрементации
	long int PlyN = 1; // Максимальное количество слоев для расчета
	long int ContextAddMk=20; // Код МК добавления МК для исполнительного ФУ
	long int MkAddMk=21; // Код МК для добавления МК соседа исполнительного ФУ
	long int AutomataMk1 = -1, AutomataMk2 = -1; // Текущая МК для автомата по индексами 1, 2
	long int GatewayMk = -1, RouterMk = -1; // МК для шлюза и маршрутизатора
	vector<long int> NetDim; // Вектор с размерностями прямоугольной сетки
};
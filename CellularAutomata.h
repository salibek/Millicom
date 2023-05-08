#pragma once
#include <vector>
#include "Consts.h"
#include "ALU.h"

// Простейший клеточный автомат (устройство для вычисления сеточных функций)
class CellularAutomat : public FU {
//private:
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	CellularAutomat(FU* BusContext, FU* Templ) : FU(BusContext) { FUtype = 18; Bus = BusContext; ProgFU(0, { 0,nullptr }); };
	CellularAutomat() : FU() { Bus = nullptr; ProgFU(0, { 0,nullptr });};
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
	int PlyInd = 0; //индекс слоя
	int PlyCurrent = 0; //индекс текушего слоя
	int ParameterInd = 0;// Индекс текущего параметра
	int PlyN = 1; // Максимальное количество слоев для расчета
	int Ind = 0; // Индекс (индексацая различных векторов Neighbours, NeighboursMk, parameters и т.д.
	void* FiringProg = nullptr; // Указатель на программу, для вычисления результата при приходе всех входящих данных
	void* ReceiveProg = nullptr; // Указатель на программу, запускаемую при приходе данных от любого соседа
	int Mode = 1; // Режим работы автомата (0 - настройка, 1 - рабочий режим)
	void* Collector = nullptr; // ­	Ссылка на контекст ФУ для вывода информации
	int IndFuOffcet = 0; // ­	Смещение индекса ФУ (индекс домножается на эту величину и к нему прибавляется индекс слоя)
//	int FUInd = 0; // Индекс ФУ-автомата
	int VarInd = -1; // Индекс выдаваемой или устанавливаемой локальной переменной
	int N_In = 0; // Количество входных данных для срабатывания
	int State = 0; // Состояние автомата
	int MkToSet = -1; // Милликоманда по умолчанию для установки ???
	double ReceiveTime=0, CalcTime=0, SendTime=0, OtherMkTime=0; // Время задержки чтения, вычисления, записи данных
	double TransferTime = 0; // Време передачи данных от одного автомата другому
};

// Менеджер простейшего клеточного автомата (устройство для вычисления сеточных функций)
class CellularAutomatManager : public FU {
	//private:
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender = nullptr) override;
	int NetType = 0;// Тип автоматной сетки 0 - не задано, 1- квадратная, 2 - треугольная, 3 гексагональная
	CellularAutomatManager(FU* BusContext, FU* Templ) : FU(BusContext) { FUtype = 19; Bus = BusContext; ProgFU(0, { 0,nullptr });};
	CellularAutomatManager() : FU() { Bus = nullptr; };
	vector<CellularAutomat> Net; //вектор сеточных автоматов
	int Dim=0; // Размерность поля автоматов (количество ФУ для каждого измерения)
	void* iniAutmataProg = nullptr; // Программа инициализации автоматов
	void* Collector = nullptr; // Ссылка на коллектор
//	void* ActivationProg = nullptr; // Программа активации ФУ при приходе всех необходимых данных
//	void* ReceiveProg = nullptr; // Программа активации при получении автоматом данных от соседей
//	void* FiringProg = nullptr; // Программа вычисления результата
	int Ind1 = -1, Ind2 = -1; // Индексы первого и второго ФУ-автомата
	int Step1 = 0, Step2 = 0; // Шаг автоматической инсрементации
//	int InCounter = 0; // Установить количество входящих данных
	int PlyN = 1; // Максимальное количество слоев для расчета
	int ContextAddMk=20; // Код МК добавления МК для исполнительного ФУ
	int MkAddMk=21; // Код МК для добавления МК соседа исполнительного ФУ
	int AutomataMk1 = -1, AutomataMk2 = -1; // Текущая МК для автомата по индексами 1, 2
};
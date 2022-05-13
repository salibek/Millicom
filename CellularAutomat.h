#pragma once
#include <vector>
#include "Consts.h"
#include "ALU.h"

// Простейший клеточный автомат (устройство для вычисления сеточных функций)
class CellularAutomat : public FU {
//private:
public:
	void ProgFU(int MK, LoadPoint Load) override;
	CellularAutomat(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; };
	CellularAutomat() : FU() { Bus = nullptr; };
	void* Manager = nullptr;// Ссылка на менеджера
	vector<CellularAutomat*> Neighbours;//Ссылки на соседей
	vector<int> NeighboursMk;// МК для соседей
	vector<double> Rez; //вектор результатов вычислений
	vector<vector<LoadPoint>> Plys; // Вектор входных данных, полученных от соседей
	vector<LoadPoint> parameters; // Список параметров
//	vector<bool> parametersF; // Список флагов поступления входных параметров
	vector<double> Vars; // Вектор локальных переменных (используется для хранения промежуточных данных вычислений)
	bool AutoSend = false; // Флаг автоматической пересылки результатов вычисления
	vector<bool> RezReady; // Флаг готовности результата (для каждого слоя расчетов)
	vector<vector<bool>> InComplectF; // Флаг поступления всех входных даных
	vector<int> InCounter; // Счетчик количества поступивших данных (после вычисления резульатов сбрасывается)
	int PlyInd = -1; //индекс слоя
	int PlyCurrent = 0; //индекс текушего слоя
	int ParameterInd = -1;// Индекс текущего параметра
	void* CalcProg = nullptr; // Указатель на программу, для вычисления результата
	void* ReceiveProg = nullptr; // Указатель на программу, запускаемую при приходе МК, если указатель установлен, то ФУ обработка входящей МК 
	int Mode = 1; // Режим работы автомата (0 - настройка, 1 - рабочий режим)
	void* CollectorFU = nullptr; // ­	Ссылка на контекст ФУ для вывода информации
	int IndFuOffcet = 0; // ­	Смещение индекса ФУ (индекс домножается на эту величину и к нему прибавляется индекс слоя)
	int FUInd = 0; // Индекс ФУ-автомата
	int VarInd = -1; // Индекс выдаваемой или устанавливаемой локальной переменной
};

// Менеджер простейшего клеточного автомата (устройство для вычисления сеточных функций)
class CellularAutomatManager : public FU {
	//private:
public:
	void ProgFU(int MK, LoadPoint Load) override;
	int NetType = 0;// Тип автоматной сетки 0 - не задано, 1- квадратная, 2 - треугольная, 3 гексагональная
	CellularAutomatManager(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; };
	CellularAutomatManager() : FU() { Bus = nullptr; };
	vector<CellularAutomat> Net; //вектор сеточных автоматов
	vector<int> Dim; // Размерность поля автоматов (количество ФУ для каждого измерения)
	void* iniAutmataProg = nullptr; // Программа инициализации автоматов
};
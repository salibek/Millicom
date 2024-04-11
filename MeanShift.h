#pragma once
#include "Consts.h"
#include <vector>
#include <map>
#include <set>
#include <fstream>

class MeanShiftPoint : public FU {
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender=nullptr) override;
	int ID = 0; // Идентификатор ФУ-исполнителя
	int NDim = 2; // Количество изменений в фазовом пространстве
	double Mass = 1; // Вес точки
	MeanShiftPoint(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; };
	MeanShiftPoint() { MeanShiftPoint(nullptr, nullptr); };
	vector<double> Coodinate; // координата точки в фазовом прострастве
	int NV; // Количество точек в множестве близлажищих точек?
	void* Manager = nullptr; // Указатель на менеджера
	vector<int> eps; // Изначальная ширина коридора для выбора точек во множество близлежащих точек
					 // В программе значение делится на 2, т.е. устанавливается диаметр, чтобы было удобнее работать в программе
	vector<int> eps_t; // Текущая ширина коридора для выбора точек во множество близлежащих точек
					 // В программе значение делится на 2, т.е. устанавливается диаметр, чтобы было удобнее работать в программе
	vector<MeanShiftPoint**>refXY; // Указатели на описание ссылки на ФУ в векторах упрорядоченных ссылок по осям
	set<MeanShiftPoint*> N; // Множество близлежащих точек
	vector<int> IdXY; // Индексы ФУ-исполнителя по всем осям координат
	double  dist(vector<double>& a, vector<double>& b); // Вычисление расстояния между двумя точками
private:
//	vector < vector<MeanShiftPoint*>>* VXY; // Указатели на точки, упорядоченные по координате X и Y
//											// Каждое изменение - это отдельная линия в векторе
//	vector<vector<MeanShiftPoint*>::iterator> vxy;  //указатели на описание ФУ в векторах VX, VY  
//													// Каждое изменение - это отдельная линия в векторе
	multimap <double,MeanShiftPoint*> Nd; // Множество близлежащих точек (атрибут - расстояние до точки)
	multimap <double, MeanShiftPoint*> N_Alph; // Близлежащие точки в сетке, упорядоченные по углу от текущей точки
	int Nneighbourhood = 14; // Величина окрестности (т.е. число точек, скоторыми данная точка связана)
	double Lmax = -1;
};

class MeanShiftCluster : public FU { // Область для поиска максимума концентрации
public:
	void ProgFU(int MK, LoadPoint Load, FU* Sender=nullptr) override;
	void Migration(); // Поиск концентрации точек
	void* Manager = nullptr; // Указатель на менеджера
	vector<double> Center; // Координата центра кластера
	int ID = 0; // Идентификатор ФУ-региона
	int NDim = 2; // Количество изменений в фазовом пространстве
	int CenterPhase = 0; // Фаза записи стартовой точки для региона
	MeanShiftPoint* CenterFU = nullptr; // ФУ наиболее близкое к центру
	double R = 1; // Радиус региона
	void MoveToPoint(MeanShiftPoint* CenterStart); // Метод установки региона в стартовую позицию
	int MassCenter(vector<double>& CenterNew, MeanShiftPoint* Point, set<MeanShiftPoint*>& Pass); // Вычисление центра масс области (Возвращается количество точек)
	// на входе ссылка на ФУ для вычисления и вектор пройденных вершин
	vector<vector<double>> MigrationHistory; // История перемещения центра области (в историю попадают центры области на каждом шаге)
	double Eps = 0.0001; // Погрешность в вычислениях при миграции
	int NExUFCall = 0; // Количество вызовов ФУ при перемещении во время поиска
	double Mass = 0; // Вычисленная масса кластера
	int NPoints = 0; // Количество точек у кластере
};

class MeanShift : public FU {
public:
	vector <vector<MeanShiftPoint*>> VXY; // Указатели на точки, упорядоченные по координате X и Y и т.д.
											// Каждое изменение - это отдельная линия в векторе
	void ProgFU(int MK, LoadPoint Load, FU* Sender=nullptr) override;
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст
	MeanShift(FU* BusContext, FU* Templ) : FU(BusContext) { FUtype = 14; ProbMaxMin = { {0,1},{0,1} }; eps = { 15,15 }; eps.resize(2);  Bus = BusContext; };
	MeanShift() : FU() { ProbMaxMin = { {0,1},{0,1} }; eps = { 16,16 }; Bus = nullptr; };
private:
	int NDim = 2; // Количество изменений в фазовом пространстве
	int NV=15; // Требуемое количество близлежащих точек для начала построения окрестной сетки вокруг узла
	vector<int> eps; // Количество анализируемых точек по осям
	vector<vector<double>> ProbMaxMin; // Максимальные и минимальные координаты по осям просотранства для генерации вычислительной сетки
	int NProb = 10; // Количество генерируемых случайных точек
	int ProbPhase = 0, EpsFaze=0; // Фаза считывания параметров для генерации множества случайных точек и фаза считывания необходимого количество точек по осей для построения вычислительной сетки

	void* NVPointErrProg = nullptr;  // Подпрограмма выдачи сообщения об ошибке при превышении количества точек 
								 //в системе над количеством требуемых точек для построения сетки 
	vector<MeanShiftCluster> Clusters; // Список регионов для поиска
	int ClusterPhase = 0; // Фаза записи информации о региона (координаты и радиус)
	int ClusterNetPhaze = 0; // Фаза записи информации о регионе
	vector<double> ClusterNetParameters; // Параметры сетки регионов
	int ClusterID = 0; // Индекс текущего региона
	double ClusterEps = 0.0001; // Погрешность в вычислениях при миграции
	void  NetGen(); // Генерация сетки
	void FileRead(LoadPoint Load); // Считывание точек пространства из файла
	void PointsGen(); // Генеация случайных точек пространства
	void Start(); // Запуск кластеризации
};
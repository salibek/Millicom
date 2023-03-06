#define _USE_MATH_DEFINES
#include "stdafx.h"
#include "MeanShift.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>

int MeanShiftCluster::MassCenter(vector<double>&CenterNew, MeanShiftPoint* Point, set<MeanShiftPoint*> &Pass) // Вычисление центра масс области (Возвращается количество точек)
// на входе ссылка на ФУ для вычисления и вектор пройденных вершин
// Вовращается количество пройденных исполнительных вершин
{
	if (Pass.count(Point)) return 0; // Если точка уже была пройдена
	if (Point->dist(Point->Coodinate, Center) > R*R)return 0;
	Pass.insert(Point); // Включаем вершину в список посмотренных
	for (int i = 0; i < NDim; i++)
		CenterNew[i]+= Point->Coodinate[i];
	int NS = 1;
	for (auto &i : Point->N)
		NS += MassCenter(CenterNew,i,Pass);
	return NS;
}

void MeanShiftCluster::Migration() // Поиск концентрации точек
{
	//	vector<double> CenterOld; // Старый центр области
	vector<double> CenterNew=Center;
	//NExUFCall = 0;
	do
	{
		Center = CenterNew;
		MigrationHistory.push_back(Center);//Записать историю перемещений
		MoveToPoint(CenterFU);
		// Вычисление центра масс области (заглушка)
		set<MeanShiftPoint*> Pass;
		NPoints = MassCenter(CenterNew, CenterFU, Pass);
		if (!NPoints) return;
		NExUFCall+= NPoints; // Подсчитываем количество вызванных ФУ
		for (auto& i : CenterNew)
			i /= NPoints;
	} while (CenterFU->dist(CenterNew, Center)>Eps);
	Mass = NPoints; // Заглушка
}

void MeanShiftCluster::MoveToPoint(MeanShiftPoint* CenterStart) // Метод установки региона в стартовую позицию
{
	CenterFU=CenterStart;
	MeanShiftPoint* MinDistFU = CenterFU;
	double MinDist = CenterFU->dist(Center, CenterFU->Coodinate);
	while(1)
	{
		for(auto &i: CenterFU->N)
			if (i->dist(Center, i->Coodinate) < MinDist)
			{
				MinDistFU = i;
				MinDist = i->dist(Center, i->Coodinate);
			}
		if (MinDist == CenterFU->dist(Center, CenterFU->Coodinate))
			break;
		CenterFU = MinDistFU;
	}	
}

void MeanShiftCluster::ProgFU(int MK, LoadPoint Load) // Область для поиска максимума концентрации
{
	switch (MK)
	{
	case 0: // Reset
		break;
	case 1: // NDimSet
		NDim = Load.toInt();
		Center.resize(NDim);
		break;
	case 5: //CenterSet Установить координаты центра региона
		Center[CenterPhase] = Load.toDouble();
		CenterPhase = (CenterPhase + 1) % NDim;
		break;
	case 10: //RSet Установить радиус региона
		R = Load.toDouble();
		break;
	case 20: // ToStartPoint Переместить регион в стартовую позицию (в нагрузке указатель на ФУ-исполнителя вычислительной сетки)
		MoveToPoint((MeanShiftPoint*)Load.Point); // Вызов метода установки региона в стартовую позицию
		break;
	case 30:// Migration Перемещение области для поиска концентрации точек
		Migration();
		break;
	case 50: // CenterOut Выдать координаты центра области
		Load.Write(Center);
		break;
	case 51: // CenterOutMk Выдать МК с координатами центра области
		MkExec(Load, { CdoubleArray,&Center });
		break;
	case 60: // HistoryOut Выдать историю перемещения области
	//	Load.Write(MigrationHistory);
		break;
	case 61: // HistoryOutMk Выдать МК с историей перемещения области
		MkExec(Load, {CintArray2,&MigrationHistory});
		break;
	}
}

double  MeanShiftPoint::dist(vector<double> &a, vector<double> &b) // Вычисление расстояния между двумя точками
{
	if (a.size() != b.size()) 
		return -1; // Не совпадают размерности пространств для точек
	double D = 0;
	for (vector<double>::iterator ua = a.begin(), ub = b.begin(); ua != a.end(); ua++, ub++)
		D += (*ua - *ub) * (*ua - *ub);
	return D;
}

void MeanShiftPoint::ProgFU(int MK, LoadPoint Load) // Поведение точки фазового пространства
{
	switch (MK)
	{
	case 100: // Gen
	{
		// Подбор точек, находящихся в области вокруг данной точки
		vector<int>eps_t(eps); // Текущая ширина коридора для выбора точек во множество близлежащих точек
		N.clear();
		for (;;) // Пока не набирается нужное количество близлежащих точек
		{
			MeanShift* MANAGER = (MeanShift*)Manager;
			for (auto uFU = MANAGER->VXY[0].begin() + ((eps_t[0] > IdXY[0]) ? 0 : IdXY[0] - eps_t[0]);
				uFU != MANAGER->VXY[0].end() && distance(MANAGER->VXY[0].begin(), uFU) <= IdXY[0] + eps_t[0]; uFU++)
			{
				if ((*uFU)->ID == ID) 
					continue; // Пропускаем данное ФУ
				int i_dim;
				for (i_dim = 1; i_dim < NDim &&
					(*uFU)->IdXY[i_dim] <= IdXY[i_dim] + eps_t[i_dim] &&
					(*uFU)->IdXY[i_dim] >= IdXY[i_dim] - eps_t[i_dim]; i_dim++);
				if (i_dim == NDim) // Если добрались до конца вектора
					N.insert(*uFU); // Формирование множества близлежащих точек N
			}

			if (N.size() >= NV)
				break;
			// Еще одна итерация с расширенными в 2 раза границами
			for (auto& i : eps_t)
				i += i;
			N.clear();
			//break; // Заглушка!!!!
		}
		// Генерация окрестной сетки
		multimap<double, MeanShiftPoint*> Distance; // Список близлежащих вершин, упорядоченных по расстоянию от точки
		multimap<double, MeanShiftPoint*> Angle; // Новый список близлежащих вершин, упорядоченных по углу
		for (auto& i : N)
			Distance.insert({ dist(this->Coodinate,i->Coodinate),i });// Список ФУ по расстояниям

		for (auto& k : Distance)
		{
			Angle.insert(pair<double, MeanShiftPoint*>
				(atan2(k.second->Coodinate[0] - Coodinate[0],
					k.second->Coodinate[1] - Coodinate[1]), k.second)); // Дабавление элементов
			if (Angle.size() < 3)continue;
			auto i = Angle.begin();
			auto st = Angle.begin();
			auto fin = Angle.begin();
			for (; i->second != k.second; i++);
			if (i == Angle.begin()) //Если точка в начале списка по углам
			{
				fin = (--Angle.end());
				st = ++Angle.begin();
			}
			else if (i == --Angle.end()) // Если точка в конце списка по углам
			{
				fin = Angle.begin();
				st = i; st--;
			}
			else
			{
				st = fin = i;
				st--;
				fin++;
			}
			double dSt = (abs(i->first - st->first)) <= M_PI ? abs(i->first - st->first) : abs(i->first + st->first);
			double dFin = (abs(i->first - fin->first)) <= M_PI ? abs(i->first - fin->first) : abs(i->first + fin->first);
			double dStFin = (abs(st->first - fin->first)) <= M_PI ? abs(st->first - fin->first) : abs(st->first + fin->first);

			//			if (dSt + dFin != dStFin)
			//				cout << i->first << " " << st->first << " " << fin->first << endl;

			if (dSt + dFin == dStFin && // Если точка находится в остром угле между двумя соседними точками
				k.first > dist(st->second->Coodinate, fin->second->Coodinate))// Расстояние между соседними точками меньше расстояния от данной точки до текущей точки
			{
				Angle.erase(i); // Удалить текущую точку из списка близлежащих точек, т.к. она не образует близлажщую сетку
//				cout << k.first << " " << dist(st->second->Coodinate, fin->second->Coodinate) << endl;
			}
			else
			{
				//		cout << i->first << " " << st->first << " " << fin->first << endl;
					//	cout<< k.first<<" " <<dist(st->second->Coodinate, fin->second->Coodinate) << endl;
			}

		}
		// Запись в контекст ФУ-исполнителя списка на близлежащие вершины списка
		N.clear();
		//		cout << Angle.size() << endl;
		//		cout<<this->Coodinate[0]<<" " << this->Coodinate[1] << endl;
		for (auto i : Angle)
		{
			N.insert(i.second);
			//			cout << i.second->Coodinate[0]<<" " << i.second->Coodinate[1]<<endl;
		}
		break;
	}
	case 101: // Addition Добавление своего описания к соседям
		for (auto i : N)
			i->N.insert(this);
		break;
/*
	case 102: // ArcsCorrect Корректировка пересекающихся ребер
	{	
		set <MeanShiftPoint*> ToDel; // Множество ребер для удаления
		do
		{
		for (auto i : ToDel)
			N.erase(i);
		ToDel.clear();
		for(auto &k: N)
			for (auto &i : k->N)
				for (auto &j : k->N)
					if (i!=j && i->N.count(this) && j->N.count(this) && k!=this &&
						k->dist(this->Coodinate,k->Coodinate)>k->dist(i->Coodinate,j->Coodinate))
						ToDel.insert(k);
		}
		while (ToDel.size()!=0);
		break;
	}
	*/
	}

}

void MeanShift::ProgFU(int MK, LoadPoint Load) // Поведение ФУ MeanShift
{
	switch (MK)
	{
	case 0: // Reset
		break;
	case 3: // NDimSet Установить количество измерений фазового пространства
		NDim = Load.toInt();
		EpsFaze = 0; // Сброс фазы считывания eps
		eps.resize(NDim);
		ProbMaxMin.resize(NDim);
		for (auto& i : ProbMaxMin)
			i.resize(2);
		break;
	case 5: // Start
		//...
		if ((Load.Type >> 1) == Dstring)
			FileRead(Load);
		//NetGen();
		for (auto& R : Clusters) // Вызов программ миграции регионов
			R.Migration();
		break;
	case 8: // NVSet Установить требуемое количество близлежащих точек для начала построения окрестной сетки вокруг узла
		NV = Load.toInt();
		break;
	case 15: // PointsGen Генерация случайных точек (праметр: макс и мин координаты по осям и количество генерируемых точек)
	case 16: // PointsGenStart Генерация случайных точек (праметры: верхний левый угол поля, правый нижний угол поля, количество точек)
		if (ProbPhase < NDim + NDim)
			ProbMaxMin[ProbPhase / 2][ProbPhase % 2] = Load.toDouble();
		else
		{
			NProb = Load.toInt();
			PointsGen(); // Генерация точек поля
			NetGen(); // Генерация сетки
			if (MK == 16)
				Start(); // Запуск кластеризации
			ProbPhase = -1; // Сброс фазы МК в нуль через строку ProbPhase++
		}
		ProbPhase++;
		break;
	case 20: // ClusterSet Создать и установить параметры региона для поиска
		if (!ClusterPhase)
		{
			Clusters.push_back({});
			Clusters.back().NDim = NDim;
			Clusters.back().Manager = this;
			Clusters.back().ID = Clusters.size() - 1; // Записать идентификатор ФУ-региона
			Clusters.back().Center.push_back(Load.toDouble());
			Clusters.back().Eps = ClusterEps;
		}
		else if (ClusterPhase == NDim)
		{
			Clusters.back().R = Load.toDouble();
			Clusters.back().MoveToPoint(VXY[0][0]);
		}
		else
			Clusters.back().Center.push_back(Load.toDouble());
		ClusterPhase = (ClusterPhase + 1) % (NDim+1);
		break;
	case 21: //ClustersReset Очистить список регионов
		Clusters.clear();
		break;
	case 23: // NRigionOut Выдать количество регионов
		Load.Write(Clusters.size());
		break;
	case 24: // NRigionOutMk Выдать МК с количеством регионов
	{
		int t = Clusters.size();
		MkExec(Load, { Cint,&t });
		break;
	}
	case 25: // ClusterIDSet Установить номер текущего региона
		ClusterID = Load.toInt();
		break;
	case 26: // ClusterCenterOut Выдать координаты региона
		if(Clusters.size() && ClusterID>=0 && ClusterID<Clusters.size())
				Load.Write(Clusters[ClusterID].Center);
		break;
	case 27: // ClusterCenterOutMk Выдать МК с координатами региона (Если индекс <0, то выдаются координаты всех областей)
		if (Clusters.size())
			if(ClusterID >= 0 && ClusterID < Clusters.size())
				MkExec(Load, {CdoubleArray, &Clusters[ClusterID].Center});
			else // Печать координат центров всех регионов
				for(auto &i:Clusters)
					MkExec(Load, { CdoubleArray, &i.Center });
		break;
	case 28: // ClusterNearestPointOut Выдать координаты точки, ближайшей к центру региона
		Load.Write(Clusters[ClusterID].CenterFU->Coodinate);
		break;
	case 29: // ClusterNearestPointOutMk Выдать МК с координатами точки, ближайшей к центру региона
		if(Clusters.size() > ClusterID && Clusters[ClusterID].CenterFU!=nullptr)
			MkExec(Load, { CdoubleArray, &Clusters[ClusterID].CenterFU->Coodinate });
		break;
	case 35: // MigrationHistoryOutMk Выдать МК с историей перемещения области (Если идентификатор региона < 0, то выводится история перемещений всех областей)
		if(Clusters.size())
			if(ClusterID>=0 && ClusterID<=Clusters.size())
				MkExec(Load, { CdoubleArray2,&Clusters[ClusterID].MigrationHistory });
			else
				for(auto &i:Clusters)
					MkExec(Load, { CdoubleArray2,&i.MigrationHistory });
		break;
	case 37: // ClusterNetSet  Установить сетку регионов. Параметры: мин и макс координаты области по кадой из осей, количество  кластеров по каждому измерению, в конце радиус областей
	{
		if (!ClusterNetPhaze)
			ClusterNetParameters.clear();
		if (ClusterNetPhaze < 3 * NDim)
			ClusterNetParameters.push_back(Load.toDouble());
		else // Построение сетки регионов
		{
			Clusters.clear();
			int NPoints = 1;
			for (int i = 2; i < NDim * 3; i+=3)
				NPoints *= round(ClusterNetParameters[i]); // Подсчет количества точек в сетке регионов
			Clusters.resize(NPoints);
			// Заглушка для 2-мерного случая
			for (int i = 0; i < round(ClusterNetParameters[2]); i ++) // Первое измерение
				for (int j = 0; j < round(ClusterNetParameters[5]); j ++) // Второе измерение
				{
					MeanShiftCluster* Reg = &Clusters[i * round(ClusterNetParameters[5]) + j];
					Reg->Eps=ClusterEps;
					Reg->NDim = NDim;
					Reg->R = Load.toDouble();
					Reg->ID = i * ClusterNetParameters[5] + j;
					Reg->Center.clear();
					Reg->Center = { ClusterNetParameters[0] + (ClusterNetParameters[1] - ClusterNetParameters[0]) / (ClusterNetParameters[2]-1) * i,
									ClusterNetParameters[3] + (ClusterNetParameters[4] - ClusterNetParameters[3]) / (ClusterNetParameters[5]-1) * j };
					Reg->Manager = this;
					Reg->MoveToPoint(VXY[0][0]);
				}
		}
		ClusterNetPhaze = (ClusterNetPhaze + 1) % (3 * NDim + 1);
	}
		break;
	case 40: // PointsOutMk Выдать точки фазового пространства
	{
		
		for (auto& i : VXY[0])
		{
			MkExec(Load, { CdoubleArray,&i->Coodinate });
		}
		break;
	}
	case 50: //ArcsOutMk Выдать список дуг
	{
		set<pair< MeanShiftPoint*, MeanShiftPoint*> >ArksOut;// Список координат
		for (auto& i : VXY[0])
		{
			pair<MeanShiftPoint*, MeanShiftPoint*> p;
			for (auto j : i->N) {
				if (i->ID < j->ID)
				{
					p.first = i; p.second = j;
				}
				else
				{
					p.first = j; p.second = i;
				}
				ArksOut.insert(p);
			}
		}
		
		for (auto &i : ArksOut)
		{
			MkExec(Load, { CdoubleArray, &i.first->Coodinate });
			MkExec(Load, { CdoubleArray, &i.second->Coodinate });
		}
		break;
	}
	case 60: // NVPointErrProgSet Установить программу реакции на событие превышения количества требуемых для окрестности точек над количеством точек в системе
		NVPointErrProg = Load.Point;
		break;
	case 71: // epsSet Установить количество анализируемых точек по оси (по умолчанию 20)
		eps[EpsFaze] = Load.toInt(20)/2;
		EpsFaze = (EpsFaze + 1) % NDim;
		break;
	case 72: // epsAllSet Установить одинаковое количество анализируемых точек для всех осей (по умолчанию 20)
	{	int t = Load.toInt(20) / 2;
		for (auto& i : eps)
			i = t;
		break;
	}
	case 75: //ClusterEpsSet Утановить погрешность для прекращения миграции кластера
		ClusterEps = Load.toDouble();
		break;
	case 80://NClusterPointCallOut Выдать количество используемых во вермя миграции исполнительных устройств для одного кластера во время миграции
		if (ClusterID >= 0 && ClusterID <= Clusters.size())
			Load.Write(Clusters[ClusterID].NExUFCall);
		break;
	case 81://NClusterPointCallOut Выдать МК с количеством используемых во вермя миграции исполнительных устройств для одного кластера во время миграции
		if (ClusterID >= 0 && ClusterID <= Clusters.size())
			MkExec(Load, { Cint,&Clusters[ClusterID].NExUFCall });
		break;
	case 85: // NAllClusterPointCallOut Выдать среднее количетство опрошенных точек пространства для всех кластеров
	case 86: // NAllClusterPointCallOutMk Выдать МК с количетством итераций миграции всех кластеров
	case 87: // NAveragePointCallOut  Выдать среднее количество миграций на один кластер
	case 88: // NAveragePointCallOutMk Выдать МК со средним количеством миграций на один кластер
	{
		int t = 0;
		for (auto & i : Clusters)
			t += i.NExUFCall;
		if(MK==85)
			Load.Write(Clusters[ClusterID].NExUFCall);
		else if(MK==86)
			MkExec(Load, { Cint,&Clusters[ClusterID].NExUFCall });
		else
		{
			double tt = (double)t / Clusters.size();
			if (MK == 87)
				Load.Write(&tt);
			else
				MkExec(Load, { Cdouble,&tt });
		}
		break;
	}
		
	case 90: // NClusterMovesOut Выдать количетство итераций миграции кластера с индексом ClusterID во время миграции
		if (ClusterID >= 0 && ClusterID <= Clusters.size())
			Load.Write(Clusters[ClusterID].MigrationHistory.size());
		break;
	case 91: // NClusterMovesOutMk Выдать МК с количетством итераций миграции кластера с индексом ClusterID во время миграции
		if (ClusterID >= 0 && ClusterID <= Clusters.size())
		{
			int t = Clusters[ClusterID].MigrationHistory.size();
			MkExec(Load, { Cint,&t });
		}
		break;
	case 92: // NAllClusterMovesOut Выдать общее количетство итераций миграции кластеров
	case 93: // NAllClusterMovesOutMk Выдать МК с общими количетством итераций миграции кластеров 
	case 94: // NAverageClusterMovesOut Выдать среднее количетство итераций миграции кластера
	case 95: // NAverageClusterMovesOutMk Выдать МК со средним количетством итераций миграции кластера
	{
		int t = 0;
		for (auto i : Clusters)
			t += i.MigrationHistory.size();
		if(MK==92)
			Load.Write(t);
		else if(MK==93)
			MkExec(Load, { Cint,&t });
		else
		{
			double tt = (double)t / Clusters.size();
			if (MK == 94)
				Load.Write(tt);
			else
				MkExec(Load, { Cdouble,&tt });
		}
		break;
	}
	case 120: // NClusterPointsOut Выдать количество точек в кластере
		if (ClusterID >= 0 && ClusterID <= Clusters.size())
			Load.Write(Clusters[ClusterID].NPoints);
		break;
	case 121: // NClusterPointsOut Выдать МК с количеством точек в кластере
		if (ClusterID >= 0 && ClusterID <= Clusters.size())
			MkExec(Load, { Cint,&Clusters[ClusterID].NPoints});
		break;
	case 125: // ClusterMassOut Выдать общую массу точек в кластере
		if (ClusterID >= 0 && ClusterID <= Clusters.size())
			Load.Write(Clusters[ClusterID].Mass);
		break;
	case 126: // ClusterMassOut Выдать МК общей массой точек кластере
		if (ClusterID >= 0 && ClusterID <= Clusters.size())
			MkExec(Load, { Cdouble,&Clusters[ClusterID].Mass });
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
}

void MeanShift::Start() // Запуск кластеризации
{
}

void MeanShift::PointsGen()
{
	for (auto& i : VXY) // Очистка предыдувщего поля
	{
		for (auto& j : i)
			delete j;
	}
	VXY.clear();
	VXY.resize(NDim);
	for (int i = 0; i < NProb; i++)
	{
		MeanShiftPoint* t = new MeanShiftPoint;
		t->Coodinate.resize(NDim);
		t->Manager = this;
		t->eps = eps;
		t->NV = NV;
		t->ID = i; // Установить индетификатор ФУ-исполнителя
		for (auto& l : t->Coodinate) // Генерация точек
			l = (double) rand() / RAND_MAX * (ProbMaxMin[l][1] - ProbMaxMin[l][0]) + ProbMaxMin[l][0];
		for (auto& k : VXY)
			k.push_back(t); // Запись указателей на исполнительные ФУ в вектора всех изменений
	}
	// Упорядочивание точек по каждой из координат
	int i = 0;
	for (auto& k : VXY)
	{
		sort(k.begin(), k.end(), [i](MeanShiftPoint* a, MeanShiftPoint* b) {return a->Coodinate[i] < b->Coodinate[i]; });
		i++;
	}
	// Расстановка ссылок в точках на их описание в VX,VY
	for (auto& k : VXY) {// Перебор всех точек
		{
			int i = 0;
			for (auto& uFU : k)
			{
				uFU->refXY.push_back(&uFU);
				uFU->IdXY.push_back(i); // Запись индекса ФУ по каждому измерению
				i++;
			}
		}
	}

}

void MeanShift::FileRead(LoadPoint Load)
{
	ifstream f; // файл для считывания исходных данных
	string FName;
	ifstream fin;
	f.open(Load.toStr());
	if (!f) return;
	vector<pair<double, MeanShiftPoint*>> VXt, VYt;
	while (!f.eof())
	{
		double x, y;
		f >> x >> y;
		MeanShiftPoint* MP = new MeanShiftPoint();
		VXt.push_back({ x, MP });
		VYt.push_back({ y, MP });
	//	MP->epsX[0] = epsX;
	//	MP->epsY[0] = epsY;
		MP->NV = NV;
		MP->Manager = this;
		MP->Coodinate.push_back(x);
		MP->Coodinate.push_back(y);
	//	MP->VX = &VX; // Ссылка на список упорядоченных вершин по координате X
	//	MP->VY = &VY; // Ссылка на список упорядоченных вершин по координате Y
	}
	// Упорядочивание векторов VXt, VYt
	sort(VXt.begin(), VXt.end());
	sort(VYt.begin(), VYt.end());
	// Формирование векторов VX, VY
	for (auto uk = VXt.begin(); uk != VXt.end(); uk++)
		VXY[0].push_back(uk->second);
	for (auto uk = VYt.begin(); uk != VYt.end(); uk++)
		VXY[1].push_back(uk->second);

}

void  MeanShift::NetGen() // Генерация сетки
{
	// Непосредственно генерация сетки
	// Определение множества близлежащих точек для построения сетки (множество N)
	if (NV > VXY[0].size()) // Если количество точек в системе меньше,
	{						// чем требуемое для окрестности, то построение сетки прерывается
		ProgExec(NVPointErrProg); // Вызов подрограммы сообщения об ошибке
		return;
	}
	for (auto FUuk : VXY[0])
		FUuk->ProgFU(100, { 0,nullptr });// Милликоманда генерации близлежащей сетки
	for (auto FUuk : VXY[0])
		FUuk->ProgFU(101, { 0,nullptr });// Милликоманда дополнения связей
//	for (auto FUuk : VXY[0])
//		FUuk->ProgFU(102, { 0,nullptr });// Милликоманда коррекции связей сетки
}
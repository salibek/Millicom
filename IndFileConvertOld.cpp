// Чтение индексного файла старого формата
#include "stdafx.h"
#include "IndFileConvertOld.h"
#include "consts.h"
#include <map>
#include <vector>
#include <array>
#include <string>
#include <string.h>
#include <fstream>
#include <algorithm>

using namespace std;

int i;

vector<vector<void*>> BackRec; // вектор адресов для обратной записи адреса
vector<int> BackRecInd; // вектор индексов для обратной записи адреса

void ConvIndStrOld(string str, ip &IP, vector<ip*> IpUk, map<int, vector<ip>*> &CapsId, vector<pair<int, int>> IpId, ICVect* ipVect, int Count) // Возвращает номер следующей ИК
{
	IP.atr = atoi(str.substr(0, str.find(' ')).c_str());
	str = str.substr(str.find(' ') + 1, string::npos);
	char DataType = str[0];
	str = str.substr(2, string::npos);

	if (DataType != 'S' && DataType != 'C')
	{
		int DInd = atoi(str.substr(str.find(' ') + 1, str.find(' ', str.find(' ') + 1)).c_str());// Индекс для указателя
		if (DInd != (Count << 2) + 2)
		{
			switch (DInd % 4)
			{
			case 0:// Указатель на ИП
				if (CapsId.count(DInd >> 2) == 0)
				{
					IP.Load = { TICInd, ipVect->at(IpId[DInd >> 2].first), IpId[DInd >> 2].second * 3};
/*					int t;
					for (t = DInd >> 2; !CapsId.count(t); t--);
					IP.Load.Point = CapsId[t];
					IP.Load.Type = TICInd;
					IP.Load.Ind = DInd >> 2; */
				}
				else
				{
					IP.Load.Point = CapsId[DInd >> 2];
					IP.Load.Type = TIC;
					IP.Load.Ind = -1;
				}
				break;
			case 1:// Указатель на атрибут
//				IP.Load.Point = &(IpUk[DInd >> 2]->atr);
//				IP.Load.Type = Tint;
			{
				IP.Load = { TICInd, ipVect->at(IpId[DInd >> 2].first), IpId[DInd >> 2].second * 3 + 1 };
/*				int t;
				for (t = DInd >> 2; !CapsId.count(t); t--);
				IP.Load.Point = CapsId[t];
				IP.Load.Type = TICInd;
				IP.Load.Ind = ((DInd >> 2) - t) * 3 + 1;
//				IP.Load.Ind = (((DInd >> 2) - t) << 2) + 1;
*/
			}
			break;
			case 2:// указатель на данные
//				IP.Load.Point = IpUk[DInd >> 2]->Load.Point;
//				IP.Load.Type = IpUk[DInd >> 2]->Load.Type;
//			break;
			case 3:// Указатель на указатель из ИП
			//	IP.Load.Point = &(IpUk[DInd >> 2]->Load);
			//	IP.Load.Type = TLoad;
				{
				IP.Load = { TICInd, ipVect->at(IpId[DInd >> 2].first), IpId[DInd >> 2].second * 3 + 2 };
				/*
					int t;
					for (t = DInd >> 2; !CapsId.count(t); t--);
					IP.Load.Point = CapsId[t];
					IP.Load.Type = TICInd;
					IP.Load.Ind = ((DInd >> 2) - t) * 3 + 2;
				//	IP.Load.Ind = (((DInd >> 2) - t) << 2) + 2;
				*/
				}
				break;
			}
			return;
		}
	}
	string *Strdat;
	int *intVar;
	double *doubleVar;
	float *floatVar;
	char *charVar;
	bool *boolVar;
	switch (DataType)
	{
	case 'S':
	case 'C':
		Strdat = new string(str.substr(0, str.rfind('"')));
		str = str.substr(str.rfind('"') + 1, string::npos);
		IP.Load.Point = Strdat;
		IP.Load.Type = Cstring;
		break;
	case 'I':
		intVar = new int;
		*intVar = atoi(str.substr(0, str.find(' ')).c_str());
		IP.Load.Point = intVar;
		IP.Load.Type = Cint;
		break;
		//		case 'C':
		//			charVar = new char;
		//			*charVar = str[2];
		//			IP.Load.Point = charVar;
		//			IP.Load.Type = Cchar;
		//			break;
	case 'B':
		boolVar = new bool;
		*boolVar = str[0]=='T' ? true : false;
		IP.Load.Point = boolVar;
		IP.Load.Type = Cbool;
		break;
	case 'F':
	case 'D':
	{
		doubleVar = new double;
		string buf = str.substr(0, str.find(' '));
		if (buf.find(',') < buf.length())
			buf[buf.find(',')] = '.';
		*doubleVar = atof(buf.c_str());
		IP.Load.Point = doubleVar;
		IP.Load.Type = Cdouble;
		break;
	}
//	case 'F':
//		floatVar = new float;
//		*floatVar = atof(str.substr(0, str.find(' ')).c_str());
//		IP.Load.Point = floatVar;
//		IP.Load.Type = Cfloat;
//		break;
	}
}

ICVect* ConvIndOld(string FileName)
{
	ICVect *icVect = new ICVect;
	ifstream in(FileName);
	map<int, vector<ip>* > CapsId; // Номера ИП, с которых начинается ИК
	vector<pair<int, int>> IpId; // Номера ИП в ИК
	if (!in) { cout << "File is not found\n"; return icVect; }
	int N;
	in >> N;
	vector<ip*> IpUk; // Указатели на ИП
	vector<ip*>::iterator IpUkIterat;
	IpUk.resize(N);
	string str;// Текущая вводимая строка из файла

			   // Формирование вектора ОА-графа (выделение памяти под ИК)
	int Count = 0; // Счетчик количества ИП в капсуле
	vector<int> LIC; // Длины капсул из ОА-графа
	vector<int> NIC; // Индексы начала капсул из ОА-графа
	int LIC_Current = 0; // Верхушка стека длин капсул ОА-графа
	map <int, int> LIC_Ind; // Индекс, с которого продолжается капсула ОА-графа
	LIC.push_back(0);
	NIC.push_back(0);
	LIC_Ind[N + 3] = 0;
	getline(in, str);
	for (int i = 0; i < N; i++)
	{
		getline(in, str);
		str = str.substr(0, str.rfind(' '));

		IpId.push_back({ LIC_Current, LIC[LIC_Current] }); // Запомнить номер ИК и индекс ИП
		LIC[LIC_Current]++;
		if (str.substr(str.rfind(' ') + 1, string::npos) == "-1") // Конец ИК?
			if (LIC_Ind.begin()->first == i + 1)
			{ // Конец разрыва ИК
				LIC_Current = LIC_Ind.begin()->second;
				LIC_Ind.erase(LIC_Ind.begin()->first);
			}
			else
			{
				//				if (i < N - 1)
				{// Новыя ИК
					LIC_Current = LIC.size();
					LIC.push_back(0);
					NIC.push_back(i + 1);
				}
			}
		else if (atoi(str.substr(str.rfind(' ') + 1, string::npos).c_str()) >> 2 != i + 1) // Разрыв ИК?
			if (LIC_Ind.begin()->first == i + 1)
			{ // Конец разрыва ИК
				int t = LIC_Current;
				LIC_Current = LIC_Ind.begin()->second;
				LIC_Ind.erase(LIC_Ind.begin()->first);
				LIC_Ind[atoi(str.substr(str.rfind(' ') + 1, string::npos).c_str()) >> 2] = t;
			}
			else
			{
				LIC_Ind[atoi(str.substr(str.rfind(' ') + 1, string::npos).c_str()) >> 2] = LIC_Current; // Индекс, с которого продолжается капсула ОА-графа
				LIC_Current = LIC.size();
				LIC.push_back(0);
				NIC.push_back(i + 1);
			}
	}
	LIC.pop_back();
	NIC.pop_back();
	icVect->resize(LIC.size());

	for (int i = 0; i < NIC.size(); i++)// Формирование таблицы индентификаторов ИК
										// и списка ИК
	{
		(*icVect)[i] = new vector<ip>();
		(*icVect)[i]->resize(LIC[i]);
		CapsId[NIC[i]] = (icVect->at(i));
	}
	// Формирование списка ссыллок на ИП
	LIC_Current = 0;
	LIC.clear();  LIC.push_back(0);
	//	NIC.clear();  NIC.push_back(0);
	LIC_Ind.clear();
	LIC_Ind[N + 3] = 0;

	in.seekg(0, ios::beg); // переход в начало файла
	getline(in, str);
	int ICNUM = 0, IPNUM = 0; // Текущий номер ИК
	int IC_Count = 0;
	auto _IpUk = IpUk.begin();
	for (int i = 0; i < N; i++)
	{
		getline(in, str);
		str = str.substr(0, str.rfind(' '));
		*_IpUk = &(*icVect->at(LIC_Current))[LIC[LIC_Current]];
		LIC[LIC_Current]++;
		if (str.substr(str.rfind(' ') + 1, string::npos) == "-1") // Конец ИК?
			if (LIC_Ind.begin()->first == i + 1)
			{ // Конец разрыва ИК
				LIC_Current = LIC_Ind.begin()->second;
				LIC_Ind.erase(LIC_Ind.begin()->first);
			}
			else
			{
				//				if (i < N - 1)
				{// Новыя ИК
					LIC_Current = LIC.size();
					LIC.push_back(0);
					//					NIC.push_back(i + 1);
				}
			}
		else if (atoi(str.substr(str.rfind(' ') + 1, string::npos).c_str()) >> 2 != i + 1) // Разрыв ИК?
			if (LIC_Ind.begin()->first == i + 1)
			{ // Конец разрыва ИК
				int t = LIC_Current;
				LIC_Current = LIC_Ind.begin()->second;
				LIC_Ind.erase(LIC_Ind.begin()->first);
				LIC_Ind[atoi(str.substr(str.rfind(' ') + 1, string::npos).c_str()) >> 2] = t;
			}
			else
			{
				LIC_Ind[atoi(str.substr(str.rfind(' ') + 1, string::npos).c_str()) >> 2] = LIC_Current; // Индекс, с которого продолжается капсула ОА-графа
				LIC_Current = LIC.size();
				LIC.push_back(0);
				//				NIC.push_back(i + 1);
			}
		_IpUk++;
	}
	LIC.pop_back();
	//	NIC.pop_back();
	icVect->resize(LIC.size());

	in.seekg(0, ios::beg); // переход в начало файла
	getline(in, str);
	int t = 0;
	for (auto &i : IpUk)
	{
		getline(in, str);
		ConvIndStrOld(str, *i, IpUk, CapsId, IpId, icVect, t++);
	}

	IpUk.clear();
	NIC.clear();
	LIC.clear();

	in.close();
	return icVect;
}
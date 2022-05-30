// StrGen.cpp: определяет точку входа для консольного приложения.

#include "stdafx.h"
#include "Consts.h"
#include "Bus.h"
#include "Console.h"
#include "StrGen.h"
#include "Lex.h"
#include <iostream>
#include <string>
#include <fstream>
#include "MeanShift.h"
#include "Bag.h"
#include "ALU.h"

using namespace std;

int main()
{
	LoadPoint LP = LoadPoint();
	BusFU Bus;

//	int t = 10;

		/*
	Console Cons; // Консоль
	ALU* alu=new ALU(&Cons);
	int MK = 2;
	string STR = "NetTemperat.ind";
	string out;
	alu->ProgFU(26, { Cstring,&STR });
	alu->ProgFU(2, { Cint,&MK });
	alu->ProgFU(1, { Cstring,&out });
	cout << out << endl;

		double x = 10;
	int Mk = 2; // МК вывода для консоли
	alu->ProgFU(26, { Tdouble,&x }); //MK Set
	x = 5;
	alu->ProgFU(33, { Tdouble,&x });// Тестируемая МК
	alu->ProgFU(2, { Cint,&Mk }); // Вывод результата

	Console Cons; // Консоль
	Lex *Lexica=new Lex((FU*) & Cons); // ФУ лексического разбора
	Lexica->Bus = &Cons;
	Lexica->ProgFU(0, { 0,nullptr });
	Lexica->ReceiverMK = 2;
	string t = "\"22\"";
//	Lexica->ProgFU(100, { Tstring,&t });
	t = "for{";
	Lexica->ProgFU(100, { Tstring,&t });
	t = "\t\"22\"";
	Lexica->ProgFU(100, { Tstring,&t });
	t = "for";
	Lexica->ProgFU(100, { Tstring,&t });
*/

//	ALU.ProgFU(26, { Cint,&t });
//	ALU.ProgFU(30, { Cint,&t });

//	string STR = "NetTemperat.ind";
//	ALU.ProgFU(26, { Cstring,&STR });
	
//	string STR = "MeanShift.txt";
//	string STR = "MeanShift.ind";
//	string STR = "ProgTest.ind";
//	Bus.ProgFU(10, {Cstring,&STR});
		//	string STR = "FSM.ind";
	//	STR = "ALE.ind";
//	LP.Type = Tstring;
//	LP.Point = &STR;

	//MSh.ProgFU(5, LP);
//	Bus.ProgFU(10, LP);

//	string STR = "Gauss.ind";
//	string STR = "ALE3.ind";
//	string STR = "MeanShift.ind";
//	string STR = "Bag.ind";
//	string STR = "NetTemperat2.ind";
//	string STR = "CompileC.ind";
//	string STR = "OAGraphOut.ind";
//	string STR = "MultyList.ind";
//	string STR = "RusVoc.ind";
	string STR = "LexTest.ind"; // Имя запускного файла

	Bus.ProgFU(10, {Cstring, &STR}); //Запуск индексного файла

//	string STR5 = "Bag5.txt"; // 5 предметов
//	string STR6 = "Bag6.txt"; // 6 предметов
//	B.ProgFU(1, { Cstring, &STR5 });
//	B.ProgFU(10, { Cint, nullptr });

	//	int temp = 8; // Тип ФУ 8 - ФУ InOut
	//	Bus.ProgFU(1, {Cint,&temp});
//	string STR = "Gauss.txt";
//	FU* G=GaussIni(nullptr,nullptr);
//	G->ProgFU(1, { Cstring,&STR });
//	STR = "Bag.txt";
//	B.ProgFU(1, {Cstring, &STR});
	system("pause");
	return 0;
}
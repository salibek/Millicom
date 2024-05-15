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
#include "ALU.h"
#include "StreamFloatALU.h"



using namespace std;

int main(int argc, char* argv[])

	{
	system("chcp 1251");
	BusFU Bus;
	string STR;
	StreamFloatALU ALU;

	//ALU.ProgFU(0, { 0, nullptr });
	/*
	* 
	if (argc == 1)
	{
		cout << "File name is not found in comand line\n";
		system("pause");
		return 0;
	}
	ifstream f(argv[1]);
	if (!f)
	{
		cout << "File is not found\n";
		system("pause");
		return 0;
	}
	f.close();
	LoadPoint LP = LoadPoint();
	STR = argv[1];
	Bus.ProgFU(10, { Cstring, &STR }); //Запуск индексного файла
	return 0;
//	int t = 10;
*/


//	string STR = "NetTemperat.ind";
	
//	string STR = "MeanShift.txt";
//	string STR = "MeanShift.ind";
//	string STR = "ProgTest.ind";
	//	string STR = "FSM.ind";
	//	STR = "ALE.ind";

//	string STR = "Gauss.ind";
//	string STR = "ALE3.ind";
//	string STR = "MeanShift.ind";
//	string STR = "NetTemperat2.ind";
//	STR = "CompileC.ind";
//	STR = "Router.ind";
//	string STR = "OAGraphOut.ind";
//	string STR = "MultyList.ind";
//	STR = "CompLab.ind";
//	STR = "AlU_test.ind";
//	string STR = "TabTest.ind";
//	string STR = "LexTest.ind"; // Имя запускного файла
//	STR = "Bag3.ind";

//	STR = "StreamFloatALU.ind";

	//	STR = "Bag4.ind";
//	STR = "Termo.ind";
	//	string STR = "ControlAoutomat.ind";

//	STR = "StreamFloatALUTest.ind";
//	STR = "ALUCellular.ind";
//	STR = "StreamAluTest.ind";
//	STR = "StreamIntALUTest.ind";
	STR = "ALE_Stream_v2.ind";
	Bus.ProgFU(10, {Cstring, &STR}); //Запуск индексного файла

	system("pause");
	return 0;
}
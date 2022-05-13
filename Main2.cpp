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

using namespace std;

int main()
{
	Console *cons = new Console();
	StrGenContext *StrGen = new StrGenContext();
/*
	StrGen->ReceiverMK = 2;
	StrGen->Receiver = cons;
	StrGen->Bus = cons;
	cons->Bus = StrGen;
	LoadPoint Point;	
	
	Point.Type = 3;
	Point.Point = static_cast<void*> (new std::string("in.txt"));
//	StrGen->ProgFU(3, Point);
*/
	string STR = "sfsdf";
	Lex *lex = new Lex();
	LoadPoint LP;
	LP.Type = TIP;
	ip IP;
	LP.Point = &IP;
	IP.Load.Point = &STR;
	IP.Load.Type = Tstring;

	lex->ProgFU(0, LP);
	BusFU Bus;
	
//	STR = "Lex.txt";
	STR = "ProgNew4.ind";
	LP.Type = Tstring;
	LP.Point = &STR;
	Bus.ProgFU(10, LP);
	system("pause");
	return 0;
}

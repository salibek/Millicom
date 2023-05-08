#pragma once
#include "Consts.h"
#include <set>
#include <map>
#include <vector>

struct MkFu
{
	int Mk;
	FU* Fu;
};

class Lex : public FU {
private:
	void LexOut(bool Copy=false, int MK = -1); // Подпрограмма выдачи лексемы
	string ABC_templ = "_QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm"; // Алфавит символов
	set<string> Seps_templ = { "//=", "!!=", "&&=", "||=", "&&=", "^^=",\
	"^=", "*=", "%=", "/=", "//", "++", "--", "-=","+=","<>", ">=", "<=", "!!", "!=", "||", "|=", "&&", "&=", "^^", "^=",\
	"<", ">", "~", "`", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "+", "=", "-", "{", "}", "[", "]", "?", ";", ":", ",", ".", "\\", "/", "|", "\""};// Алфавит разделителей
	set<char> ABC, Digit;
	set<string> Seps;
	string Mul = "*"; // Знак умножения, который вставлеяется между числом и мнеоникой ( 10x <=> 10*x)
	IC_type StartProg = nullptr, StopProg = nullptr, FinProg = nullptr; // Программы, выполняемые при запуске процесса лексического анализа
	IC_type ErrProg = nullptr; // Программа обработки ошибки
	char FloatPoint = '.'; // Разделитель целой и дробной частей числа
	char HintChar = '_'; // Разделитель разрядов числа
	vector<string>True = { "true","True" }; // Мнемоники булевых значений
	vector<string>False = { "false","False" };
	string ProgBbracket = "}"; // Закрывающаяся программная скобка
	int S = 0; // Номер состояния распознающего автомата
	bool Work = true; // Флаг рабочего режима лексера
	int MnemoAtr = -2, SeperatAtr = -4, IntAtr = -10, DoubleAtr = -3, BoolAtr = -5, StrAtr = -6, CharAtr = -11; // Атрибуты лексем
	int ib = 0, SizeBuf = 5; // Текущая позиция выходной ИП выходной лексемы и размер буфера
	ip* LexBuf; // Буфер выходных лексем
//	vector<ip> *ErrProg=nullptr; // Программа, запускаемая при ошибке
	map<int, MkFu> UnicAtr; // Список специфических атрибутов, по которым идет обработки другими ФУ
	// Буфер описаний лексем {"dsfsdf",-3,"dfsdfs",""}
	FU* Receiver = nullptr;
public:
	int ReceiverMK = 0; // Милликоманды для приемника лексемы
	void ProgFU(int MK, LoadPoint Load);
	Lex(FU *BusContext, FU *Templ=nullptr);
	Lex();
};
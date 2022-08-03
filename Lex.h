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
	void LexOut(bool Copy=false, int MK = -1);
	set<int> FinMk; // Множество индексов МК для потребителя, характерных для финального состояния
	string ABC_templ = "_QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm"; // Алфавит символов
	string Digit_templ = "0123456789"; // --- Алфавит чисел
	string Digit_seps_templ = "."; // --- Алфавит разделителей чисел
	set<string> Seps_templ = { "//=", "!!=", "&&=", "||=", "&&=", "^^=",\
	"^=", "*=", "%=", "/=", "//", "++", "--", "-=","+=","<>", ">=", "<=", "!!", "!=", "||", "|=", "&&", "&=", "^^", "^=",\
	"<", ">", "~", "`", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "+", "=", "-", "{", "}", "[", "]", "?", ";", ":", ",", ".", "\\", "/", "|", "\""};// Алфавит разделителей
	set<char> ABC, Digit, DigitSeps; // --- DigitSeps - множество, хранящее разделители целой и дробной части чисел
	set<string> Seps;
	IC_type StartProg = nullptr, StopProg = nullptr, FinProg = nullptr; // Программы, выполняемые при запуске процесса лексического анализа
	string EOL = "\n"; // Символы окончания строки
	set<string> TrueConst = { "true","True" }; // Константы true
	set<string> FalseConst = { "false","False" }; // Константы false
	int ProgLevel = 0; // Уровень скобочной конструкции
	char ProgBracket = '{', ProgFinBracket = '}'; // Символы программных скобкок
	void* TabErrProg = nullptr; // программа обработки ошибки табуляции
public:
	void ProgFU(int MK, LoadPoint Load);
	Lex(FU *BusContext=nullptr, FU *Templ=nullptr);
	Lex() ;
	FU *Receiver = nullptr;
	int ReceiverMK = 0;
	int S = 0; // Номер состояния распознающего автомата
	bool Work = true; // Флаг рабочего режима лексера
	int MnemoAtr = -2, SeperatAtr=-4, IntAtr=-10, DoubleAtr=-3, BoolAtr=-5, StrAtr=-6, ConstAtr=-13;
	int ib = 0, SizeBuf = 5; // Текущая позиция выходной ИП выходной лексемы и размер буфера
	ip *LexBuf; // Буфер выходных лексем
	void *ErrProg=nullptr; // Программа, запускаемая при ошибке
	map<int,MkFu> UnicAtr; // Список специфических атрибутов, по которым идет обработки другими ФУ
	// Буфер описаний лексем {"dsfsdf",-3,"dfsdfs",""}
//	void Debug(char i, int S, string FigureBuf); // --- для отладки, позже удалить
};
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

struct RegProg // Парочка регулярного выражения и указателя на обработчика этой выделенной лексемы
{
	string Reg;
	void* Prog = nullptr;
};

class Lex : public FU {
private:
	void LexOut(bool Copy=false, int MK = -1);
	vector<RegProg> RegVect; // Вектор регулярных выражений для распознания лексем
	string LexAccum; // Буфер для хранения выделенной лексемы
	set<int> FinMk; // Множество индексов МК для потребителя, характерных для финального состояния
	string ABC_templ = "_QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm"; // Алфавит символов
	string Digit_templ = "0123456789"; // --- Алфавит чисел
	string DigitOCT_templ = "01234567"; // --- Алфавит чисел
	string DigitHEX_templ= "0123456789ABCDEF"; // --- Алфавит чисел
	string Digit_seps_templ = "."; // --- Алфавит разделителей чисел
	set<string> Seps_templ = { "//=", "!!=", "&&=", "||=", "&&=", "^^=",\
	"^=", "*=", "%=", "/=", "//", "++", "--", "-=","+=","<>", ">=", "<=", "!!", "!=", "||", "|=", "&&", "&=", "^^", "^=",\
	"<", ">", "~", "`", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "+", "=", "-", "{", "}", "[", "]", "?", ";", ":", ",", ".", "\\", "/", "|", "\""};// Алфавит разделителей
	set<char> ABC, Digit, DigitHEX, DigitOCT, DigitBIN, DigitSeps; // --- DigitSeps - множество, хранящее разделители целой и дробной части чисел
	set<string> Seps;
	IC_type StartProg = nullptr, StopProg = nullptr, FinProg = nullptr; // Программы, выполняемые при запуске процесса лексического анализа
	string EOL = "\n"; // Символы окончания строки
	set<string> TrueConst = { "true","True" }; // Константы true
	set<string> FalseConst = { "false","False" }; // Константы false
	int ProgLevel = 0; // Уровень скобочной конструкции
	string ProgBracket = "{", ProgFinBracket = "}"; // Символы программных скобкок
	void* TabErrProg = nullptr; // программа обработки ошибки табуляции
	void* ErrProg = nullptr; // Программа, запускаемая при ошибке
	bool TabMode = false; // Режим расстановки скобок по знакам табуляции
public:
	void ProgFU(int MK, LoadPoint Load);
	Lex(FU *BusContext=nullptr, FU *Templ=nullptr);
	Lex() ;
	vector<FU*> Receiver = { nullptr }; // Стек указаталей не ФУ-приемники
	vector<int> ReceiverMK = { 0 }; // Стек МК для приемников
	int S = 0; // Номер состояния распознающего автомата
	bool Work = true; // Флаг рабочего режима лексера
//	int MnemoAtr = -2, SeperatAtr = -4, IntAtr = -10, DoubleAtr = -3, BoolAtr = -5, StrAtr = -6, ConstAtr = -13;
	int MnemoAtr = -2, SeperatAtr = -4, IntAtr = -13, DoubleAtr = -13, BoolAtr = -13, StrAtr = -13, ConstAtr = -13;
	int ib = 0, SizeBuf = 5; // Текущая позиция выходной ИП выходной лексемы и размер буфера
	ip *LexBuf; // Буфер выходных лексем
	map<int,MkFu> UnicAtr; // Список специфических атрибутов, по которым идет обработки другими ФУ
	int LastUnicAtr = 0; // Последний уникальный атрибут (чтобы можно было потом добавить к нему МК и адрес ФУ)
	// Буфер описаний лексем {"dsfsdf",-3,"dfsdfs",""}
//	void Debug(char i, int S, string FigureBuf); // --- для отладки, позже удалить
};
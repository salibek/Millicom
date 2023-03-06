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

struct RegProg // ������� ����������� ��������� � ��������� �� ����������� ���� ���������� �������
{
	string Reg;
	void* Prog = nullptr;
};

class Lex : public FU {
private:
	void LexOut(bool Copy=false, int MK = -1);
	vector<RegProg> RegVect; // ������ ���������� ��������� ��� ����������� ������
	string LexAccum; // ����� ��� �������� ���������� �������
	set<int> FinMk; // ��������� �������� �� ��� �����������, ����������� ��� ���������� ���������
	string ABC_templ = "_QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm"; // ������� ��������
	string Digit_templ = "0123456789"; // --- ������� �����
	string DigitOCT_templ = "01234567"; // --- ������� �����
	string DigitHEX_templ= "0123456789ABCDEF"; // --- ������� �����
	string Digit_seps_templ = "."; // --- ������� ������������ �����
	set<string> Seps_templ = { "//=", "!!=", "&&=", "||=", "&&=", "^^=",\
	"^=", "*=", "%=", "/=", "//", "++", "--", "-=","+=","<>", ">=", "<=", "!!", "!=", "||", "|=", "&&", "&=", "^^", "^=",\
	"<", ">", "~", "`", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "+", "=", "-", "{", "}", "[", "]", "?", ";", ":", ",", ".", "\\", "/", "|", "\""};// ������� ������������
	set<char> ABC, Digit, DigitHEX, DigitOCT, DigitBIN, DigitSeps; // --- DigitSeps - ���������, �������� ����������� ����� � ������� ����� �����
	set<string> Seps;
	IC_type StartProg = nullptr, StopProg = nullptr, FinProg = nullptr; // ���������, ����������� ��� ������� �������� ������������ �������
	string EOL = "\n"; // ������� ��������� ������
	set<string> TrueConst = { "true","True" }; // ��������� true
	set<string> FalseConst = { "false","False" }; // ��������� false
	int ProgLevel = 0; // ������� ��������� �����������
	string ProgBracket = "{", ProgFinBracket = "}"; // ������� ����������� �������
	void* TabErrProg = nullptr; // ��������� ��������� ������ ���������
	void* ErrProg = nullptr; // ���������, ����������� ��� ������
	bool TabMode = false; // ����� ����������� ������ �� ������ ���������
public:
	void ProgFU(int MK, LoadPoint Load);
	Lex(FU *BusContext=nullptr, FU *Templ=nullptr);
	Lex() ;
	vector<FU*> Receiver = { nullptr }; // ���� ���������� �� ��-���������
	vector<int> ReceiverMK = { 0 }; // ���� �� ��� ����������
	int S = 0; // ����� ��������� ������������� ��������
	bool Work = true; // ���� �������� ������ �������
//	int MnemoAtr = -2, SeperatAtr = -4, IntAtr = -10, DoubleAtr = -3, BoolAtr = -5, StrAtr = -6, ConstAtr = -13;
	int MnemoAtr = -2, SeperatAtr = -4, IntAtr = -13, DoubleAtr = -13, BoolAtr = -13, StrAtr = -13, ConstAtr = -13;
	int ib = 0, SizeBuf = 5; // ������� ������� �������� �� �������� ������� � ������ ������
	ip *LexBuf; // ����� �������� ������
	map<int,MkFu> UnicAtr; // ������ ������������� ���������, �� ������� ���� ��������� ������� ��
	int LastUnicAtr = 0; // ��������� ���������� ������� (����� ����� ���� ����� �������� � ���� �� � ����� ��)
	// ����� �������� ������ {"dsfsdf",-3,"dfsdfs",""}
//	void Debug(char i, int S, string FigureBuf); // --- ��� �������, ����� �������
};
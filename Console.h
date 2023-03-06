// ФУ Консоль
#pragma once
#include "Consts.h"
#include <fstream>
#include <string>
#include <map>

class MkRef { // Парочка Мк и ссылка для записи результата
public:
	int Mk = -1;
	LoadPoint Ref = {0,nullptr};
};

class Console : public FU
{
private:
	map<int, string> AtrMnemo; // Атрибуты мнемоник
	string prefix="";
	string filename = "";
	ofstream fout; // ??
	ofstream fin; // ??
	FILE* streamOut, *streamIn; // Входной/выходной файлы
	string Sep = " ", VectSep=",", End = "\n"; // Строка-разделитель и строка в конце вывода
	string quote = ""; // Символ кавычек при выводе строки
	string FloatPoint = "."; // Разделитель дробной части
	string ArrayBracketStart = "[", ArrayBracketFin = "]"; // Начальная и конечная скобки для вывода вектора
	vector<string> TrueVar = { "true","True" }; // Вектор значений "правда"
	vector<string> FalseVar = { "false","False" }; // Вектор значений "ложь"
//	vector<LoadPoint> *NewVect; // Указатель на вектор
	void* NoVarToOutProg = nullptr; // Подрограммы реакции на ошибку "Нет переменной для вывода"
	void* WrongFormatProg = nullptr; // Подрограммы реакции на ошибку "Неправильный формат"
	void* WrongVectFormatProg = nullptr; // Подрограммы реакции на ошибку "Неправильный формат"
	LoadPoint Var = {0,nullptr};
	void* WrongInterFormat = nullptr; // Подпрограмма обработки ошибки 'Неправильный формат введенных данных'
	vector<MkRef> VarOutBuf; // Вектора Мк и указателей для вывода результата
	set<string> True = { "true", "True" }, False = {"false", "False"}; // Множество значений true false
	string inStr; // Последняя введенная строка
	void* InputFormatErrProg = nullptr; // Программа ошибки формата входных данных
public:
	void ProgFU(int MK, LoadPoint Load) override;
	Console(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; FUtype = 1; };
	Console() : FU() { Bus = nullptr; };
private:
};

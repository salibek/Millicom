// ФУ Консоль
#pragma once
#include "Consts.h"
#include <fstream>
#include <string>
#include <map>
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
	string FloatPoint = "."; // Разделитель дробной части
	string ArrayBracketStart = "[", ArrayBracketFin = "]"; // Начальная и конечная скобки для вывода вектора
	vector<LoadPoint>VarBuf; // Буфер переменных
	vector<int>MkOutBuf; // Буфер МК для выдачи результатов
	LoadPoint NewVar = { 0,nullptr }; // Указатель на новую переменную
	vector<string> TrueVar = { "true","True" }; // Вектор значений "правда"
	vector<string> FalseVar = { "false","False" }; // Вектор значений "ложь"
//	vector<LoadPoint> *NewVect; // Указатель на вектор
	void* NoVarToOutProg = nullptr; // Подрограммы реакции на ошибку "Нет переменной для вывода"
	void* WrongFormatProg = nullptr; // Подрограммы реакции на ошибку "Неправильный формат"
	void* WrongVectFormatProg = nullptr; // Подрограммы реакции на ошибку "Неправильный формат"
public:
	void ProgFU(int MK, LoadPoint Load) override;
	Console(FU* BusContext, FU* Templ) : FU(BusContext) { Bus = BusContext; FUtype = 1; };
	Console() : FU() { Bus = nullptr; };
private:
};

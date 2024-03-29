#pragma once
#include "stdafx.h"
#include "Consts.h"
#include <functional>

// Контекст АЛУ
class ALUContext
{
public:
	vector<LoadPoint> accumVect; // Векторный аккумулятор
	unsigned int accumType = 0; // Тип данных
	double		accum = 0;		// Скалярный аккумулятор
	string		accumStr;		// строковой аккулятор
	int Ind = 0; // Индекс элемента вектора
	bool IndF = false; // флаг установки индекса
	void* ConfineStart = nullptr, *ConfineExpression = nullptr, *ConfineBorder = nullptr, *ConfineInc = nullptr; // Конфайн
	// TLoadArray - тип нагрузка, обозначающий вектор нагрузок
//		vector<LoadPoint>* accumPoint = nullptr;	// Указатель на аккумулятор (вектор, матрица и т.п.)
	vector<int> MkOut; // вектор МК для резульатов
	vector<LoadPoint> OutAdr; // вектор адресов для записи результатов
};

class ALU : FU
{
	void		sub(LoadPoint load);
	void		div(LoadPoint load);
	void		div_int(LoadPoint load);
	void		mult(LoadPoint load);
	void		inc(LoadPoint load);
	void		dec(LoadPoint load);
public:
	unsigned int accumType = 0; // Тип данных
	double		accum = 0;		// Скалярный выходной аккумулятор (из него другие ФУ могут считать значение аккумулятора по ссылке)
	string		accumStr;		// строковой аккулятор
	void* accumPoint = nullptr;	// Указатель на аккумулятор (вектор, матрица и т.п.)
	vector<int> MkOut; // вектор МК для резульатов
	vector<LoadPoint> OutAdr; // вектор адресов для записи результатов

	vector<ALUContext> Stack = {};
	ALU(void* parent, FU* Templ = nullptr) { Bus = (FU*)parent; Parent = parent; FUtype = 17; ProgFU(0, { 0, nullptr }); };
	void ProgFU(int MK, LoadPoint Load) override;
	void* Parent = nullptr;
	void		add(LoadPoint load);
	void		Clear();
	void*		VarNew(LoadPoint load); // New value of accum
	void*		ErrProg = nullptr; // Программа ошибки вычислений
	void*		VectErrProg = nullptr; // Программа ошибки векторной операции
	void*		OutOfRangeErrProg = nullptr; // Программа ошибки Выход индекса за пределы разрешенного диапазона
	void		set(LoadPoint load);
	void		error_msg(int error_code);
	void		calc(int MK, LoadPoint load);
	LoadPoint	get();
	bool		getLogic();
	bool		getSign(); // 0 - положительное число, 1 - отрицательное
	bool		getLogic(LoadPoint load);
	void		Out(LoadPoint); // ������ ������������ � ����������
	void		fu_min(LoadPoint load);
	void		fu_max(LoadPoint load);
	void		fu_cos(LoadPoint Load);
	void		fu_sin(LoadPoint Load);
	void		fu_tan(LoadPoint Load);
	void		fu_asin(LoadPoint Load);
	void		fu_acos(LoadPoint Load);
	void		fu_atan(LoadPoint Load);
	void		fu_mod(LoadPoint load);
	void		fu_sqrt(LoadPoint Load);
	void		fu_pow(LoadPoint load);
	void		fu_abs(LoadPoint Load);
	void		fu_ceil(LoadPoint Load);
	void		fu_floor(LoadPoint Load);
	void		fu_round(LoadPoint Load);
	void		fu_log(LoadPoint Load);
	void		fu_random();
	void		fu_inverse(LoadPoint load);
	void		fu_and(LoadPoint load);
	void		fu_or(LoadPoint load);
	void		fu_xor(LoadPoint load);
	void		fu_or_bit(LoadPoint load);
	void		fu_and_bit(LoadPoint load);
	void		fu_mr_bit(LoadPoint load);
	void		fu_ml_bit(LoadPoint load);
	void		fu_inv_bit(LoadPoint load);
	void		EQ(LoadPoint load);
	void		NotEQ(LoadPoint load);
	void		Bigger(LoadPoint load);
	void		BiggerEQ(LoadPoint load);
	void		Smaller(LoadPoint load);
	void		SmallerEQ(LoadPoint load);
	void		Remainder(LoadPoint load);
	void		XOR_BIT(LoadPoint load);
	void		Compar3Way(LoadPoint load);// Трехстороннее сравнение
	void	getCos(LoadPoint Load);
	void	getSin(LoadPoint Load);
	// Векторные операции
	void	append(LoadPoint Load);
	void		emptyvect(); // Создать пустой вектор
	void		concat(LoadPoint Load); // Конкатенация векторов
	void        vecmult(LoadPoint Load);// умножение векторов
	// Подрограммы сообщений об ошибках

};
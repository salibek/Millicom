#pragma once
#include "stdafx.h"
#include "Consts.h"
#include <functional>

//bool comp(LoadPoint x, LoadPoint y);
// Контекст АЛУ
class MkAdr {
public:
	int Mk = -1;
	LoadPoint Adr = {0, nullptr};
};

class ALUContext
{
public:
	vector<LoadPoint>* accumVect; // Векторный аккумулятор
	unsigned int accumType = 0; // Тип данных
	double		accum = 0;		// Скалярный аккумулятор
	string		accumStr;		// строковой аккулятор
	long int Ind = 0; // Индекс элемента вектора
	bool IndF = false; // флаг установки индекса
	long int IndAutoInc = 0; // Величина автоинкрементации индекса вектора
	void* ConfineStart = nullptr, * ConfineExpression = nullptr, * ConfineBorder = nullptr, * ConfineInc = nullptr; // Конфайн
	// TLoadArray - тип нагрузка, обозначающий вектор нагрузок
		vector<LoadPoint>* accumPoint = nullptr;	// Указатель на аккумулятор (вектор, матрица и т.п.)
//	vector<int> MkOut; // вектор МК для  резульатов
//	vector<LoadPoint> OutAdr; // вектор адресов для записи результатов
	vector <MkAdr> OutMkAdr; // Буфер для указания порядка следования выдачи векторных результатов вычисления

	bool VectOut = false; // Векторная выдача результата
};

class ALU : FU
{
//	bool comp(LoadPoint x, LoadPoint y); // Подпрограмма для сравнения элементов вектора
//	void ProgExec(void* Uk, unsigned int CycleMode = 0, FU* Bus = nullptr, vector<ip>::iterator* Start = nullptr) override; // Исполнение программы из ИК
//	void ProgExec(LoadPoint Uk, unsigned int CycleMode = 0, FU* Bus = nullptr, vector<ip>::iterator* Start = nullptr) override; // Исполнение программы из ИК

//	vector<int> ExecCounter; // ���� ��������� ���������� ������������ � ��
//	long int ExecRepeat = 1; // ������� ���������� ���������� ������������

public:
	unsigned int accumType = 0; // Тип данных
	double		accum = 0;		// Скалярный выходной аккумулятор (из него другие ФУ могут считать значение аккумулятора по ссылке)
	string		accumStr;		// строковой аккулятор
	void* accumVect = nullptr;	// Указатель на аккумулятор (вектор, матрица и т.п.)
	long int Ind = -1; // Индекс вектора или ИК

	vector<ALUContext> Stack = {}; // Стек аккумуляторов
	vector<long int> Anses = {}; // Стек ans (хранятся индексы уровней аккумулятора, которые являютс ans-ами
	vector<LoadVect_type> VectStore = {}; // Вектор временных векторных аккумуляторов

	ALU(FU* parent, FU* Templ = nullptr) { Bus = (FU*)parent; Parent = parent; FUtype = 17; ProgFU(0, { 0, nullptr });
	Alu = (FU*)this; // Настроить ссылку на АЛУ на самого себя
	ALUCreating = false;
	};
	void ProgFU(long int MK, LoadPoint Load, FU* Sender = nullptr) override;
	FU* Receiver = nullptr; // Приемщик данных
	FU* Copy() override; // Программа копирования ФУ
	FU* TypeCopy() override; // Создать ФУ такого же типа (не копируя контекст
	void VectOperation(long int MK, LoadPoint Load); // Реализацая векторных операций
	void		add(LoadPoint load);
	void		Clear();
	void* VarNew(LoadPoint load); // New value of accum
	long int   MKExt=-1, MkOutExt; // Внешняя МК для выполнения
	void* ErrProg = nullptr; // Программа ошибки вычислений
	void* VectErrProg = nullptr; // Программа ошибки векторной операции
	void* OutOfRangeErrProg = nullptr; // Программа ошибки Выход индекса за пределы разрешенного диапазона
	void* TypeMismatchErrProg = nullptr; // Программа ошибки несоответствия типов
	void* DivByZeroErrProg = nullptr; // Программа ошибки деления на нуль
	void		set(LoadPoint load);
	void		error_msg(int error_code);
	void		calc(int MK, LoadPoint load);
	void		sub(LoadPoint load);
	void		div(LoadPoint load);
	void		div_int(LoadPoint load);
	void		mult(LoadPoint load);
	void		inc(LoadPoint load);
	void		dec(LoadPoint load);
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
	void		fu_log10(LoadPoint Load);
	void		fu_log2(LoadPoint Load);
	void		fu_exp(LoadPoint Load);
	void		fu_random(LoadPoint Load);
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
	void		emptyvect(long size); // Создать пустой вектор
	void		concat(LoadPoint Load); // Конкатенация векторов
	void        vecmult(LoadPoint Load);// умножение векторов
	void length(LoadPoint Load);
	void clear(LoadPoint Load);
	void push_back(LoadPoint Load);
	void emplace_back(LoadPoint Load);
	void pop_back(LoadPoint Load);
	void insert(LoadPoint Load);
	void emplace(LoadPoint Load);
	void pop_backMk(LoadPoint Load);
	void Sort(LoadPoint Load, bool revers=false); // Сортировка Revers - флаг обратной сортировки
	void Reverse(LoadPoint Load);
	void lenMk(LoadPoint Load);
	void pop(LoadPoint Load);
	void del(LoadPoint Load);
	// Подрограммы сообщений об ошибках

};
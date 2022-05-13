	#pragma once
	#include "stdafx.h"
	#include "AluGeneral.h"
	#include "Consts.h"

	class Accumulator
	{
		void		sub(LoadPoint load);
		void		div(LoadPoint load);
		void		div_int(LoadPoint load);
		void		mult(LoadPoint load);
		void		inc(LoadPoint load);
		void		dec(LoadPoint load);
//		void		*Parent; // Ссылка на родительсний ФУ АЛУ
	public:
//		Accumulator(void* parent) { Parent = parent; };
		LoadPoint	accumulatorOld; // Старый аккумулятор (потом удалить)
		unsigned int accumType = 0; // Тип данных в аккумуляторе
		double		accumulator;		// Скалярный аккумулятор
		string		accumulatorStr;		// строковой аккулятор
		void*		accumulatorPoint;	// Указатель на аккумулятор (вектор, матрица и т.п.)
		void* Parent = nullptr;

		void		add(LoadPoint load);
		void		Clear();
		void*		VarNew(LoadPoint load); // New value of accum
		void		set(LoadPoint load);
		void		error_msg(int error_code);
		void		calc(int MK, LoadPoint load);
		LoadPoint	get();
		bool		getLogic();
		bool		getSign(); // 0 - положительное число, 1 - отрицательное
		bool		getLogic(LoadPoint load);
		void		Out(LoadPoint); // ������ ������������ � ����������
		//	~Accumulator() { Clear(); };
		void		fu_min(LoadPoint load);
		void		fu_max(LoadPoint load);
		void		fu_cos();
		void		fu_sin();
		void		fu_tan();
		void		fu_asin();
		void		fu_acos();
		void		fu_atan();
		void		fu_mod(LoadPoint load);
		void		fu_sqrt();
		void		fu_pow(LoadPoint load);
		void		fu_abs();
		void		fu_ceil();
		void		fu_floor();
		void		fu_round();
		void		fu_log();
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

		LoadPoint	getCos();
		LoadPoint	getSin();
	};
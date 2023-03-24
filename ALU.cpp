#include "stdafx.h"
#include "ALU.h"
#include "EnumMk.h"
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

bool		ALU::getSign() // 0 - положительное число, 1 - отрицательное
{
	switch (Stack.back().accumType >> 1)
	{
	case Dint:
	case Dchar:
	case Dfloat:
	case Ddouble:
		return Stack.back().accum < 0;
		break;
	default:
		return 0;
	}
}

void		ALU::EQ(LoadPoint Load)
{
	sub(Load);
	bool* t = new bool(!getLogic());
	set({ Cbool,t });
};
void		ALU::NotEQ(LoadPoint Load)
{
	sub(Load);
	bool* t = new bool(getLogic());
	set({ Cbool,t });
};
void		ALU::Bigger(LoadPoint Load)
{
	sub(Load);
	bool* t = new bool(!getSign() && getLogic());

	set({ Cbool,t });
};
void		ALU::BiggerEQ(LoadPoint Load)
{
	sub(Load);
	bool* t = new bool(!getSign());
	set({ Cbool,t });
};
void		ALU::Smaller(LoadPoint Load)
{
	sub(Load);
	bool* t = new bool(getSign());
	set({ Cbool,t });
};
void		ALU::SmallerEQ(LoadPoint Load)
{
	sub(Load);
	bool* t = new bool(getSign() || !getLogic());
	set({ Cbool,t });
};

void		ALU::Remainder(LoadPoint Load)
{
	sub(Load);
	bool* t = new bool(getSign() || !getLogic());
	set({ Cbool,t });
};

void		ALU::Clear() // ������� ������������
{
	Stack.back().accum = 0;
	Stack.back().accumType = Tint;
}

void	ALU::error_msg(int error_code)
{
	switch (error_code)
	{
	case 1:
		cout << "Not registered operation!";
		break;
	case 2:
		cout << "Not allowed operation!";
		break;
	case 3:
		cout << "Divided by zero!";
		break;
	}
}

void ALU::ProgFU(int MK, LoadPoint Load)
{
	MK %= FUMkRange;
	ProgExec(PrefixProg); // Выполнить префиксную программу

	bool LoadDelFlag = false; // Флаг удаления нагрузки со временными данными
	if (MK == ProgExecMk || MK == CalcMk) // выполнение программы
	{
		ProgExec(Load);
		//if (Stack.size() == 1)
		accum = Stack.back().accum; // Записать в выходной аккумулятор
		accumType = Stack.back().accumType; // Записать в выходной аккумулятор
		accumStr = Stack.back().accumStr; // Записать в выходной аккумулятор
		for(auto i: Stack.back().OutMkAdr)
			if(i.Adr.Point==__nullptr)
				i.Adr.Write(Stack.back().accum);
			else
	 			MkExec(i.Mk, { Cdouble,  &Stack.back().accum }); // Сделать преобразование типов попозже
		Stack.back().OutMkAdr.clear(); // Очистить стек выходных МК и адресов
	}
	else
	{
		if (MK >= 25 && MK < 200 && LoadPoint::isVector(Stack.back().accumType)) // Векторные операции
		{
			VectOperation(MK, Load);
			return;
		}

		if (MK >= 25 && MK < 900 && Load.isProg()) // Арифметико-логическсое выражение со ссылкой в нагрузке
		{
			LoadDelFlag = true;
			Stack.push_back({});
			ProgExec(Load);
			if (LoadPoint::isVector(Stack.back().accumType))
				Load = { CLoadVect,&Stack.back().accumVect };
			else {
				double tt = Stack.back().accum;
				Load = { Cdouble,&tt }; // Запомнить значение вычисленного аккумулятора
			}
			Stack.pop_back(); // Удалить аккумулятор из стека
		}
		switch (MK)
		{
		case 0: // Reset
			Stack.clear();
			Stack.push_back({});
			Stack.back().accumType = Cdouble;
			Stack.back().accum = 0;
			Accum = { Cdouble,&accum };
			break;
		case 3: // OutMk Выдать МК со значением аккумулятора
			if (Stack.back().accumType >> 1 == Dstring)
				MkExec(Load, { Stack.back().accumType,&Stack.back().accumStr }, Bus, true);
			else if (Stack.back().accumType >> 1 == DLoadVect) // вектор
			{
				if (Stack.back().IndF)
				{
					int Ind = Stack.back().Ind;
					if (Ind < 0) Ind = Stack.back().accumVect->size() + Ind;
					if (!(Ind >= 0 && Ind < Stack.back().accumVect->size()))
					{
						ProgExec(OutOfRangeErrProg); // Ошибка выхода индекса за пределы диапазона
						break;
					}
					else
					{
						MkExec(Load, Stack.back().accumVect->at(Ind), Bus, true);
						Stack.back().Ind += Stack.back().IndAutoInc;
					}
				}
				else
					MkExec(Load, {TLoadVect, Stack.back().accumVect });
			}
			else
				MkExec(Load, { Cdouble,&Stack.back().accum }, Bus, true); // Заглушка!!!
			break;
		case 2: // Out Выдать значение аккумулятора
			if (accumType >> 1 == Dstring)
				Load.Write(Stack.back().accumStr);
			else if (Stack.back().accumType >> 1 == DLoadArray) // Вектор
			{
				if (Stack.back().IndF)
					if (!(Stack.back().Ind >= 0 && Stack.back().Ind < Stack.back().accumVect->size()))
					{
						ProgExec(OutOfRangeErrProg); // Ошибка выхода индекса за пределы диапазона
						break;
					}
					else
						Load.WriteFromLoad(Stack.back().accumVect->at(Stack.back().Ind));
				if (Load.Type == TLoadArray || Load.Type == Tvoid)
				{
					Load.Write(Stack.back().accumVect);
					Stack.back().Ind += Stack.back().IndAutoInc;
				}
				else
					;// Сообщение о несоответствии типов
			}
			else
				Load.Write(Stack.back().accum);
			break;
		case E_MK::PUSH: //  Push Сделать еще один уровень аккумулятора
			Stack.push_back({});
			ProgFU(E_MK::SET, Load);
			break;
		case 5: //AccumAdrOut Выдать адрес выходного аккумулятора
			Load.Write((void*)&Accum);
			break;
		case 6: //AccumAdrOutMk Выдать МК с адресом выходного аккумулятора
			MkExec(MK, { Cdouble,(void*)&Accum });
			break;
		case 10: // OutMkAdrClear Очистить буфер МК и адресов для выдачи результата
			Stack.back().OutMkAdr.clear();
			break;
		case 11: // OutMkAdd Добавить адрес для выдачи результата вычисления
			Stack.back().OutMkAdr.push_back({});
			Stack.back().OutMkAdr.back().Mk = Load.toInt();
			break;
		case 12: // OutAdrAdd Добавить адрес для выдачи результатов
			Stack.back().OutMkAdr.push_back({ -1, Load });
			break;
			// Операции с аккумулятором
		case 15: // Push Добавить новый аккумулятор (аналог Set, только сначала создается новый аккумулятор)
			Stack.push_back({});
			ProgFU(E_MK::SET, Load);
			break;
		case 16: // Pop Выдать значение аккумулятора и удалить аккмумулятор
		case 17: // PopMk Выдать МК со значением аккумулятора и удалить аккмумулятор
			if (MK == 16)
				if (Load.isDigit() && Load.isDigitBool(Stack.back().accumType))
					Load.Write(Stack.back().accum);
				else if (Load.isStrChar() && Load.isDigitBool(Stack.back().accumType))
					Load.Write(Stack.back().accumStr);
				else
					Load.Write(Stack.back().accumVect);
			else
			{
				if (Load.isDigit() && Load.isDigitBool(Stack.back().accumType))
					MkExec(Load, LoadPoint::TypeMinimizeOut(Stack.back().accum, Stack.back().accumType | 1));
				else if (Load.isStrChar() && Load.isDigitBool(Stack.back().accumType))
					MkExec(Load, { Stack.back().accumType,&Stack.back().accumStr });
				else
					Load.Write(Stack.back().accumVect);
			}
			if (Stack.size() > 1)
				Stack.pop_back();
			break;

		case 13:// CopyOut Выдать копию аккумулятора
		case 14:// CopyOutMK Выдать МК с копией аккумулятора
		case 18:// CopyConstOut Выдать копию аккумулятора и сделать значение константой
		case 19:// CopyConstOutMK Выдать МК с копией аккумулятора и сделать значение константой
		{
			LoadPoint t = { accumType,&Stack.back().accum };
			if (LoadPoint::isStrChar(accumType))
				t.Point = &Stack.back().accumStr;
			else
				if (LoadPoint::isVector(accumType))
					t.Point = &Stack.back().accumVect;
			t.Type = t.Type | 1; // Установить константу для того, чтобы клонировать
			t=t.Clone();
			if (MK == 13 || MK == 14)
				t.Type -= 1; // Установить тип переменной
			if (MK == 13 || MK == 18)
				Load.WriteFromLoad(t);
			else
				MkExec(Load,t);
		}
			break;
			// Программы обработки ошибок
		case 20: // ErrProgSet Программа обработки ошибки любого типа
			ErrProg = Load.Point;
			break;
		case 21: // VectorErrProgSet Программа обработки ошибки векторной операции
			VectErrProg = Load.Point;
			break;
		case 22: // IndexOutOfRangeProgSet Программа обработки ошибки выхода за допустимые пределы индекса
			OutOfRangeErrProg = Load.Point;
			break;
		case 23: // TypeMismatchErrProgSet Установить программу отработки ошибки несоотвествия типов
			TypeMismatchErrProg = Load.Point;
			break;
		case 24: // DivByZeroErrProgSet Установить программу отработки ошибки деление на нуль
			DivByZeroErrProg = Load.Point;
			break;
		

		case E_MK::RESET_A: // Reset Сброс аккумулятора
			Stack.back().accumType = Cdouble;
			Stack.back().accum = 0;
			break;
		case E_MK::SET: // Установить значение в аккумулятор
			//	case E_MK::SET: // Set
			set(Load);
			break;
		case E_MK::INC: // inc
			inc(Load);
			//set(Load);
			break;
		case E_MK::DEC: // dec
			dec(Load);
			//set(Load);
			break;
		case E_MK::SUM:
			add(Load);
			break;
		case E_MK::SUB:
			sub(Load);
			break;
		case E_MK::MULT:
			mult(Load);
			break;
		case E_MK::DIV:
			div(Load);
			break;
		case E_MK::MIN:
			fu_min(Load);
			break;
		case E_MK::MAX:
			fu_max(Load);
			break;
		case E_MK::COS:
			fu_cos(Load);
			break;
		case E_MK::SIN:
			fu_sin(Load);
			break;
		case E_MK::TAN:
			fu_tan(Load);
			break;
		case E_MK::ACOS:
			fu_acos(Load);
			break;
		case E_MK::ASIN:
			fu_asin(Load);
			break;
		case E_MK::ATAN:
			fu_atan(Load);
			break;
		case E_MK::MOD:
			fu_mod(Load);
			break;
		case E_MK::SQRT:
			fu_sqrt(Load);
			break;
		case E_MK::POW:
			fu_pow(Load);
			break;
		case E_MK::ABS:
			fu_abs(Load);
			break;
		case E_MK::CEIL:
			fu_ceil(Load);
			break;
		case E_MK::FLOOR:
			fu_floor(Load);
			break;
		case E_MK::ROUND:
			fu_round(Load);
			break;
		case E_MK::LOG:
			fu_log(Load);
			break;
		case E_MK::RANDOM:
			fu_random(Load);
			break;
		case E_MK::AND:
			fu_and(Load);
			break;
		case E_MK::OR:
			fu_or(Load);
			break;
		case E_MK::INV:
			fu_inverse(Load);
			break;
		case E_MK::DIV_INT:
			div_int(Load);
			break;
		case E_MK::XOR:
			fu_xor(Load);
			break;
		case E_MK::INV_BIT:
			fu_inv_bit(Load);
			break;
		case E_MK::OR_BIT:
			fu_or_bit(Load);
			break;
		case E_MK::AND_BIT:
			fu_and_bit(Load);
			break;
		case E_MK::MR_BIT:
			fu_mr_bit(Load);
			break;
		case E_MK::ML_BIT:
			fu_ml_bit(Load);
			break;
		case E_MK::EQ:
			EQ(Load);
			break;
		case E_MK::NotEQ:
			NotEQ(Load);
			break;
		case E_MK::Bigger:
			Bigger(Load);
			break;
		case E_MK::BiggerEQ:
			BiggerEQ(Load);
			break;
		case E_MK::Smaller:
			Smaller(Load);
			break;
		case E_MK::SmallerEQ:
			SmallerEQ(Load);
			break;
		case E_MK::Remainder:
			Remainder(Load);
			break;
		case E_MK::XOR_BIT:
			XOR_BIT(Load);
			break;
		case E_MK::Compar3Way:
			Compar3Way(Load);
			break;


		case 198: // SumVect Поэлементное векторное +
		case 199: // MulVect Поэлементное векторное *
		{
			if (!Load.isVector(Stack.back().accumType)) break;
			vector<LoadPoint> Rez;
			if (Load.isDigitBool()) // Одна операция ко всем элементам вектора
			{
				Stack.push_back({}); // Создать дополнительные аккумулятор
				LoadVect_type v1 = (Stack.end() - 2)->accumVect; // Запомнить ссылку на исходный вектор
				for (auto& i : *v1)
				{
					if (!i.isDigitBool())
					{
						ProgExec(ErrProg); // Программа отработки ошибки
						ProgExec(VectErrProg); // Программа отработки ошибки векторных операций
						Stack.pop_back();
						break;
					}
					ProgFU(E_MK::SET, i);
					ProgFU(MK - 300, Load);
					Rez.push_back({ Stack.back().accumType,MakeLoadFromDouble(Stack.back().accum,Stack.back().accumType | 1) }); // Запись константы

				}
				Stack.pop_back(); // Удалить дополнительный аккумулятор
				*Stack.back().accumVect = Rez;
				break;
			}

			if (Load.isVector() && LoadPoint::isInt(Stack.back().accumType) && Load.toInt() >= 0) // Мультиплексия вектора
			{
				int N = *(int*)Load.Point;
				for (int i = 0; i < N; i++)
					for (auto& j : *Stack.back().accumVect)
						Rez.push_back(j.Clone());
				*Stack.back().accumVect = Rez;
				break;
			}

			if (Load.isVector() && Stack.back().accumType >> 1 == Dint && Load.toInt() >= 0)
				if (!Load.isVector() || !(Load.isVector() && Stack.back().accumType >> 1 == DLoadVect))
				{
					ProgExec(ErrProg); // Программа отработки ошибки
					ProgExec(VectErrProg); // Программа отработки ошибки векторных операций
					break;
				}
			Stack.push_back({}); // Создать дополнительные аккумулятор
			LoadVect_type v1 = (Stack.end() - 2)->accumVect;

			for (auto i = v1->begin(), j = ((LoadVect_type)Load.Point)->begin(); // выполнение векторных операций
				i != v1->end() && j != ((LoadVect_type)Load.Point)->end(); i++, j++)
			{
				ProgFU(E_MK::SET, *i);
				ProgFU(MK - 300, *j);
				Rez.push_back({ Stack.back().accumType,MakeLoadFromDouble(Stack.back().accum,Stack.back().accumType | 1) }); // Запись константы
			}

			Stack.back().accumVect->clear();
			delete Stack.back().accumVect;
			Stack.pop_back(); // Удалить дополнительный аккумулятор
			*Stack.back().accumVect = Rez;
			break;
		}

			// Векторные операции
		case 269: //IndAutoIncSet Установить величину автоинкрементации индекса вектора
			Stack.back().IndAutoInc = Load.toInt();
			break;
		case 270: //IndSet Установить индекс вектора (nil в нагрузке вызывает сброс индекса, т.е. по команде Out выдается вектор, а не элемент)
			if (Load.Point == nullptr)
			{
				Stack.back().IndF = false;
				Stack.back().Ind = 0;
				break;
			}
			if (!Load.isDigitBool())
			{
				ProgExec(ErrProg); // Программа отработки ошибки
				ProgExec(VectErrProg); // Программа отработки ошибки векторных операций
				break;
			}
			Stack.back().IndF = true;
			Stack.back().Ind = Load.toInt();
			break;
		case 271: //IndInc Инремент индекса вектора
			if (Stack.back().IndF)
				Stack.back().Ind++;
			break;
		case 272: //IndDec Декремент индекса вектора
			if (Stack.back().IndF)
				Stack.back().Ind--;
			break;
		case 273: // IndAdd Прибавить смещение к индексу вектора
			if (Stack.back().IndF)
				Stack.back().Ind += Load.toInt();
			break;
		case 274: // VectValByIndSet Установить значение аккумулятора по индексу в нагрузке
		{	int i = Load.toInt();
		if (Stack.size() < 1 || 1 >= Stack.back().accumVect->size() || i < 0)
		{
			ProgExec(ErrProg);
			ProgExec(OutOfRangeErrProg);
			ProgStopAll = true;
			break;
		}
		ProgFU(E_MK::SET, Stack.back().accumVect->at(i));
		}
		case 275: // ConfineAppend Добавить элемент в предпредпредыдущий аккумулятор из векторa предыдущего аккумулятора по индексу из текущего аккумулятора (для конфайна)
		{	int i = Load.toInt();
		if (Stack.size() < 3 || !LoadPoint::isVector((Stack.end() - 3)->accumType))
		{
			ProgExec(ErrProg);
			ProgExec(VectErrProg);
			ProgStopAll = true;
			break;
		}
		if (i < 0 || i >= (Stack.end() - 3)->accumVect->size())
		{
			ProgExec(ErrProg);
			ProgExec(OutOfRangeErrProg);
			ProgStopAll = true;
			break;
		}
		(Stack.end() - 3)->accumVect->push_back((Stack.end() - 2)->accumVect->at((int)Stack.back().accum).Clone());
		}
		case 276: // PrevIndSet Установить индекс у предыдущего аккумулятора
			if (!Load.isDigitBool() || Stack.size() < 2)
			{
				ProgExec(ErrProg);
				break;
			}
			if (LoadPoint::isVector(Stack.back().accumType))
			{
				ProgExec(ErrProg);
				ProgExec(VectErrProg);
				break;
			}
			(Stack.end() - 2)->Ind = Load.toInt();
			break;
		case 277: // VectValSet Установить в аккумулятор значение вектора из нагрузки по индексу в аккумуляторе
		case 278: // VectValPush Положить значение вектора из пред. аккумулятора по индексу текущего аккумулятора (если Load=nil, то вектор из предыдущего аккумулятора
			if (!Load.isVector() || !LoadPoint::isDigitBool(Stack.back().accumType))
			{
				ProgExec(ErrProg); // Программа отработки ошибки
				ProgExec(VectErrProg); // Программа отработки ошибки векторных операций
				break;
			}
			if (MK == 277)
				ProgFU(E_MK::SET, { Load.Type,Load.LoadVect() }); // Установить значение из элемента вектора
			else
				ProgFU(E_MK::PUSH, (Stack.end() - 2)->accumVect->at(Load.toInt())); // Добавить значение из элемента вектора
			break;
		case 280: // VectCreat Создать новый вектор
			emptyvect();
			break;
		case 281: // VectDel Удалить вектор
			Stack.back().accumVect->resize(0); // Добавить удаление каждого элемента вектора!!!
 			delete Stack.back().accumVect;
			Stack.back().accumVect = nullptr;
			Stack.back().IndF = false;
			break;
		case 282: // VectPop Отделить вектор от аккумулятора и выдать адрес вектора
			if(!LoadPoint::isVector(accumType)) break;
			{LoadPoint t = { TLoadVect, Stack.back().accumVect,-1};
			Load.Write(t);
			}
			Stack.back().accumVect = nullptr;
			Stack.back().IndF = false;
			break;
		case 283: // VectPopMk Отделить вектор от аккумулятора и выдать МК с адресом вектора
			if (!LoadPoint::isVector(accumType)) break;
			{LoadPoint t = { TLoadVect, Stack.back().accumVect,-1 };
			MkExec(Load, t); }
			Stack.back().accumVect = nullptr;
			Stack.back().IndF = false;
			break;
		case 285: // VectClear Очистить вектор
			if (Stack.back().accumType >> 1 != DLoadVect)
			{
				ProgExec(ErrProg); // Программа отработки ошибки
				ProgExec(VectErrProg); // Программа отработки ошибки векторных операций
				break;
			}
			if (Stack.back().accumVect == nullptr)
				Stack.back().accumVect = new vector<LoadPoint>;
			Stack.back().accumVect->clear();
			break;
		case 290: // Append Добавить элемент в вектор
			append(Load.Clone());
			break;

			// Реализация поэлементных векторных операций
			// Двуоперандные векторные операции

		default:
			CommonMk(MK, Load);
			break;
		}
		if (LoadDelFlag) // Удаление врЕменных данных
			Load.VarDel();
		// Переписывание результатов в буфер для считывания другими ФУ
		accum = Stack.back().accum;
		accumStr = Stack.back().accumStr;
		accumVect = &Stack.back().accumVect;
		accumType = Stack.back().accumType;
	}
	ProgExec(PostfixProg); // Выполнить постфиксную программу
}

void ALU::VectOperation(int MK, LoadPoint Load) // Реализацая векторных операций
{
	if (MK == E_MK::SUM && Load.isVector()) // Конкатенация векторов
	{
		concat(Load);
		return;
	}

	if (MK == E_MK::MULT) // Конкатенация векторов
	{
		vecmult(Load);
		return;
	}
	if (MK == 198 || MK == 199)
	{
		if (!Load.isVector())
		{
			ProgExec(ErrProg);
			ProgExec(VectErrProg);
			ProgExec(TypeMismatchErrProg);
			return;
		}
		if (MK == 198)// Векторное поэлементное сложение
			MK = E_MK::SUM;
		if (MK == 199)// Векторное мультиплицирование
			MK = E_MK::MULT;
	}

	if (Load.isDigitBool()) // Одна операция к каждому элементу вектора
	{
		Stack.push_back({});
		for (auto& i : *(Stack.end()-2)->accumVect)
		{
			ProgFU(E_MK::SET, i);
			ProgFU(MK, Load);
			i.Write(Stack.back().accum);
		}
		Stack.pop_back();
	}
	else // Векторная операция
	{
		vector<LoadPoint>* t = Stack.back().accumVect;
		Stack.push_back({});
		auto j = ((vector<LoadPoint>*)Load.Point)->begin();
		auto i = (Stack.end() - 2)->accumVect->begin();
		for (; i != (Stack.end() - 2)->accumVect->end() && j != ((vector<LoadPoint>*)Load.Point)->end(); i++, j++)
		{
			ProgFU(E_MK::SET, *i);
			ProgFU(MK,*j);
			i->Write(Stack.back().accum);
		}
		Stack.pop_back();
	}
}

void	ALU::calc(int MK, LoadPoint Load)
{
	switch (MK)
	{
	default:
		error_msg(1);
		break;
	}
}

void ALU::XOR_BIT(LoadPoint Load)
{



	if (Load.Point == nullptr) {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{

		Stack.back().accum = static_cast<int>(Stack.back().accum) ^ Load.toInt();
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}

	else {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void ALU::inc(LoadPoint Load)
{
	if (Load.Point == nullptr)
		Stack.back().accum++;
	else
		Stack.back().accum += Load.toDouble();
}

void ALU::dec(LoadPoint Load) // Декреминт если нагрузки нет, то декремируется аккумулятор
{
	if (Load.Point == nullptr)
		Stack.back().accum--;
	else
		Stack.back().accum -= Load.toDouble();
}
void ALU::emptyvect() {
	Stack.back().accumType = TLoadVect;
	Stack.back().accumVect = new vector<LoadPoint>;
	Stack.back().accumVect->clear();
	Stack.back().IndF = false;
}
void ALU::append(LoadPoint Load) //добавление
{
	Stack.back().accumVect->push_back(Load.Clone());
	Stack.back().accumVect->back().Type = (Stack.back().accumVect->back().Type | 1) - 1; // Установить флаг переменной!!!
}
void ALU::concat(LoadPoint Load) // Конкатенация векторов
{
	vector<LoadPoint> t; // Создадим временный вектор для результата
	for (auto& i : *Stack.back().accumVect)
		t.push_back(i.Clone());
	for (auto& i : *(vector<LoadPoint>*)Load.Point)
		t.push_back(i.Clone());
	*Stack.back().accumVect = t;
}

void ALU::vecmult(LoadPoint Load) { // Размножение вектора
	int N = Stack.back().accumVect->size();
	for(int k=Load.toInt()-1; k; k--)
		for (int i = 0; i < N; i++)
			Stack.back().accumVect->push_back(Stack.back().accumVect->at(i).Clone());
}

void	ALU::add(LoadPoint Load) // Сложение
{
	if (Load.Point == nullptr) {
		return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		if (Stack.back().accumType >> 1 != DLoadVect) // Если второй операнд не вектор, то ошибка
		{
			ProgExec(ErrProg); // Запуск программы ошибки 
			ProgExec(VectErrProg); // Запуск программы ошибки векторной операции
			return;
		}
		Stack.back().accumType = TLoadVect; // Сделать результат вектором-переменной
		concat(Load); // Конкатенация
		return;
	}
	else if (Load.Type >> 1 == Dstring)
	{
		if (Stack.back().accumType >> 1 == Dstring)
			Stack.back().accumStr += Load.toStr();
		else if (Stack.back().accumType >> 1 == DLoadVect) {
			for (int i = 0; i < Stack.back().accumVect->size(); i++) {
				if (Stack.back().accumVect->at(i).Type >> 1 == Dstring)
				{
					Stack.back().accumVect->at(i).Write(Stack.back().accumVect->at(i).toStr() + Load.toStr());
				}
			}
		}
	}

	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
			Stack.back().accum += Load.toDouble();
			if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
				Stack.back().accumType = Cint;
			else
				Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect) // Мультисложение вектора и числа
	{
		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(Stack.back().accumVect->at(i).toDouble() + Load.toDouble());
			}
		}
	}
}

void	ALU::sub(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(Stack.back().accumVect->at(i).toDouble() - (((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble());
				}

			}
		}
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}

	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum -= Load.toDouble();
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(Stack.back().accumVect->at(i).toDouble() - Load.toDouble());
			}
		}
	}
	else {
		//	 ((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void	ALU::mult(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if ((((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					if (Stack.back().accumVect->at(i).isDigit()) {
						Stack.back().accumVect->at(i).Write(Stack.back().accumVect->at(i).toDouble() * (((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble());
					}
					else if (Stack.back().accumVect->at(i).Type >> 1 == Dstring) {
						string l;
						int k = (((vector<LoadPoint>*)Load.Point)->begin() + i)->toInt();
						for (int j = 0; j < k; j++)
							l += (((vector<LoadPoint>*)Load.Point)->begin() + i)->toStr();
					}
				}
			}
		}
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	else if (Stack.back().accumType >> 1 == Dstring) {
		string k;
		int l;
		//l = Load.toStr();
		l = Load.toInt();
		//for (int i = 0; i < l.size(); i++)
		//	k += Stack.back().accumStr;
		for (int i = 0; i < l; i++)
			k += Stack.back().accumStr;
		Stack.back().accumStr = k;
	}
	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum *= Load.toDouble();
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(Stack.back().accumVect->at(i).toDouble() * Load.toDouble());
			}
			else if (Stack.back().accumVect->at(i).Type >> 1 == Dstring) {
				std::string k = Stack.back().accumVect->at(i).toStr();
				for (int j = 0; j < Load.toDouble(); j++)
					Stack.back().accumVect->at(i).Write(Stack.back().accumVect->at(i).toStr() + k);
			}
		}
	}

	else {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void	ALU::div(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		ProgExec(DivByZeroErrProg);
		return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(Stack.back().accumVect->at(i).toDouble() / (((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble());
				}

			}
		}
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}

	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum /= Load.toDouble();
		if (Stack.back().accum != int(Stack.back().accum)) // Если результат деления дробный, то будет дробный тип результата
			Stack.back().accumType = Tdouble;
		else if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{
		int i;
		for (i = 0; i < Stack.back().accumVect->size(); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(Stack.back().accumVect->at(i).toDouble() / Load.toDouble());
			}
		}
	}
	else if(Load.isVector() && Load.isVector(Stack.back().accumType)) { // Два вектора
		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(Stack.back().accumVect->at(i).toDouble() / Load.toDouble());
			}
		}

		return;
	}
}

void	ALU::div_int(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//		((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	if (Load.isVector() && Stack.back().accumType >> 1 == DLoadVect) {
		int N = min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size());
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (int i = 0; i < N; i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write((Stack.back().accumVect->at(i).toInt()) / (((vector<LoadPoint>*)Load.Point)->begin() + i)->toInt());
				}

			}
		}
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}

	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = static_cast<int>(Stack.back().accum) / Load.toInt();
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)

			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect) // Мультисложение вектора и числа
	{
		for (int i = 0; i < Stack.back().accumVect->size(); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(Stack.back().accumVect->at(i).toInt() / Load.toInt());
			}
		}

	}
	else
	{
		ProgExec(ErrProg);
		ProgExec(TypeMismatchErrProg);
	}
}

LoadPoint ALU::get()// выдать , проверку на данные нужно сделать
{
	switch (Stack.back().accumType >> 1)
	{
	case Dint:
	{
		int t = round(Stack.back().accum);
		return{
			Stack.back().accumType, &t
		};

	}
	default:
		return{ 0,nullptr };
	}

	//	ALU.print();

}

void ALU::Out(LoadPoint Load) // �������� ����������� � ����������
{
	if (Load.Type % 2 != 0) return; // Если константа, то запись в нее не производим
	if (Load.Point == nullptr) {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	if (Load.Type >> 1 == Dstring)
	{
		*(string*)Load.Point = Stack.back().accumStr;
		return;
	}
	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		switch (Stack.back().accumType)
		{
		case Tdouble:;
		}
		{
			LoadPoint t = { (unsigned int)Stack.back().accumType, &Stack.back().accum };
			Load.Write(t);
		}
	}

	else {
		//		((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void* ALU::VarNew(LoadPoint Load)//создать новую переменную
{

	switch (Load.Type >> 1)
	{
	case Dint:
		return new int(*(int*)Load.Point);
	case Dfloat:
		return new float(*(float*)Load.Point);
	case Ddouble:
		return new double(*(double*)Load.Point);
	case Dchar:
		return new char(*(char*)Load.Point);
	case Dbool:
		return new bool(*(bool*)Load.Point);
	case Dstring:
		return new string(*(string*)Load.Point);
	}
}

void		ALU::set(LoadPoint Load) // Установить значение в аккумулятор
{
	if (Load.Point == nullptr) {
		Stack.back().accumType = Cint;
		Stack.back().accum = 0;
	}
	else if (Load.Type >> 1 == Dstring)
	{
		Stack.back().accumType = Load.Type;
		Stack.back().accumStr = Load.toStr();
	}
	else if (Load.isDigitBool())
	{
		Stack.back().accum = Load.toDouble();
		Stack.back().accumType = Load.Type;
	}
	else if (Load.Type >> 1 == DLoadVect)
	{
		Stack.back().accumVect = (vector<LoadPoint>*) Load.Point;
		Stack.back().accumType = Load.Type;
	}
	else {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		//return;
	}
	if (Stack.size() == 1) // Переписать в выходной аккумулятор
	{
		accumType = Stack.back().accumType;
		accum = Stack.back().accum;
		//	accumPoint = Stack.back().accumPoint;
	}
}

bool		ALU::getLogic()
{


	switch (Stack.back().accumType >> 1)
	{
	case Dint:
	case Dfloat:
	case Ddouble:
	case Dchar:
	case Dbool:
		return Stack.back().accum != 0;
	case Dstring:
		return Stack.back().accumStr != "";
	}

}

bool		ALU::getLogic(LoadPoint Load) // ����������� ����������� ����//все типы переходят в bool
{
	return false;
}

void	ALU::fu_max(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(max(Stack.back().accumVect->at(i).toDouble(), (((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble()));
				}

			}
		}
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = max(Stack.back().accum, Load.toDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(max(Stack.back().accumVect->at(i).toDouble(), Load.toDouble()));
			}
		}
	}
	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void	ALU::fu_min(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(min(Stack.back().accumVect->at(i).toDouble(), (((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble()));
				}

			}
		}
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = min(Stack.back().accum, Load.toDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(min(Stack.back().accumVect->at(i).toDouble(), Load.toDouble()));
			}
		}
	}
	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void	ALU::fu_cos(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		int i = 0;

		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(cos(Stack.back().accumVect->at(i).toDouble()));
				}

			}
		}
		else			if (Load.isDigitBool(Stack.back().accumType))
			Stack.back().accum = cos(Load.toDouble());
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"

			return;
	}
	if (Load.Type >> 1 == DLoadVect) {

		int i = 0;

		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(cos((((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble()));
				}

			}
		}
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = cos(Load.toDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(cos(Load.toDouble()));
			}
		}
	}
	else {
		return;
	}
}

void	ALU::fu_sin(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		int i = 0;

		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(sin(Stack.back().accumVect->at(i).toDouble()));
				}

			}
		}
		else			if (Load.isDigitBool(Stack.back().accumType))
			Stack.back().accum = sin(Load.toDouble());
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
			return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(sin((((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble()));
				}

			}
		}
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = sin(Load.toDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(sin(Load.toDouble()));
			}
		}
	}
	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void	ALU::fu_acos(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		int i = 0;

		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(acos(Stack.back().accumVect->at(i).toDouble()));
				}

			}
		}
		else			if (Load.isDigitBool(Stack.back().accumType))
			Stack.back().accum = acos(Load.toDouble());
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
			return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(acos((((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble()));
				}

			}
		}
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = acos(Load.toDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(acos(Load.toDouble()));
			}
		}
	}
	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void	ALU::fu_asin(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		int i = 0;

		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(asin(Stack.back().accumVect->at(i).toDouble()));
				}

			}
		}
		else			if (Load.isDigitBool(Stack.back().accumType))
			Stack.back().accum = asin(Load.toDouble());
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
			return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(asin((((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble()));
				}

			}
		}
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = asin(Load.toDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(asin(Load.toDouble()));
			}
		}
	}
	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void	ALU::fu_tan(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		int i = 0;

		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(tan(Stack.back().accumVect->at(i).toDouble()));
				}

			}
		}
		else			if (Load.isDigitBool(Stack.back().accumType))
			Stack.back().accum = tan(Load.toDouble());
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
			return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(tan((((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble()));
				}

			}
		}
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = tan(Load.toDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(tan(Load.toDouble()));
			}
		}
	}
	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void	ALU::fu_atan(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		int i = 0;

		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(atan(Stack.back().accumVect->at(i).toDouble()));
				}

			}
		}
		else			if (Load.isDigitBool(Stack.back().accumType))
			Stack.back().accum = atan(Load.toDouble());
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
			return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(atan((((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble()));
				}

			}
		}
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = atan(Load.toDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(atan(Load.toDouble()));
			}
		}
	}
	else {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void	ALU::fu_mod(LoadPoint Load)//остаток
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write((Stack.back().accumVect->at(i).toInt() % (((vector<LoadPoint>*)Load.Point)->begin() + i)->toInt()));
				}

			}
		}
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{

		Stack.back().accum = static_cast<int>(Stack.back().accum) % Load.toInt();
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(Stack.back().accumVect->at(i).toInt() % Load.toInt());
			}
		}
	}
	else {
		return;
	}
}

void	ALU::fu_sqrt(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		int i = 0;

		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(sqrt(Stack.back().accumVect->at(i).toDouble()));
				}

			}
		}
		else			if (Load.isDigitBool(Stack.back().accumType))
			Stack.back().accum = sqrt(Load.toDouble());
		else
			return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(sqrt((((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble()));
				}

			}
		}
		else
			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = sqrt(Load.toDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(sqrt(Load.toDouble()));
			}
		}
	}
	else {
		return;
	}
}

void	ALU::fu_pow(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(pow(Stack.back().accumVect->at(i).toDouble(), (((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble()));
				}

			}
		}
		else
			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = pow(Stack.back().accum, Load.toDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(pow(Stack.back().accumVect->at(i).toDouble(), Load.toDouble()));
			}
		}
	}
	else {
		return;
	}
}

void	ALU::fu_abs(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		int i = 0;

		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(abs(Stack.back().accumVect->at(i).toDouble()));
				}

			}
		}
		else			if (Load.isDigitBool(Stack.back().accumType))
			Stack.back().accum = abs(Load.toDouble());
		else
			// Запуск подпрограммы ошибки "Нет операнда"
			return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(abs((((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble()));
				}

			}
		}
		else

			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = abs(Load.toDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(abs(Load.toDouble()));
			}
		}
	}
	else {

		return;
	}
}

void	ALU::fu_ceil(LoadPoint Load)//округление вверх
{
	if (Load.Point == nullptr) {
		int i = 0;

		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(ceil(Stack.back().accumVect->at(i).toDouble()));
				}

			}
		}
		else			if (Load.isDigitBool(Stack.back().accumType))
			Stack.back().accum = ceil(Load.toDouble());
		else
			// Запуск подпрограммы ошибки "Нет операнда"
			return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(ceil((((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble()));
				}

			}
		}
		else

			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = ceil(Load.toDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(ceil(Load.toDouble()));
			}
		}
	}
	else {
		return;
	}
}

void	ALU::fu_floor(LoadPoint Load)//округление вниз
{
	if (Load.Point == nullptr) {
		int i = 0;

		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(floor(Stack.back().accumVect->at(i).toDouble()));
				}

			}
		}
		else			if (Load.isDigitBool(Stack.back().accumType))
			Stack.back().accum = floor(Load.toDouble());
		else
			//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
			return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(floor((((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble()));
				}

			}
		}
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = floor(Load.toDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(floor(Load.toDouble()));
			}
		}
	}
	else {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void	ALU::fu_round(LoadPoint Load)//просто округление
{
	if (Load.Point == nullptr) {
		int i = 0;

		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(round(Stack.back().accumVect->at(i).toDouble()));
				}

			}
		}
		else			if (Load.isDigitBool(Stack.back().accumType))
			Stack.back().accum = round(Load.toDouble());
		else
			//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
			return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(round((((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble()));
				}

			}
		}
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = round(Load.toDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(round(Load.toDouble()));
			}
		}
	}
	else {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void	ALU::fu_log(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		int i = 0;

		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(log(Stack.back().accumVect->at(i).toDouble()));
				}

			}
		}
		else
			return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		int i = 0;
		if (Stack.back().accumType >> 1 == DLoadVect) {
			for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
				if (Stack.back().accumVect->at(i).isDigit() && (((vector<LoadPoint>*)Load.Point)->begin() + i)->isDigit()) {
					Stack.back().accumVect->at(i).Write(log((((vector<LoadPoint>*)Load.Point)->begin() + i)->toDouble()));
				}

			}
		}
		else
			return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = log(Load.toDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else if (Load.isDigitBool() && Stack.back().accumType >> 1 == DLoadVect)
	{

		int i;
		for (i = 0; i < min(Stack.back().accumVect->size(), ((vector<LoadPoint>*)Load.Point)->size()); i++) {
			if (Stack.back().accumVect->at(i).isDigit()) {
				Stack.back().accumVect->at(i).Write(log(Load.toDouble()));
			}
		}
	}
	else {
		return;
	}
}

void	ALU::fu_random(LoadPoint Load)
{

	if(Load.Point==nullptr)
		Stack.back().accum = rand();
	else
		Stack.back().accum = rand()%Load.toInt();
}

void	ALU::getCos(LoadPoint Load)
{
}

void	ALU::getSin(LoadPoint Load)
{
}


void		ALU::fu_inverse(LoadPoint Load)//обратное
{
	if (Load.Point == nullptr) {
		return;
	}
	if (Load.Type >> 1 == Dstring)
	{
		if (Load.toStr().length() != 0)
			Stack.back().accum = 0;
		else
			Stack.back().accum = 1;
		return;
	}
	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		if (Load.toDouble() == 0)
			Stack.back().accum = 1;
		else
			Stack.back().accum = 0;
	}

	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void		ALU::fu_and(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		return;
	}
	if (Load.Type >> 1 == Dstring)
	{
		if (Stack.back().accumType >> 1 == Dstring)
			if (Load.toStr().length() != 0 && Stack.back().accumStr.length() != 0)
				Stack.back().accum = 1;
			else
				Stack.back().accum = 0;
		return;
	}
	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		if (Load.toDouble() == 0 || Stack.back().accum == 0)
			Stack.back().accum = 0;
		else
			Stack.back().accum = 1;
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}

	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void		ALU::fu_or(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	if (Load.Type >> 1 == Dstring)
	{
		if (Stack.back().accumType >> 1 == Dstring)
			if (Load.toStr().length() != 0 || Stack.back().accumStr.length() != 0)
				Stack.back().accum = 1;
			else
				Stack.back().accum = 0;
		else
			//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		if (Load.toDouble() == 0 && Stack.back().accum == 0)
			Stack.back().accum = 0;
		else
			Stack.back().accum = 1;
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}

	else {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void		ALU::fu_xor(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	if (Load.Type >> 1 == Dstring)
	{
		if (Stack.back().accumType >> 1 == Dstring)
			if (Load.toStr().length() != 0 || Stack.back().accumStr.length() != 0)
				Stack.back().accum = 1;
			else
				Stack.back().accum = 0;
		else
			//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		if ((Load.toDouble() != 0 && Stack.back().accum == 0) || (Load.toDouble() == 0 && Stack.back().accum != 0))
			Stack.back().accum = 1;
		else
			Stack.back().accum = 0;
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}

	else {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void		ALU::fu_or_bit(LoadPoint Load)//bit побитовые логические операции
{
	{
		if (Load.Point == nullptr) {
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
			return;
		}
		else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
		{

			Stack.back().accum = static_cast<int>(Stack.back().accum) | Load.toInt();
			if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
				Stack.back().accumType = Cint;
			else
				Stack.back().accumType = max(Stack.back().accumType, Load.Type);
		}

		else {
			//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
		}
	}
}

void		ALU::fu_and_bit(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = static_cast<int>(Stack.back().accum) & Load.toInt();
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}

	else {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void		ALU::fu_mr_bit(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = static_cast<int>(Stack.back().accum) >> Load.toInt();
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}

	else {
		return;
	}
}

void		ALU::fu_ml_bit(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = static_cast<int>(Stack.back().accum) << Load.toInt();
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}

	else {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}

void		ALU::fu_inv_bit(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = ~Load.toInt();
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}

	else {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
}
void		ALU::Compar3Way(LoadPoint Load) {// Трехстороннее сравнение
	if (Load.Point == nullptr) {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = ((Stack.back().accum > 0) ? 1 : ((Stack.back().accum == Load.toDouble()) ? 0 : -1));

		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	
}
void ALU::length(LoadPoint Load) {
	if (accumType >> 1 == DLoadVect || accumType >> 1 == Dstring)
		Load.Write(Stack.back().accumVect->size());
	else
		Load.Write(0);
}
void ALU::lenMk(LoadPoint Load) {
	int l;
	if (accumType >> 1 == DLoadVect || accumType >> 1 == Dstring) {
		l = Stack.back().accumVect->size();
		MkExec(Load, { Cint,&l });
	}
	else {
		l = 0;
		MkExec(Load, { Cint,&l });
	}
}
void ALU::clear(LoadPoint Load) {
	if (Load.Point == nullptr) {

		return;
	}
	else if (Load.isStr()) {

		Stack.back().accumType >> 1 == Dstring;
		string s;
		Stack.back().accumStr = s;
	}
	else if (Load.Type >> 1 == DLoadVect) {
		Stack.back().accumType >> 1 == DLoadVect;
		vector<LoadPoint> v;
		*Stack.back().accumVect = v;
	}
}
void ALU::push_back(LoadPoint Load) {
	if (Load.Point == nullptr) {
		return;
	}
	else if (Load.isVector(accumType)) {

		if (Load.isDigitBool() || Load.isStr()) {
			Stack.back().accumVect->push_back(Load);
		}

	}
	else if (Load.IsStr(accumType) && (Load.isStrChar(accumType)))
		Stack.back().accumStr += Load.toStr();
}

void ALU::emplace_back(LoadPoint Load) {
	if (Load.Point == nullptr) {

		return;
	}
	else if (accumType >> 1 == DLoadVect) {

		if (Load.isDigitBool() || Load.isStr()) {
			Stack.back().accumVect->emplace_back(Load);
		}
	}
}
void ALU::pop_back(LoadPoint Load) {
	if (Stack.back().accumVect->empty() && Stack.back().accumStr.empty()) {
		return;
	}
	else if (accumType >> 1 == DLoadVect) {
		Load.Write(Stack.back().accumVect->back());
		Stack.back().accumVect->pop_back();
	}
	Load.Write(Stack.back().accumStr.back());
	Stack.back().accumStr.pop_back();
}
void ALU::insert(LoadPoint Load) {
	if (Load.Point == nullptr || Stack.back().IndF == false) {
		return;
	}
	else{

		Stack.back().accumVect->insert( Stack.back().accumVect->begin()+ Stack.back().Ind, Load.Clone());
	}
}
void ALU::emplace(LoadPoint Load) {
	if (Load.Point == nullptr) {
		return;
	}
	else if(accumType>>1 == DLoadVect) {

		Stack.back().accumVect->emplace( Stack.back().accumVect->begin() + Stack.back().Ind, Load.Clone());

	}
	else if (Load.isChar()) {
		Stack.back().accumStr.insert(Stack.back().Ind, Load.toStr());
	}
}

void ALU::pop_backMk(LoadPoint Load) {
	
	if (!Stack.back().accumVect->empty()) {
		return;
	}
	MkExec(Load, { Cdouble, &Stack.back().accumVect });
	Stack.back().accumVect->pop_back();

}

void ALU::pop(LoadPoint Load) { //???
	
	if (Stack.back().accumVect->empty() && Stack.back().accumStr.empty()) {
		return;
	}
	if (Load.isInt()) {
		if (accumType >> 1 == DLoadVect)
			Stack.back().accumVect->erase(Stack.back().accumVect->begin() + Load.toDouble(), Stack.back().accumVect->end());
		else
			Stack.back().accumStr.erase(Stack.back().accumStr.begin() + Load.toDouble(), Stack.back().accumStr.end());
	}
}

void ALU::del(LoadPoint Load) {

	if (Stack.back().accumVect->empty() && Stack.back().accumStr.empty()) {
		return;
	}
	if (Load.isInt()) {
		if (accumType >> 1 == DLoadVect)
			Stack.back().accumVect->erase(Stack.back().accumVect->begin() + Load.toDouble());
		else
			Stack.back().accumStr.erase(Stack.back().accumStr.begin() + Load.toDouble());
	}
}

void ALU::Reverse(LoadPoint Load) {

	if (Stack.back().accumVect->empty() && Stack.back().accumStr.empty()) {
		return;
	}
	if (accumType >> 1 == DLoadVect) {
		reverse(Stack.back().accumVect->begin(), Stack.back().accumVect->end());
	}
	reverse(Stack.back().accumStr.begin(), Stack.back().accumStr.end());
	
}

void ALU::Sort(LoadPoint Load, bool revers) {
	if (Load.Point != nullptr)
		set(Load); // Если нагрузка непустая, то установить аккумулятор

	if (Stack.back().accumVect->empty() ||  Stack.back().accumStr.empty()) {
		return;
	}
	if (accumType >> 1 == DLoadVect) {
		sort(Stack.back().accumVect->begin(), Stack.back().accumVect->end(), [&revers]( LoadPoint  &x, LoadPoint  &y)->bool {
			if (x.isDigit() && y.isDigit()) {
				return revers ? x.toDouble() > y.toDouble() : x.toDouble() < y.toDouble();
			}
			if (x.isStr() && y.isStr()) {
				return revers ? x.toStr() > y.toStr(): x.toStr() < y.toStr();
			}
			return false;
			});
	}
	else if (accumType >> 1 == Dstring)
	{
		sort(Stack.back().accumStr.begin(), Stack.back().accumStr.end());
	}
	else // Ошибка типов
	{
		ProgExec(ErrProg);
		ProgExec(TypeMismatchErrProg);
	}
}
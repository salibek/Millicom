#include "stdafx.h"
#include "AluGeneral.h"
#include "Accumulator.h"
#include "EnumMk.h"
#include <string>
#include <cmath>
#include <algorithm>

bool		Accumulator::getSign() // 0 - положительное число, 1 - отрицательное
{
	switch (accumulatorOld.Type >> 1)
	{
	case Dint:
	{
	return *(int*)accumulatorOld.Point < 0;
	break; }
	case Dchar:
		return 0;
	case Dfloat:
		return *((float*)accumulatorOld.Point) < 0.0;
	case Ddouble:
		return *((double*)accumulatorOld.Point) < 0.0;
	default:
		return 0;
	}
}

void		Accumulator::EQ(LoadPoint Load)
{
	sub(Load);
	bool* t = new bool(!getLogic());
	set({Cbool,t});
};
void		Accumulator::NotEQ(LoadPoint Load)
{
	sub(Load);
	bool* t = new bool(getLogic());
	set({ Cbool,t });
};
void		Accumulator::Bigger(LoadPoint Load)
{
	sub(Load);
	bool* t = new bool(!getSign() && getLogic());

	set({ Cbool,t });
};
void		Accumulator::BiggerEQ(LoadPoint Load)
{
	sub	(Load);
	bool* t = new bool(!getSign());
	set({ Cbool,t });
};
void		Accumulator::Smaller(LoadPoint Load)
{
	sub(Load);
	bool* t = new bool(getSign());
	set({ Cbool,t });
};
void		Accumulator::SmallerEQ(LoadPoint Load)
{
	sub(Load);
	bool* t = new bool(getSign() || !getLogic());
	set({ Cbool,t });
};

void		Accumulator::Remainder(LoadPoint Load)
{
	sub(Load);
	bool* t = new bool(getSign() || !getLogic());
	set({ Cbool,t });
};


void		Accumulator::Clear() // ������� ������������
{
	if (accumulatorOld.Point == nullptr || accumulatorOld.Type == 0) return;
	switch (accumulatorOld.Type>>1)
	{
	case Dint:
		delete (int*)accumulatorOld.Point;	  break;
	case Dchar:
		delete (char*)accumulatorOld.Point;  break;
	case Dfloat:
		delete (float*)accumulatorOld.Point; break;
	case Ddouble:
		delete (double*)accumulatorOld.Point;break;
	case Dbool:
		delete (bool*)accumulatorOld.Point;  break;
	case Dstring:
		delete (string*)accumulatorOld.Point;break;
	}
	accumulatorOld.Type = 0;
}

void	Accumulator::error_msg(int error_code)
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

void	Accumulator::calc(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case E_MK::RESET_A: // Reset
		// ������� ������������
		accumulatorOld.Type = Cint;
		accumulatorOld.Point = new int(0);
		break;
	case E_MK::SET: // Set
		set(Load);
		break;
	case E_MK::INC: // inc
		inc(Load);
		set(Load);
		break;
	case E_MK::DEC: // dec
		dec(Load);
		set(Load);
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
		fu_cos();
		break;
	case E_MK::SIN:
		fu_sin();
		break;
	case E_MK::TAN:
		fu_tan();
		break;
	case E_MK::ACOS:
		fu_acos();
		break;
	case E_MK::ASIN:
		fu_asin();
		break;
	case E_MK::ATAN:
		fu_atan();
		break;
	case E_MK::MOD:
		fu_mod(Load);
		break;
	case E_MK::SQRT:
		fu_sqrt();
		break;
	case E_MK::POW:
		fu_pow(Load);
		break;
	case E_MK::ABS:
		fu_abs();
		break;
	case E_MK::CEIL:
		fu_ceil();
		break;
	case E_MK::FLOOR:
		fu_floor();
		break;
	case E_MK::ROUND:
		fu_round();
		break;
	case E_MK::LOG:
		fu_log();
		break;
	case E_MK::RANDOM:
		fu_random();
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
	case E_MK::Compar3Way:
		Compar3Way(Load);
	default:
		error_msg(1);
		break;
	}
}

void Accumulator::XOR_BIT(LoadPoint Load)
{
	switch (accumulatorOld.Type >> 1)
	{
	case Dint :
		*(int*)accumulatorOld.Point^=Load.ToInt();
		break;
	case Dchar:
		*(char*)accumulatorOld.Point ^= Load.ToChar();
		break;
	}
}

void Accumulator::inc(LoadPoint Load)
{
	switch (Load.Type >> 1)
	{
	case Dint:
		(*(int*)Load.Point)++;
		break;
	case Dchar:
		(*(char*)Load.Point)++;
		break;
	case Dfloat:
		(*(float*)Load.Point)++;
		break;
	case Ddouble:
		(*(double*)Load.Point)++;
		break;
	}
}

void Accumulator::dec(LoadPoint Load) // Декреминт если нагрузки нет, то декремируется аккумулятор
{
	switch (Load.Type >> 1)
	{
	case Dint:
		(*(int*)Load.Point)--;
		break;
	case Dchar:
		(*(char*)Load.Point)--;
		break;
	case Dfloat:
		(*(float*)Load.Point)--;
		break;
	case Ddouble:
		(*(double*)Load.Point)--;
		break;
	}
}

void	Accumulator::add(LoadPoint Load) // Сложение
{
	if (Load.Point == nullptr) {
		((ALUGeneral*)Parent)->ProgExec(((ALUGeneral*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	if (Load.isDigitBool() && Load.isDigitBool(accumType))
	{
		accumulator += Load.ToDouble();
		if (accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			accumType = Cint;
		else
			accumType = max(accumType,Load.Type);
	}
	else if (Load.Type >> 1 == Dstring)
		accumulatorStr += Load.ToStr();
	/*
	char		tmp = accumulatorOld.Type;
	char		tmp2 = Load.Type;
	char		r_tmp = w_type[tmp] > w_type[tmp2] ? tmp : tmp2;

	if (((tmp == Tstring || tmp == Cstring) && (tmp2 != Tstring || tmp2 != Cstring)))
		error_msg(2);
	switch (r_tmp>>1)
	{
	case Dint:
	{int *a=new int(accumulatorOld.ToInt());
//	*a = accumulator.ToInt();
	*a += Load.ToInt();
	accumulatorOld.Clear();
	unsigned int t = accumulatorOld.Type;
	accumulatorOld = { t, (void*)a };
	break; }
	case Dchar:
	{char* a = (char*)Load.Point, * b = (char*)accumulatorOld.Point;
	*a = *a + *b;
	accumulatorOld.Point = (void*)a;
	break; }
	case Dfloat:
	{float* a = (float*)Load.Point, * b = (float*)accumulatorOld.Point;
	*a = *a + *b;
	accumulatorOld.Point = (void*)a;
	break; }
	case Ddouble:
	{double* a = (double*)Load.Point, * b = (double*)accumulatorOld.Point;
	*a = *a + *b;
	accumulatorOld.Point = (void*)a;
	break; }
	}
	accumulatorOld.Type = r_tmp;
	*/
}

void	Accumulator::sub(LoadPoint Load)
{
	char		tmp = accumulatorOld.Type;
	char		tmp2 = Load.Type;
	char		r_tmp = tmp > tmp2 ? tmp : tmp2;

	if (((tmp == Tstring || tmp == Cstring || tmp2 == Tstring || tmp2 == Cstring)))
		error_msg(2);
	switch (r_tmp>>1)
	{
	case Dint:
	{int* b = (int*)Load.Point, * a = (int*)accumulatorOld.Point;
	*a = *a - *b;
	accumulatorOld.Point = (void*)a;
	break; }
	case Dchar:
	{char* a = (char*)Load.Point, * b = (char*)accumulatorOld.Point;
	*a = *a - *b;
	accumulatorOld.Point = (void*)a;
	break; }
	case Dfloat:
	{float* a = (float*)Load.Point, * b = (float*)accumulatorOld.Point;
	*a = *a - *b;
	accumulatorOld.Point = (void*)a;
	break; }
	case Ddouble:
	{double* a = (double*)Load.Point, * b = (double*)accumulatorOld.Point;
	*a = *a - *b;
	accumulatorOld.Point = (void*)a;
	break; }
	}
	accumulatorOld.Type = r_tmp;
}

void	Accumulator::mult(LoadPoint Load)
{
	char		tmp = accumulatorOld.Type;
	char		tmp2 = Load.Type;
	char		r_tmp = w_type[tmp] > w_type[tmp2] ? tmp : tmp2;

	if (((tmp == Tstring || tmp == Cstring || tmp2 == Tstring || tmp2 == Cstring)))
		error_msg(2);
	switch (r_tmp>>1)
	{
	case Dint:
	{int* b = (int*)Load.Point, * a = (int*)accumulatorOld.Point;
	*a = *a * *b;
	accumulatorOld.Point = (void*)a;
	break; }
	case Dchar:
	{char* a = (char*)Load.Point, * b = (char*)accumulatorOld.Point;
	*a = *a * *b;
	accumulatorOld.Point = (void*)a;
	break; }
	case Dfloat:
	{float* a = (float*)Load.Point, * b = (float*)accumulatorOld.Point;
	*a = *a * *b;
	accumulatorOld.Point = (void*)a;
	break; }
	case Ddouble:
	{double* a = (double*)Load.Point, * b = (double*)accumulatorOld.Point;
	*a = *a * *b;
	accumulatorOld.Point = (void*)a;
	break; }
	}
	accumulatorOld.Type = r_tmp;
}

void	Accumulator::div(LoadPoint Load)
{
	if (!Load.isDigit())
	{
		// Сообщение об ошибке...
		return;

	}
	char		tmp = accumulatorOld.Type;
//	char		tmp2 = w_type[Load.Type];
//	char		r_tmp = w_type[tmp] > w_type[tmp2] ? tmp : tmp2;

//	if (((tmp == Tstring || tmp == Cstring || tmp2 == Tstring || tmp2 == Cstring)))
//		error_msg(2);
//	else if ((int)Load.Point == 0)
//		error_msg(3);
	if (accumulatorOld.Type >> 1 != Ddouble)
	{
		double* t = new double();
		*t = accumulatorOld.ToDouble(0);
		accumulatorOld.VarDel();
		accumulatorOld.Type = Tdouble;
		accumulatorOld.Point = t;
	}
	*(double*)accumulatorOld.Point /= Load.ToDouble();
//	cout << "Accum: "<<accumulator.ToDouble() << endl;;
	/*
	switch (r_tmp>>1)
	{
	case Dint:
	{int* b = (int*)Load.Point, * a = (int*)accumulator.Point;

	*a = *a / *b;
	accumulator.Point = (void*)a;
	break; }
	case Dchar:
	{char* a = (char*)Load.Point, * b = (char*)accumulator.Point;
	*a = *a / *b;
	accumulator.Point = (void*)a;
	break; }
	case Dfloat:
	{float* a = (float*)Load.Point, * b = (float*)accumulator.Point;
	*a = *a / *b;
	accumulator.Point = (void*)a;
	break; }
	case Ddouble:
	{double* a = (double*)Load.Point, * b = (double*)accumulator.Point;
	*a = *a / *b;
	accumulator.Point = (void*)a;
	break; }
	}
	accumulator.Type = r_tmp;
	*/
}

void	Accumulator::div_int(LoadPoint Load)
{
	char		tmp = accumulatorOld.Type;
	char		tmp2 = w_type[Load.Type];

	if (((tmp == Tstring || tmp == Cstring || tmp2 == Tstring || tmp2 == Cstring)))
		error_msg(2);
	else if (Load.ToInt() == 0)
		error_msg(3);
	int* t = new int;
	int tt = accumulatorOld.ToInt() / Load.ToInt();
	*t = tt;
	accumulatorOld = {Tint, (void*)t };
}

LoadPoint Accumulator::get()
{
	return accumulatorOld;
//	accumulator.print();
}

void Accumulator::Out(LoadPoint Load) // �������� ����������� � ����������
{
	if (Load.Point == nullptr) return;
	switch (Load.Type>>1)
	{
		case Dint:
			if ((accumulatorOld.Type >> 1) == Dint)
				*(int*) Load.Point = *(int*)accumulatorOld.Point;
			break;
	}
}

void* Accumulator::VarNew(LoadPoint Load)
{
	switch (Load.Type>>1)
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

void		Accumulator::set(LoadPoint Load) // Установить значение в аккумулятор
{
	if (Load.isDigitBool())
		accumulator = Load.ToDouble();
	else if (Load.Type >> 1 == Dstring)
		accumulatorStr = Load.ToStr();
	else // Работа с вектором и матрицами
		;

//	if (Load.Point==nullptr)
//		{accumulatorOld = { Tbool, new int(0) }; return;}
//	accumulatorOld.Clear();
//	accumulatorOld.Type = Load.Type;
//	accumulatorOld.Point = VarNew(Load);
}

bool		Accumulator::getLogic() // ����������� ����������� ����
{
	int		type = accumulatorOld.Type;

	switch (type>>1)
	{
	case Dint:
		return *(int*)accumulatorOld.Point;
	case Dfloat:
		return *(float*)accumulatorOld.Point;
	case Ddouble:
		return *(double*)accumulatorOld.Point;
	case Dchar:
		return *(char*)accumulatorOld.Point;
	case Dbool:
		return *(bool*)accumulatorOld.Point;
	case Dstring:
		return *(string*)accumulatorOld.Point!="";
	}
}

bool		Accumulator::getLogic(LoadPoint Load) // ����������� ����������� ����
{
	int		type = Load.Type;

	switch (type >> 1)
	{
	case Dint:
		return *(int*)accumulatorOld.Point;
	case Dfloat:
		return *(float*)accumulatorOld.Point;
	case Ddouble:
		return *(double*)accumulatorOld.Point;
	case Dchar:
		return *(char*)accumulatorOld.Point;
	case Dbool:
		return *(bool*)accumulatorOld.Point;
	case Dstring:
		return *(string*)accumulatorOld.Point != "";
	}
}

void	Accumulator::fu_max(LoadPoint Load)
{
	char		tmp1 = accumulatorOld.Type;
	char		tmp2 = Load.Type;
	char		r_tmp = 0;
	if (tmp2 >> 1 == Ddouble || tmp1 >> 1 == Ddouble)
		r_tmp = Cdouble;
	else if(tmp2 >> 1 == Dfloat || tmp1 >> 1 == Dfloat)
		r_tmp = Cfloat;
	else if (tmp2 >> 1 == Dint || tmp1 >> 1 == Dint)
		r_tmp = Cint;
	else if (tmp2 >> 1 == Dstring && tmp1 >> 1 == Dstring)
		r_tmp = Dstring;
	else
		error_msg(2);
//	else if ((int)Load.Point == 0)
//		error_msg(3);

//	char		tmp2 = w_type[Load.Type];
	//	char		r_tmp = w_type[tmp] > w_type[tmp2] ? tmp : tmp2;

	double a_old = accumulatorOld.ToDouble(); // Старое значение аккумулятора

//	if (((tmp == Tstring || tmp == Cstring || tmp2 == Tstring || tmp2 == Cstring)))
//		error_msg(2);
//	else if ((int)Load.Point == 0)
//		error_msg(3);

	switch (r_tmp >> 1)
	{
	case Dint:
	{//int b = Load.ToInt(), a = accumulator.Point;
	int b = Load.ToInt();
	if (b > accumulatorOld.ToInt())
	{
		accumulatorOld.Clear();
		int* a = new int(b);
		accumulatorOld = { Cint,(void*)a };
	}
	break; }
	case Dchar:
	{char* a = (char*)Load.Point, * b = (char*)accumulatorOld.Point;
	*a = *a > * b ? *a : *b;
	accumulatorOld.Point = (void*)a;
	break; }
	case Dfloat:
	{float* a = (float*)Load.Point, * b = (float*)accumulatorOld.Point;
	*a = *a > * b ? *a : *b;
	accumulatorOld.Point = (void*)a;
	break; }
	case Ddouble:
	{double* a = (double*)Load.Point, * b = (double*)accumulatorOld.Point;
	*a = *a > * b ? *a : *b;
	accumulatorOld.Point = (void*)a;
	break; 
	}
	case Dstring:
		string* s1 = (string*)Load.Point, * s2 = (string*)accumulatorOld.Point;
		*s1 = *s1 > * s2 ? *s1 : *s2;
		accumulatorOld.Point = (string*)s1;
	}
	accumulatorOld.Type = r_tmp;
	if(accumulatorOld.ToDouble()> a_old)
		((FU*)((FU*)Parent))->ProgExec(((ALUGeneral*)Parent)->ThreadStack.back().MaxProg);
}

void	Accumulator::fu_min(LoadPoint Load)
{
	char		tmp = accumulatorOld.Type;
	char		tmp2 = w_type[Load.Type];
	char		r_tmp = w_type[tmp] > w_type[tmp2] ? tmp : tmp2;

	double a_old = accumulatorOld.ToDouble(); // Старое значение аккумулятора

	if (((tmp == Tstring || tmp == Cstring || tmp2 == Tstring || tmp2 == Cstring)))
		error_msg(2);
	else if (*(int*)Load.Point == 0)
		error_msg(3);
	switch (r_tmp >> 1)
	{
	case Dint:
	{int* b = (int*)Load.Point, * a = (int*)accumulatorOld.Point;
	*a = *a < * b ? *a : *b;
	accumulatorOld.Point = (void*)a;
	break; }
	case Dchar:
	{char* a = (char*)Load.Point, * b = (char*)accumulatorOld.Point;
	*a = *a < * b ? *a : *b;
	accumulatorOld.Point = (void*)a;
	break; }
	case Dfloat:
	{float* a = (float*)Load.Point, * b = (float*)accumulatorOld.Point;
	*a = *a < * b ? *a : *b;
	accumulatorOld.Point = (void*)a;
	break; }
	case Ddouble:
	{double* a = (double*)Load.Point, * b = (double*)accumulatorOld.Point;
	*a = *a < * b ? *a : *b;
	accumulatorOld.Point = (void*)a;
	break;
	}
	case Dstring:
		string* s1 = (string*)Load.Point, * s2 = (string*)accumulatorOld.Point;
		*s1 = *s1 < * s2 ? *s1 : *s2;
		accumulatorOld.Point = (string*)s1;
	}
	accumulatorOld.Type = r_tmp;
	if (accumulatorOld.ToDouble() > a_old)
		((FU*)((FU*)Parent))->ProgExec(((ALUGeneral*)Parent)->ThreadStack.back().MaxProg);
}

void	Accumulator::fu_cos()
{
	char		tmp = accumulatorOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	*a = cos(*a);
	accumulatorOld.Point = (void *)a;
	accumulatorOld.Type = Tdouble;
}

void	Accumulator::fu_sin()
{
	char		tmp = accumulatorOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	*a = sin(*a);
	accumulatorOld.Point = (void*)a;
	accumulatorOld.Type = Tdouble;
}

void	Accumulator::fu_acos()
{
	char		tmp = accumulatorOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	*a = acos(*a);
	accumulatorOld.Point = (void*)a;
	accumulatorOld.Type = Tdouble;
}

void	Accumulator::fu_asin()
{
	char		tmp = accumulatorOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	*a = asin(*a);
	accumulatorOld.Point = (void*)a;
	accumulatorOld.Type = Tdouble;
}

void	Accumulator::fu_tan()
{
	char		tmp = accumulatorOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	*a = tan(*a);
	accumulatorOld.Point = (void*)a;
	accumulatorOld.Type = Tdouble;
}

void	Accumulator::fu_atan()
{
	char		tmp = accumulatorOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	*a = cos(*a);
	accumulatorOld.Point = (void*)a;
	accumulatorOld.Type = Tdouble;
}

void	Accumulator::fu_mod(LoadPoint Load)
{
	char		tmp = accumulatorOld.Type;
	char		tmp2 = Load.Type;

	if ((tmp >> 1) != Dint || (tmp2 >> 1) != Dint)
		error_msg(2);
	
	int* b = (int*)Load.Point;
	int* a = (int*)accumulatorOld.Point;
	*a = *a % *b;
	accumulatorOld.Point = (void*)a;
	accumulatorOld.Type = Tint;
}

void	Accumulator::fu_sqrt()
{
	char		tmp = accumulatorOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	*a = sqrt(*a);
	accumulatorOld.Point = (void*)a;
	accumulatorOld.Type = Tdouble;
}

void	Accumulator::fu_pow(LoadPoint Load)
{
	char		tmp = accumulatorOld.Type;
	char		tmp2 = Load.Type;

	if ((tmp >> 1) == Dstring || (tmp2 >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	double* b = (double*)Load.Point;
	*a = pow(*a, *b);
	accumulatorOld.Point = (void*)a;
	accumulatorOld.Type = Tdouble;
}

void	Accumulator::fu_abs()
{
	char		tmp = accumulatorOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	*a = abs(*a);
	accumulatorOld.Point = (void*)a;
	accumulatorOld.Type = tmp;
}

void	Accumulator::fu_ceil()
{
	char		tmp = accumulatorOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	*a = ceil(*a);
	accumulatorOld.Point = (void*)a;
	accumulatorOld.Type = tmp;
}

void	Accumulator::fu_floor()
{
	char		tmp = accumulatorOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	*a = floor(*a);
	accumulatorOld.Point = (void*)a;
	accumulatorOld.Type = tmp;
}

void	Accumulator::fu_round()
{
	char		tmp = accumulatorOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	*a = round(*a);
	accumulatorOld.Point = (void*)a;
	accumulatorOld.Type = tmp;
}

void	Accumulator::fu_log()
{
	char		tmp = accumulatorOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	*a = log(*a);
	accumulatorOld.Point = (void*)a;
	accumulatorOld.Type = Tdouble;
}

void	Accumulator::fu_random()
{
	char		tmp = accumulatorOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	*a = rand();
	accumulatorOld.Point = (void*)a;
	accumulatorOld.Type = tmp;
}

LoadPoint	Accumulator::getCos()
{
	char		tmp = accumulatorOld.Type;
	LoadPoint   Load;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	*a = cos(*a);
	Load.Type = Ddouble;
	Load.Point = (void*)a;
	return Load;
}

LoadPoint	Accumulator::getSin()
{
	char		tmp = accumulatorOld.Type;
	LoadPoint   Load;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)accumulatorOld.Point;
	*a = sin(*a);
	Load.Type = Ddouble;
	Load.Point = (void*)a;
	return Load;
}


void		Accumulator::fu_inverse(LoadPoint Load)
{
	bool		t = !getLogic();
	accumulatorOld.Point = new bool(*(bool*)Load.Point);
	accumulatorOld.Type = Tbool;

	bool		*val = (bool *)accumulatorOld.Point;
	*val = t;
}

void		Accumulator::fu_and(LoadPoint Load)
{
	bool t1 = getLogic();
	bool t2 = getLogic(Load);

	accumulatorOld.Point = new bool(*(bool*)Load.Point);
	accumulatorOld.Type = Tbool;
	*(bool*)accumulatorOld.Point = t1 && t2;
}

void		Accumulator::fu_or(LoadPoint Load)
{
	bool t1 = getLogic();
	bool t2 = getLogic(Load);

	accumulatorOld.Point = new bool(*(bool*)Load.Point);
	accumulatorOld.Type = Tbool;
	*(bool*)accumulatorOld.Point = t1 || t2;
}

void		Accumulator::fu_xor(LoadPoint Load)
{
	bool t1 = getLogic();
	bool t2 = getLogic(Load);

	accumulatorOld.Point = new bool(*(bool*)Load.Point);
	accumulatorOld.Type = Tbool;
	*(bool*)accumulatorOld.Point = (!t1 || !t2) && (t1 || t2);
}

void		Accumulator::fu_or_bit(LoadPoint Load)
{
	char t1 = Load.Type >> 1;
	char t2 = accumulatorOld.Type >> 1;

	if (t1 == Dstring || t2 == Dstring || t1 == Ddouble || t2 == Ddouble
		|| t1 == Dfloat || t2 == Dfloat)
		error_msg(2);
	else
	{
		int		*a = (int *)accumulatorOld.Point;
		int		*b = (int *)Load.Point;

		*a = *a | *b;
	}
}

void		Accumulator::fu_and_bit(LoadPoint Load)
{
	char t1 = Load.Type >> 1;
	char t2 = accumulatorOld.Type >> 1;

	if (t1 == Dstring || t2 == Dstring || t1 == Ddouble || t2 == Ddouble
		|| t1 == Dfloat || t2 == Dfloat)
		error_msg(2);
	else
	{
		int		*a = (int *)accumulatorOld.Point;
		int		*b = (int *)Load.Point;

		*a = *a & *b;
	}
}

void		Accumulator::fu_mr_bit(LoadPoint Load)
{
	char t1 = Load.Type >> 1;
	char t2 = accumulatorOld.Type >> 1;

	if (t1 == Dstring || t2 == Dstring || t1 == Ddouble || t2 == Ddouble
		|| t1 == Dfloat || t2 == Dfloat)
		error_msg(2);
	else
	{
		int		*a = (int *)accumulatorOld.Point;
		int		*b = (int *)Load.Point;

		*a = *a >> *b;
	}
}

void		Accumulator::fu_ml_bit(LoadPoint Load)
{
	char t1 = Load.Type >> 1;
	char t2 = accumulatorOld.Type >> 1;

	if (t1 == Dstring || t2 == Dstring || t1 == Ddouble || t2 == Ddouble
		|| t1 == Dfloat || t2 == Dfloat)
		error_msg(2);
	else
	{
		int		*a = (int *)accumulatorOld.Point;
		int		*b = (int *)Load.Point;

		*a = *a << *b;
	}
}

void		Accumulator::fu_inv_bit(LoadPoint Load)
{
	char t1 = Load.Type >> 1;
	char t2 = accumulatorOld.Type >> 1;

	if (t1 == Dstring || t2 == Dstring || t1 == Ddouble || t2 == Ddouble
		|| t1 == Dfloat || t2 == Dfloat)
		error_msg(2);
	else
	{
		int		*a = (int *)accumulatorOld.Point;
		int		*b = (int *)Load.Point;

		*a = ~*a;
	}
}
void		Accumulator::Compar3Way(LoadPoint Load){// Трехстороннее сравнение
	double T1 = accumulatorOld.ToDouble();
	double T2 = Load.ToDouble();
	accumulatorOld.Write((T1>0)? 1:((T1==T2)?0:-1) );
}
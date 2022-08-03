#include "stdafx.h"
#include "ALU.h"
#include "EnumMk.h"
#include <string>
#include <cmath>
#include <algorithm>

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
	if (MK == ProgExecMk || MK == CalcMk) // выполнение программы
	{
		ProgExec(Load);
		//if (Stack.size() == 1)
		accum = Stack.back().accum; // Записать в выходной аккумулятор
		accumType = Stack.back().accumType; // Записать в выходной аккумулятор
		accumStr = Stack.back().accumStr; // Записать в выходной аккумулятор
		accumPoint = Stack.back().accumPoint; // Записать в выходной аккумулятор
		for (auto i : Stack.back().MkOut) // Выполнить все МК
			MkExec(i, { Cdouble,  &Stack.back().accum }); // Сделать преобразование типов попозже
		for (auto i : Stack.back().OutAdr) // Записать результат во все выходные переменные
			i.Write(Stack.back().accum);
	}
	else
	{
		if (MK >= 25 && Load.IsProg()) // Арифметико-логическсое выражение со ссылкой в нагрузке
		{
			Stack.push_back({});
			ProgExec(Load);
			double tt = Stack.back().accum;
			Load = { Cdouble,&tt }; // Запомнить значение вычисленного аккумулятора
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
		case 2: // OutMk Выдать МК со значением аккумулятора
			if (Stack.back().accumType >> 1 == Dstring)
				MkExec(Load, { Stack.back().accumType,&Stack.back().accumStr }, Bus, true);
			else
				MkExec(Load, { Cdouble,&Stack.back().accum }, Bus, true);
			break;
		case 1: // Out Выдать значение аккумулятора
			if (accumType >> 1 == Dstring)
				Load.Write(Stack.back().accumStr);
			else
				Load.Write(Stack.back().accum);
			break;
		case 3: // Push Сделать еще один уровень аккумулятора
			Stack.push_back({});
			Stack.back().AccumPopBlock = true;
			break;
		case 5: //AccumAdrOut Выдать адрес выходного аккумулятора
			Load.Write((void*)&Accum);
			break;
		case 6: //AccumAdrOutMk Выдать МК с адресом выходного аккумулятора
			MkExec(MK, { Cdouble,(void*)&Accum });
			break;
		case 10: // OutMkAdd Добавить адрес для выдачи результата вычисления
			Stack.back().MkOut.push_back(Load.ToInt());
			break;
		case 11: // OutMkClear Очистить буфер МК для выдачи результата
			Stack.back().MkOut.clear();
			break;
		case 12: // OutAdrAdd Добавить адрес для выдачи результатов
			Stack.back().OutAdr.push_back(Load);
			break;
		case 13: // OutAdrClear Очислить буфер адресов для записи выходного результата
			Stack.back().OutAdr.clear();
			break;
		case E_MK::RESET_A: // Reset
			// ������� ������������
			Stack.back().accumType = Cdouble;
			Stack.back().accum = 0;
			break;
		case 26:
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
			break;
		case E_MK::Compar3Way:
			Compar3Way(Load);
			break;
		default:
			CommonMk(MK, Load);
			break;
		}
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

		Stack.back().accum = static_cast<int>(Stack.back().accum) ^ Load.ToInt();
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}

	else {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}



	/*switch (Stack.back().accumType >> 1)
	{
	case Dint :
	{
		int t = round(Stack.back().accum);
		t ^= Load.ToInt();
		Stack.back().accum = t;
		break;
	}
	case Dchar:
	{
		char t = round(Stack.back().accum);
		t ^= Load.ToChar();
		Stack.back().accum=t;
		break;
	}
	}*/
}

void ALU::inc(LoadPoint Load)
{
	Stack.back().accum++;
}

void ALU::dec(LoadPoint Load) // Декреминт если нагрузки нет, то декремируется аккумулятор
{
	Stack.back().accum--;
}

void	ALU::add(LoadPoint Load) // Сложение
{
	if (Load.Point == nullptr) {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	if (Load.Type >> 1 == DLoadVect) {
		if (Stack.back().accumType >> 1 == DLoadVect) {
			
		}
	}
	else if (Load.Type >> 1 == Dstring)
	{
		if (Stack.back().accumType >> 1 == Dstring)
			Stack.back().accumStr += Load.ToStr();
		else
			//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum += Load.ToDouble();
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

void	ALU::sub(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum -= Load.ToDouble();
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
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
	if (Stack.back().accumType >> 1 == DLoadVect) {
	//	for (int i = 0; i < Stack.back().accumVect.size(); i++) {

	//	}
	}
	else if (Stack.back().accumType >> 1 == Dstring) {
		string k;
		int l;
		//l = Load.ToStr();
		l = Load.ToInt();
		//for (int i = 0; i < l.size(); i++)
		//	k += Stack.back().accumStr;
		for (int i = 0; i < l; i++)
			k += Stack.back().accumStr;
		Stack.back().accumStr = k;
	}
	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum *= Load.ToDouble();
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

void	ALU::div(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum /= Load.ToDouble();
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}

	/*
	if (!Load.isDigit())
	{
		// Сообщение об ошибке...
		return;

	}
	char		tmp = ALUOld.Type;
//	char		tmp2 = w_type[Load.Type];
//	char		r_tmp = w_type[tmp] > w_type[tmp2] ? tmp : tmp2;

//	if (((tmp == Tstring || tmp == Cstring || tmp2 == Tstring || tmp2 == Cstring)))
//		error_msg(2);
//	else if ((int)Load.Point == 0)
//		error_msg(3);
	if (ALUOld.Type >> 1 != Ddouble)
	{
		double* t = new double();
		*t = ALUOld.ToDouble(0);
		ALUOld.VarDel();
		ALUOld.Type = Tdouble;
		ALUOld.Point = t;
	}
	*(double*)ALUOld.Point /= Load.ToDouble();
//	cout << "Stack.back().accum: "<<ALU.ToDouble() << endl;;
	/*
	switch (r_tmp>>1)
	{
	case Dint:
	{int* b = (int*)Load.Point, * a = (int*)ALU.Point;

	*a = *a / *b;
	ALU.Point = (void*)a;
	break; }
	case Dchar:
	{char* a = (char*)Load.Point, * b = (char*)ALU.Point;
	*a = *a / *b;
	ALU.Point = (void*)a;
	break; }
	case Dfloat:
	{float* a = (float*)Load.Point, * b = (float*)ALU.Point;
	*a = *a / *b;
	ALU.Point = (void*)a;
	break; }
	case Ddouble:
	{double* a = (double*)Load.Point, * b = (double*)ALU.Point;
	*a = *a / *b;
	ALU.Point = (void*)a;
	break; }
	}
	ALU.Type = r_tmp;
	*/

}

void	ALU::div_int(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//		((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = static_cast<int>(Stack.back().accum) /Load.ToInt();
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}

	else {
		//		((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
	/*
	char		tmp = ALUOld.Type;
	char		tmp2 = w_type[Load.Type];

	if (((tmp == Tstring || tmp == Cstring || tmp2 == Tstring || tmp2 == Cstring)))
		error_msg(2);
	else if (Load.ToInt() == 0)
		error_msg(3);
	int* t = new int;
	int tt = ALUOld.ToInt() / Load.ToInt();
	*t = tt;
	ALUOld = {Tint, (void*)t };
	*/
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
	/*
	if (Load.Point == nullptr) return;
	switch (Load.Type>>1)
	{
		case Dint:
			if ((ALUOld.Type >> 1) == Dint)
				*(int*) Load.Point = *(int*)ALUOld.Point;
			break;
	}
	*/
}

void* ALU::VarNew(LoadPoint Load)//создать новую переменную
{

	switch (Load.Type >> 1)
	{
	case Dint:
		//Stack.back().accumType = Tint;
		return new int(*(int*)Load.Point);
	case Dfloat:
		//Stack.back().accumType = Tfloat;
		return new float(*(float*)Load.Point);
	case Ddouble:
		//Stack.back().accumType = Tdouble;
		return new double(*(double*)Load.Point);
	case Dchar:
		//Stack.back().accumType = Tchar;
		return new char(*(char*)Load.Point);
	case Dbool:
		//Stack.back().accumType = Tbool;
		return new bool(*(bool*)Load.Point);
	case Dstring:
		//Stack.back().accumType = Tstring;
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
		Stack.back().accumStr = Load.ToStr();
	}
	else if (Load.isDigitBool())
	{
		Stack.back().accum = Load.ToDouble();
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
		accumPoint = Stack.back().accumPoint;
	}
}

bool		ALU::getLogic() // ����������� ����������� ����//???
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
	/*
	int		type = Load.Type;

	switch (type >> 1)
	{
	case Dint:
		return *(int*)ALUOld.Point;
	case Dfloat:
		return *(float*)ALUOld.Point;
	case Ddouble:
		return *(double*)ALUOld.Point;
	case Dchar:
		return *(char*)ALUOld.Point;
	case Dbool:
		return *(bool*)ALUOld.Point;
	case Dstring:
		return *(string*)ALUOld.Point != "";
	}
	*/
	return false;
}

void	ALU::fu_max(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = max(Stack.back().accum, Load.ToDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
	/*
	char		tmp1 = ALUOld.Type;
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

	double a_old = ALUOld.ToDouble(); // Старое значение аккумулятора

//	if (((tmp == Tstring || tmp == Cstring || tmp2 == Tstring || tmp2 == Cstring)))
//		error_msg(2);
//	else if ((int)Load.Point == 0)
//		error_msg(3);

	switch (r_tmp >> 1)
	{
	case Dint:
	{//int b = Load.ToInt(), a = ALU.Point;
	int b = Load.ToInt();
	if (b > ALUOld.ToInt())
	{
		ALUOld.Clear();
		int* a = new int(b);
		ALUOld = { Cint,(void*)a };
	}
	break; }
	case Dchar:
	{char* a = (char*)Load.Point, * b = (char*)ALUOld.Point;
	*a = *a > * b ? *a : *b;
	ALUOld.Point = (void*)a;
	break; }
	case Dfloat:
	{float* a = (float*)Load.Point, * b = (float*)ALUOld.Point;
	*a = *a > * b ? *a : *b;
	ALUOld.Point = (void*)a;
	break; }
	case Ddouble:
	{double* a = (double*)Load.Point, * b = (double*)ALUOld.Point;
	*a = *a > * b ? *a : *b;
	ALUOld.Point = (void*)a;
	break;
	}
	case Dstring:
		string* s1 = (string*)Load.Point, * s2 = (string*)ALUOld.Point;
		*s1 = *s1 > * s2 ? *s1 : *s2;
		ALUOld.Point = (string*)s1;
	}
	ALUOld.Type = r_tmp;
	if(ALUOld.ToDouble()> a_old)
		((FU*)((FU*)Parent))->ProgExec(((Threader*)Parent)->ThreadStack.back().MaxProg);
		*/
}

void	ALU::fu_min(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = min(Stack.back().accum, Load.ToDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}

	/*
	char		tmp = ALUOld.Type;
	char		tmp2 = w_type[Load.Type];
	char		r_tmp = w_type[tmp] > w_type[tmp2] ? tmp : tmp2;

	double a_old = ALUOld.ToDouble(); // Старое значение аккумулятора

	if (((tmp == Tstring || tmp == Cstring || tmp2 == Tstring || tmp2 == Cstring)))
		error_msg(2);
	else if (*(int*)Load.Point == 0)
		error_msg(3);
	switch (r_tmp >> 1)
	{
	case Dint:
	{int* b = (int*)Load.Point, * a = (int*)ALUOld.Point;
	*a = *a < * b ? *a : *b;
	ALUOld.Point = (void*)a;
	break; }
	case Dchar:
	{char* a = (char*)Load.Point, * b = (char*)ALUOld.Point;
	*a = *a < * b ? *a : *b;
	ALUOld.Point = (void*)a;
	break; }
	case Dfloat:
	{float* a = (float*)Load.Point, * b = (float*)ALUOld.Point;
	*a = *a < * b ? *a : *b;
	ALUOld.Point = (void*)a;
	break; }
	case Ddouble:
	{double* a = (double*)Load.Point, * b = (double*)ALUOld.Point;
	*a = *a < * b ? *a : *b;
	ALUOld.Point = (void*)a;
	break;
	}
	case Dstring:
		string* s1 = (string*)Load.Point, * s2 = (string*)ALUOld.Point;
		*s1 = *s1 < * s2 ? *s1 : *s2;
		ALUOld.Point = (string*)s1;
	}
	ALUOld.Type = r_tmp;
	if (ALUOld.ToDouble() > a_old)
		((FU*)((FU*)Parent))->ProgExec(((Threader*)Parent)->ThreadStack.back().MaxProg);
		*/
}

void	ALU::fu_cos(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = cos(Load.ToDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
	/*
	char		tmp = ALUOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)ALUOld.Point;
	*a = cos(*a);
	ALUOld.Point = (void *)a;
	ALUOld.Type = Tdouble;
	*/
}

void	ALU::fu_sin(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = sin(Load.ToDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
	/*
	char		tmp = ALUOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)ALUOld.Point;
	*a = sin(*a);
	ALUOld.Point = (void*)a;
	ALUOld.Type = Tdouble;
	*/
}

void	ALU::fu_acos(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = acos(Load.ToDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
	/*
	char		tmp = ALUOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)ALUOld.Point;
	*a = acos(*a);
	ALUOld.Point = (void*)a;
	ALUOld.Type = Tdouble;
	*/
}

void	ALU::fu_asin(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = asin(Load.ToDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
	/*
	char		tmp = ALUOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)ALUOld.Point;
	*a = asin(*a);
	ALUOld.Point = (void*)a;
	ALUOld.Type = Tdouble;
	*/
}

void	ALU::fu_tan(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = tan(Load.ToDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
	/*
	char		tmp = ALUOld.Type;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)ALUOld.Point;
	*a = tan(*a);
	ALUOld.Point = (void*)a;
	ALUOld.Type = Tdouble;
	*/
}

void	ALU::fu_atan(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = atan(Load.ToDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
	/*
		char		tmp = ALUOld.Type;

		if ((tmp >> 1) == Dstring)
			error_msg(2);

		double* a = (double*)ALUOld.Point;
		*a = cos(*a);
		ALUOld.Point = (void*)a;
		ALUOld.Type = Tdouble;
		*/
}

void	ALU::fu_mod(LoadPoint Load)//остаток
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{

		Stack.back().accum = static_cast<int>(Stack.back().accum) % Load.ToInt();
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

void	ALU::fu_sqrt(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = sqrt(Load.ToDouble());
		if (Stack.back().accumType >> 1 == Dbool && Load.Type >> 1 == Dbool)
			Stack.back().accumType = Cint;
		else
			Stack.back().accumType = max(Stack.back().accumType, Load.Type);
	}
	else {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
	/*
		char		tmp = ALUOld.Type;

		if ((tmp >> 1) == Dstring)
			error_msg(2);

		double* a = (double*)ALUOld.Point;
		*a = sqrt(*a);
		ALUOld.Point = (void*)a;
		ALUOld.Type = Tdouble;
		*/
}

void	ALU::fu_pow(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = pow(Stack.back().accum, Load.ToDouble());
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

void	ALU::fu_abs(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = abs(Load.ToDouble());
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

void	ALU::fu_ceil(LoadPoint Load)//округление вверх
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = ceil(Load.ToDouble());
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

void	ALU::fu_floor(LoadPoint Load)//округление вниз
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = floor(Load.ToDouble());
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

void	ALU::fu_round(LoadPoint Load)//просто округление
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = round(Load.ToDouble());
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

void	ALU::fu_log(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = log(Load.ToDouble());
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

void	ALU::fu_random()
{

	Stack.back().accum = rand();
}

void	ALU::getCos(LoadPoint Load)
{

	/*
		char		tmp = ALUOld.Type;
		LoadPoint   Load;

		if ((tmp >> 1) == Dstring)
			error_msg(2);

		double* a = (double*)ALUOld.Point;
		*a = cos(*a);
		Load.Type = Ddouble;
		Load.Point = (void*)a;
		return Load;
		*/
}

void	ALU::getSin(LoadPoint Load)
{

	/*
	char		tmp = ALUOld.Type;
	LoadPoint   Load;

	if ((tmp >> 1) == Dstring)
		error_msg(2);

	double* a = (double*)ALUOld.Point;
	*a = sin(*a);
	Load.Type = Ddouble;
	Load.Point = (void*)a;
	return Load;
	*/
}


void		ALU::fu_inverse(LoadPoint Load)//обратное
{
	if (Load.Point == nullptr) {
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	if (Load.Type >> 1 == Dstring)
	{
		if (Load.ToStr().length() != 0)
			Stack.back().accum = 0;
		else
			Stack.back().accum = 1;
		return;
	}
	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		if (Load.ToDouble() == 0)
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
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}
	if (Load.Type >> 1 == Dstring)
	{
		if (Stack.back().accumType >> 1 == Dstring)
			if (Load.ToStr().length() != 0 && Stack.back().accumStr.length() != 0)
				Stack.back().accum = 1;
			else
				Stack.back().accum = 0;
		//else
		//	((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
		return;
	}
	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		if (Load.ToDouble() == 0 || Stack.back().accum == 0)
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
			if (Load.ToStr().length() != 0 || Stack.back().accumStr.length() != 0)
				Stack.back().accum = 1;
			else
				Stack.back().accum = 0;
		else
			//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		if (Load.ToDouble() == 0 && Stack.back().accum == 0)
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
			if (Load.ToStr().length() != 0 || Stack.back().accumStr.length() != 0)
				Stack.back().accum = 1;
			else
				Stack.back().accum = 0;
		else
			//((Threader*)Parent)->ProgExec(((Threader*)Parent)->UncompatableTypesErrProg);
			return;
	}
	else if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		if ((Load.ToDouble() != 0 && Stack.back().accum == 0) || (Load.ToDouble() == 0 && Stack.back().accum != 0))
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

			Stack.back().accum = static_cast<int>(Stack.back().accum) | Load.ToInt();
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
		Stack.back().accum = static_cast<int>(Stack.back().accum) & Load.ToInt();
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
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = static_cast<int>(Stack.back().accum) >> Load.ToInt();
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

void		ALU::fu_ml_bit(LoadPoint Load)
{
	if (Load.Point == nullptr) {
		//((Threader*)Parent)->ProgExec(((Threader*)Parent)->NoOperandErrProg); // Запуск подпрограммы ошибки "Нет операнда"
		return;
	}

	if (Load.isDigitBool() && Load.isDigitBool(Stack.back().accumType))
	{
		Stack.back().accum = static_cast<int>(Stack.back().accum) << Load.ToInt();
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
		Stack.back().accum = ~Load.ToInt();
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
		Stack.back().accum = ((Stack.back().accum > 0) ? 1 : ((Stack.back().accum == Load.ToDouble()) ? 0 : -1));

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
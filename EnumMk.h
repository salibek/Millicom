#pragma once


enum E_MK
{
	RESET,					// 0
	PROG,					// 1
	PROG_CYCLE,				// 2
	PROG_POST,				// 3
	PROG_BREAK,				// 4
	YES_PROG,				// 5	
	YES_PROG_CYCLE,			// 6
	YES_PROG_POST,			// 7
	YES_PROG_BREAK,			// 8
	NO_PROG,				// 9
	NO_PROG_CYCLE,			// 10
	NO_PROG_POST,			// 11
	NO_PROG_BREAK,			// 12
	BREAK,					// 13
	CONTINUE,				// 14
	CALC,					// 15
	DEEP_OUT,				// 16
	DEEP_OUT_MK,			// 17
	GO,						// 18
	YES_GO,					// 19
	NO_GO,					// 20
	OUT_SET,				// 21
	OUT_MK_SET,				// 22
	OUT,					// 23
	OUT_MK,					// 24
	
	// �������� � ������������� 
	RESET_A,				// 25
	SETA,					// 26
	GET,					// 27
	GET_LOGIC,				// 28
	OUT_A,					// 29
	SUM,					// 30
	SUB,					// 31
	DIV,					// 32
	MULT,					// 33
	INC,					// 34
	DEC,					// 35
	MIN,					// 36
	MAX,					// 37
	COS,					// 38
	SIN,					// 39
	TAN,					// 40
	ASIN,					// 41
	ACOS,					// 42
	ATAN,					// 43
	MOD,					// 44
	SQRT,					// 45
	POW,					// 46
	ABS,					// 47
	CEIL,					// 48
	FLOOR,					// 49
	ROUND,					// 50
	LOG,					// 51
	RANDOM,					// 52
	INV,					// 53
	AND,					// 54
	OR,						// 55
	DIV_INT,				// 56
	XOR,					// 57
	INV_BIT,				// 58
	OR_BIT, 				// 59
	AND_BIT,				// 60
	MR_BIT,					// 61
	ML_BIT,					// 62
	EQ,						// 63
	NotEQ,					// 64
	Bigger,					// 65
	BiggerEQ,				// 66
	Smaller,				// 67
	SmallerEQ,				// 68
	Remainder,				// 69
	XOR_BIT,				// 70
	Compar3Way,				// 71
	PUSH,					// 72
	
	SET = 26,					// 1
};
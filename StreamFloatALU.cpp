#include "StreamFloatALU.h"

using namespace std;

void StreamFloatALU::ProgFU(int MK, LoadPoint Load, FU* Sender)
switch (MK)
{
case 0: //Reset
Rez = 0;
OperandsCounter = 0;
Redy = false;
RezStack.clear();
ReseiverMk.clear();
ReseiverContext.clear();
AngleMode = 0;
break;
case 1: // RezSet ���������� ���������
Rez = Load.toDouble();
break;
case 5: //RezOut ������ ���������
Load.Write(Rez);
break;
case 6: //RezOutMk ������ �� � �����������
MKExec(Load, { Cdouble, &Rez });
break;
case 10: // ZProgSet
ZProg = Load.Point;
break;
case 20: // ZExec
if (Rez == 0)
ProgExec(Load);
case 30: // AngleModeSet ���������� ����� ��������� ���� (0 � �������, 1 - �������)
AngleMode = Load.toInt();
break;
case 40: // OperandsReset ����� ���������
OperandsCounter = 0;
for (auto& i : Foperands)
i = false;
break;
case 70: // ReadySet � ���������� ���� ���������� ���������� �� ��������� true
Redy = Load.toBool(true);
break;
case 80: // ReadyOut � ������ ���� ���������� ����������
Load.Write(Redy);
break;
case 81: // ReadyOutMk � ������ ���� ���������� ����������
MKExec(Load, { Cbool, &Ready });
break;
case 90: // ReadyExec � ��������� ������������, ���� ���������� ���� ���������� ����������
if (Redy)
ProgExec(Load);
break;
case 100: // ReadyNotExec � ��������� ������������, ���� �� ���������� ���� ���������� ����������
if (!Redy)
ProgExec(Load);
break;
case 110: // OutRezBlockSet � ���������� ���� ���������� ������ ����������
OutRezBlock = Load.toBool();
break;
case 120 // Push � ��������� ��������� � ����
if (Load.isEmpty())
RezStack.push_back(Rez);
else
RezStack.push_back(Load.toDouble());
break;
case 130: // Pop � �������� �������� �� ����� �����������
if (RezStack.size() == 0) {
ProgExec(RezStackIsEmpyProg());
ProgExec(ErrProg());
break;
}
if (Load.isEmpty())
rez = RezStack.back();
else
Load.Write(RezStack.back());
RezStack.pop_back();
break;
case 131: // PopMk � �������� ����������� �� ��������� �� ����� �����������
{if (RezStack.size() == 0) {
ProgExec(RezStackIsEmpyProg());
ProgExec(ErrProg());
break;
}
if (Load.isEmpty())
rez = RezStack.back();
else {
double temp = RezStack.back();
RezStack.pop_back();
MKExec(Load, { Cdouble, &temp });
}}
break;
case 140: // StackOut[Mk] � ������ �������� �� ����� �����������
Load.Write(RezStack.back());
break;
case 141: // StackOut[Mk] � ������ �������� �� ����� �����������
{
auto temp = RezStack.back();
MKExec(Load, { Cdouble, &temp });
break;
}
case 150: // NOperandSet ���������� ���������� ���������
OperandsCounter = Load.toInt();
break;
case 160: // ReceiverReset ����� ��������� ����������� ����������
ReseiverMk.clear();
ReseiverContext.clear();
break;
case 170: // ReceiverSet ���������� ������ �� ��������� ����������
// Implement logic
break;
case 180: // ReceiverMkSet ���������� �� ��� ��������� ����������
if (ReseiverMk.size() == ReseiverContext.size())
ReseiverContextpush_back(nullptr);
ReseiverMk.push_back(Load.toInt());
break;
case 190: // RezProgSet ���������� ������ �� ������������, ����������� ��� ��������� ����������
ReseiverContext.push_back();
break;
case 200: // Op1Out ������ ������ �������
if(Operands.size() == 0)
break;
else {
Load.Write(Operands[0]);
}
case 201: // Op1OutMk ������ ������ �������
if (Operands.size() == 0)
break;
else {
double temp = Operands[0];
MkExec(Load{ Cdouble, &temp });
}
case 210: // Op1Out ������ ������ �������
if (Operands.size() == 1)
break;
else {
Load.Write(Operands[1]);
}
case 211: // Op1OutMk ������ ������ �������
if (Operands.size() == 1)
break;
else {
double temp = Operands[1];
MkExec(Load{ Cdouble, &temp });
}
case 220: // OperetionProgSet ���������� ��������� ��� ����������� ��������
OperetionProg = nullptr;
break;
case 230: // Operand1Set ���������� ������ ������� (��� ����������� ��������)
Operands.Clear();
OperandsCounter = 1;
Foperands.Clear();
Foperands.Push_back(true);
break;
case 240: // Operand2Set ���������� ������ ������� (��� ����������� ��������)
if (Operands.size() < 2) {
Operands.push_back(Load.toDouble());
Foperands.push_back(true);
}
break;
case 250: // Operand3Set ���������� ������ ������� (��� ����������� ��������)
if (Operands.size() < 3) {
Operands.push_back(Load.toDouble());
Foperands.push_back(true);
}
break;
case 260: // PiOut[Mk], Pi2Out[Mk], Pi12Out[Mk], EOut[Mk] ������ ����� ��, 2��, ��/2, e.
if (Load.toInt() == 1) {
MKExec(Load, { Cdouble, &consts::pi });
}
else if (Load.toInt() == 2) {
MKExec(Load, { Cdouble, &consts::two_pi });
}
else if (Load.toInt() == 3) {
MKExec(Load, { Cdouble, &consts::pi_over_two });
}
else if (Load.toInt() == 4) {
MKExec(Load, { Cdouble, &consts::e });
}
break;
case 270: // ZRrogSet ���������� ������ �� ������������ ��� ������������
ZRrogProg = Load.Point;
break;
case 280: // NZRrogSet ���������� ������ �� ������������ ��� !=
NZRrogProg = Load.Point;
break;
case 290: // LRrogSet ���������� ������ �� ������������ <
LRrogProg = Load.Point;
break;
case 300: // BRrogSet ���������� ������ �� ������������ >
BRrogProg = Load.Point;
break;
case 310: // LZRrogSet ���������� ������ �� ������������ <=
LZRrogProg = Load.Point;
break;
case 320: // BZRrogSet ���������� ������ �� ������������ >=
BZRrogProg = Load.Point;
break;
case 330: // ZExec ��������� ������������ ��� ==
if (Rez == 0)
ProgExec(Load);
break;
case 340: // LExec ��������� ������������ ��� <
if (Rez < 0)
ProgExec(Load);
break;
case 350: // BExec ��������� ������������ ��� >
if (Rez > 0)
ProgExec(Load);
break;
case 360: // LZExec ��������� ������������ ��� <=
if (Rez <= 0)
ProgExec(Load);
break;
case 370: // BZExec ��������� ������������ ��� >=
if (Rez >= 0)
ProgExec(Load);
break;
case 380: // NZExec ��������� ������������ ��� !=
if (Rez != 0)
ProgExec(Load);
break;
case 390: // ZFOut[Mk] ������ ���� ����
MKExec(Load, { Cbool, &(Rez == 0) });
break;
case 400: // BOut[Mk] ������ ���� ������
MKExec(Load, { Cbool, &(Rez > 0) });
break;
case 410: // LOut[Mk] ������ ���� ������
MKExec(Load, { Cbool, &(Rez < 0) });
break;
case 420: // BZOut[Mk] ������ ���� ������ �����
MKExec(Load, { Cbool, &(Rez >= 0) });
break;
case 430: // LZOut[Mk] ������ ���� ������ �����
MKExec(Load, { Cbool, &(Rez <= 0) });
break;
case 440: // NZOut[Mk] ������ ���� �� �����
MKExec(Load, { Cbool, &(Rez != 0) });
break;
default:
CommonMk(MK, Load, Sender);
break;
}
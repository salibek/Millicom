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
case 1: // RezSet Установить результат
Rez = Load.toDouble();
break;
case 5: //RezOut Выдать результат
Load.Write(Rez);
break;
case 6: //RezOutMk Выдать МК с результатом
MKExec(Load, { Cdouble, &Rez });
break;
case 10: // ZProgSet
ZProg = Load.Point;
break;
case 20: // ZExec
if (Rez == 0)
ProgExec(Load);
case 30: // AngleModeSet установить режим измерения угла (0 – радианы, 1 - градусы)
AngleMode = Load.toInt();
break;
case 40: // OperandsReset Сброс операндов
OperandsCounter = 0;
for (auto& i : Foperands)
i = false;
break;
case 70: // ReadySet – установить флаг готовности результата по умолчанию true
Redy = Load.toBool(true);
break;
case 80: // ReadyOut – выдать флаг готовности результата
Load.Write(Redy);
break;
case 81: // ReadyOutMk – выдать флаг готовности результата
MKExec(Load, { Cbool, &Ready });
break;
case 90: // ReadyExec – выполнить подпрограмму, если установлен флаг готовности результата
if (Redy)
ProgExec(Load);
break;
case 100: // ReadyNotExec – выполнить подпрограмму, если не установлен флаг готовности результата
if (!Redy)
ProgExec(Load);
break;
case 110: // OutRezBlockSet – установить флаг блокировки выдача результата
OutRezBlock = Load.toBool();
break;
case 120 // Push – поместить результат в стек
if (Load.isEmpty())
RezStack.push_back(Rez);
else
RezStack.push_back(Load.toDouble());
break;
case 130: // Pop – выдавить значение из стека результатов
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
case 131: // PopMk – выдавить миликоманду со значением из стека результатов
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
case 140: // StackOut[Mk] – выдать значение из стека результатов
Load.Write(RezStack.back());
break;
case 141: // StackOut[Mk] – выдать значение из стека результатов
{
auto temp = RezStack.back();
MKExec(Load, { Cdouble, &temp });
break;
}
case 150: // NOperandSet Установить количество операндов
OperandsCounter = Load.toInt();
break;
case 160: // ReceiverReset Сброс установок получателей результата
ReseiverMk.clear();
ReseiverContext.clear();
break;
case 170: // ReceiverSet Установить ссылку на приемника результата
// Implement logic
break;
case 180: // ReceiverMkSet Установить МК для приемника результата
if (ReseiverMk.size() == ReseiverContext.size())
ReseiverContextpush_back(nullptr);
ReseiverMk.push_back(Load.toInt());
break;
case 190: // RezProgSet Установить ссылку на подпрограмму, запускаемую при получении результата
ReseiverContext.push_back();
break;
case 200: // Op1Out выдать первый операнд
if(Operands.size() == 0)
break;
else {
Load.Write(Operands[0]);
}
case 201: // Op1OutMk выдать первый операнд
if (Operands.size() == 0)
break;
else {
double temp = Operands[0];
MkExec(Load{ Cdouble, &temp });
}
case 210: // Op1Out выдать первый операнд
if (Operands.size() == 1)
break;
else {
Load.Write(Operands[1]);
}
case 211: // Op1OutMk выдать первый операнд
if (Operands.size() == 1)
break;
else {
double temp = Operands[1];
MkExec(Load{ Cdouble, &temp });
}
case 220: // OperetionProgSet Установить программу для специальной операции
OperetionProg = nullptr;
break;
case 230: // Operand1Set Установить первый операнд (для специальной операции)
Operands.Clear();
OperandsCounter = 1;
Foperands.Clear();
Foperands.Push_back(true);
break;
case 240: // Operand2Set Установить второй операнд (для специальной операции)
if (Operands.size() < 2) {
Operands.push_back(Load.toDouble());
Foperands.push_back(true);
}
break;
case 250: // Operand3Set Установить третий операнд (для специальной операции)
if (Operands.size() < 3) {
Operands.push_back(Load.toDouble());
Foperands.push_back(true);
}
break;
case 260: // PiOut[Mk], Pi2Out[Mk], Pi12Out[Mk], EOut[Mk] Выдача числа ПИ, 2ПИ, ПИ/2, e.
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
case 270: // ZRrogSet Установить ссылку на подпрограмму при переполнении
ZRrogProg = Load.Point;
break;
case 280: // NZRrogSet Установить ссылку на подпрограмму при !=
NZRrogProg = Load.Point;
break;
case 290: // LRrogSet Установить ссылку на подпрограмму <
LRrogProg = Load.Point;
break;
case 300: // BRrogSet Установить ссылку на подпрограмму >
BRrogProg = Load.Point;
break;
case 310: // LZRrogSet Установить ссылку на подпрограмму <=
LZRrogProg = Load.Point;
break;
case 320: // BZRrogSet Установить ссылку на подпрограмму >=
BZRrogProg = Load.Point;
break;
case 330: // ZExec Выполнить подпрограмму при ==
if (Rez == 0)
ProgExec(Load);
break;
case 340: // LExec Выполнить подпрограмму при <
if (Rez < 0)
ProgExec(Load);
break;
case 350: // BExec Выполнить подпрограмму при >
if (Rez > 0)
ProgExec(Load);
break;
case 360: // LZExec Выполнить подпрограмму при <=
if (Rez <= 0)
ProgExec(Load);
break;
case 370: // BZExec Выполнить подпрограмму при >=
if (Rez >= 0)
ProgExec(Load);
break;
case 380: // NZExec Выполнить подпрограмму при !=
if (Rez != 0)
ProgExec(Load);
break;
case 390: // ZFOut[Mk] Выдать флаг нуля
MKExec(Load, { Cbool, &(Rez == 0) });
break;
case 400: // BOut[Mk] Выдать флаг больше
MKExec(Load, { Cbool, &(Rez > 0) });
break;
case 410: // LOut[Mk] Выдать флаг меньше
MKExec(Load, { Cbool, &(Rez < 0) });
break;
case 420: // BZOut[Mk] Выдать флаг больше равно
MKExec(Load, { Cbool, &(Rez >= 0) });
break;
case 430: // LZOut[Mk] Выдать флаг меньше равно
MKExec(Load, { Cbool, &(Rez <= 0) });
break;
case 440: // NZOut[Mk] Выдать флаг не равно
MKExec(Load, { Cbool, &(Rez != 0) });
break;
default:
CommonMk(MK, Load, Sender);
break;
}
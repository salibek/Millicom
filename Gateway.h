// ФУ Шлюз
#pragma once

#include "FUini.h"

class ForwardChannel : public FU
{
//	FUProgram: TFUProgram; // Ссылка на программу реализации логики работы ФУ
	FU* Receiver = nullptr; // Приемник данных
	int ReceiverMk = -1; // Милликоманда для шлюза-приемщика
	FU* GatewayFU = nullptr; // Ссылка на ФУ шлюз, к которому принадлежит канал
	int N = 0; // Номер канала
	// bool BackChannel; // Флаг обратного направления передачи информации
	double ByteTime = 0; // Время передачи одного байта информации по линии
	int ServiseLength = 0; // Длина служебного сообщения одного кадра
	int BatchWidth = 0; // Длина кадра передаваемой информации
	double AcceptTime = 0; // Время подтверждения передачи пакета
	double WaitTime = 0; // Время ожидания подтверждения

};
//PSluceChannel = ^ TSluceChannel;

class BackChennel : public FU
{
	FU* ContextUk = nullptr; // Указатель на контекст ФУ шлюза
	int N_Chanel = 0; // номер канала, по которому пришла милликоманда
	FU* GatewayFU = nullptr; // Ссылка на ФУ шлюз, к которому принадлежит канал
	int TransCount = 0; // Счетчики передач милликоманд в одну и другую стороны
//	QReceiv, QReceivTail: PAtrData; // Очередь контекстов получателей милликоманд
};

class Gateway : public FU
{
	FU* ContextUk = nullptr; // Указатель на контекст ФУ шлюза
	int N_Chanel = 0; // номер канала, по которому пришла милликоманда
	FU* Receiver = nullptr; // ссылка на канал, по которому пришла Мк
	int TransCount = 0; // Счетчики передач милликоманд в одну и другую стороны
//	QReceiv, QReceivTail: PAtrData; // Очередь контекстов получателей милликоманд

	BackChennel BChennel; // Контекст для обратного канала

//	CurrentContext: PGatewaySimulator; // указатель на текущий контекст (ФУ ответственное за передачу информации в одну или в другую сторону)
	double TransDelay = 0; // Задержка передачи данных
	int Index = 0, Direction = 0; // Индекс текущего канала передачи данных
	int ChannelDim = 0; // Размерность массива каналов
	vector<ForwardChannel> Chennels;
//	FU* Node1 = nullptr, Node2 = nullptr; // Указатели на исполнительные ФУ, которые необходимо соединить
	double SluceCPU = 0; // Время работы шлюза

	int ProbFunk = 0; // Закон распределения вероятности помех ( 0 - равномерный, 1- экспоненциальный, 2 - эрланг, 3- нормальный)
	// Параметры случайного процесса возникновения помех
	double alpha = 0; // Альфа для экспоненциального распределения
	double k = 0; // параметр k распределения Эралнга
	double o = 0; // Параметр О распределения Эрланга
	double mu = 0; // Коэффицент мю нормального распределения
	double sigma = 0; // Коэффицент сигма для номрального распределения
	double PFail = 0; // Вероятность ошибки для равномерного распределения при передаче одного бита информации

	double ByteTime = 0; // Время передачи одного байта информации по линии
	int ServiseLength = 0; // Длина служебного сообщения одного кадра
	int BatchWidth = 0; // Длина кадра передаваемой информации
	double AcceptTime = 0; // Время подтверждения передачи пакета
	double WaitTime = 0; // Время ожидания подтверждения

public:
	void ProgFU(int MK, LoadPoint Load) override;
	Gateway(FU* BusContext, FU* Templ)
	{
		Bus = BusContext;
	};
};
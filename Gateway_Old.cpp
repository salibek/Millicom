#include "stdafx.h"
#include "consts.h"
#include "Gateway.h"
#include "FUini.h"

using namespace std;

void Gateway::ProgFU(int MK, LoadPoint Load)
{

	switch (MK)
	{
	case 0: // Сброс
	{
		Index = 0;
		ChannelDim = 0;
		TransDelay = 0; // Задержка передачи данных
		Direction = 0;
		SluceCPU = 0;
//		QReceiv = nullptr; QReceivTail = nullptr;
//			BackChennel.QReceiv = nullptr; BackChennel.QReceivTail = nullptr;
		TransCount = 0; 
//		BackChennel.TransCount = 0;

		Receiver = nullptr;
		N_Chanel = -1;
//			BackChennel.Receiver = nullptr;
//			BackChennel.N_Chanel = -1;

		BatchWidth = 0;
		ServiseLength = 0;
		ByteTime = 0;
		AcceptTime = 500;
		WaitTime = 500;

		ProbFunk = 0;
			// Параметры случайного процесса возникновения помех
		alpha = 1; // Альфа для экспоненциального распределения
		k = 1; // параметр k распределения Эралнга
		o = 1; // Параметр О распределения Эрланга
		mu = 1; // Коэффицент мю нормального распределения
		sigma = 1; // Коэффицент сигма для номрального распределения
		PFail = 0;
	}	break;

	case 1: // Создать канал и установить приемник
		break;
	case 5: // Выдать ссылку на созданный канал
//		Load.Write(int());
		break;
	case 10: // Выдать милликоманду со ссылкой на созданный канал
//		Load.Write(int(MKCount));
		break;
	case 15: // Установить счетчик количества сшиваемых узлов сетки
		break;
	case 20: // Установить первый и второй узел, с которых наобходимо произвести соединенеие узлов
		break;
	case 25: // Установить первый и второй узел, с которых наобходимо произвести соединенеие узлов
		break;
	case 30: // Сшить поля для теста по расчету электрического поля (в нагрузке направление сшивания 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз)
		break;
	case 35: // Установить время задержки передачи милликоманды через шлюз
		break;
	case 40: // Выдать милликоману с числом пройденных через шлюз милликоманд туда (Farvard)
//		Load.Write(int(MKCount));
		break;
	case 45: // Выдать милликоману с числом пройденных через шлюз милликоманд обратно (Back)
//		Load.Write(int(MKCount));
		break;
	case 50:// Выдать милликоману с числом пройденных через шлюз милликоманд туда и обратно (FarwardBack)
//		Load.Write(int(MKCount));
		break;
	case 55: // Выдать число милликоманд, ожидающищх передачу туда (Farvard)
//		Load.Write(int(MKCount));
		break;
	case 60: // Выдать число милликоманд, ожидающищх передачу туда (Back)
//		Load.Write(int(MKCount));
		break;
	case 65: // Выдать число милликоманд, ожидающищх передачу туда (FarwardBack)
//		Load.Write(int(MKCount));
		break;
	case 70: // Установить длину служебного сообщения одного кадра
		break;
	case 75: // Установить длину пакет
		break;
	case 80: // Установить время подтверждения передачи пакета
		break;
	case 85: // Установить время подтверждения передачи
		break;
	case 90: // Установить время передачи одного байта по линии связи
		break;
	case 95: // Установить закон распределения помех (0-нет 1 - равномерный, 2- экспоненциальный, 3 - эрланг, 4- нормальный)
		break;
	case 100: // Альфа для экспоненциального распределения
		break;
	case 105: // параметр k распределения Эралнга
		break;
	case 110: // Параметр О распределения Эрланга
		break;
	case 115: // Коэффицент мю нормального распределения
		break;
	case 120: // вероятность ошибки для равномерного распределения при передаче одного бита информации
		break;
	case 125: // Скорость передачи бит за еденицу модельного воемени
		break;

	default:
		CommonMk(MK, Load);
		break;
	}
}
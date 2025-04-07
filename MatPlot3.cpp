#include "MatPlot.h"
#include <cmath>
#if MatPlotInclude
namespace plt = matplotlibcpp;
#endif // MatPlotInclude

void MatPlot::ProgFU(long int MK, LoadPoint Load, FU* Sender) {


	MK %= FUMkRange;
	int qual = 2;
	switch (MK)
	{
	case 0: //Reset
		X.clear(); Y.clear();
		MeshX.clear();
		MeshY.clear();
		Z.clear();
		break;
#if MatPlotInclude

	case 1: // Plot Нарисовать график (На входе тип графика)
	{
		plt::rcparams({
	{"figure.facecolor", Format1[0]},  // Цвет фона всего рисунка
	{"axes.facecolor", Format1[1]},   // Цвет фона области графика
	{"axes.edgecolor", Format1[2]},    // Цвет рамки области графика
	{"axes.grid", Format1[3]},         // Включить сетку
	{"grid.color", Format1[4]},        // Цвет сетки
	{"grid.alpha", Format1[5]}         // Прозрачность сетки
			});
		numCols = Y.size() - 1;
		switch (Load.toInt()) {
		case 0:		// 2мерный линейный график
		{
			plt::subplot2grid(rows, cols, rowId, colId); //поместить график в окно
			plt::plot(X, Y, { {"color", Format0[0]},              // Set line color 
			{"marker", Format0[1]},               // Set marker type (circle in this case) 
			{"markersize",Format0[2]},
			{"linewidth",Format0[3]},
			{"markerfacecolor",Format0[4]},
			{"markeredgecolor",Format0[5]}
				});
			qual++;
		}
		break;
		case 1:		//Столбчатая диаграмма
		{
			plt::subplot2grid(rows, cols, rowId, colId);
			plt::bar(X, Y, Format0[0], "-", barWidth,
				{
					{"color", Format0[4]}
				});

		}
		break;
		case 2:		//Столбчатая диаграмма
		{
			plt::subplot2grid(rows, cols, rowId, colId);
			plt::barh(X, Y, Format0[0], "-", barWidth,
				{
					{"color", Format0[4]}
				});
		}
		break;
		case 3:		//Горизонтальная столбчатая диаграмма
		{
			plt::subplot2grid(rows, cols, rowId, colId);
			plt::hist(Y, numCols, Format0[0], histAlpha, histSort);
		}
		break;
		case 4:		//Круговая диаграмма
		{
			plt::subplot2grid(rows, cols, rowId, colId);
			plt::pie(X);
		}
		break;
		case 5:		//Boxplot диаграмма
		{
			Z0.push_back(X);
			Z0.push_back(Y);
			plt::boxplot(Z);
		}
		break;
		case 6:		//2мерная диаграмма рассеивания
			plt::scatter(X, Y, scatterSize, { {"marker",Format0[1]},{"edgecolors",Format0[0]},{"cmap","magma"} });
			break;
		case 7:	//3мерная диаграмма рассеивания
		{
			PyObject* ax = plt::chart();
			plt::scatter(ax, X, Y, X, "Red");
		}
		break;
		case 8: //3мерный линейный график
		{
			PyObject* ax = plt::chart();
			plt::plot3(ax, X, X, X, Format0[0], lineWidth3);
		}
		break;
		case 9:	//Линейный график в полярных координатах
		{
			PyObject* ax = plt::chart_polar(111);
			plt::plot(X, Y,
				{ {"color", Format0[0]},              // Set line color 
			{"marker", Format0[1]},               // Set marker type (circle in this case) 
			{"markersize",Format0[2]},
			{"linewidth",Format0[3]},
			{"markerfacecolor",Format0[4]},
			{"markeredgecolor",Format0[5]}
				});
		}
		break;
		case 10:	//3мерный график с цветовой картой
		{
			PyObject* ax = plt::chart();
			plt::Clear3DChart(ax);
			std::vector<std::vector<double>> x, y, z;
			for (int i = 0; i < X.size() - 1; i++)
			{
				std::vector<double> vvX, vvY, vvZ;
				for (int j = 0; j < Y.size() - 1; j++)
				{
					vvX.push_back(X[i]);
					vvY.push_back(Y[j]);
					vvZ.push_back(Z[i * Y.size() + j]);
					//vvZ.push_back(::std::sin(::std::hypot(X[i],Y[j])));
				}
				z.push_back(vvZ);
				x.push_back(vvX);
				y.push_back(vvY);

			}

			plt::surface3DMap(ax, x, y, z, "viridis", 0.8);
		}
		break;
		case 11:	//3мерный график одноцветный
		{
			PyObject* ax = plt::chart();
			plt::Clear3DChart(ax); //Оси графика
			std::vector<std::vector<double>> x, y, z; //Пустые матрицы для значений 3-мерного графика
			for (int i = 0; i < X.size() - 1; i++)
			{
				std::vector<double> vvX, vvY, vvZ; 
				for (int j = 0; j < Y.size() - 1; j++)
				{
					vvX.push_back(X[i]); //Добавление в матрицу значения по оси X(из массива X)
					vvY.push_back(Y[j]); //Добавление в матрицу значения по оси Y(из массива Y)
					vvZ.push_back(Z[i * Y.size() + j]); //Добавление в матрицу значения по оси Z(из массива Z)
					//vvZ.push_back(::std::sin(::std::hypot(X[i],Y[j])));
				}
				z.push_back(vvZ);
				x.push_back(vvX);
				y.push_back(vvY);

			}
			plt::surface3D(ax, x, y, z, Format0[0], 0.8);
		}
		break;
		}


		break;
	case 5: // XAdd Добавить координату по X
	{
		int a = Load.toDouble();
		X.push_back(a);
	}
	break;
	case 6: // StartSet Установить начальное заначение отрезка по X
		Start = Load.toDouble();
		break;
	case 7:	// EndSet Установить конечное заначение отрезка по X
		End = Load.toDouble();
		break;
	case 8:	// hSet Установить шаг точек отрезка по X
		X.clear();
		h = Load.toDouble();
		N = 0;
		for (double x = Start; x <= End; x += h) // Генерация точек по оси X
		{
			N++;
			X.push_back(x);
		}
		break;
	case 9:	// NSet Установть количестов точек отрезка по X
		X.clear();
		N = Load.toDouble();
		h = (End - Start) / (N - 1);
		for (double x = Start; x <= End; x += h) // Генерация точек по оси X
		{
			N++;
			X.push_back(x);
		}

		break;
	case 10: // XVectSet // Установить вектор по координате X
		if (!Load.isVect()) break;
		X.resize(Load.toVect()->size());
		{
			auto L = Load.toVect()->begin();
			for (auto x = X.begin(); x != X.end(); x++, L++)
				*x = L->toDouble();
		}
		break;
	case 11: // YVectSet // Установить вектор по координате Y
		if (!Load.isVect()) break;

		Y.resize(Load.toVect()->size());
		{
			auto L = Load.toVect()->begin();
			for (auto y = Y.begin(); y != Y.end(); y++, L++)
				*y = L->toDouble();
		}
		break;
	case 12: // MeshSet // Установить сетку координат для 3-мерного графика
		if (!Load.isVect()) break;
		//...
		break;
	case 13: // ZSet // Установить значения для 3-мерного графика
		if (!Load.isVect()) break;
		Z.resize(Load.toVect()->size());
		{
			auto L = Load.toVect()->begin();
			for (auto z = Z.begin(); z != Z.end(); z++, L++)
				*z = L->toDouble();
		}
		break;

	case 15:	// hYSet Установить шаг точек отрезка по Y
	{
		Y.clear();
		hY = Load.toDouble();
		N = 0;
		for (double y = YStart; y <= YEnd; y += hY) // Генерация точек по оси Y
		{
			N++;
			Y.push_back(y);
		}
	}
	break;
	case 16:	// NYSet Установть количестов точек отрезка по Y
	{
		Y.clear();
		Ny = Load.toDouble();
		N = 0;
		hY = (YEnd - YStart) / (Ny - 1);
		for (double y = YStart; y <= YEnd; y += hY) // Генерация точек по оси Y
		{
			N++;
			Y.push_back(y);
		}
	}
	break;
	case 17:	// hZSet Установить шаг точек отрезка по Z
	{
		Z.clear();
		hZ = Load.toDouble();
		N = 0;
		for (double z = Start; z <= End; z += hZ) // Генерация точек по оси Z
		{
			N++;
			Z.push_back(z);
		}
	}
	break;
	case 18:	// NZSet Установть количестов точек отрезка по Z
	{
		Z.clear();
		Nz = Load.toDouble();
		hZ = (End - Start) / (N - 1);
		for (double z = Start; z <= End; z += hZ) // Генерация точек по оси Z
		{
			N++;
			Z.push_back(z);
		}
	}
	break;

	case 20: // XVectOut Выдать вектор значений по оси X
		Load.Write(X);
		break;
	case 21: // XVectOutMk Выдать МК с вектором значений по оси X
		MkExec(Load, { CintArray, &X });
		break;
	case 22:  // XLoadVectOut Выдать вектор значений по оси X
		//Load.Write(X);
		//break;
	case 23: // XLoadVectOutMk Выдать МК с вектором значений по оси X
	{
		vector<LoadPoint> t;
		t.resize(X.size());
		auto tuk = t.begin();
		for (auto xuk = X.begin(); xuk != X.end(); xuk++)
		{
			*tuk = { Cint,new double };
			*(double*)(tuk->Point) = *xuk;
		}
		if (MK == 23)MkExec(Load, { CintArray, &X });
		else Load.Write(t);
		for (auto tuk = t.begin(); tuk != t.end(); tuk++)
			delete (double*)tuk->Point;
	}
	break;
	case 25: // YVectOut Выдать вектор значений по оси Y
	//	Load.Write(Y);
	//	break;
	case 26: // YVectOutMk Выдать вектор значений по оси Y
	{
		vector<LoadPoint> t;
		t.resize(Y.size());
		auto tuk = t.begin();
		for (auto xuk = Y.begin(); xuk != Y.end(); xuk++)
		{
			*tuk = { Cint,new double };
			*(double*)(tuk->Point) = *xuk;
		}
		if (MK == 23)MkExec(Load, { CintArray, &Y });
		else Load.Write(t);
		for (auto tuk = t.begin(); tuk != t.end(); tuk++)
			delete (double*)tuk->Point;
	}
	break;
	case 27: // ZVectOut Выдать вектор значений по оси Z
	//	Load.Write(Z);
	//	break;
	case 28: // ZVectOutMk Выдать вектор значений по оси Z
	{
		vector<LoadPoint> t;
		t.resize(Z.size());
		auto tuk = t.begin();
		for (auto xuk = Z.begin(); xuk != Z.end(); xuk++)
		{
			*tuk = { Cint,new double };
			*(double*)(tuk->Point) = *xuk;
		}
		if (MK == 23)MkExec(Load, { CintArray, &Z });
		else Load.Write(t);
		for (auto tuk = t.begin(); tuk != t.end(); tuk++)
			delete (double*)tuk->Point;
	}
	break;

	case 35: // DxSet
		Dx = Load.toDouble();
		break;
	case 36: // DySet
		Dy = Load.toDouble();
		break;
	case 39: // YStartSet
		YStart = Load.toDouble();
		break;
	case 40: // YEndSet
		YEnd = Load.toDouble();
		break;
	case 45: // NxSet
	{
		Nx = Load.toDouble();
		N = Load.toInt();
	}
		break;
	case 46: // NySet
		Ny = Load.toInt();
		break;
	case 50: //MeshN Сгенерировать сетку для отображения 3-мерного графика по количеству точек
	{
		MeshX.clear();
		MeshY.clear();
		Dy = (YEnd - YStart) / (Ny - 1);
		Dx = (XEnd - XStart) / (Nx - 1);
		MeshY.resize(Ny);
		for (int i = 0; i < Ny; i++)
			MeshY[i].resize(Nx);
		for (int i = 0; i < Ny; i++)
			for (int j = 0; j < Nx; j++)
				MeshY[i][j] = YStart + i * Dy;
		for (int i = 0; i < Ny; i++)
			for (int j = 0; j < Nx; j++)
				MeshX[i][j] = XStart + j * Dy;
	}

	break;
	case 51: //MeshD Сгенерировать сетку для отображения 3-мерного графика по длине шага
	{
		Nx = Ny = 0;
		for (double y = YStart; y <= YEnd; y += Dy)
		{
			MeshY.push_back({});
			Ny++;
			for (double x = XStart; x <= XEnd; x += Dx);

		}
	}
	break;

	case 55: // YAdd Добавить значение функции
		Y.push_back(Load.toDouble());
		break;
	case 56: // // Y3DAdd Добавить значение функции для 3-мерного графика
		Z.push_back(Load.toDouble());
		break;

	case 60: // XSort Упорядочить данные по оси X по возрастанию
		for (int k = 0; k < X.size() - 1; k++)
			for (int i = 0; i < X.size() - 1 - k; i++)
				if (X[i] > X[i + 1])
				{
					swap(X[i], X[i + 1]);
					swap(Y[i], Y[i + 1]);
				}
		break;

	case 61: // PlotTitleSet Добавить заголовок
	{
		plotTitle = Load.toStr();
		plt::title(plotTitle);
	}
	break;
	case 62: // xTitleSet Подписать ось X
	{
		xTitle = Load.toStr();
		plt::xlabel(xTitle);
	}
	break;
	case 63: // yTitleSet Подписать ось Y
	{
		yTitle = Load.toStr();
		plt::ylabel(yTitle);
	}
	break;
	case 65: //LegendSet Добавить легенду
	{
		legendText = Load.toStr();
		plt::named_plot(legendText, X, Y);
		plt::legend();
	}
	break;
	case 66: //TextSet Добавить текст на график
	{
		plotText = Load.toStr();
		plt::text(xAxe, yAxe, plotText);
	}
	break;
	case 67: //xTextSet Добавить координату X поля с текстом
		xAxe = Load.toDouble();
		break;
	case 68: //yTextSet Добавить координату X поля с текстом
		yAxe = Load.toDouble();
		break;
	case 71: //MarkFaceSet Установить цвет  маркера
		Format0[4] = Load.toStr();
		break;
	case 72: //MarkEdgeSet Установить цвет границ маркера
		Format0[5] = Load.toStr();
		break;
	case 73: //RowsSet Установить количество столбцов фигуры
		rows = Load.toInt();
		break;
	case 74: //ColsSet Установить количество столбцов фигуры
		cols = Load.toInt();
		break;
	case 75: //RowIdSet Установить номер строки графика
		rowId = Load.toInt();
		break;
	case 76: //ColIdSet Установить номер столбца графика
		colId = Load.toInt();
		break;
	case 77: // BarmWidthSet Установить цвет фона всего рисунка
		barmWidth = Load.toDouble();
		break;
	case 78: // BarWidthSet Установить ширину столбцов
		barWidth = Load.toDouble();
		break;
	case 79: // HistAlphaSet Установить прозрачность гистограммы
		histAlpha = Load.toDouble();
		break;
	case 80: // NumColsSet Установить количество частей гистограммы + !!!вывод графика (обязательная команда)
	{
		numCols = Load.toInt();
		plt::show();
	}
	break;
	case 81: // HistSortSet Сортировать значения гистограммы
		histSort = Load.toBool();
		break;
	case 82: // ScatterSizeSet Установить размер маркера
		scatterSize = Load.toDouble();
		break;
	case 86: // zTitleSet Добавить название оси Z
	{
		zTitle = Load.toStr();
		plt::set_zlabel(zTitle);
	}
	break;
	case 89: //Line3Width Указать толщину 3-мерного линейного графика
		lineWidth3 = Load.toInt();
		break;
	case 94: // FigureColorSet Установить цвет фона всего рисунка
		Format1[0] = Load.toStr();
		break;
	case 95: // AxesColorSet Установить цвет фона области графика
		Format1[1] = Load.toStr();
		break;
	case 96: // EdgeColorSet Установить цвет рамки области графика
		Format1[2] = Load.toStr();
		break;
	case 97: // GridBoolSet Включить сетку
		Format1[3] = Load.toStr();
		break;
	case 98: // GridColorSet Установить цвет сетки
		Format1[4] = Load.toStr();
		break;
	case 99: // GridAlphaSet Установить прозрачность сетки
		Format1[5] = Load.toStr();
		break;
	case 100: // PlotFormatSet Установить формат графика
		PlotFormat = Load.toStr();
		break;
	case 101: // MarkSet Установить маркер
		Format0[1] = Load.toStr();
		break;
	case 102: // MarkWidthSet Установить размер маркера (для линейного)
		Format0[2] = Load.toStr();
		break;
	case 103: // LineColorSet Установить цвет графика
	{
		Format0[0] = Load.toStr();

	}
	break;
	case 104: // LineWidthSet Установить ширину графика (для линейного)
		Format0[3] = Load.toStr();
		break;
#endif // MatPlotInclude

	default:
		CommonMk(MK, Load, Sender);
		break;
	}
	}
}

FU* MatPlot::Copy() // Программа копирования ФУ
{
	return new MatPlot(Bus, this);
}

FU* MatPlot::TypeCopy() // Создать ФУ такого же типа (не копируя контекст)
{
	return new MatPlot(Bus, nullptr);
}
// Операции ввода-вывода данных
void MatPlot::WriteToCSV(const std::string& fileName) {
	// Открываем файл для записи
	std::ofstream outFile(fileName);
	if (!outFile.is_open()) {
		throw std::runtime_error("Не удалось открыть файл для записи: " + fileName);
	}

	// Записываем заголовки
	for (size_t i = 0; i < excelHeaders.size(); ++i) {
		outFile << excelHeaders[i];
		if (i < excelHeaders.size() - 1) {
			outFile << ","; // Разделитель
		}
	}
	outFile << "\n"; // Завершаем строку

	// Записываем данные
	for (const auto& row : excelData) {
		for (size_t i = 0; i < row.size(); ++i) {
			outFile << row[i];
			if (i < row.size() - 1) {
				outFile << ","; // Разделитель
			}
		}
		outFile << "\n"; // Завершаем строку
	}

	// Закрываем файл
	outFile.close();

	// Для отладки: сообщение об успешной записи
	std::cout << "Данные успешно записаны в CSV-файл: " << fileName << std::endl;
}

void MatPlot::WriteToExcel(const std::string& fileName) {
	// Создаем новый Excel-файл
/*	lxw_workbook* workbook = workbook_new(fileName.c_str());
	if (!workbook) {
		throw std::runtime_error("Не удалось создать файл Excel: " + fileName);
	}

	// Добавляем новый лист в книгу
	lxw_worksheet* worksheet = workbook_add_worksheet(workbook, nullptr);
	if (!worksheet) {
		workbook_close(workbook);
		throw std::runtime_error("Не удалось создать лист Excel.");
	}

	// Записываем заголовки в первую строку
	for (size_t col = 0; col < excelHeaders.size(); ++col) {
		worksheet_write_string(worksheet, 0, col, excelHeaders[col].c_str(), nullptr);
	}

	// Записываем данные в следующие строки
	for (size_t row = 0; row < excelData.size(); ++row) {
		for (size_t col = 0; col < excelData[row].size(); ++col) {
			worksheet_write_number(worksheet, row + 1, col, excelData[row][col], nullptr);
		}
	}

	// Закрываем файл и освобождаем ресурсы
	if (workbook_close(workbook) != LXW_NO_ERROR) {
		throw std::runtime_error("Ошибка при закрытии файла Excel: " + fileName);
	}

	// Для отладки: выводим сообщение о завершении
	std::cout << "Данные успешно записаны в файл Excel: " << fileName << std::endl;
	*/
}

void MatPlot::ReadFromExcel(const std::string& fileName) {
	// Открываем Excel-файл
	std::ifstream file(fileName);
	if (!file.is_open()) {
		throw std::runtime_error("Не удалось открыть файл: " + fileName);
	}

	excelData.clear();
	excelHeaders.clear();

	std::string line;
	bool isHeader = true; // Флаг для чтения первой строки как заголовков

	while (std::getline(file, line)) {
		std::istringstream stream(line);
		std::string cell;
		std::vector<double> row;

		if (isHeader) {
			// Первая строка обрабатывается как заголовки
			while (std::getline(stream, cell, ',')) {
				excelHeaders.push_back(cell);
			}
			isHeader = false;
		}
		else {
			// Остальные строки обрабатываются как числовые данные
			while (std::getline(stream, cell, ',')) {
				try {
					row.push_back(std::stod(cell)); // Преобразование строки в число
				}
				catch (...) {
					row.push_back(0.0); // Если ошибка, добавляем 0.0
				}
			}
			excelData.push_back(row);
		}
	}

	file.close();

	// Если ориентация данных - по столбцам, выполняем транспонирование
	if (FileDataOrientation) {
		std::vector<std::vector<double>> transposed(excelHeaders.size(), std::vector<double>(excelData.size(), 0));
		for (size_t i = 0; i < excelData.size(); ++i) {
			for (size_t j = 0; j < excelData[i].size(); ++j) {
				transposed[j][i] = excelData[i][j];
			}
		}
		excelData = transposed;
	}
}

void MatPlot::ReadFromCSV(const std::string& fileName) {
	// Проверяем доступность файла
	std::ifstream file(fileName);
	if (!file.is_open()) {
		throw std::runtime_error("Не удалось открыть файл: " + fileName);
	}

	// Очищаем старые данные перед чтением
	excelData.clear();
	excelHeaders.clear();

	std::string line;
	bool isHeader = true; // Первая строка — заголовки

	while (std::getline(file, line)) {
		std::istringstream stream(line);
		std::string cell;
		std::vector<double> row;

		if (isHeader) {
			// Считываем заголовки из первой строки
			while (std::getline(stream, cell, ',')) {
				excelHeaders.push_back(cell);
			}
			isHeader = false;
		}
		else {
			// Считываем данные из остальных строк
			while (std::getline(stream, cell, ',')) {
				try {
					row.push_back(std::stod(cell)); // Преобразование строки в число
				}
				catch (...) {
					row.push_back(0.0); // Если ошибка, подставляем 0
				}
			}
			excelData.push_back(row);
		}
	}

	file.close();

	// Если установлена ориентация данных по столбцам, транспонируем
	if (FileDataOrientation) {
		std::vector<std::vector<double>> transposed(excelHeaders.size(), std::vector<double>(excelData.size(), 0));
		for (size_t i = 0; i < excelData.size(); ++i) {
			for (size_t j = 0; j < excelData[i].size(); ++j) {
				transposed[j][i] = excelData[i][j];
			}
		}
		excelData = transposed;
	}

	// Для отладки: вывод данных в консоль
	std::cout << "Заголовки: ";
	for (const auto& header : excelHeaders) {
		std::cout << header << " ";
	}
	std::cout << std::endl;

	std::cout << "Данные:" << std::endl;
	for (const auto& row : excelData) {
		for (const auto& value : row) {
			std::cout << value << " ";
		}
		std::cout << std::endl;
	}
}

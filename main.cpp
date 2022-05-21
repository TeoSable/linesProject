// Подключение библиотек и заголовочных файлов
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <thread>
#include "Line.h"

using namespace sf;

// Случайная генерация точек на поле с исключением пересечений
std::vector<Dot> generateDots(int N, int d);

// Поиск индекса отрезка по индексам точек
int lineIndex(int i1, int i2, int size); 

// Проверка двух отрезков на пересечением методом ориентированных площадей
bool intersect(Line& l1, Line& l2);

int main() 
{
	const float d = 60.0f; // Размер клетки
	RenderWindow window(VideoMode(600, 700), "Lines", Style::Close); // Окно отрисовки

	// Фоновое изображение (игровое поле)
	Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("background.png"))
		return -1;
	Sprite background(backgroundTexture);

	// Баннер с текстом о состоянии игры под игровым полем (изначально стоит надпись "Player 1's turn")
	Texture bannerTexture;
	bannerTexture.loadFromFile("banners.png");
	bannerTexture.setSmooth(true);
	Sprite banner(bannerTexture);
	banner.setPosition(0.0f, 600.0f);
	banner.setTextureRect(IntRect(0, 100, 600, 200));

	// Создание массива из 10 точек 
	std::vector<Dot> dots = generateDots(10, d);

	// Создание массива отрезков между точками. 
	// Соответствие между индексами точек и отрезков взаимно однозначное, что позволит в дальнейшем обращаться
	// к отрезкам, зная индексы его концов, с помощью цикла, аналогичного циклу создания.
	std::vector<Line> lines;
	for (int i = 0; i < dots.size() - 1; i++) {
		for (int j = i + 1; j < dots.size(); j++) {
			Line newLine(dots[i], dots[j], 0);
			lines.push_back(newLine); 
		}
	}

	// Переменные, отвечающие за состояние игры:
	bool lockClick = false; // Флажок, отвечающий за единственность сигнала от нажатия мыши
	int curPlayerNum = 1; // Число, определяющее номер игрока, чей ход сейчас идёт
	Dot* selection1 = NULL; // Указатель на первую выделенную точку
	Dot* selection2 = NULL; // Указатель на вторую выделенную точку
	bool gameEnd = false; // Флажок, отвечающий за достижение конца игры

	// Цикл игрового процесса (работает, пока открыто отрисовочное окно)
	while (window.isOpen()) {
		// Проверка на следующие события:
		Event evnt;
		while (window.pollEvent(evnt)) {
			switch (evnt.type)
			{
			case Event::Closed: // Нажата кнопка "Закрыть окно": отрисовочное окно закрывается, цикл завершается.
				window.close();
				break;
			case Event::MouseButtonReleased: // Отпущена кнопка мыши: программа может снова регистрировать нажатия, на что указывает флажок
				lockClick = false;
			}
		}

		// Отрисовочное окно очищается, отрисовывается фон
		window.clear(Color::White);
		window.draw(background);

		// Далее идёт проверка на нахождение мыши над точками и их нажатие
		for (int i = 0; i < dots.size(); i++) {
			dots[i].checkHover(window);
			dots[i].checkClick(window, curPlayerNum, lockClick);
		}

		// Проходит проверка на наличие выделенных точек, указатели на них передаются в соответствующие переменные
		// Указатель на единственную выделенную точку всегда записывается в ячейку selection1
		for (int i = 0; i < dots.size(); i++) {
			if (dots[i].getSelection() == curPlayerNum) {
				if (selection1 == NULL)
					selection1 = &dots[i];
				else if (selection1 != &dots[i])
					selection2 = &dots[i];
			}
			// Невыделенные точки выписываются из соответствующих ячеек 
			// Первой всегда освобождается ячейка selection2
			else if (selection2 == &dots[i])
				selection2 = NULL;
			else if (selection1 == &dots[i]) {
				selection1 = selection2;
				selection2 = NULL;
			}
		}
		
		// Если выделено две точки, программа приступает к проверке их соединимости отрезком текущего цвета
		if (selection2 != NULL) {
			// Определяются численные индексы выделенных точек
			int index1 = std::find(dots.begin(), dots.end(), *selection1) - dots.begin();
			int index2 = std::find(dots.begin(), dots.end(), *selection2) - dots.begin();
			// index1 всегда больше index2, т.к. так построена нумерация соответствующих отрезков
			if (index1 > index2)
				std::swap(index1, index2);
			// По функции определяется индекс соответствующего отрезка в массиве отрезков
			int curLineIndex = lineIndex(index1, index2, dots.size());

			// Начинается проверка данного отрезка на пересечения с другими:
			// Проверяется, что отрезок не построен ни одним из игроков;
			// Проверяется, что отрезок не пересекает других отрезков цвета оппонента.
			// При невыполнении хотя бы одного условия флажок обнаружения пересечений ставится на true
			bool intersectDetected = false;
			if (lines[curLineIndex].getOwner() != 0)
				intersectDetected = true;
			else {
				for (int k = 0; k < lines.size(); k++) {
					intersectDetected = intersectDetected || (k != curLineIndex
						&& intersect(lines[k], lines[curLineIndex])
						&& lines[k].getOwner() != 0
						&& lines[k].getOwner() != curPlayerNum);
				}
			}

			// Если пересечение обнаружено, то выделенной остаётся только последняя выделенная точка
			if (intersectDetected) {
				selection1->setSelection(0);
				selection1 = selection2;
				selection2 = NULL;
			}
			// Если пересечений не обнаружено, отрезок присваивается текущему игроку, а ход переходит к следующему
			else {
				lines[curLineIndex].setOwner(curPlayerNum);
				selection1->setSelection(0);
				selection2->setSelection(0);
				selection1 = NULL;
				selection2 = NULL; 
				if (curPlayerNum == 1) {
					curPlayerNum = 2;
					banner.setTextureRect(IntRect(0, 200, 600, 300));
				}
				else {
					curPlayerNum = 1;
					banner.setTextureRect(IntRect(0, 100, 600, 200));
				}

				// Выполняется проверка на достижение конца игры.
				// При завершении игры каждый отрезок должен быть:
				// а) уже присвоеным одному из игроков;
				// б) неприсваиваемым игроку, к которому переходит ход.
				// Если для какого-либо отрезка оба условия не выполнены, игра продолжается.
				gameEnd = true;
				for (int k = 0; k < lines.size(); k++) {
					if (lines[k].getOwner() == 0) {
						bool lineUsable = true;
						for (int l = 0; l < lines.size(); l++) {
							lineUsable = lineUsable && (k == l ||
								(intersect(lines[k], lines[l]) && (curPlayerNum == lines[l].getOwner() || lines[l].getOwner() == 0))
								|| !(intersect(lines[k], lines[l])));
						}
						if (lineUsable) {
							gameEnd = false;
							break;
						}
					}
				}
			}
		}

		// Отрисовываются отрезки, затем поверх них точки
		for (int k = 0; k < lines.size(); k++)
			lines[k].Draw(window);
		for (int i = 0; i < dots.size(); i++) {
			dots[i].Draw(window);
		}

		// При достижении конца игры включается баннер с номером победившего игрока, окно отрисовывается, а через 6 секунд игра закрывается;
		if (gameEnd == true) {
			if (curPlayerNum == 2)
				banner.setTextureRect(IntRect(0, 300, 600, 400));
			else
				banner.setTextureRect(IntRect(0, 400, 600, 500));
			window.draw(banner);
			window.display();
			std::this_thread::sleep_for(std::chrono::seconds(6));
			window.close();
		}
		// Иначе окно отрисовывается и игра продолжается.
		else {
			window.draw(banner);
			window.display();
		}
	}
	return 0;
}

// Случайная генерация точек на поле с исключением пересечений
std::vector<Dot> generateDots(int N, int d) {
	std::vector<int> xs(N), ys(N);
	std::mt19937 mersenne(time(0)); // Инициализация вихря Мерсенна
// Программа пробует сгенерировать N координат точек на клетках размера d. 
tryRandomize:
	for (int i = 0; i < N; i++) {
		xs[i] = mersenne() % 9 + 1;
		ys[i] = mersenne() % 9 + 1;
	}

	// Далее созданные координаты проверяются на:
	// а) совпадение точек;
	// б) принадлежание трёх точек одной прямой.
	// При невыполнении данных условий генератор срабатывает заново.
	for (int i = 0; i < N - 1; i++) {
		for (int j = i + 1; j < N; j++)
			if (xs[i] == xs[j] && ys[i] == ys[j])
				goto tryRandomize;
	}

	for (int i = 0; i < N - 2; i++) {
		for (int j = i + 1; j < N + 1; j++) {
			for (int k = j + 1; k < N; k++) {
				if (ys[i] == ys[j] && ys[i] == ys[k])
					goto tryRandomize;
				if (float(xs[i] - xs[j]) / (ys[i] - ys[j]) == float(xs[i] - xs[k]) / (ys[i] - ys[k]))
					goto tryRandomize;
			}
		}
	}

	// При успешном задании координат точек генерируется и возвращается массив точек с соответствующими координатами.
	std::vector<Dot> dots(N);
	for (int i = 0; i < N; i++)
		dots[i] = (Dot(xs[i] * d, ys[i] * d));

	return dots;
}

// Проверка двух отрезков на пересечение с помощью метода ориентированных площадей
bool intersect(Line& l1, Line& l2) {
	float ax = l1.getDot1()->getX(), bx = l1.getDot2()->getX(), cx = l2.getDot1()->getX(), dx = l2.getDot2()->getX(),
		ay = l1.getDot1()->getY(), by = l1.getDot2()->getY(), cy = l2.getDot1()->getY(), dy = l2.getDot2()->getY();
	float areaABC = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
	float areaABD = (bx - ax) * (dy - ay) - (by - ay) * (dx - ax);
	float areaCDA = (cx - dx) * (cy - ay) - (cy - dy) * (cx - ax);
	float areaCDB = (cx - dx) * (cy - by) - (cy - dy) * (cx - bx);
	return areaABC * areaABD < 0 && areaCDA * areaCDB < 0;
}

// Поиск индекса отрезка по индексам точек
int lineIndex(int i1, int i2, int size) {
	int ind = 0;
	int i = 0;
	// Нужный индекс находится по циклу, аналогичному циклу создания отрезков.
	while (i != i1) {
		for (int j = i + 1; j < size; j++)
			ind++;
		i++;
	}
	ind += i2 - i1 - 1;
	return ind;
}
#pragma once
#include "Dot.h"

// Класс "Отрезок" между двумя точками типа Dot
class Line
{
private:
	// Поля отрезка:
	Dot& dot1; // Ссылка на первую точку отрезка
	Dot& dot2; // Ссылка на вторую точку отрезка
	RectangleShape body; // Прямоугольник, являющийся визуализацией отрезка
	int owner; // Владелец отрезка (0 - владельца нет, 1 и 2 - номера игроков)
public:
	// Методы отрезка:
	Line(Dot& _dot1, Dot& _dot2, int owner); // Конструктор из двух точек и номера владельца

	// Отрисовка отрезка
	void Draw(RenderWindow& window) {
		window.draw(body);
	}

	// Получение указателей на концы отрезка 
	Dot* getDot1() {
		return &dot1;
	}
	Dot* getDot2() {
		return &dot2;
	}

	// Получение номера владельца отрезка
	int getOwner() {
		return owner;
	}

	// Задание нового владельца отрезка (с изменением его внешнего вида)
	void setOwner(int newOwner);

	~Line() {} // Деструктор.
};


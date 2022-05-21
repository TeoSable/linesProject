#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

// Заголовочный файл класса "Точка"
class Dot
{
private:
	// Поля точки:
	CircleShape circle; // Круглая точка, отрисовываемая на экране
	RectangleShape hitbox; // Прямоугольное поле, регистрирующее нажатие на точку
	float x, y; // Координаты точки
	enum selectionState { noOne, player1, player2 } selection; // Переменная, отвечающая за выделение точки игроками
public:
	// Методы точки:
	Dot() = default; // Конструктор по умолчанию
	Dot(float _x, float _y); // Конструктор по координатам
	~Dot(); // Деструктор

	// Получение х и у координат точки соответственно
	float getX() const{
		return x;
	}
	float getY() const{
		return y;
	}

	// Переопределённый оператор равенства для точки (нужен только для поиска точки в массиве)
	bool operator==(const Dot& other) const {
		if (x == other.getX() && y == other.getY())
			return true;
		return false;
	}

	// Получение статуса выделения точки: 0 - не выделена, 1 и 2 - соответствующие номера игроков.
	int getSelection() {
		if (selection == noOne)
			return 0;
		if (selection == player1)
			return 1;
		if (selection == player2)
			return 2;
		return -1;
	}

	// Отрисовка точки
	void Draw(RenderWindow& window) {
		window.draw(circle);
	}

	// Проверка на парение мыши над точкой
	void checkHover(RenderWindow& window);

	// Проверка на нажатие точки мышью
	bool checkClick(RenderWindow& window, int playerNum, bool& lockClick);

	// Метод задания статуса выделения вручную
	void setSelection(int selector);
};


#include "Dot.h"

Dot::Dot(float _x, float _y) : x(_x), y(_y) {
	circle.setRadius(10);
	circle.setPointCount(30);
	circle.setPosition(_x, _y);
	circle.setOrigin(10, 10);
	circle.setFillColor(Color::White);
	circle.setOutlineThickness(3);
	circle.setOutlineColor(Color::Black);

	hitbox.setSize(Vector2f(20, 20));
	hitbox.setOrigin(10, 10);
	hitbox.setPosition(_x, _y);

	selection = noOne;
}

Dot::~Dot() {}

void Dot::checkHover(RenderWindow& window) {
	if (selection == noOne) {
		Vector2i mousePos = Mouse::getPosition(window);
		if (mousePos.x > hitbox.getPosition().x - hitbox.getSize().x / 2 && mousePos.x < hitbox.getPosition().x + hitbox.getSize().x / 2
			&&
			mousePos.y > hitbox.getPosition().y - hitbox.getSize().y / 2 && mousePos.y < hitbox.getPosition().y + hitbox.getSize().y / 2)
		{
			circle.setRadius(14);
			circle.setOrigin(14, 14);
			hitbox.setSize(Vector2f(28, 28));
		}
		else {
			circle.setRadius(10);
			circle.setOrigin(10, 10);
		}
	}
}

bool Dot::checkClick(RenderWindow& window, int playerNum, bool& lockClick)
{
	Vector2i mousePos = Mouse::getPosition(window);
	
	if (Mouse::isButtonPressed(Mouse::Left) && !lockClick &&
		mousePos.x > hitbox.getPosition().x - hitbox.getSize().x / 2 && mousePos.x < hitbox.getPosition().x + hitbox.getSize().x / 2 &&
		mousePos.y > hitbox.getPosition().y - hitbox.getSize().y / 2 && mousePos.y < hitbox.getPosition().y + hitbox.getSize().y / 2)
	{
		if (selection == noOne) {
			circle.setRadius(14);
			circle.setOrigin(14, 14);
			hitbox.setSize(Vector2f(28, 28));
			if (playerNum == 1) {
				selection = player1;
				circle.setOutlineColor(Color::Red);
			}
			else {
				selection = player2;
				circle.setOutlineColor(Color::Blue);
			}
		}
		else {
			selection = noOne;
			circle.setRadius(10);
			circle.setOrigin(10, 10);
			hitbox.setSize(Vector2f(20, 20));
			circle.setOutlineColor(Color::Black);
		}
		lockClick = true;
		return true;
	}
	return false;
}

void Dot::setSelection(int selector)
{
	if (!(selector == 0 && selection == noOne || selector == 1 && selection == player1 || selector == 2 && selection == player2)) {
		if (selector == 0) {
			selection = noOne;
			circle.setRadius(10);
			circle.setOrigin(10, 10);
			hitbox.setSize(Vector2f(20, 20));
			circle.setOutlineColor(Color::Black);
		}
		else {
			circle.setRadius(14);
			circle.setOrigin(14, 14);
			hitbox.setSize(Vector2f(28, 28));
			if (selector == 1) {
				selection = player1;
				circle.setOutlineColor(Color::Red);
			}
			else {
				selection = player2;
				circle.setOutlineColor(Color::Blue);
			}
		}
	}
}

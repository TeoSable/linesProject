#include "Line.h"
#include <cmath>

Line::Line(Dot& _dot1, Dot& _dot2, int _owner) : dot1(_dot1), dot2(_dot2), owner(_owner) {
	float distance = sqrt(pow((_dot1.getX() - _dot2.getX()), 2) + pow((_dot1.getY() - _dot2.getY()), 2));
	float pi = atan(1) * 4;

	body.setPosition(_dot1.getX(), _dot1.getY());
	body.setSize(Vector2f(distance, 5.0f));
	body.setOrigin(1, 3);
	if (_dot1.getY() > _dot2.getY())
		body.rotate(-180 / pi * acos((_dot2.getX() - _dot1.getX()) / distance));
	else
		body.rotate(180 / pi * acos((_dot2.getX() - _dot1.getX()) / distance));

	if (_owner == 1) {
		body.setFillColor(Color::Red);
	}
	else if (_owner == 2) {
		body.setFillColor(Color::Blue);
	}
	else {
		body.setFillColor(Color::Transparent);
	}
}

void Line::setOwner(int newOwner){
	owner = newOwner;
	if (newOwner == 1)
		body.setFillColor(Color::Red);
	else if (newOwner == 2)
		body.setFillColor(Color::Blue);
	else
		body.setFillColor(Color::Transparent);
}

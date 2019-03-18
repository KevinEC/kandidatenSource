#include "Transform.h"


Transform::Transform()
{
}


Transform::~Transform()
{
}

void Transform::scale()
{
}

void Transform::rotate()
{
}

void Transform::translate(Card & kort)
{
	Animate animation = Animate();
	animation.newPage();
	kort.x = 500;
	kort.y = 100;

	kort.setpos(500, 100);

	//för att rect kort ska funka

}

void Transform::translate(Card & kort, const int x, const int y)
{
	if(!kort.isDragged) {
		this->deltaX = x - kort.rect.getX1();
		this->deltaY = y - kort.rect.getY1();
	}

	float newx = x - deltaX;
	float newy = y - deltaY;

	float widthX = newx + kort.width;
	float heightY = newy + kort.height;

	kort.setpos(newx, newy);
	kort.rect.set(newx, newy, widthX, heightY);
	
}
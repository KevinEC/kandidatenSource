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
	kort.x = x;
	kort.y = y;
	kort.setpos(x, y);
	float widthX = kort.x + kort.width;
	float heightY = kort.y + kort.height;
	kort.rect.set(kort.x, kort.y, widthX, heightY);
	
}
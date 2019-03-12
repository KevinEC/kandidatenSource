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
}


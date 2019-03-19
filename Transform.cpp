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

void Transform::translate()
{
	Animate animation = Animate();
	animation.newPage();

}

//Calculate the new coordinates for card taking into consideration the relative position to the top left corner
float * Transform::translate(const float x, const float y, const float mx, const float my, const bool isDragged)
{
	if(!isDragged) {
		this->deltaX = mx - x;
		this->deltaY = my - y;
		CI_LOG_I("updated delta var");
	}

	float newx = mx - this->deltaX;
	float newy = my - this->deltaY;

	return new float[2]{ newx, newy };

}
#include "Transform.h"


Transform::Transform()
{
}

Transform::~Transform()
{
}

float Transform::scale(glm::vec2 v1, glm::vec2 v2)
{
	float res = sqrt(v1.x*v1.x + v1.y*v1.y) - sqrt(v2.x*v2.x + v2.y*v2.y);
	if (res != 0) return res;
	else return 1;
	//glm::length(v1) - glm::length(v2);
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
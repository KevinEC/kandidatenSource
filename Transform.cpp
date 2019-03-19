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

	
	//scalea, offset är kort.x - mouseclickx, kort.y - mouseclick.y.  
	//kort.rect.set(kort.x + offset.x, kort.y + offset.y, widthX, heightY);
	//kort.rect.set(kort.x + offset.x, kort.y + offset.y, widthX, heightY);
	//kort.rect.offset(offset);
	//kort.rect.offsetCenterTo(offset);
	//kort.rect.offsetCenterTo(vec2(x, y) - offset);
	

	

}
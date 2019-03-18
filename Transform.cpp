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
	//float deltax = x - kort.x;
	vec2 pos = kort.rect.getUpperLeft();
	vec2 offset = pos - vec2(x, y);
	kort.x = x;
	kort.y = y;

	//float value = ci::math<float>::clamp(kort.x);
	
	kort.setpos(kort.x, kort.y);
	float widthX = kort.x + kort.width;
	float heightY = kort.y + kort.height;
	
	kort.rect.set(kort.x + offset.x, kort.y + offset.y, widthX, heightY);
	//kort.rect.offset(offset);
	//kort.rect.offsetCenterTo(offset);
	//kort.rect.offsetCenterTo(vec2(x, y) - offset);
	

	

}
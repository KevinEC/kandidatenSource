#include "Card.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <string>
#include <iostream>

using namespace ci;
using namespace ci::app;
using namespace std;


Card::Card()
{
	x = 200;
	y = 100;
	title = "Hej Hilma";
	/*
string rubrik
string brödtext
substring brödtext; (ta 10 första orden i brödtext;)
img img1
img img2
taget från xml
*/

}


Card::~Card()
{
}

Card::Card(float n, float m, const float width, const float height) {
	x = n;
	y = m;
	this->width = width;
	this->height = height;
	//title = "hej kevin";
	string s;
	isClicked = false;
	isDragged = false;

	rect = Rectf(n, m, width, height);
	transform = Transform();


	/*
string rubrik
string brödtext
img img1
img img2
taget från xml
*/

}

/*
State getcurrentstate() {
	return currentstate;
}
*/

void Card::setpos(float m, float n)
{
	x = m;
	y = n;
}


void Card::mouseDrag(MouseEvent event)
{
	
	if (rect.contains(event.getPos())) {
		this->isClicked = true;
		this->title = "du har dragit på rektangeln";
		CI_LOG_I(this->title);
		CI_LOG_I(event.getPos() );

		float mx = event.getX();
		float my = event.getY();
		float *coords =  transform.translate(this->rect.getX1(), this->rect.getY1(), mx, my, isDragged);
		this->setpos(coords[0], coords[1]);
		this->rect.set(coords[0], coords[1], coords[0] + rect.getWidth(), coords[1] + rect.getHeight());
		//CI_LOG_I("rect: " << title);
		isDragged = true;
	}
	else {
		isDragged = false;
	}
	
	//CI_LOG_I(event.getX);
}
void Card::mouseDown(MouseEvent event)
{
	

	if (rect.contains(event.getPos())) {
		this->isClicked = true;
		this->title = "du har klickat på rektangeln";
		CI_LOG_I("title: " << title);
		//this->title = "du har klickat på rektangeln";
		//x += 20;
		//y += 20;
		//Transform::translate(*this, x, y);
		//CI_LOG_I("rect: " << title);
	}
	else {
		this->isClicked = false;
	}

}
void Card::update() {
	if (isClicked) {
		title = "du har klickat på rektangeln";
	}

}
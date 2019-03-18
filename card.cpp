#include "Card.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Transform.h"
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

Card::Card(int n, int m, const int width, const int height) {
	x = n;
	y = m;
	this->width = width;
	this->height = height;
	//title = "hej kevin";
	string s;
	isClicked = false;

	rect = Rectf(n, m, width, height);


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

void Card::setpos(int m, int n)
{
	x = m;
	y = n;
}


void Card::mouseDrag(MouseEvent event)
{
	if (rect.contains(event.getPos())) {
		this->isClicked = true;
		this->title = "du har dragit på rektangeln";
		
		//this->title = "du har klickat på rektangeln";
		//x += 20;
		//y += 20;
		x = event.getX();
		y = event.getY();
		float deltax = x - rect.getX1() ;
		float deltay = y + rect.getY1();
		CI_LOG_I("title: " << title);
		
		Transform::translate(*this, x, y);
		//CI_LOG_I("rect: " << title);
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

}
void Card::update() {
	if (isClicked) {
		title = "du har klickat på rektangeln";
	}

}
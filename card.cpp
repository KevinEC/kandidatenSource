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
string br�dtext
substring br�dtext; (ta 10 f�rsta orden i br�dtext;)
img img1
img img2

taget fr�n xml
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
	title = "hej kevin";
	string s;
	isClicked = false;

	rect = Rectf(n, m, width, height);


	/*
string rubrik
string br�dtext
img img1
img img2

taget fr�n xml
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
	
	//CI_LOG_I(event.getX);
}
void Card::mouseDown(MouseEvent event)
{

}
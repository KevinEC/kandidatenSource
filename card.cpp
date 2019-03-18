#include "Card.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <string>
#include <iostream>

using namespace ci;
using namespace ci::app;
using namespace std;

Rectf rect;

Card::Card()
{
	x = 100;
	y = 200;
	title = "detta är ett test";
	rect = Rectf(x, y, x + 100, y + 100);
}


Card::~Card()
{
}

Card::Card(float n, float m) {
	x = n;
	y = m;
	title = "test"; 




}
/*
Rectf Card::getrect(float x, float y)
{
	Rectf rect = Rectf(x, y, x + 50, y + 50);
	return rect;
}
*/
void Card::setpos(float n, float m) {
	x = n;
	y = m;
}


/*
State getcurrentstate() {
	return currentstate;
}






*/
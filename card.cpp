#include "Card.h"
#include "cinder/app/App.h"
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

Card::Card(int n, int m) {
	x = n;
	y = m;
	string s; 


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
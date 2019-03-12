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
	x = 0;
	y = 0;
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

Card::Card(float n, float m) {
	x = n;
	y = m;
	string s; 


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
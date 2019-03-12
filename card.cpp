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
	x = 100;
	y = 200;
	title = "detta är ett test";
}


Card::~Card()
{
}

Card::Card(int n, int m) {
	x = n;
	y = m;
	title = "test"; 




}
void Card::setpos(int n, int m) {
	x = n;
	y = m;
}

/*
State getcurrentstate() {
	return currentstate;
}






*/
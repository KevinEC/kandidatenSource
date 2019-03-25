#include "Card.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <string>
#include <iostream>

using namespace ci;
using namespace ci::app;
using namespace std;

const static svg::DocRef svgDoc = svg::Doc::create(loadAsset("cardV2.svg"));

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

Card::Card(float n, float m, const float width, const float height) 
{
	x = n;
	y = m;
	this->width = width;
	this->height = height;
	isClicked = false;
	isDragged = false;

	rect = Rectf(n, m, width, height);
	transform = Transform();
	this->initSvg();


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

void Card::renderTexture()
{
	TextBox text = TextBox().text("testing").color(ColorA::white());
	
	Surface srf = text.render();
	this->text =  gl::Texture::create(srf);
}

void Card::initSvg()
{
	//svg::Rect svgCardBg = svgDoc->find<svg::Rect>("cardBg");
	
	//in order to render properly and to be able to scale easier we should probably use Cairo

	std::list<svg::Node*> children = svgDoc->getChildren();
	for (auto const& i : children) {
		CI_LOG_I("child id: " << i->getId() );
	}

	cairo::SurfaceImage srfImg(svgDoc->getWidth(), svgDoc->getHeight(), true);
	cairo::Context ctx(srfImg);
	ctx.scale(0.2, 0.2);
	ctx.render(*svgDoc);
	srfImg.flush();
	this->cardBg = gl::Texture::create(srfImg.getSurface());


	//this->cardBg = srf;
}

void Card::mouseDrag(MouseEvent event)
{
	//set a bool to true when rect.contains is true once. Dont set to false until mouseUp to avoid mouse getting outside the rect
	if (isClicked) 
	{
		this->title = "du har dragit på rektangeln";
		float mx = event.getX();
		float my = event.getY();
		float *coords = transform.translate(this->rect.getX1(), this->rect.getY1(), mx, my, isDragged);
		this->setpos(coords[0], coords[1]);
		this->rect.set(coords[0], coords[1], coords[0] + rect.getWidth(), coords[1] + rect.getHeight());
		delete coords;
		isDragged = true;
	}
	else 
	{
		isDragged = false;
	}
	
}

void Card::mouseDown(MouseEvent event)
{
	if (rect.contains(event.getPos())) 
	{
		this->isClicked = true;
		this->isFront = true;
		this->title = "du har klickat på rektangeln";
		CI_LOG_I("title: " << title);
	}
	else 
	{
		this->isClicked = false;
	}
}

void Card::mouseUp(MouseEvent event)
{
	CI_LOG_I("mouseUp");
	this->isClicked = false;
	this->isDragged = false;
}

void Card::touchesBegan(TouchEvent event) 
{
	for (const auto &touch : event.getTouches()) 
	{
		if (rect.contains(touch.getPos())) 
		{
			this->isClicked = true;
			this->isFront = true;
			this->title = "du har klickat på rektangeln";
			CI_LOG_I("title: " << title);
		}
		else 
		{
			this->isClicked = false;
		}
	}
}

void Card::touchesMoved(TouchEvent event) 
{
	for (const auto &touch : event.getTouches()) 
	{
		//set a bool to true when rect.contains is true once. Dont set to false until mouseUp to avoid mouse getting outside the rect
		if (isClicked) 
		{
			this->title = "du har dragit på rektangeln";
			float mx = touch.getX();

			float my = touch.getY();
			float *coords = transform.translate(this->rect.getX1(), this->rect.getY1(), mx, my, isDragged);
			this->setpos(coords[0], coords[1]);
			this->rect.set(coords[0], coords[1], coords[0] + rect.getWidth(), coords[1] + rect.getHeight());
			delete coords;
			isDragged = true;
		}
		else 
		{
			isDragged = false;
		}
	}
}

void Card::touchesEnded(TouchEvent event) 
{
	CI_LOG_I("touchesEnded");
	this->isClicked = false;
	this->isDragged = false;
}

void Card::update() 
{
	if (isClicked) 
	{
		title = "du har klickat på rektangeln";
	}
}
#include "Card.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <string>
#include <iostream>

using namespace ci;
using namespace ci::app;
using namespace std;

const static svg::DocRef svgDoc = svg::Doc::create(loadAsset("cardMod.svg"));

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

Card::Card(const float x1, const float y1, std::string title, std::string body) 
{
	x = x1;
	y = y1;

	cardSize = 1.2;
	width = 336.0f*cardSize;
	height = 500.0f*cardSize;

	this->title = title;
	this->body = body;


	isClicked = false;
	isDragged = false;

	rect = Rectf(x1, y1, x1 + width, y1 + height);
	transform = Transform();
	this->initElements();


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

gl::TextureRef Card::renderTexture(TextBox &text)
{
	Surface srf = text.render();
	return gl::Texture::create(srf);
}

void Card::initElements()
{
	paddingX = 22.0f*cardSize;
	elementWidth = 292.0f *cardSize;
	//const ColorA textColor = ColorA(65, 64, 66, 0.5f);
	const ColorA textColor = ColorA::black();
	const Font titleFont = Font(loadAsset("fonts/Montserrat.ttf"), 20.0*cardSize);
	const Font bodyFont = Font(loadAsset("fonts/Raleway.ttf"), 14.0*cardSize);

	updateElementCoords();

	TextBox titleBox = TextBox().text(title).font(titleFont).color(textColor).size(elementWidth, TextBox::GROW);
	TextBox bodyBox = TextBox().text(body).font(bodyFont).color(textColor).size(elementWidth, TextBox::GROW);

	CI_LOG_I("CARD COORD:   x: " << x << "  y: " << y);
	CI_LOG_I("TEXTBOX:   " << titleCo);


	titleTex = renderTexture(titleBox);
	bodyTex = renderTexture(bodyBox);
}

void Card::updateElementCoords()
{
	paddingX = 22.0f*cardSize;
	elementWidth = 292.0f *cardSize;

	float imgY = 22.0f*cardSize;
	float titleY = 268.0f*cardSize;
	float bodyY = 320.0f*cardSize;
	float tagsY = 459.0f*cardSize;

	imgCo = vec2(x + paddingX, y + imgY);
	titleCo = vec2(x + paddingX, y + titleY);
	bodyCo = vec2(x + paddingX, y + bodyY);
	tagsCo = vec2(x + paddingX, y + tagsY);
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
		updateElementCoords();
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
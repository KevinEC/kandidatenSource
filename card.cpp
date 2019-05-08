#include "card.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace bluecadet::core;
using namespace bluecadet::views;
using namespace bluecadet::touch;

Card::Card()
{
	x = 200;
	y = 100;
	title = "Hej Hilma";
}

Card::~Card()
{
}

Card::Card(const float x1, const float y1, std::string title, std::string body) 
{
	x = x1;
	y = y1;

	cardSize = 1.0f;
	width = 336.0f*cardSize;
	height = 500.0f*cardSize;

	this->title = title;
	this->body = body;

	isClicked = false;
	isDragged = false;

	rect = Rectf(x1, y1, x1 + width, y1 + height);

	twoTouches = false;
	initFingDist = 0;

	transform = Transform();
	initElements();
	setStyles();
}

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
	//create bluecadet touchview
	object = make_shared<TouchView>();
	object->setSize({ 336.0f*cardSize, 500.0f*cardSize });
	object->setPosition({ x,y });
	object->setDragEnabled(true);
	object->setMultiTouchEnabled(true);

	//create border
	StrokedRoundedRectViewRef border = make_shared<StrokedRoundedRectView>();
	//border->setBackgroundColor(borderColor);
	border->setStrokeColor(borderColor);
	border->setCornerRadius(borderRadius);
	border->setSize({ object->getSize() });
	object->addChild(border);
}

void Card::updateElementCoords()
{
	paddingX = 22.0f*cardSize;
	elementWidth = 292.0f *cardSize;

	const Font titleFont = Font(montserrat, 20.0*cardSize);
	const Font bodyFont = Font(raleway, 14.0*cardSize);

	TextBox titleBox = TextBox().text(title).font(titleFont).color(textColor).size(elementWidth, 30 * cardSize);
	TextBox bodyBox = TextBox().text(body).font(bodyFont).color(textColor).size(elementWidth, 129 * cardSize);

	//Texturer kan inte skapas i en renderings loop..?
	/*titleTex = renderTexture(titleBox);
	bodyTex = renderTexture(bodyBox);*/

	float imgY = 22.0f*cardSize;
	float titleY = 268.0f*cardSize;
	float bodyY = 320.0f*cardSize;
	float tagsY = 459.0f*cardSize;

	imgCo = vec2(x + paddingX, y + imgY);
	titleCo = vec2(x + paddingX, y + titleY);
	bodyCo = vec2(x + paddingX, y + bodyY);
	tagsCo = vec2(x + paddingX, y + tagsY);
}

void Card::update() 
{
	updateElementCoords();
}

void Card::setStyles()
{
	object->setBackgroundColor(Color::hex(0xfcfcfc)); // off-white
	borderColor = Color::hex(0xbcbcbc);
	borderRadius = 5.0f;
}

void Card::renderCard() 
{
	gl::color(bgColor);
    int cornerSegments = 5;
	gl::drawSolidRoundedRect(rect, borderRadius, cornerSegments);

	gl::color(borderColor);
	gl::drawStrokedRoundedRect(rect, borderRadius, cornerSegments);

	gl::color(Color::white());
	gl::draw(titleTex, titleCo);
	gl::draw(bodyTex, bodyCo);

}
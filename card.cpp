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
	initDist = 0;
	currDist = 0;

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
	/*
	paddingX = 22.0f*cardSize;
	elementWidth = 292.0f *cardSize;
	//const ColorA textColor = ColorA(65, 64, 66, 0.5f);
	textColor = ColorA::hex(0x1d1d1d);
	montserrat = loadAsset("fonts/Montserrat.ttf");
	raleway = loadAsset("fonts/Raleway.ttf");

	const Font titleFont = Font(montserrat , 20.0*cardSize);
	const Font bodyFont = Font(raleway, 14.0*cardSize);

	updateElementCoords();

	TextBox titleBox = TextBox().text(title).font(titleFont).color(textColor).size(elementWidth, 30*cardSize);
	TextBox bodyBox = TextBox().text(body).font(bodyFont).color(textColor).size(elementWidth, 129*cardSize);


	titleTex = renderTexture(titleBox);
	bodyTex = renderTexture(bodyBox);*/

	//create bluecadet touchview
	object = make_shared<TouchView>();
	object->setSize({ 336.0f*cardSize, 500.0f*cardSize });
	object->setPosition({ x,y });
	object->setDragEnabled(true);
	object->setMultiTouchEnabled(true);
	object->setTransformOrigin(object->getCenter());

	//link touchevents
	object->getSignalTouchBegan().connect([=](bluecadet::touch::TouchEvent e) {
		this->handleTouchBegan(&e);
	});
	object->getSignalTouchMoved().connect([=](bluecadet::touch::TouchEvent e) {
		this->handleTouchMoved(&e);
	});
	object->getSignalTouchEnded().connect([=](bluecadet::touch::TouchEvent e) {
		this->handleTouchMoved(&e);
	});

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

void Card::handleTouchBegan(bluecadet::touch::TouchEvent* touchEvent)
{
	// init vars for conditional handling of touchpoints
	firstTouchPoint = true;
	firstTouchId = 0;
	maxDist = 0;
	initDist = -1;

	// add touchpoints on began
	activeTouches.insert(make_pair(touchEvent->touchId, *touchEvent));
}

void Card::handleTouchMoved(bluecadet::touch::TouchEvent* touchEvent)
{
	// update the touchpoint coord
	activeTouches.at(touchEvent->touchId) = *touchEvent;

	// reset vars too determine the loongest distance between all active touchpoints
	maxDist = 0;
	currDist = 0;

	float sumX = 0;
	float sumY = 0;

	if ( object->getNumTouches() >= 2 )
	{
		//find longest dist
		for (auto touch : activeTouches)
		{
			//save the first touch point id
			if (firstTouchPoint) {
				firstTouchId = touch.first;
				firstTouchPoint = false;
			}

			// measure the distance from the first touchpoint on the card to each individual touchpoint
			currDist = abs(glm::distance(activeTouches.at(firstTouchId).globalPosition, touch.second.globalPosition));

			// determine the longest distance
			if (currDist > maxDist) maxDist = currDist;

			sumX += touch.second.globalPosition.x;
			sumY += touch.second.globalPosition.y;

			
		}

		// fool lösning too save the inital distance
		if (initDist == -1) initDist = maxDist;
		cardSize = maxDist / initDist;

		// determine mid point
		float midX = sumX / activeTouches.size();
		float midY = sumY / activeTouches.size();
		vec2 midPos = { midX, midY };


		object->setTransformOrigin(midPos - object->getCenter());
		if (cardSize > 0.2 && cardSize < 1.5) {
			object->setScale(cardSize);
		}
		vec2 newPos = midPos - object->getCenter();

		//object->setPosition(newPos);

	}

}

void Card::handleTouchEnded(bluecadet::touch::TouchEvent* touchEvent)
{
	activeTouches.clear();
}

void Card::update() 
{
	updateElementCoords();
}

void Card::handleTouchBegan(const bluecadet::touch::TouchEvent& touchEvent) {}
void Card::handleTouchMoved(const bluecadet::touch::TouchEvent& touchEvent) {}
void Card::handleTouchEnded(const bluecadet::touch::TouchEvent& touchEvent) {}

void Card::setStyles()
{
	object->setBackgroundColor(Color::hex(0xfcfcfc)); // off-white
	borderColor = Color::hex(0xbcbcbc);
	borderRadius = 5.0f;
}

void Card::renderCard() {

	int cornerSegments = 5;

	gl::color(bgColor);

	gl::drawSolidRoundedRect(rect, borderRadius, cornerSegments);

	gl::color(borderColor);

	gl::drawStrokedRoundedRect(rect, borderRadius, cornerSegments);

	gl::color(Color::white());

	gl::draw(titleTex, titleCo);
	gl::draw(bodyTex, bodyCo);


}


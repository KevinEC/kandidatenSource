#include "card.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace bluecadet::core;
using namespace bluecadet::views;
using namespace bluecadet::touch;
using namespace bluecadet::text;

Card::Card()
{
	x = 200.0f;
	y = 100.0f;
	titleText = "empty";
	angle = 0;
	lastanglechange = 0;
}

Card::~Card()
{
}

Card::Card(const float x1, const float y1, pair<string, string> title, pair<string, string> body, string imgPath)
{
	x = x1;
	y = y1;
	angle = 0;
	lastanglechange = 0;

	cardSize = 1.0f;
	width = 336.0f*cardSize;
	height = 500.0f*cardSize;

	this->titleText = title.first;
	this->bodyText = body.first;
	this->imgPath = imgPath;

	isClicked = false;
	isDragged = false;
	flipped = false;

	twoTouches = false;
	initFingDist = 0;

	transform = Transform();
	setStyles();
	initElements();
}

gl::TextureRef Card::renderTexture(StyledTextLayoutRef text)
{
	auto srf = text->renderToSurface();
	return gl::Texture::create(srf);
}

void Card::initElements()
{
	//create bluecadet touchview
	object = make_shared<TouchView>();
	object->setSize({ width, height });
	object->setPosition({ x,y });
	//object->moveToFront();
	object->setBackgroundColor(Color::white());
	object->setDebugDrawTouchPath(false);
	object->setDragEnabled(false);
	object->setMultiTouchEnabled(true);
	object->setHidden(false);
	//object->setTransformOrigin(object->getPosition());

	//link touchevents
	object->getSignalTouchBegan().connect([=](bluecadet::touch::TouchEvent e) {
		this->handleTouchBegan(&e);
		//hasbeentransformed = false;
		//hasbeentransformed = true;
		//object->setDragEnabled(false);
	});
	object->getSignalTouchMoved().connect([=](bluecadet::touch::TouchEvent e) {
		this->handleTouchMoved(&e);
		//hasbeentransformed = false;
		//object->setDragEnabled(false);
	});
	object->getSignalTouchEnded().connect([=](bluecadet::touch::TouchEvent e) {
		this->handleTouchEnded(&e);


		//hasbeentransformed = true;
		//object->setDragEnabled(false);
	});
	object->getSignalTapped().connect([=](bluecadet::touch::TouchEvent e) {
		this->handleTouchTapped(&e);
	});


	/* FIRST SET COMMON VIEWS */

	//create border
	border = make_shared<StrokedRoundedRectView>();
	border->setBackgroundColor(bgColor);
	border->setStrokeWidth(borderWidth);
	border->setStrokeColor(borderColor);
	border->setCornerRadius(borderRadius);
	border->setSize({ object->getSize() });
	object->addChild(border);

	
	//init Font manager
	FontManager::getInstance()->setup(getAssetPath("fonts/fonts.json"));

	// main content box
	float size = 0.9f;
	float padding = (1.0f - size) / 2;

	/* CREATE FRONT AND BACK LAYOUTS*/
	
	//create front layout content box
	contentBoxFront = make_shared<BaseView>();
	contentBoxFront->setSize({ object->getSize() * size });
	contentBoxFront->setPosition({ object->getSize() * padding });
	object->addChild(contentBoxFront);

	//create back layout content box
	contentBoxBack = make_shared<BaseView>();
	contentBoxBack->setSize({ object->getSize() * size });
	contentBoxBack->setPosition({ object->getSize() * padding });
	object->addChild(contentBoxBack);

	//create and load image
	image = make_shared<ImageView>();
	DataSourceRef imgUrl = DataSourceUrl::create(Url::Url(imgPath));
	auto srf = loadImage(imgUrl, ImageSource::Options());
	image->setTexture(gl::Texture::create(srf));

	//init all views
	roundImgBorder = make_shared<StrokedRoundedRectView>();
	imgMask = make_shared<MaskView>();
	title = make_shared<TextView>();
	body = make_shared<TextView>();
	colorLayer = make_shared<StrokedRoundedRectView>();

	setFrontLayout();
	setBackLayout();

	contentBoxFront->setHidden(true);
	contentBoxBack->setHidden(false);

	
	//move the touchView to front to ensure touch is not blocked by child views
	object->moveToFront();
	
}

void Card::setStyles()
{
	bgColor = Color::hex(0xfcfcfc); // off-white
	borderColor = Color::hex(0xbcbcbc);
	borderRadius = 8.0f;
	borderWidth = 1.0f;


	textColor = Color::hex(0x393939);
}

void Card::toggleView()
{

	// set transformorigin 
	//object->setTransformOrigin(object->convertGlobalToLocal(object->getCenter()));

	//animate me
	object->cancelAnimations();
	// --> scale up
	object->getTimeline()->apply(&object->getScale(), vec2(object->getScale() + 0.05f), 0.1f, EaseOutExpo());

	// --> change content

	//front side layout
	if (!flipped)
	{
		flipped = !flipped;

		setFrontLayout();
		contentBoxBack->setHidden(true);
		contentBoxFront->setHidden(false);
	}
	//back side layout
	else
	{
		flipped = !flipped;

		setBackLayout();
		contentBoxBack->setHidden(false);
		contentBoxFront->setHidden(true);
	}

	// scale back to finish animation
	object->getTimeline()->appendTo(&object->getScale(), vec2(object->getScale()), 0.16f, EaseOutBack(2.0f));

}

void Card::setFrontLayout()
{
	//set img
	float imgHeight = 200.0f;

	image->setWidth(contentBoxFront->getWidth());
	image->setHeight(imgHeight);
	image->setScaleMode(bluecadet::views::ImageView::ScaleMode::COVER);


	//create img rounded mask 
	float borderPaddingOffset = 10.0f;
	vec2 offsetPos = { -borderPaddingOffset / 2, -borderPaddingOffset / 2 };

	roundImgBorder->setWidth(image->getWidth());
	roundImgBorder->setHeight(image->getHeight());
	roundImgBorder->setCornerRadius(borderRadius);
	roundImgBorder->setBackgroundColor(bgColor); // color doesnt matter just set it to enable the background
	roundImgBorder->setStrokeColor(bgColor);
	roundImgBorder->setStrokeWidth(borderPaddingOffset);
	roundImgBorder->setSmoothness(1.0f);

	//create mask
	imgMask->setMaskType(MaskView::MaskType::REVEAL);
	imgMask->setMask(roundImgBorder);
	imgMask->addChild(image);
	contentBoxFront->addChild(imgMask);

	//create title
	title->setText(titleText);
	title->setTextColor(textColor);
	title->setFontSize(20.0f);
	title->setFontFamily("Montserrat");
	title->setFontWeight(bluecadet::text::FontWeight::Medium);
	title->setTextAlign(TextAlign::Left);
	title->setPosition({ 0, 220.0f });
	title->setWidth(contentBoxFront->getWidth());
	//title->setBackgroundColor(Color::hex(0x0f2ff3)); // nice for debugging
	contentBoxFront->addChild(title);

	//create body
	float relativeYpos = title->getPositionConst().y + title->getHeight() + 20.0f;
	float relativeHeight = contentBoxFront->getHeight() - relativeYpos;

	body->setText(bodyText);
	body->setTextColor(textColor);
	body->setFontSize(15.0f);
	body->setFontFamily("Raleway");
	body->setPosition({ 0, relativeYpos });
	body->setWidth(contentBoxFront->getWidth());
	body->setTextAlign(TextAlign::Left);
	//body->setBackgroundColor(Color::hex(0x0fdf43)); // nice for debugging
	body->setHeight(relativeHeight);
	contentBoxFront->addChild(body);
}

void Card::setBackLayout()
{
	// set image 
	image->setHeight(object->getHeight());
	image->setWidth(object->getWidth());
	image->setScaleMode(bluecadet::views::ImageView::ScaleMode::COVER);

	//update mask
	imgMask->setMask(border);
	imgMask->addChild(image);
	object->addChild(imgMask);

	// colorLayer
	colorLayer->setWidth(object->getWidth());
	colorLayer->setHeight(object->getHeight());
	/*colorLayer->setStrokeWidth(18.5f);
	colorLayer->setStrokeColor(Color::white());*/
	colorLayer->setCornerRadius(borderRadius);
	colorLayer->setBackgroundColor(ColorA(0.0f, 0.0f, 0.0f, 0.5f));
	imgMask->addChild(colorLayer);

	//need to add this again cuz the mask covers it
	object->addChild(contentBoxBack);

	// title
	vec2 titlePos = { 0.0f, contentBoxBack->getHeight() / 2 };
	title->setPosition(titlePos);
	title->setTextColor(Color::white());
	title->setTextAlign(TextAlign::Center);
	title->setWidth(contentBoxBack->getWidth());
	contentBoxBack->addChild(title);

}


void Card::animateTranslate(const vec2 pos)
{
	object->getTimeline()->apply(&object->getPosition(), pos , 2.0f, EaseOutExpo());
}

void Card::handleTouchTapped(bluecadet::touch::TouchEvent* touchEvent)
{
	//object->setTransformOrigin(object->convertGlobalToLocal(object->getCenter()));
	object->moveToFront();
	toggleView();
}


void Card::inserttouchevent(bluecadet::touch::TouchEvent* touchEvent) {

	for (int i = 0; i < activeTouches.size(); i++)
	{
		if (activeTouches[i].touchId == touchEvent->touchId) {
			currentpos[i] = activeTouches[i].globalPosition;
			activeTouches[i] = *touchEvent;
			activeTouches[i].touchId = touchEvent->touchId;

		}
	}
}
void Card::handleTouchBegan(bluecadet::touch::TouchEvent* touchEvent)
{	
	object->moveToFront();
	activeTouches.insert(activeTouches.begin(), *touchEvent);
	currentpos.insert(currentpos.begin(), touchEvent->globalPosition);

}

void Card::handleTouchMoved(bluecadet::touch::TouchEvent* touchEvent)
{
	inserttouchevent(touchEvent);

	vec2 a1, b1, a2, b2, c1, c2;

	//drag object with 1 touch
	if (object->getNumTouches() == 1 && activeTouches.size() != 2) {
		ci::vec2 newPosition = object->getPosition();
		newPosition.x += activeTouches[0].globalPosition.x - currentpos[0].x;
		newPosition.y += activeTouches[0].globalPosition.y - currentpos[0].y;
		object->setPosition(newPosition);
	}
	//ROTATION AND SCALING
	if (object->getNumTouches() == 2 && activeTouches.size() == 2)
	{
		//if we have done any other transformorigin
		//object->setTransformOrigin(vec2(0, 0));
		object->setDragEnabled(false);
		a1 = currentpos[0];
		b1 = currentpos[1];
		a2 = activeTouches[1].globalPosition;
		b2 = activeTouches[0].globalPosition;

		auto c1 = (a1 + b1)*0.5f;
		auto c2 = (a2 + b2)*0.5f;

		//measure the distance
		currDist = sqrt(pow(a2.x - b2.x, 2) + pow(a2.y - b2.y, 2));
		float prevDist = sqrt(pow(a1.x - b1.x, 2) + pow(a1.y - b1.y, 2));

		vec2 v1 = b1 - a1;
		vec2 v2 = b2 - a2;
		v1 = glm::normalize(v1);
		v2 = glm::normalize(v2);
		vec2 p1 = object->getPosition();


		float deltaAngle = atan2(v2.y, v2.x) - atan2(v1.y, v1.x);
		CI_LOG_I(glm::degrees(deltaAngle));
		
		angle += deltaAngle;
		

		cardSize *= currDist / prevDist;

		mat2 R = mat2(cos(deltaAngle), sin(deltaAngle), -sin(deltaAngle), cos(deltaAngle));


		CI_LOG_I("test: " << abs((glm::degrees(lastanglechange) - glm::degrees(deltaAngle))));
		
		vec2 p2 = c2 + R * (p1 - c1)*(currDist / prevDist);
		//tried to make logic for not transforming when 1 touch is ended.
		//if (!hasbeentransformed){
			//&& ((lastanglechange == 0) || ( abs((glm::degrees(lastanglechange) - glm::degrees(deltaAngle))) < 90))){	
		object->setScale(cardSize);
		object->setPosition(p2);
		object->setRotation(angle);

		//lastanglechange = deltaAngle;


		//hasbeentransformed = true;
		//}

	}
	//if we have more then 2 touches on the object, only translate with the first touchpoint on the object.
	else if (object->getNumTouches() > 2) {
		ci::vec2 newPosition = object->getPosition();
		newPosition.x += activeTouches[activeTouches.size() - 1].globalPosition.x - currentpos[currentpos.size() - 1].x;
		newPosition.y += activeTouches[activeTouches.size() - 1].globalPosition.y - currentpos[currentpos.size() - 1].y;
		object->setPosition(newPosition);
	}
	
}

void Card::handleTouchEnded(bluecadet::touch::TouchEvent* touchEvent)
{
	//call for remove touch event. 
	//maybe add a timer where we wait for set amount of time before deleting, might solve the problem with rotation when removing 1 finger and not 2. 
	//calling for function for removing the touchevent from our vectors.
	removetouchevent(touchEvent);
	/*for (int i = 0; i < activeTouches.size(); ++i) {

	activeTouches.clear();
	*/

}

void Card::removetouchevent(bluecadet::touch::TouchEvent* touchEvent) {

	for (int i = 0; i < activeTouches.size(); ++i) {

		if (activeTouches[i].touchId == touchEvent->touchId) {
			//CI_LOG_I("activetouches size: " << activeTouches.size());
			activeTouches.erase(activeTouches.begin() + i);
			currentpos.erase(currentpos.begin() + i);
		}
	}
}

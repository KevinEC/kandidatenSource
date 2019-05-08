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
	titleText = "Hej Hilma";
}

Card::~Card()
{
}

Card::Card(const float x1, const float y1, std::string title, std::string body, std::string imgPath)
{
	x = x1;
	y = y1;

	cardSize = 1.0f;
	width = 336.0f*cardSize;
	height = 500.0f*cardSize;

	this->titleText = title;
	this->bodyText = body;
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
	object->setSize({ 336.0f, 500.0f });
	object->setPosition({ x,y });
	object->setDragEnabled(true);
	object->setMultiTouchEnabled(true);
	object->setTransformOrigin(object->getCenter());

	//link touchevents
	object->getSignalTapped().connect([=](bluecadet::touch::TouchEvent e) {
		this->handleTouchTapped(&e);
	});

	object->getSignalTouchBegan().connect([=](bluecadet::touch::TouchEvent e) {
		this->handleTouchBegan(&e);
	});
	object->getSignalTouchMoved().connect([=](bluecadet::touch::TouchEvent e) {
		this->handleTouchMoved(&e);
	});
	object->getSignalTouchEnded().connect([=](bluecadet::touch::TouchEvent e) {
		this->handleTouchMoved(&e);
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


	//move the touchView to front to ensure touch is not blocked by chuld views
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
	object->setTransformOrigin(object->convertGlobalToLocal(object->getCenter()));

	//reset scale
	object->setScale(1.0f);
	
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

void Card::handleTouchTapped(bluecadet::touch::TouchEvent* touchEvent)
{
	toggleView();
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

	object->moveToFront();
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


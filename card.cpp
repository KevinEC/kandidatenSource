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
	x = 200;
	y = 100;
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

	rect = Rectf(x1, y1, x1 + width, y1 + height);

	twoTouches = false;
	initDist = 0;
	currDist = 0;

	transform = Transform();
	setStyles();
	initElements();

}

void Card::setpos(float m, float n)
{
	x = m;
	y = n;
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
	border->setBackgroundColor(bgColor);
	border->setStrokeWidth(borderWidth);
	border->setStrokeColor(borderColor);
	border->setCornerRadius(borderRadius);
	border->setSize({ object->getSize() });
	object->addChild(border);

	// main content box
	float size = 0.9f;
	float padding = (1.0f - size) / 2;

	//create main content box
	BaseViewRef contentBox = make_shared<BaseView>();
	contentBox->setSize({ object->getSize() * size });


	contentBox->setPosition({ object->getSize() * padding });
	object->addChild(contentBox);


	//create img
	float imgHeight = 200.0f;

	ImageViewRef image = make_shared<ImageView>();	
	DataSourceRef imgUrl = DataSourceUrl::create(Url::Url(imgPath));
	auto srf = loadImage(imgUrl, ImageSource::Options());

	image->setTexture(gl::Texture::create(srf));
	image->setWidth(contentBox->getWidth());
	image->setHeight(imgHeight);
	image->setScaleMode(bluecadet::views::ImageView::ScaleMode::COVER);
	//contentBox->addChild(image);


	//create img rounded mask 
	float borderPaddingOffset = 10.0f;
	vec2 offsetPos = { -borderPaddingOffset / 2, -borderPaddingOffset / 2 };

	StrokedRoundedRectViewRef roundImgBorder = make_shared<StrokedRoundedRectView>();
	//roundImgBorder->setPosition(offsetPos);
	roundImgBorder->setWidth(image->getWidth());
	roundImgBorder->setHeight(image->getHeight());
	roundImgBorder->setCornerRadius(borderRadius);
	roundImgBorder->setBackgroundColor(bgColor); // color doesnt matter just set it to enable the background
	roundImgBorder->setStrokeColor(bgColor);
	roundImgBorder->setStrokeWidth(borderPaddingOffset);
	roundImgBorder->setSmoothness(1.0f);
	//image->addChild(roundImgBorder);

	//create mask
	MaskViewRef imgMask = make_shared<MaskView>();
	imgMask->setMaskType(MaskView::MaskType::REVEAL);
	imgMask->setMask(roundImgBorder);
	imgMask->addChild(image);
	contentBox->addChild(imgMask);


	//init Font manager
	FontManager::getInstance()->setup(getAssetPath("fonts/fonts.json"));

	//create title
	auto title = make_shared<TextView>();
	title->setText(titleText);
	title->setTextColor(textColor);
	title->setFontSize(20.0f);
	title->setFontFamily("Montserrat");
	title->setFontWeight(bluecadet::text::FontWeight::Medium);
	title->setPosition({ 0, 220.0f });
	title->setWidth(contentBox->getWidth());
	//title->setBackgroundColor(Color::hex(0x0f2ff3)); // nice for debugging

	contentBox->addChild(title);

	//create body
	float relativeYpos = title->getPositionConst().y + title->getHeight() + 20.0f;
	float relativeHeight = contentBox->getHeight() - relativeYpos;

	auto body = make_shared<TextView>();
	body->setText(bodyText);
	body->setTextColor(textColor);
	body->setFontSize(15.0f);
	body->setFontFamily("Raleway");
	body->setPosition({ 0, relativeYpos });
	body->setWidth(contentBox->getWidth());
	body->setTextAlign(bluecadet::text::TextAlign::Left);
	//body->setBackgroundColor(Color::hex(0x0fdf43)); // nice for debugging
	body->setHeight(relativeHeight);
	contentBox->addChild(body);

	//move the touchView to front to ensure touch is enabled
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

void Card::renderCard() {



}

void Card::draw()
{
	gl::clear();
	//gl::draw(titleTexture, {900, 900});
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
#include "card.h"
#include "bluecadet/core/BaseApp.h"
#include "bluecadet/views/TouchView.h"
#include "bluecadet/views/TextView.h"
#include "bluecadet/views/ImageView.h"
#include "cinder/Signals.h"

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
	angle = 0;
	//currentpos = 0;
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
	angle = 0;

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
	object->moveToFront();
	object->setBackgroundColor(Color::white());
	object->setDebugDrawTouchPath(true);
	object->setDragEnabled(true);
	object->setMultiTouchEnabled(true);
	object->setTransformOrigin(object->getCenter());
	object->setHidden(false);

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

	/*auto labelView = make_shared<TextView>();
	//labelView->setBackgroundColor(ColorA(0, 0, 0, 0.25f));
	labelView->setPadding(10, 10);
	labelView->setWidth(object->getWidth());
	labelView->setFontSize(50.0f);
	labelView->setTextColor(Color::black());
	labelView->setTextAlign(bluecadet::text::TextAlign::Center);
	labelView->setText("hej detta är ett test");
	object->addChild(labelView);*/

	auto labelView = make_shared<TextView>();
	labelView->setPadding(10, 10);
	labelView->setWidth(object->getWidth());
	labelView->setFontSize(50.0f);
	labelView->setTextColor(Color::black());
	labelView->setTextAlign(bluecadet::text::TextAlign::Center);

	labelView->setText("This is a test");

	object->addChild(labelView);
	

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


void Card::handleTouchBegan(bluecadet::touch::TouchEvent* touchEvent)
{
	// init vars for conditional handling of touchpoints
	firstTouchPoint = true;
	firstTouchId = 0;
	maxDist = 0;
	initDist = -1;

	// add touchpoints on began
	activeTouches.insert(activeTouches.begin(), *touchEvent);
	currentpos.insert(currentpos.begin(), touchEvent->globalPosition);
}

void Card::handleTouchMoved(bluecadet::touch::TouchEvent* touchEvent)
{
	// update the touchpoint coord
	//activeTouches.push_back(*touchEvent);
	//activeTouches.at(touchEvent->touchId) = *touchEvent;

	// reset vars too determine the loongest distance between all active touchpoints
	maxDist = 0;
	currDist = 0;

	float sumX = 0;
	float sumY = 0;
	vec2 a1, b1, a2, b2, c1, c2;
	for (int i = 0; i < activeTouches.size(); i++)
	{

		if (activeTouches[i].touchId == touchEvent->touchId) {
			CI_LOG_I("vi hittade samma touch som flyttas: " << activeTouches.size());
			//currentpos[i] = activeTouches[i].globalPosition;
			currentpos.insert(currentpos.begin(), activeTouches[i].globalPosition);
			activeTouches[i] = *touchEvent;

		}
	}

	if (object->getNumTouches() >= 2 && object->hasMovingTouches())
	{
		
		a1 = currentpos[0];
		b1 = currentpos[1];
		a2 = activeTouches[0].globalPosition;
		b2 = activeTouches[1].globalPosition;
			
			CI_LOG_I("activetouches: " << activeTouches.size());
			//CI_LOG_I(touch.second.globalPosition)

			// measure the distance from the first touchpoint on the card to each individual touchpoint
			//currDist = abs(glm::distance(a2, b2));
			//int prevdist = abs(glm::distance(a1, b1));
			
			float c1x = (a1.x + b1.x) / 2;
			float c1y = (a1.y + b1.y) / 2;
			float c2x = (a2.x + b2.x) / 2;
			float c2y = (a2.y + b2.y) / 2;



			currDist = sqrt(pow(a2.x - b2.x, 2) + pow(a2.y - b2.y, 2));
			float prevDist = sqrt(pow(a1.x - b1.x, 2) + pow(a1.y - b1.y, 2));


			c1 = vec2(c1x, c1y);
			c2 = vec2(c2x, c2y);

			vec2 v2 = b1 - a1;
			vec2 v1 = b2 - a2;
			v1 = glm::normalize(v1);
			v2 = glm::normalize(v2);
			vec2 temp = object->getCenter() - c1;


			float deltaAngle = atan2(v2.y, v2.x) - atan2(v1.y, v1.x);
			//float deltaAngle = glm::orientedAngle(v1, v2);
			//CI_LOG_I("touch.second: " << touch.second.globalPosition.x << "active: " << activeTouches.at(firstTouchId).globalPosition.x << "currentpos: " << currentpos[0]);
			CI_LOG_I("a:2 " << a2 << "b:2 " << b2 << "a1: " << a1 << "b1: "<< b1);

			temp = rotate(temp, deltaAngle);

			angle += deltaAngle;
			
			//if (angle > M_PI || angle < -M_PI) angle = 0;
			CI_LOG_I("angle: " << glm::degrees(angle));

			if (prevDist != 0 && prevDist == prevDist) {
				temp = temp * (currDist / prevDist);
				cardSize *= currDist / prevDist;
			}
			
		
		mat2 R = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));


		// determine mid point
		float midX = sumX / activeTouches.size();
		float midY = sumY / activeTouches.size();
		//vec2 midPos = { midX, midY };
		
		vec2 p2 = c2 + (R*(object->getCenter() - c1))*cardSize;
		
		
		
		object->setTransformOrigin(c2 - object->getCenter());
		//object->setTransformOrigin(object->convertGlobalToLocal(object->getCenter()));
		//object->setTransformOrigin(object->getCenter());
		//object->setTransformOrigin(p2);
		if (cardSize > 0.4 && cardSize < 1.5) {
			CI_LOG_I("cardSize: " << cardSize << "angle: " << angle << "p2: " << p2);
			object->setScale(cardSize);
			object->setPosition(temp + c2);
			object->setRotation(angle);
		}


	}
	//object->setDragEnabled(true);

}

void Card::handleTouchEnded(bluecadet::touch::TouchEvent* touchEvent)
{
	for (int i = 0; i < activeTouches.size(); ++i) {
		if (activeTouches[i].touchId == touchEvent->touchId) {
			activeTouches.erase(activeTouches.begin() + i);
			//currentpos.erase(currentpos.begin() + i);
			
			
		}
	}
	
}
/*
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
*/
/*
void Card::touchesBegan(TouchEvent event)
{
	for (const auto &touch : event.getTouches()) //event.getTouches()) returns std::vector<Touch>
	{
		if (rect.contains(touch.getPos()))
		{
			if (this->isClicked == true) // rect contains two touch points // && rect.contains(lastTouch.getPos()
			{
				this->twoTouches = true;
				this->initFingDist = glm::distance(lastTouch.getPos(), touch.getPos());
				CI_LOG_I("initial finger distance: " << initFingDist);
			//	CI_LOG_I("vi har två fingrar på rektangeln");
			}
			else
			{
				this->twoTouches = false;
				lastTouch = touch;
			}

			this->isClicked = true;
			this->isFront = true;
			this->title = "du har klickat på rektangeln";

			CI_LOG_I("title: " << title);
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
			updateElementCoords();

			delete coords;
			isDragged = true;
		}

		if (this->twoTouches) // rect contains two active touch points
		{
			float currFingDist = glm::distance(lastTouch.getPos(), touch.getPos());
			float size = currFingDist / this->initFingDist;
			this->cardSize = size;
			CI_LOG_I("size: " << size);

			if (this->rect.getWidth()*size > 300 & this->rect.getWidth()*size < 1500)
			{
				this->rect.scaleCentered(size);
			}
			//float *coord = transform.rotate();
		}
	}
}

void Card::touchesEnded(TouchEvent event)
{
	//CI_LOG_I("touchesEnded");
	this->isClicked = false;
	this->isDragged = false;
	this->twoTouches = false;
	//this->lastTouch = event.getTouches;
}
*/
#include "card.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <string>
#include <iostream>

using namespace ci;
using namespace ci::app;
using namespace std;


Card::Card()
{
	x = 200;
	y = 100;
	title = "Hej Hilma";
	/*
string rubrik
string br�dtext
substring br�dtext; (ta 10 f�rsta orden i br�dtext;)
img img1
img img2
taget fr�n xml
*/

}

Card::~Card()
{
}

Card::Card(const float x1, const float y1, std::string title, std::string body) 
{
	x = x1;
	y = y1;
    angle = 0;
    initAngle = 0;
	initDist = 0;
   
	cardSize = 1.0f;
	width = 336.0f*cardSize;
	height = 500.0f*cardSize;

	this->title = title;
	this->body = body;

	isClicked = false;
	isDragged = false;
    isScaled = false;

	rect = Rectf(x1, y1, x1 + width, y1 + height);

	twoTouches = false;

	transform = Transform();
	initElements();
	setStyles();
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
	textColor = ColorA::hex(0x1d1d1d);
	montserrat = loadAsset("fonts/Montserrat.ttf");
	raleway = loadAsset("fonts/Raleway.ttf");

	const Font titleFont = Font(montserrat , 20.0*cardSize);
	const Font bodyFont = Font(raleway, 14.0*cardSize);

	updateElementCoords();

	TextBox titleBox = TextBox().text(title).font(titleFont).color(textColor).size(elementWidth, 30*cardSize);
	TextBox bodyBox = TextBox().text(body).font(bodyFont).color(textColor).size(elementWidth, 129*cardSize);

	//CI_LOG_I("CARD COORD:   x: " << x << "  y: " << y);
	//CI_LOG_I("TEXTBOX:   " << titleCo);


	titleTex = renderTexture(titleBox);
	bodyTex = renderTexture(bodyBox);
}

void Card::updateElementCoords()
{
	paddingX = 22.0f*cardSize;
	elementWidth = 292.0f *cardSize;

	const Font titleFont = Font(montserrat, 20.0*cardSize);
	const Font bodyFont = Font(raleway, 14.0*cardSize);

	TextBox titleBox = TextBox().text(title).font(titleFont).color(textColor).size(elementWidth, 30 * cardSize);
	TextBox bodyBox = TextBox().text(body).font(bodyFont).color(textColor).size(elementWidth, 129 * cardSize);

	float imgY = 22.0f*cardSize;
	float titleY = 268.0f*cardSize;
	float bodyY = 320.0f*cardSize;
	float tagsY = 459.0f*cardSize;

	imgCo = vec2(x + paddingX, y + imgY);
	titleCo = vec2(x + paddingX, y + titleY);
	bodyCo = vec2(x + paddingX, y + bodyY);
	tagsCo = vec2(x + paddingX, y + tagsY);

}
/*
void Card::mouseDrag(MouseEvent event)
{
	//set a bool to true when rect.contains is true once. Dont set to false until mouseUp to avoid mouse getting outside the rect
	if (isClicked) 
	{
		this->title = "du har dragit p� rektangeln";
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
		this->title = "du har klickat p� rektangeln";
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
void Card::touchesBegan(TouchEvent event) 
{
	for (const auto &touch : event.getTouches()) //event.getTouches()) returns std::vector<Touch>
	{
		if (rect.contains(touch.getPos())) 
		{
            activeTouchesOnCard.push_back(touch);
			if (this->isClicked == true) // rect contains two touch points // && rect.contains(lastTouch.getPos()
			{
				this->twoTouches = true;
                this->initVec = activeTouchesOnCard[1].getPos() - activeTouchesOnCard[0].getPos();
				//this->initDist = glm::distance(activeTouchesOnCard[1].getPos(), activeTouchesOnCard[0].getPos());
				this->initDist = sqrt(pow(lastTouch.getX() - touch.getX(), 2) + pow(lastTouch.getY() - touch.getY(), 2));
			//	CI_LOG_I("vi har tv� fingrar p� rektangeln");
			}
			else 
			{
				this->twoTouches = false;
				lastTouch = touch;
			}
			
			this->isClicked = true;
			this->isFront = true;
			this->title = "du har klickat p� rektangeln";
			
		//	CI_LOG_I("title: " << title);
		}
	}
}

void Card::touchesMoved(TouchEvent event) 
{
	for (const auto &touch : event.getTouches()) 
	{
		
		translate(touch);
		scaling(touch);
		rotation(touch);
		/*
		if (this->twoTouches) // rect contains two active touch points - activeTouchesOnCard.size() > 1
		{
            /********************************
            *           SCALING             *
            ********************************
            vec2 currVec = lastTouch.getPos() - touch.getPos();
            this->cardSize = glm::length(currVec) / glm::length(initVec);
            
            if (cardSize > 0.2 && cardSize < 2) // don't scale for very small/big scale factors
            {
                //CI_LOG_I("size: " << this->cardSize);
                if (this->rect.getHeight()*this->cardSize > 300 && this->rect.getHeight()*this->cardSize < 600) // don't exceed max size
                {
                    this->rect.scaleCentered(this->cardSize);
                    /*this->scaleMat = transform.scaling(this->cardSize);
					CI_LOG_I("scale Matrix: " << scaleMat);
					this->rect.transform(scaleMat);

                    this->isScaled = true;

                    /************************
                    *       ROTATION        *
                    ************************
                    
                }
            }
            //this->angle = transform.rotateCard(initVec, currVec);

            CI_LOG_I("Angle: " << glm::degrees(this->angle));

            this->rotMat = transform.rotate(initVec, currVec);
            //this->rect.transform(mat);

            this->initVec = currVec;
		}*/
	}
}

void Card::touchesEnded(TouchEvent event) 
{
	//CI_LOG_I("touchesEnded");
    for (auto &touch : event.getTouches())
    {
        for (int i = 0; i < activeTouchesOnCard.size(); ++i)
        {
            if (touch.getId() == activeTouchesOnCard[i].getId())
            {
                this->lastRotTouch = activeTouchesOnCard[i];
                activeTouchesOnCard.erase(activeTouchesOnCard.begin() + i);     
                // if first touch is removed => move second touch to first slot
            }
        }
    }
    
    if (activeTouchesOnCard.empty())
    {
        this->isClicked = false;
        this->isDragged = false;
        this->isScaled = false;
    }
    else if (activeTouchesOnCard.size() < 2) this->twoTouches = false;
	
}

void Card::translate(TouchEvent::Touch touch)
{
	if (isClicked && !activeTouchesOnCard.empty() && touch.getId() == activeTouchesOnCard[0].getId())
	{
		float mx = touch.getX();
		float my = touch.getY();

		float oldX = this->rect.getX1();
		float oldY = this->rect.getY1();

		float *coords = transform.translate(oldX, oldY, mx, my, isDragged);
		//this->setpos(coords[0], coords[1]);
		this->transMat = transform.translateCard(oldX, oldY, coords[0], coords[1]);

		//this->rect.set(coords[0], coords[1], coords[0] + rect.getWidth(), coords[1] + rect.getHeight());
		//updateElementCoords();
		this->rect.transform(transMat);

		delete coords;
		//set a bool to true when rect.contains is true once. Dont set to false until mouseUp to avoid mouse getting outside the rect
		isDragged = true;
	}
}

void Card::scaling(TouchEvent::Touch touch)
{
	if (activeTouchesOnCard.size() > 1)//(this->twoTouches) // rect contains two active touch points - activeTouchesOnCard.size() > 1
	{
		vec2 currVec = activeTouchesOnCard[activeTouchesOnCard.size()-2].getPos() - activeTouchesOnCard.back().getPos();
		//vec2 currVec = lastTouch.getPos() - touch.getPos();
		//float currDist = glm::distance(lastTouch.getPos(), touch.getPos());
		float currDist = sqrt(pow(lastTouch.getX() - touch.getX(), 2) + pow(lastTouch.getY() - touch.getY(), 2));
		//this->cardSize = glm::length(currVec) / glm::length(initVec);
		//this->cardSize = transform.scale(currVec,initVec); // nya/gamla
		
		if(initDist != 0) this->cardSize = currDist / this->initDist;
		
		CI_LOG_I("cardSize: " << cardSize);

		if (cardSize > 0.2 && cardSize < 2) // don't scale for very small/big scale factors
		{
			// don't exceed max size
			if (this->rect.getHeight()*this->cardSize > 300 && this->rect.getHeight()*this->cardSize < 600)
			{
				this->rect.scaleCentered(this->cardSize);

				this->isScaled = true;
			}
		}
		this->initVec = currVec;
		this->initDist = currDist;
	}
}

void Card::rotation(TouchEvent::Touch touch)
{
	if (this->twoTouches) // rect contains two active touch points - activeTouchesOnCard.size() > 1
	{
		vec2 currVec = lastTouch.getPos() - touch.getPos();
		//this->angle = transform.rotateCard(initVec, currVec);

		//CI_LOG_I("Angle: " << glm::degrees(this->angle));

		this->rotMat = transform.rotate(initVec, currVec);
		//this->rect.transform(mat);

		this->initVec = currVec;
	}
}

void Card::update() 
{
	updateElementCoords();

}

void Card::setStyles()
{
	bgColor = Color::hex(0xfcfcfc); // off-white
	borderColor = Color::hex(0xbcbcbc);
	borderRadius = 5.0f;
}

void Card::renderCard() {
    int cornerSegments = 5;
	gl::color(bgColor);
  
    gl::pushModelMatrix();
	

        
    /*if (!activeTouchesOnCard.empty())//(this->angle != 0 && isScaled)
    { 
        gl::translate(activeTouchesOnCard[0].getX(), activeTouchesOnCard[0].getY());
        //gl::translate(this->rect.getUpperLeft().x, this->rect.getUpperLeft().y);   // translate origin
        gl::rotate(this->angle);
        gl::translate(-activeTouchesOnCard[0].getX(), -activeTouchesOnCard[0].getY());
        lastTouch = activeTouchesOnCard[0];
        //gl::translate(-this->rect.getUpperLeft().x, -this->rect.getUpperLeft().y);  // translate back
    }*/
   /*else if(lastRotTouch.getX() > 0)
    { 
        gl::translate(this->lastRotTouch.getX(), this->lastRotTouch.getY());
       // gl::translate(this->rect.getUpperLeft().x, this->rect.getUpperLeft().y);   // translate origin
        gl::rotate(this->angle);
        //gl::translate(-this->rect.getUpperLeft().x, -this->rect.getUpperLeft().y);
       gl::translate(-this->lastRotTouch.getX(), -this->lastRotTouch.getY());
     }
        */

    gl::drawSolidRoundedRect(rect, borderRadius, cornerSegments);
	gl::color(borderColor);

    gl::drawStrokedRoundedRect(rect, borderRadius, cornerSegments);

    gl::color(Color::white());
    gl::draw(titleTex, titleCo);
    gl::draw(bodyTex, bodyCo);

    gl::popModelMatrix();  
    
}
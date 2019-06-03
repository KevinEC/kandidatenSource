#include "Story.h"
#include <chrono>
#include <thread>
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

using namespace bluecadet::core;
using namespace bluecadet::views;
using namespace bluecadet::touch;

/* TO DO
    - Content
    - delay moveToFront/Back
*/

vec2 frontPos;  // top-card position
vec2 backPos;   // bottom-card position

int front, back; // index to front/back card 

// default constructor
Story::Story()
{
    vec2 windowSize{ 1920, 1080 };
    storyView = make_shared<TouchView>();
    storyView->moveToFront();
    storyView->setDragEnabled(true);
    storyView->setBackgroundColor(Color::white());
    storyView->setDebugDrawTouchPath(true);
    storyView->setMultiTouchEnabled(true);
    storyView->setSize(vec2{ 0.5f*windowSize.x, windowSize.y });
    storyView->setTransformOrigin(0.5f * storyView->getSize());
    storyView->setHidden(false);

 //   storyView->getSignalTouchBegan().connect([=](const bluecadet::touch::TouchEvent& e) { handleTouchBegan(e); });
 //   storyView->getSignalTouchEnded().connect([=](const bluecadet::touch::TouchEvent& e) { handleTouchBegan(e); });
  //  storyView->getSignalTouchEnded().connect([=](const bluecadet::touch::TouchEvent& e) { handleTouchEnded(e); });

    auto labelView = make_shared<TextView>();
    labelView->setPadding(10, 10);
    labelView->setWidth(storyView->getWidth());
    labelView->setFontSize(50.0f);
    labelView->setTextColor(Color::black());
    labelView->setTextAlign(bluecadet::text::TextAlign::Center);
    labelView->setText("STORYMODE YAY");
    
    storyView->addChild(labelView);

}

// constructor
Story::Story(string name, Cards* cards) : storyCards(cards)
{
    front = 0;
    back = storyCards->allcards.size() - 1;

	this->name = name;

    // story mode main view with touch
    vec2 windowSize{ 1920, 1080 };
    storyView = make_shared<TouchView>();
    storyView->moveToFront();
    storyView->setDragEnabled(false);
    //storyView->setBackgroundColor(Color::white());
    storyView->setMultiTouchEnabled(true);
    storyView->setSize(vec2{ 0.5f*windowSize.x, windowSize.y });
    storyView->setTransformOrigin(0.5f * storyView->getSize());
    storyView->setHidden(false);

    // connect touch functions
    storyView->getSignalTouchBegan().connect([=](const bluecadet::touch::TouchEvent& e) { });
    storyView->getSignalTouchMoved().connect([=](const bluecadet::touch::TouchEvent& e) { handleTouchMoved(&e); });
    storyView->getSignalTouchEnded().connect([=](const bluecadet::touch::TouchEvent& e) { });

	int offset = 0;

    // set up views for card content
	for (int i = 1; i < cards->allcards.size() / 2; i+=2)
	{
		setUpCard(storyView, &offset, cards->allcards[i], "noTitle", cards->allcards[i+1]);
		offset += 3;
	}
	//set up first view with title
	setUpCard(storyView, &offset, cards->allcards[0], name);


    // make top card red 
    //storyView->getChildren().back()->setBackgroundColor(Color::hex(0xFF0000));
    storyCards->allcards.back()->object->setBackgroundColor(Color::hex(0xD1D2D3));      // funkar inte

    // fixed position for top- and bottom-card
    frontPos = storyView->getChildren().back()->getPositionConst();
    backPos = storyView->getChildren().front()->getPositionConst();
}

Story::~Story(){}

void Story::setUpCard(TouchViewRef view, int *offset, Card *card, string name, Card *card2)
{
    // container view for card
    auto cardView = make_shared<StrokedRoundedRectView>();
    cardView->setSize(vec2{ 0.75f*storyView->getHeight(), 0.5f*storyView->getWidth() });
    cardView->setTransformOrigin(0.5f * cardView->getSize());
    cardView->setBackgroundColor(Color::hex(0xfcfcfc));
    cardView->setStrokeColor(Color::hex(0xbcbcbc));
    cardView->setCornerRadius(8.0f);
    cardView->moveToFront(); // newest loaded card on top
    cardView->setCenter(vec2{ storyView->getCenter().x, (storyView->getCenter().y - *offset) }); // offset card position



	// set up card views for double info
	// if card2 is defined
	if (card2->titleText != "empty")
	{
		setUpHeader(cardView, name);
		setUpBody(cardView, card, card2);
		setUpImage(cardView, card);
	}
	// set up first card content views
	else {
		setUpHeader(cardView, name);
		setUpBody(cardView, card);
		setUpImage(cardView, card);
	}
	storyViews.push_back(cardView);
    view->addChild(cardView);
}

void Story::setUpHeader(StrokedRoundedRectViewRef view, string name)
{
	//default argument is noTitle
	if (name != "noTitle")
	{
		auto headerView = make_shared<TextView>();
		headerView->setWidth(0.65f*view->getWidth());
		headerView->setPosition({ view->getWidth()*0.3f, view->getHeight()*0.1f });
		headerView->setTransformOrigin(0.5f * headerView->getSize());
		//headerView->setCenter(vec2{ view->getCenter().x - 50, view->getCenter().y - 0.7f*view->getHeight() });
		headerView->setFontSize(50.0f);
		headerView->setTextColor(Color::black());
		headerView->setFontFamily("Montserrat");
		headerView->setText(name);

		view->addChild(headerView);
	}

}

void Story::setUpBody(StrokedRoundedRectViewRef view, Card *card, Card *card2)
{
	auto bodyView = make_shared<TextView>();

	bodyView->setWidth(0.45f*view->getWidth());
	bodyView->setPosition(vec2{ view->getWidth()*0.05, view->getHeight()*0.5f });
	//bodyView->setCenter(vec2{ view->getCenter().x - 50 , view->getCenter().y - 0.5f*view->getHeight() });
	bodyView->setTransformOrigin(0.5f * bodyView->getSize());
	bodyView->setFontSize(22.0f);
	bodyView->setFontFamily("Raleway");
	bodyView->setTextColor(Color::black());
	bodyView->setText(card->bodyText);

	view->addChild(bodyView);

	// if card2 is defined -> setup second layout
	if (card2->titleText != "empty")
	{
		auto bodyView2 = make_shared<TextView>();

		bodyView2->setWidth(0.6f*view->getWidth());
		bodyView2->setHeight(view->getHeight()*0.28f);
		bodyView2->setPosition(vec2{ view->getWidth()*0.33, view->getHeight()*0.15f });
		//bodyView2->setCenter(vec2{ view->getCenter().x - 50 , view->getCenter().y - 0.5f*view->getHeight() });
		bodyView2->setTransformOrigin(0.5f * bodyView2->getSize());
		bodyView2->setFontSize(22.0f);
		bodyView2->setFontFamily("Raleway");
		bodyView2->setTextColor(Color::black());
		bodyView2->setText(card2->bodyText);

		view->addChild(bodyView2);
	} 
}

void Story::setUpImage(StrokedRoundedRectViewRef view, Card *card, Card *card2)
{
    auto imageView = make_shared<ImageView>();
	DataSourceRef imgUrl = DataSourceUrl::create(Url::Url(card->imgPath));
	auto srf = loadImage(imgUrl, ImageSource::Options());
	imageView->setTexture(gl::Texture::create(srf));
	imageView->setPosition({ view->getWidth()*0.6f, view->getHeight()*0.46f });
    imageView->setTransformOrigin(0.5f * imageView->getSize());
	imageView->setWidth(view->getWidth()*0.4f);
	imageView->setHeight(view->getHeight()*0.4f);
	imageView->setScaleMode(bluecadet::views::ImageView::ScaleMode::COVER);

    view->addChild(imageView);

	if (card2->titleText != "empty")
	{
		auto imageView2 = make_shared<ImageView>();
		DataSourceRef imgUrl = DataSourceUrl::create(Url::Url(card2->imgPath));
		auto srf = loadImage(imgUrl, ImageSource::Options());
		imageView2->setTexture(gl::Texture::create(srf));
		imageView2->setPosition({ view->getWidth()*0.1f, view->getHeight()*0.1f });
		imageView2->setTransformOrigin(0.5f * imageView2->getSize());
		imageView2->setWidth(view->getWidth()*0.4f);
		imageView2->setHeight(view->getHeight()*0.4f);
		imageView2->setScaleMode(bluecadet::views::ImageView::ScaleMode::COVER);
		
		view->addChild(imageView2);
	}
}


//bool handleMoveToFront(&BaseView kid)
//{
//    kid->moveToFront();
//    return true;
//	if ((curr - prev) > 0)
//	{
//		// swipe up
//	}
//	else
//	{
//		// swipe down 
//	}
//
	//if (e->touchTarget->hasReachedDragThreshold())
	//{
	//    e->touchTarget->cancelTouches();
	//    
	//    // move full stack of cards up :)
	//    auto kids = e->target->getChildren();
	//    for (auto &kid : kids)
	//        kid->setPosition(vec2{ kid->getPositionConst().x, kid->getPositionConst().y - 3 });

	//    /* TOP CARD TRANSFORMATIONS */

	//    // move up card
	//    e->target->getChildren().back()->getTimeline()->appendTo(&e->target->getChildren().back()->getPosition(), vec2{50,-300}, 0.2f, easeInExpo);
	//    
	//    // scale card        
	//    e->target->getChildren().back()->getTimeline()->appendTo(&e->target->getChildren().back()->getScale(), vec2(0.8f), 0.2f, easeInQuad)
	//        .startTime(e->target->getChildren().back()->getTimeline()->getCurrentTime() + 0.4f);
	//    
	//    // move card to bottom of stack
	//    e->target->getChildren().back()->moveToBack();
	//    
	//    // move card down
	//    e->target->getChildren().front()->getTimeline()->appendTo(&e->target->getChildren().front()->getPosition(), vec2{ cardPos.x, cardPos.y }, 1.0f)
	//        .startTime(e->target->getChildren().front()->getTimeline()->getCurrentTime() + 1.0f);

	//    // scale back
	//    e->target->getChildren().front()->getTimeline()->appendTo(&e->target->getChildren().front()->getScale(), vec2(1.0f), 0.2f)
	//        .startTime(e->target->getChildren().front()->getTimeline()->getCurrentTime() + 2.5f);
	//}
//}



void Story::handleTouchMoved(const bluecadet::touch::TouchEvent* e)
{
    storyView->setDragThreshold(150);   // length of touch drag    

    auto prev = storyView->getPrevGlobalTouchPos().y;
    auto curr = storyView->getGlobalTouchPos().y;

    storyCards->tangLayout(storyView->getGlobalPosition(), storyView->getBounds());
    
    // card swipe animation
    if (storyView->hasReachedDragThreshold())
    {
        storyView->cancelTouches();

        if ((curr - prev) > 0)          /* SWIPE DOWN = GO TO PREVIOUS STORY CARD = BRING BOTTOM CARD TO FRONT */
            swipeDown();
     
        else                            /* SWIPE UP = GO TO NEXT STORY CARD = MOVE TOP CARD TO BACK */
            swipeUp();
    }
}

void Story::handleTouchEnded(const bluecadet::touch::TouchEvent* e)
{}

void Story::swipeUp()        // translate, scale and alpha out allcards[front] 
{
    //      view    ==  storyCards->allcards.front()->object->

    CI_LOG_I("first: " << storyCards->allcards[front]->object->isHidden());

    // move full stack of cards up to restore offset :)
    auto kids = storyView->getChildren();
    for (auto &kid : kids)
        kid->setPosition(vec2{ kid->getPositionConst().x, kid->getPositionConst().y - 3 });


    // move up card     - fast
    storyView->getChildren().back()->getTimeline()->appendTo(&storyView->getChildren().back()->getPosition(), vec2{ 50,-300 }, 0.2f, easeInExpo);

    // scale card       - medium
    storyView->getChildren().back()->getTimeline()->appendTo(&storyView->getChildren().back()->getScale(), vec2(0.8f), 0.2f, easeInQuad)
        .startTime(storyView->getChildren().back()->getTimeline()->getCurrentTime() + 0.4f);


   // storyCards->allcards[front]->object->getTimeline()->appendTo(&storyCards->allcards[front]->object->getAlpha(), 0.0f, 0.8f);           // funkar inte
   // storyCards->allcards[front]->object->getTimeline()->appendTo(&storyCards->allcards[front]->object->getScale(), vec2(2.0f), 0.8f);     // funkar inte


    // move card to bottom of stack 
    storyView->getChildren().back()->moveToBack();

    // move card down   - slow
    storyView->getChildren().front()->getTimeline()->appendTo(&storyView->getChildren().front()->getPosition(), vec2{ backPos.x, backPos.y }, 1.0f)
        .startTime(storyView->getChildren().front()->getTimeline()->getCurrentTime() + 1.0f);

    // scale back       - not to be seen
    storyView->getChildren().front()->getTimeline()->appendTo(&storyView->getChildren().front()->getScale(), vec2(1.0f), 0.2f)
        .startTime(storyView->getChildren().front()->getTimeline()->getCurrentTime() + 2.5f);
}

void Story::swipeDown()         // scale, alpha in and translate allcards[back] 
{
    // scale card        - not to be seen
    storyView->getChildren().front()->getTimeline()->appendTo(&storyView->getChildren().front()->getScale(), vec2(0.8f), 0.2f, easeInQuad);

    // move card down   - fast
    storyView->getChildren().front()->getTimeline()->appendTo(&storyView->getChildren().front()->getPosition(), vec2{ 50,800 }, 0.4f, easeInExpo)
        .startTime(storyView->getChildren().front()->getTimeline()->getCurrentTime() + 0.4f);


    // scale back       - medium
    storyView->getChildren().front()->getTimeline()->appendTo(&storyView->getChildren().front()->getScale(), vec2(1.0f), 0.2f)
        .startTime(storyView->getChildren().front()->getTimeline()->getCurrentTime() + 1.0f);

    storyView->getChildren().front()->getTimeline()->appendTo(&storyView->getChildren().front()->getAlpha(), 0.0f, 0.8f)
        .startTime(storyView->getChildren().front()->getTimeline()->getCurrentTime() + 1.5f);


    // storyView->getChildren().front()->moveToFront(); // move card to front of stack


    // move full stack of cards down to restore offset :)
    auto kids = storyView->getChildren();
    for (auto &kid : kids)
        kid->setPosition(vec2{ kid->getPositionConst().x, kid->getPositionConst().y + 3 });

    // move card up     - slow
    storyView->getChildren().back()->getTimeline()->appendTo(&storyView->getChildren().back()->getPosition(), vec2{ frontPos.x, frontPos.y }, 1.0f)
        .startTime(storyView->getChildren().back()->getTimeline()->getCurrentTime() + 2.5f);
}

vector<pair<string, Cards*>> Story::sort(vector<string>* titles, vector<pair<string, string>>* bodies, vector<string>* imgPaths)
{
	vector<pair<string, Cards*>> allStories;

	pair<string, string> noTitle = make_pair("ingen titel", "no title");

	// set story names
	for (int i = 0; i < titles->size(); i++)
	{
		allStories.push_back( make_pair(titles->at(i), new Cards(titles->at(i))) );
	}

	// separate the different body texts by their Story
	for (int i = 0; i < bodies->size(); i++)
	{
		// the database sucks so we hard coded the intervals for the story bodies
		// in order for better organization and actual code modularity keys needs to be implemented in the database

		if (i < 3)
		{
			allStories.at(0).second->addCard(new Card(storyView->getGlobalPosition().x, storyView->getGlobalPosition().y, noTitle, bodies->at(i), imgPaths->at(i)));
		}
		else if (i < 20)
		{
			allStories.at(1).second->addCard(new Card(storyView->getGlobalPosition().x, storyView->getGlobalPosition().y, noTitle, bodies->at(i), imgPaths->at(i)));

		}
		else {
			allStories.at(2).second->addCard(new Card(storyView->getGlobalPosition().x, storyView->getGlobalPosition().y, noTitle, bodies->at(i), imgPaths->at(i)));

		}
	}

	return allStories;
}
#include "Story.h"

/* TO DO
    - Content
    - Move after a touch point in puck
*/


vec2 cardPos;

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
Story::Story(Cards* cards) : storyCards(cards)
{
    
    // story mode view 
    vec2 windowSize{ 1920, 1080 };
    storyView = make_shared<TouchView>();
    storyView->moveToFront();
    storyView->setDragEnabled(false);
    storyView->setBackgroundColor(Color::white());
    storyView->setMultiTouchEnabled(true);
    storyView->setSize(vec2{ 0.5f*windowSize.x, windowSize.y });
    storyView->setTransformOrigin(0.5f * storyView->getSize());
    storyView->setHidden(false);

    // touches began - might not be needed
    storyView->getSignalTouchBegan().connect([=](const bluecadet::touch::TouchEvent e) {  });

    // touches moved
    storyView->getSignalTouchMoved().connect([=](const bluecadet::touch::TouchEvent e) { handleTouchMoved(&e); });

    // touches ended - might not be needed
    storyView->getSignalTouchEnded().connect([=](const bluecadet::touch::TouchEvent e) {  });

    int offset = 0;

    // set up views for card content
    for(auto card : cards->allcards)
    {
        // container view for card
        auto cardView = make_shared<StrokedRoundedRectView>();
        cardView->setSize(vec2{ 0.8f*storyView->getHeight(), 0.6f*storyView->getWidth() });
        cardView->setTransformOrigin(0.5f * cardView->getSize());
        cardView->setBackgroundColor(Color::hex(0x1d1d1d));
        cardView->setStrokeColor(Color::white());
        cardView->setCornerRadius(5.0f);
        cardView->moveToFront();
        cardView->setCenter(vec2{storyView->getCenter().x, (storyView->getCenter().y - offset)}); // offset card position
        offset += 3;

            // header view
            auto headerView = make_shared<TextView>();
            headerView->setPadding(20,20);
            headerView->setWidth(0.7f*cardView->getWidth());
            headerView->setTransformOrigin(0.5f * headerView->getSize());
            headerView->setCenter(vec2{ cardView->getCenter().x, cardView->getCenter().y - 0.7f*cardView->getHeight() });
            headerView->setFontSize(50.0f); 
            headerView->setBackgroundColor(Color::white());
            headerView->setTextColor(Color::black());
            headerView->setTextAlign(bluecadet::text::TextAlign::Center);
            headerView->setText("STORYMODE YAY");
       
            // body view
            auto bodyView = make_shared<TextView>();
            bodyView->setPadding(20, 20);
            bodyView->setWidth(0.5f*cardView->getWidth());
            //bodyView->setPosition(vec2{});
            bodyView->setCenter(vec2{ cardView->getCenter().x - 0.2f*cardView->getWidth(), headerView->getCenter().y + headerView->getHeight() });
            headerView->setTransformOrigin(0.5f * headerView->getSize());
            bodyView->setFontSize(20.0f);
            bodyView->setBackgroundColor(Color::white());
            bodyView->setTextColor(Color::black());
            bodyView->setTextAlign(bluecadet::text::TextAlign::Center);
            bodyView->setText("a little longer text about rainbows and unicorns, clouds and other shit. some text some text some text some text some text some text some text some text yaaay");

            // image view -- god send help
            auto imageView = make_shared<ImageView>();
            imageView->setWidth(0.2f*cardView->getWidth());
            //imageView->setTransformOrigin(0.5f * imageView->getSize());
            imageView->setCenter(vec2{ cardView->getCenter().x + 0.5*cardView->getWidth(), headerView->getCenter().y + headerView->getHeight() });
            //imageView->setBackgroundColor(Color::white());

        cardView->addChild(headerView);
        cardView->addChild(bodyView);
        cardView->addChild(imageView);
        storyView->addChild(cardView);
    }

    // make top card red 
    storyView->getChildren().back()->setBackgroundColor(Color::hex(0xFF0000));
    cardPos = storyView->getChildren().front()->getPositionConst();

    //CI_LOG_I("kids: " << storyView->getNumChildren());
}

Story::~Story(){}

void Story::handleTouchBegan(const bluecadet::touch::TouchEvent* e)
{
}

void Story::handleTouchMoved(const bluecadet::touch::TouchEvent* e)
{
	// card swipe animation
	e->touchTarget->setDragThreshold(150);   // length of touch drag

	auto prev = e->touchTarget->getPrevGlobalTouchPos().y;
	auto curr = e->touchTarget->getGlobalTouchPos().y;

	if ((curr - prev) > 0)
	{
		// swipe up
	}
	else
	{
		// swipe down 
	}

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
}

void Story::handleTouchEnded(const bluecadet::touch::TouchEvent* e)
{
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
		else if (i < 17)
		{
			allStories.at(1).second->addCard(new Card(storyView->getGlobalPosition().x, storyView->getGlobalPosition().y, noTitle, bodies->at(i), imgPaths->at(i)));

		}
		else {
			allStories.at(2).second->addCard(new Card(storyView->getGlobalPosition().x, storyView->getGlobalPosition().y, noTitle, bodies->at(i), imgPaths->at(i)));

		}
	}

	return allStories;
}
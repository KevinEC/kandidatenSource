#include "Story.h"

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

    // call touchesBegan - perhaps not needed
    storyView->getSignalTouchBegan().connect([=](const bluecadet::touch::TouchEvent& e) { handleTouchBegan(e); });
    // call touchesEnded - perhaps not needed
    storyView->getSignalTouchEnded().connect([=](const bluecadet::touch::TouchEvent& e) { handleTouchEnded(e); });

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
Story::Story(Cards cards) : storyCards(cards)
{
    // story mode view - touchable & dragable
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

    int offset = 0;

    for(auto &card : cards.allcards)
    {
        // container view for card
        auto cardView = make_shared<StrokedRoundedRectView>();
        cardView->setSize(vec2{ 0.8f*storyView->getHeight(), 0.6f*storyView->getWidth() });
        cardView->setBackgroundColor(Color::hex(0x1d1d1d));
        cardView->setStrokeColor(Color::white());
        cardView->setCornerRadius(5.0f);
        cardView->moveToFront();
        // offset card position
        cardView->setCenter(vec2{storyView->getCenter().x, (storyView->getCenter().y - offset)});
        offset += 5;

            // header view
            auto headerView = make_shared<TextView>();
            headerView->setPadding(20,20);
            headerView->setWidth(0.7f*cardView->getWidth());
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
            bodyView->setFontSize(20.0f);
            bodyView->setBackgroundColor(Color::white());
            bodyView->setTextColor(Color::black());
            bodyView->setTextAlign(bluecadet::text::TextAlign::Center);
            bodyView->setText("a little longer text about rainbows and unicorns, clouds and other shit. some text some text some text some text some text some text some text some text yaaay");

            // image view -- god send help
            auto imageView = make_shared<ImageView>();
            imageView->setWidth(0.2f*cardView->getWidth());
            imageView->setCenter(vec2{ cardView->getCenter().x + 0.5*cardView->getWidth(), headerView->getCenter().y + headerView->getHeight() });
            //imageView->setBackgroundColor(Color::white());


        cardView->addChild(headerView);
        cardView->addChild(bodyView);
        cardView->addChild(imageView);
        storyView->addChild(cardView);
    }

    CI_LOG_I("kids: " << storyView->getNumChildren());
}

Story::~Story()
{
}

void Story::handleTouchBegan(const bluecadet::touch::TouchEvent & touchEvent)
{
}

void Story::handleTouchMoved(const bluecadet::touch::TouchEvent & touchEvent)
{
}

void Story::handleTouchEnded(const bluecadet::touch::TouchEvent & touchEvent)
{
}




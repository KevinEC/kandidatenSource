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




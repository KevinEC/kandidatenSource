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

    // call touchesBegan
    storyView->getSignalTouchBegan().connect([=](const bluecadet::touch::TouchEvent& e) { handleTouchBegan(e); });

    // call touchesEnded
    storyView->getSignalTouchEnded().connect([=](const bluecadet::touch::TouchEvent& e) { handleTouchEnded(e); });
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




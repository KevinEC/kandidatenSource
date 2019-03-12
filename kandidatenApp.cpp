#include "Cards.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/System.h"
#include "cinder/Rand.h"
#include "cinder/Log.h"

#include <vector>
#include <map>
#include <list>
#include <iostream>

//Controller-app

using namespace ci;
using namespace ci::app;
using namespace std;

struct TouchPoint {
	TouchPoint() {} //default constructor
	TouchPoint(const vec2 &initialPt, const Color &color) : mColor(color), mTimeOfDeath(-1.0)
	{
		mLine.push_back(initialPt);
	}

	void addPoint(const vec2 &pt) { mLine.push_back(pt); }

	void draw() const
	{
		if (mTimeOfDeath > 0) // are we dying? then fade out
			gl::color(ColorA(mColor, (double(mTimeOfDeath) - getElapsedSeconds()) / 2.0));
		else
			gl::color(mColor);

		gl::draw(mLine);
	}

	void startDying() { mTimeOfDeath = getElapsedSeconds() + 2.0f; } // two seconds 'til dead

	bool isDead() const { return getElapsedSeconds() > mTimeOfDeath; }

	PolyLine2f		mLine;
	Color			mColor;
	float			mTimeOfDeath;
};

class kandidatenApp : public App {
public:
	void setup() override;
	void update() override;
	void draw() override;

	void mouseDrag(MouseEvent event) override;
	void mouseDown(MouseEvent event) override;
	void	touchesBegan(TouchEvent event) override;
	void	touchesMoved(TouchEvent event) override;
	void	touchesEnded(TouchEvent event) override;
	vec2 mMouseLoc;
	vec2 lastclick;
	vector<Rectf> test;
	bool test2 = false;
	Card card1;
	Cards kort;
	int i;



private:
	map<uint32_t, TouchPoint>	mActivePoints;
	list<TouchPoint>			mDyingPoints;
	//p = C:\Users\chris\Downloads\kort;
   //ImageSourceRef img = loadImage(fs::path("C:\Users\chris\Downloads\kort"), "png", "");
};

void prepareSettings(kandidatenApp::Settings *settings)
{
	settings->setMultiTouchEnabled(true);

}

void kandidatenApp::setup()
{
	CI_LOG_I("MT: " << System::hasMultiTouch() << " Max points: " << System::getMaxMultiTouchPoints());
	//Cards allakort = databasecaller();
	i = 0;
	kort = Cards(); 
	
}

void kandidatenApp::touchesBegan(TouchEvent event)
{
	CI_LOG_I(event);

	for (const auto &touch : event.getTouches()) {
		Color newColor(CM_HSV, Rand::randFloat(), 1, 1);
		mActivePoints.insert(make_pair(touch.getId(), TouchPoint(touch.getPos(), newColor)));
	}

}

void kandidatenApp::touchesMoved(TouchEvent event)
{
	CI_LOG_I(event);
	for (const auto &touch : event.getTouches()) {
		mActivePoints[touch.getId()].addPoint(touch.getPos());
	}

}

void kandidatenApp::touchesEnded(TouchEvent event)
{
	CI_LOG_I(event);
	for (const auto &touch : event.getTouches()) {
		mActivePoints[touch.getId()].startDying();
		mDyingPoints.push_back(mActivePoints[touch.getId()]);
		mActivePoints.erase(touch.getId());
	}
}

void kandidatenApp::mouseDown(MouseEvent event)
{
	//mMouseLoc = event.getPos();
	lastclick = event.getPos();

	CI_LOG_I(kort.testkort.title);
	CI_LOG_I(kort.testkort.x);
	CI_LOG_I(kort.testkort.y);

}

void kandidatenApp::update()
{

}

void kandidatenApp::mouseDrag(MouseEvent event) {
	mMouseLoc = event.getPos();
	test2 = true;

	mActivePoints[i++].addPoint(event.getPos());
}


void kandidatenApp::draw()
{
	gl::clear(Color(0, 50, 0));
	//gl::drawSolidCircle(getWindowCenter(), 200);
	Rectf test[100];
	gl::enableAlphaBlending();
	gl::clear(Color(0.1f, 0.1f, 0.1f));

	for (const auto &activePoint : mActivePoints) {
		activePoint.second.draw();
	}

	for (auto dyingIt = mDyingPoints.begin(); dyingIt != mDyingPoints.end(); ) {
		dyingIt->draw();
		if (dyingIt->isDead())
			dyingIt = mDyingPoints.erase(dyingIt);
		else
			++dyingIt;
	}

	// draw yellow circles at the active touch points
	gl::color(Color(1, 1, 0));
	for (const auto &touch : getActiveTouches())
		gl::drawStrokedCircle(touch.getPos(), 20);

	if (test2) {
		Rectf rect(mMouseLoc.x, mMouseLoc.y, mMouseLoc.x + 50, mMouseLoc.y - 50);

		//Rectf rect2(mMouseLoc.x -100, mMouseLoc.y - 100, mMouseLoc.x - 50, mMouseLoc.y - 50);
		test[0] = rect;
		//test[1] = rect2;

		//gl::drawSolidRect(test[i]);
		//gl::drawSolidRect(rect);
		//i++;
		//gl::drawSolidRect(rect2);
	}


}

CINDER_APP(kandidatenApp, RendererGl, prepareSettings)

/*
card databasecaller() {
	return all cards;
}


*/

#include "Cards.h"
#include "dataBaseController.h"
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

using namespace ci;
using namespace ci::app;
using namespace std;

//Controller-app

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
	void mouseUp(MouseEvent event) override;
	void	touchesBegan(TouchEvent event) override;
	void	touchesMoved(TouchEvent event) override;
	void	touchesEnded(TouchEvent event) override;
	vec2 mMouseLoc;
	vec2 lastclick;
	vector<Rectf> test;
	bool test2 = false;
	Card card1;
	Cards kort;
	Cards kort2;
	int i;
	dataBaseController db;



private:
	map<uint32_t, TouchPoint>	mActivePoints;
	list<TouchPoint>			mDyingPoints;

};

void prepareSettings(kandidatenApp::Settings *settings)
{
	settings->setMultiTouchEnabled(true);

}

void kandidatenApp::setup()
{
	CI_LOG_I("MT: " << System::hasMultiTouch() << " Max points: " << System::getMaxMultiTouchPoints());

	i = 0;
	kort = Cards();
	kort2 = Cards();
	kort2.rectKort.renderTexture();

	/*- connect to data base -*/
	db = dataBaseController("online", "xml", "http://www.student.itn.liu.se/~chrad171/databas/databas/media/write.xml");
	CI_LOG_I("db: " << db.tree);

	/*- extract categories -*/
	std::vector<std::string> categories;
	db.extractCategories(categories);

	/*- extract card titles -*/
	std::vector<std::string> titles;
	db.extractTitles(titles);

	/*- extract bodytexts -*/
	std::vector<std::string> bodyText;
	db.extractBodies(bodyText);

	/*- extract image paths -*/
	std::vector<std::string> imgPath;
	db.extractImgPaths(imgPath);
	/*for (std::string n : imgPath)
	{
		CI_LOG_I(n);
	}*/

	/*- extract card categories -*/
	std::vector<std::vector<std::string> > cardCategory;
	db.extractCardCats(cardCategory);
	int counter = 0;
	for(int i = 0; i < cardCategory.size() - 1; ++i){
		CI_LOG_I("Här är kategori för kort på plats: " << i);
		
	for (std::string n : cardCategory[i])
	{
		CI_LOG_I(n);
	}
	}

	CI_LOG_I("cardcat: " << cardCategory.size() << "imgpath: " << imgPath.size() << "bodytext: " << bodyText.size() << "titles: " << titles.size());
	



	disableFrameRate();
	gl::enableVerticalSync(false);

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
	kort.rectKort.mouseDown(event);
	kort2.rectKort.mouseDown(event);

}

void kandidatenApp::mouseDrag(MouseEvent event) {
	mMouseLoc = event.getPos();
	test2 = true;

	mActivePoints[i++].addPoint(event.getPos());
	kort.rectKort.mouseDrag(event);
	kort2.rectKort.mouseDrag(event);
}

void kandidatenApp::mouseUp(MouseEvent event) {

	kort.rectKort.mouseUp(event);
	kort2.rectKort.mouseUp(event);
}

void kandidatenApp::update()
{

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
	for (const auto &touch : getActiveTouches()) {
		gl::drawStrokedCircle(touch.getPos(), 20);
	}






	gl::drawSolidRect(kort.rectKort.rect);
	gl::drawSolidRect(kort2.rectKort.rect);
	gl::draw(kort2.rectKort.text, vec2(50, 50));


		gl::drawSolidRect(kort.rectKort.rect);
		gl::drawSolidRect(kort2.rectKort.rect);
		gl::draw(kort2.rectKort.text, vec2(50, 50));
		


}

CINDER_APP(kandidatenApp, RendererGl, prepareSettings)
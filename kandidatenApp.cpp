#include "Cards.h"
#include "dataBaseController.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/System.h"
#include "cinder/Rand.h"
#include "cinder/Log.h"
#include "cinder/DataSource.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"

#include <vector>
#include <map>
#include <list>
#include <iostream>

// Fix for old version of Cairo lib
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }

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

class kandidatenApp : public App
{
public:
	void setup() override;
	void update() override;
	void draw() override;

	void	mouseDrag(MouseEvent event) override;
	void	mouseDown(MouseEvent event) override;
	void	mouseUp(MouseEvent event) override;
	void	touchesBegan(TouchEvent event) override;
	void	touchesMoved(TouchEvent event) override;
	void	touchesEnded(TouchEvent event) override;

	void	renderCategories();
	void	selectCategories(int enable[]);

	gl::Texture2dRef texture;
	gl::Texture2dRef background;

	Surface mysurf;
	vec2 mMouseLoc;
	vec2 lastclick;
	vector<Rectf> test;
	bool test2 = false;
	Card card1;
	Cards kort;
	Cards kort2;
	int i;
	dataBaseController dbc;
	vector<pair<string, Cards*>> allCategories;



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

	background = gl::Texture::create( loadImage(loadAsset("background.png")));

	i = 0;
	kort2 = Cards();

	/*- connect to data base -*/
	dbc = dataBaseController("online", "xml", "http://www.student.itn.liu.se/~chrad171/databas/databas/media/write.xml");
	// CI_LOG_I("db: " << db.tree);

	/*- extract categories -*/
	vector<string> categories;
	dbc.extractCategories(categories);

	/*- extract card titles -*/
	vector<pair<string, string>> titles;
	dbc.extractTitles(titles);

	/*- extract bodytexts -*/
	vector<pair<string, string>> bodyText;
	dbc.extractBodies(bodyText);

	/*- extract image paths -*/
	vector<string> imgPath;
	dbc.extractImgPaths(imgPath);

	ci::Area area = Area(kort2.rectKort.rect);
	//texture = gl::Texture2d::create(loadImage(loadUrl("http://www.student.itn.liu.se/~chrad171/databas/databas/media/virus.jpg")));
	//texture->setCleanBounds(area);

	mysurf = Surface(400, 600, true);
	Surface mysurf(loadImage(loadUrl("http://www.student.itn.liu.se/~chrad171/databas/databas/media/virus.jpg")), SurfaceConstraintsDefault(), false);
	texture = gl::Texture2d::create(mysurf);
	texture->setCleanBounds(area);

	/*- extract card categories -*/
	vector<vector<string> > cardCategory;
	dbc.extractCardCats(cardCategory);

	CI_LOG_I("sizes: " << categories.size() << " " << titles.size() << " " << bodyText.size() << " " << imgPath.size() << " " << cardCategory.size());
	

	kort = Cards(&titles, &bodyText);

	Cards allCards = Cards();
	allCategories = allCards.categorize(&titles, &bodyText, &categories, &cardCategory);

	int enabledCategories[7] = { 1, 0, 0, 0, 0, 0, 0 };
	selectCategories(enabledCategories);

	disableFrameRate();
	gl::enableVerticalSync(true);
}

void kandidatenApp::touchesBegan(TouchEvent event)
{
	//CI_LOG_I(event);
	kort.rectKort.touchesBegan(event);
	kort2.rectKort.touchesBegan(event);

	for (const auto &touch : event.getTouches())
	{
		Color newColor(CM_HSV, Rand::randFloat(), 1, 1);
		mActivePoints.insert(make_pair(touch.getId(), TouchPoint(touch.getPos(), newColor)));

		//lastclick = touch.getPos();
	}

}

void kandidatenApp::touchesMoved(TouchEvent event)
{
	//CI_LOG_I(event);
	test2 = true;

	kort.rectKort.touchesMoved(event);
	kort2.rectKort.touchesMoved(event);

	for (const auto &touch : event.getTouches())
	{
		mActivePoints[touch.getId()].addPoint(touch.getPos());

		//mMouseLoc = touch.getPos();
	}
}

void kandidatenApp::touchesEnded(TouchEvent event)
{
	//CI_LOG_I(event);
	kort.rectKort.touchesEnded(event);
	kort2.rectKort.touchesEnded(event);

	for (const auto &touch : event.getTouches())
	{
		mActivePoints[touch.getId()].startDying();
		mDyingPoints.push_back(mActivePoints[touch.getId()]);
		mActivePoints.erase(touch.getId());
	}
}

void kandidatenApp::mouseDown(MouseEvent event)
{
	//mMouseLoc = event.getPos();
	lastclick = event.getPos();
	kort.mouseDown(event);

}

void kandidatenApp::mouseDrag(MouseEvent event) {
	mMouseLoc = event.getPos();
	test2 = true;

	mActivePoints[i++].addPoint(event.getPos());
	kort.mouseDrag(event);
}

void kandidatenApp::mouseUp(MouseEvent event) {

	kort.mouseUp(event);
}

void kandidatenApp::update()
{

}

void kandidatenApp::renderCategories()
{
	for (auto &categorie : allCategories)
	{
		categorie.second->renderCards();
	}
}

// list of bools for every categorie. Type 1 to enable a categorie.
// Categories are ordered as the following list:
// Protte, Virus, Celler, Molekyler, gener, livsprocesser, sjukdomar
void kandidatenApp::selectCategories(int enable[])
{
	int index = 0;
	for (auto &categorie : allCategories)
	{
		if (enable[index]) {
			categorie.second->render = true;
		}
		else {
			categorie.second->render = false;
		}
		index++;
	}
}

void kandidatenApp::draw()
{

	//gl::clear(Color(0, 50, 0));
	//gl::drawSolidCircle(getWindowCenter(), 200);
	gl::enableAlphaBlending();

	gl::clear(Color(0.1f, 0.1f, 0.1f));
	gl::draw(background);

	for (const auto &activePoint : mActivePoints) {
		activePoint.second.draw();
	}

	for (auto dyingIt = mDyingPoints.begin(); dyingIt != mDyingPoints.end(); )
	{
		dyingIt->draw();
		if (dyingIt->isDead())
			dyingIt = mDyingPoints.erase(dyingIt);
		else
			++dyingIt;
	}

	// draw yellow circles at the active touch points

	gl::color(Color(1, 1, 0));
	for (const auto &touch : getActiveTouches())
	{
		gl::drawStrokedCircle(touch.getPos(), 20);
	}

	renderCategories();

}



CINDER_APP(kandidatenApp, RendererGl, prepareSettings)

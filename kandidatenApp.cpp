#include "Cards.h"
#include "Story.h"
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

#include "bluecadet/core/BaseApp.h"
#include "bluecadet/views/TouchView.h"

#include "cinder/Signals.h"


using namespace ci;
using namespace ci::app;
using namespace std;

using namespace bluecadet::core;
using namespace bluecadet::views;
using namespace bluecadet::touch;


//Controller-app

class kandidatenApp : public BaseApp
{
public:
	static void prepareSettings(ci::app::App::Settings* settings);

	void setup() override;
	void addView(BaseViewRef view);

	void	renderCategories();
	void	selectCategories(int enable[]);
    void    setUpTang();

    void	handleTouchBegan(const bluecadet::touch::TouchEvent& touchEvent);
    void	handleTouchMoved(const bluecadet::touch::TouchEvent& touchEvent);
    void	handleTouchEnded(const bluecadet::touch::TouchEvent& touchEvent);

    ivec2 windowSize{ 1920, 1080 };
    vector<bluecadet::touch::TouchEvent> tangibleTouch;

	gl::Texture2dRef texture;
	gl::Texture2dRef background;

	vec2 mMouseLoc;
	vec2 lastclick;
	int i;
	dataBaseController dbc;
	vector<pair<string, Cards*>> allCategories;


};

void kandidatenApp::prepareSettings(ci::app::App::Settings* settings) {
	settings->setHighDensityDisplayEnabled(true);

	SettingsManager::getInstance()->setup(settings, ci::app::getAssetPath("../assets/settings.json"), [](SettingsManager * manager) {
		manager->mFullscreen = false;
		manager->mWindowSize = ivec2(1920, 1080);
		manager->mDisplaySize = ivec2(1920, 1080);
		manager->mConsole = false;
		manager->mShowMinimap = false;
		manager->mShowStats = true;
		manager->mShowTouches = true;
		manager->mMinimizeParams = true;
        manager->mNativeTouchEnabled = true;    // for table
	});
}

// may not be needed. Used to add a View from Blue cadet to the scene root
void kandidatenApp::addView(BaseViewRef view)
{
	getRootView()->addChild(view);
}

void kandidatenApp::setup()
{
	BaseApp::setup();
    setUpTang();

	CI_LOG_I("MT: " << System::hasMultiTouch() << " Max points: " << System::getMaxMultiTouchPoints());

	background = gl::Texture::create( loadImage(loadAsset("background.png")));

	i = 0;


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

	/*- extract card categories -*/
	vector<vector<string> > cardCategory;
	dbc.extractCardCats(cardCategory);

	CI_LOG_I("sizes: " << categories.size() << " " << titles.size() << " " << bodyText.size() << " " << imgPath.size() << " " << cardCategory.size());
	


	Cards allCards = Cards();
	allCategories = allCards.categorize(&titles, &bodyText, &categories, &cardCategory);

	int enabledCategories[7] = { 1, 0, 0, 0, 0, 0, 0 };
	selectCategories(enabledCategories);

	disableFrameRate();
	gl::enableVerticalSync(true);

	renderCategories();
}

void kandidatenApp::renderCategories()
{
	for (auto &categorie : allCategories)
	{
		categorie.second->renderCards();
		addView(categorie.second->view);
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


/********************************
*         TANGIBLE STUFF        *
********************************/
void kandidatenApp::setUpTang()
{
    auto tangView = make_shared<TouchView>();
    tangView->moveToBack();
    tangView->setDragEnabled(false);
    tangView->setDebugDrawTouchPath(true);
    tangView->setMultiTouchEnabled(true);
    tangView->setSize(windowSize);
    tangView->setTransformOrigin(0.5f * tangView->getSize());

     // call touchesBegan
    tangView->getSignalTouchBegan().connect([=](const bluecadet::touch::TouchEvent& e){ handleTouchBegan(e); });

    // call touchesEnded
    tangView->getSignalTouchEnded().connect([=](const bluecadet::touch::TouchEvent& e) { handleTouchEnded(e); });

    addView(tangView); // add to root
}
void kandidatenApp::handleTouchBegan(const bluecadet::touch::TouchEvent& touchEvent) 
{
    // fill array to check if touches are a tangible object
    if (tangibleTouch.size() < 4) {
        tangibleTouch.push_back(touchEvent);
    }
    else {
        tangibleTouch.push_back(touchEvent);
        tangibleTouch.erase(tangibleTouch.begin());
        CI_LOG_I("tangSize: " << tangibleTouch.size());
    }


    if (tangibleTouch.size() == 4) {
        int count = 0;
        int mindist = 1000;
        int dist;
        float angle;

        for (int j = 0; j < tangibleTouch.size(); ++j) {
            for (int i = 0; i < tangibleTouch.size(); ++i) {

                dist = glm::distance(tangibleTouch[j].localPosition, tangibleTouch[i].localPosition);
                angle = glm::degrees(atan2(abs(tangibleTouch[j].localPosition.x - tangibleTouch[i].localPosition.x), abs(tangibleTouch[j].localPosition.y - tangibleTouch[i].localPosition.y)));

                // CI_LOG_I("dist: " << dist);
                //CI_LOG_I("angle: " << angle);

                if (dist != 0) {
                    if (dist < mindist) mindist = dist;
                }
                if (dist < 90 && dist > 50) {
                    count++;
                    //CI_LOG_I("counter" << count);
                }
            }
        }
        if (count != 0 && count == 8) {
            CI_LOG_I("counter" << count);
            CI_LOG_I("Vi lyckades hitta tangible enheten");
            CI_LOG_I("mindist: " << mindist);
            
            
                //pseudo-kod
                //if(mindist == storymodedist), call story inst = new story(storycards)
                //if mindist == categorydist), call categorymode
                //if mindist == reset, call resetfunction.

            Story inst;
        }
    }
}
void kandidatenApp::handleTouchMoved(const bluecadet::touch::TouchEvent& touchEvent) {}
void kandidatenApp::handleTouchEnded(const bluecadet::touch::TouchEvent& touchEvent)
{
    for (int i = 0; i < tangibleTouch.size(); ++i)
    {
        if (touchEvent.touchId == tangibleTouch[i].touchId)
            tangibleTouch.erase(tangibleTouch.begin() + i);
    }
}


CINDER_APP(kandidatenApp, RendererGl(RendererGl::Options().msaa(4).stencil(true)), kandidatenApp::prepareSettings)

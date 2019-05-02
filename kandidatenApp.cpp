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
    Story inst;

	gl::Texture2dRef texture;
	gl::Texture2dRef background;

	vec2 mMouseLoc;
	vec2 lastclick;
	int i;
	dataBaseController dbc;
	dataBaseController dbcstory;
	vector<pair<string, Cards*>> allCategories; // MAIN CONTAINER OF CARDS SORTED BY CATEGORY

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
        manager->mNativeTouchEnabled = true;    // true for table
	});
}

// may not be needed. Used to add a View from Bluecadet to the scene root
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


	/****************************   
    *       DATABASE STUFF      *
    *****************************/
    vector<string> categories;
    vector<pair<string, string>> titles;
    vector<pair<string, string>> bodyText;
    vector<string> imgPath;
    vector<vector<string> > cardCategory;
	vector<string> storytitles;
	vector<pair<string, string>> storybodies;
	vector<string> storyimgPath;

    {
        dbc = dataBaseController("online", "xml", "http://www.student.itn.liu.se/~chrad171/databas/databas/media/write.xml");
       dbcstory = dataBaseController("online", "xml", "http://www.student.itn.liu.se/~chrad171/databas/databas/media/stories.xml");

        dbc.extractCategories(categories);
        dbc.extractTitles(titles);
        dbc.extractBodies(bodyText);
        dbc.extractImgPaths(imgPath);
        dbc.extractCardCats(cardCategory);
		dbcstory.extractStorytitles(storytitles);
		dbcstory.extractstoryBodies(storybodies);
		dbcstory.extractstoryImgPaths(storyimgPath);
    }
	// 7 83 83 83 83
	CI_LOG_I("sizes: " << categories.size() << " " << titles.size() << " " << bodyText.size() << " " << imgPath.size() << " " << cardCategory.size());
	CI_LOG_I("test" << storytitles.size() << "storybodies" << storybodies.size() << "storyimgpath" << storyimgPath.size());
    

    /********************************
    *       FILL MAIN CONTAINER     *
    *********************************/
	Cards allCards = Cards();
	allCategories = allCards.categorize(&titles, &bodyText, &categories, &cardCategory);

	int enabledCategories[7] = { 1, 0, 0, 0, 0, 0, 0 };
	selectCategories(enabledCategories);
    /*********************************/


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
    bool istangible = false;

    // fill array to check if touches are a tangible object
    CI_LOG_I("touchid: " << touchEvent.touchId);
    if (tangibleTouch.size() < 4) {
        tangibleTouch.push_back(touchEvent);
    }
    else {
        tangibleTouch.push_back(touchEvent);
        tangibleTouch.erase(tangibleTouch.begin());
        CI_LOG_I("tangSize: " << tangibleTouch.size());
    }

    // when array could contain tangible object
    if (tangibleTouch.size() == 4) {

        // make sure touchIDs are a sequence
        for (int i = 0; i < 4; ++i) 
        {
            if (tangibleTouch[i].touchId == touchEvent.touchId - 3 + i) 
            {
                CI_LOG_I("4 tangila punkter eftervarandra");
                istangible = true;
            }    
            else istangible = false;
         
        }

        int count = 0;
        int mindist = 1000;
        int maxdist;
        int dist;
        float angle;
        bool big = false;

        if(istangible){
            
            // find min dist between spikes to ID puck
            for (int j = 0; j < tangibleTouch.size(); ++j) {
                for (int i = 0; i < tangibleTouch.size(); ++i) {
                    dist = glm::distance(tangibleTouch[j].localPosition, tangibleTouch[i].localPosition);
                    angle = glm::degrees(atan2(abs(tangibleTouch[j].localPosition.x - tangibleTouch[i].localPosition.x), abs(tangibleTouch[j].localPosition.y - tangibleTouch[i].localPosition.y)));

                    CI_LOG_I("dist: " << dist);
                    //CI_LOG_I("angle: " << angle);

                    // save minimum distance
                    if (dist != 0) {
                        if (dist < mindist && dist > 20) mindist = dist;
                        if (dist > 250) big = true;
                    }

                    // edge distances
                    if (dist > 80 && dist < 150) count++;
                }
            }
            
            // tangible object found 
            if (count != 0 && count >= 8 && !big) {
                CI_LOG_I("counter" << count);
                CI_LOG_I("Vi lyckades hitta tangible enheten");
                CI_LOG_I("mindist: " << mindist);

                // storymode
                if (mindist < 60 && mindist > 30) {
                    CI_LOG_I("storypucken hittad");
                    addView(inst.storyView);
                    inst.storyView->setHidden(false);

                    // translate view of active cards
                    for (auto &categorie : allCategories)
                    {
                        categorie.second->view->setSize(vec2{ 0.5f*windowSize.x, windowSize.y });
                        categorie.second->view->setGlobalPosition(ivec2{ 960,0 });

                        auto kids = categorie.second->view->getChildren();
                        for (auto &kid : kids)
                        {
                            if (!(kid->isHidden())) kid->setScale(0.5); // scale kid
                        }
                    }
                }
            }
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
    
    // puck is lifted 
    if (tangibleTouch.empty())
    {
        inst.storyView->setHidden(true); // hide storymode 
        for (auto &categorie : allCategories) // make active cards fullscreen
        {
            categorie.second->view->setSize(windowSize);
            categorie.second->view->setGlobalPosition(ivec2{ 0,0 });

            auto kids = categorie.second->view->getChildren();
            for (auto &kid : kids)
            {
                if (!(kid->isHidden())) kid->setScale(1); // scale kid
            }
        }
    }
}


CINDER_APP(kandidatenApp, RendererGl(RendererGl::Options().msaa(4).stencil(true)), kandidatenApp::prepareSettings)

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
#include "bluecadet/views/ImageView.h"
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
    vector<bluecadet::touch::TouchEvent> movingTang;
    bool storyMode = false;
   
    Story cardStory;
    Cards storyCards;

	gl::Texture2dRef texture;
	gl::Texture2dRef background;

	int i;
	dataBaseController dbc;
	dataBaseController dbcstory;
	vector<pair<string, Cards*>> allCategories; // MAIN CONTAINER OF CARDS SORTED BY CATEGORY
    // container of cards for stories

};

void kandidatenApp::prepareSettings(ci::app::App::Settings* settings) {
	settings->setHighDensityDisplayEnabled(true);

	SettingsManager::getInstance()->setup(settings, ci::app::getAssetPath("../assets/settings.json"), [](SettingsManager * manager) {
		manager->mFullscreen = false;
		manager->mWindowSize = ivec2(1920, 1080);
		manager->mDisplaySize = ivec2(1920, 1080);
		manager->mConsole = false;
		manager->mShowMinimap = false;
		manager->mShowStats = false;
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

	CI_LOG_I("MT: " << System::hasMultiTouch() << " Max points: " << System::getMaxMultiTouchPoints());

	//create background image
	ImageViewRef bg = make_shared<ImageView>();
	auto srf = loadImage(loadAsset("background.png"));
	bg->setTexture(gl::Texture::create(srf));
	bg->setWidth(getRootView()->getWidth());
	bg->setHeight(getRootView()->getHeight());
	bg->setScaleMode(bluecadet::views::ImageView::ScaleMode::COVER);
	getRootView()->addChild(bg);

	i = 0;

	/****************************   
    *       DATABASE STUFF      *
    *****************************/
    vector<string> categories;
    vector<pair<string, string>> titles;
    vector<pair<string, string>> bodyText;
    vector<string> imgPath;
    vector<vector<string> > cardCategory;
    // stories
	vector<string> storytitles;
	vector<pair<string, string>> storybodies;
	vector<string> storyimgPath;

    {
        dbc = dataBaseController("online", "xml", "http://www.student.itn.liu.se/~chrad171/databas/databas/media/noVid.xml");
     
        dbc.extractCategories(categories);
        dbc.extractTitles(titles);
        dbc.extractBodies(bodyText);
        dbc.extractImgPaths(imgPath);
        dbc.extractCardCats(cardCategory);

        dbcstory = dataBaseController("online", "xml", "http://www.student.itn.liu.se/~chrad171/databas/databas/media/stories.xml");

		dbcstory.extractStorytitles(storytitles);
		dbcstory.extractstoryBodies(storybodies);
		dbcstory.extractstoryImgPaths(storyimgPath);
    }

	// 7 83 83 83 83
	CI_LOG_I("sizes: " << categories.size() << " " << titles.size() << " " << bodyText.size() << " " << imgPath.size() << " " << cardCategory.size());
	CI_LOG_I("test" << storytitles.size() << "storybodies" << storybodies.size() << "storyimgpath" << storyimgPath.size());
    
	//3 st första, 17 andra story, 8 sista story.
	int counter = 0;
	vector < pair <string, Cards*>> story(3);
    story[0].first = storytitles[0];
    story[1].first = storytitles[1];
    story[2].first = storytitles[2];
        

    for(auto bodies : storyimgPath) 
    {
		++i;
		if (i <= 3) 
        {
			//story1.
		}

	}
    //storyCards = story1[0].second;

    /********************************
    *       FILL MAIN CONTAINER     *
    *********************************/
	Cards allCards = Cards();
	allCategories = allCards.categorize(&titles, &bodyText, &imgPath, &categories, &cardCategory);

	int enabledCategories[7] = { 1, 0, 0, 0, 0, 0, 0 };
	selectCategories(enabledCategories);
    /*********************************/
    setUpTang(); // view for tangible objects 

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
		if (enable[index]) 
			categorie.second->render = true;
		
		else 
			categorie.second->render = false;
		
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

     // call touch functions
    tangView->getSignalTouchBegan().connect([=](const bluecadet::touch::TouchEvent& e){ handleTouchBegan(e); });
    tangView->getSignalTouchMoved().connect([=](const bluecadet::touch::TouchEvent& e) { handleTouchMoved(e); });
    tangView->getSignalTouchEnded().connect([=](const bluecadet::touch::TouchEvent& e) { handleTouchEnded(e); });

    // fill story cards & instantiate storymode
    storyCards = *allCategories[0].second;
    cardStory = Story(storyCards);

    addView(tangView); // add tang touch view to root



      // testing storymode on computer
    /*
    
    {
        addView(cardStory.storyView);
        cardStory.storyView->setHidden(false);
        storyMode = true;
        
        //updateCardPos()
        for (auto &categorie : allCategories)
        {
          //  Cards::translateCards( *(categorie.second), cardStory.storyView->getPositionConst() );
            
            categorie.second->view->setSize(vec2{ 0.5f*windowSize.x, windowSize.y });
            categorie.second->view->setGlobalPosition(ivec2{ 960,0 });
            categorie.second->view->setTransformOrigin(0.5f * categorie.second->view->getSize());

            auto kids = categorie.second->view->getChildren();
            for (auto &kid : kids)
            {
                if (!(kid->isHidden())) kid->setScale(0.5); // scale kid
            }
        }
    }
 */
}

void kandidatenApp::handleTouchBegan(const bluecadet::touch::TouchEvent& touchEvent) 
{
    bool istangible = false;
    vec2 storyPos{ 0,0 };

    // fill array to check if touches are a tangible object
    if (tangibleTouch.size() < 4) 
        tangibleTouch.push_back(touchEvent);
    else 
    {
        tangibleTouch.push_back(touchEvent);
        tangibleTouch.erase(tangibleTouch.begin());
    }

    // when array could contain tangible object
    if (tangibleTouch.size() == 4) 
    {
        // make sure touchIDs are a sequence
        for (int i = 0; i < 4; ++i) 
        {
            if (tangibleTouch[i].touchId == touchEvent.touchId - 3 + i) 
                istangible = true;   

            else istangible = false;
        }

        int count = 0;  // number of distances that are on the outer triangle of puck
        int mindist = 1000;
        //int maxdist;
        int dist;
        bool big = false;

        if (istangible) 
        {
            // find min dist between spikes to ID puck
            for (int j = 0; j < tangibleTouch.size(); ++j)
            {
                for (int i = 0; i < tangibleTouch.size(); ++i)
                {
                    dist = glm::distance(tangibleTouch[j].localPosition, tangibleTouch[i].localPosition);

                    // save minimum distance
                    if (dist != 0)
                    {
                        if (dist < mindist && dist > 20) mindist = dist;
                        if (dist > 250) big = true;
                    }

                    // edge distances
                    if (dist > 80 && dist < 150)
                    {
                        count++;
                        storyPos = tangibleTouch[j].globalPosition;
                    }
                }
            }
        }

        // tangible object found 
        if (count != 0 && count >= 8 && !big) 
        {
            // add storymode view
            if (mindist < 60 && mindist > 30) 
            {
                cardStory.storyView->setPosition(storyPos);   // set position of story mode at puck's position
                addView(cardStory.storyView);
                storyMode = true;
                cardStory.storyView->setHidden(false);

                //updateCardPos()
                for (auto &categorie : allCategories)
                {
                    categorie.second->view->setSize(vec2{ 0.5f*windowSize.x, windowSize.y });
                    categorie.second->view->setGlobalPosition(ivec2{ 960,0 });
                    categorie.second->view->setTransformOrigin(0.5f * categorie.second->view->getSize());

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

void kandidatenApp::handleTouchMoved(const bluecadet::touch::TouchEvent& touchEvent) 
{
    if (storyMode)
    {
        // fill array to check if touches are a tangible object
        if (movingTang.size() < 4)
            movingTang.push_back(touchEvent);
        else
        {
            movingTang.push_back(touchEvent);
            movingTang.erase(movingTang.begin());
        }

        if (movingTang.size() == 4)
        {
            cardStory.storyView->setPosition(movingTang[0].globalPosition);   // set position of story mode at puck's position
            //updateCardPos()
        }
    }
}

void kandidatenApp::handleTouchEnded(const bluecadet::touch::TouchEvent& touchEvent)
{
    for (int i = 0; i < tangibleTouch.size(); ++i)
    {
        if (touchEvent.touchId == tangibleTouch[i].touchId)
            tangibleTouch.erase(tangibleTouch.begin() + i);
    }
    
    // puck is lifted 
    if (storyMode && tangibleTouch.empty())
    {
        storyMode = false;
        cardStory.storyView->setHidden(true);
        movingTang.clear();

        for (auto &categorie : allCategories) // make active cards fullscreen
        {
            categorie.second->view->setSize(windowSize);
            categorie.second->view->setGlobalPosition(ivec2{ 0,0 });
            categorie.second->view->setTransformOrigin(0.5f * categorie.second->view->getSize());

            auto kids = categorie.second->view->getChildren();
            for (auto &kid : kids)
            {
                if (!(kid->isHidden())) kid->setScale(1); // scale kid back
            }
        }
    }
}


CINDER_APP(kandidatenApp, RendererGl(RendererGl::Options().msaa(4).stencil(true)), kandidatenApp::prepareSettings)

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

	CI_LOG_I("MT: " << System::hasMultiTouch() << " Max points: " << System::getMaxMultiTouchPoints());

	background = gl::Texture::create( loadImage(loadAsset("background.png")));

	i = 0;

	/*- connect to data base -*/
	dbc = dataBaseController("online", "xml", "http://www.student.itn.liu.se/~chrad171/databas/databas/media/noVid.xml");
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
	allCategories = allCards.categorize(&titles, &bodyText, &imgPath, &categories, &cardCategory);

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


CINDER_APP(kandidatenApp, RendererGl(RendererGl::Options().msaa(4).stencil(true)), kandidatenApp::prepareSettings)

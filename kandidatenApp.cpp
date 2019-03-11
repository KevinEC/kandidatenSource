#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class kandidatenApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void kandidatenApp::setup()
{
	// load xml-file
}

void kandidatenApp::mouseDown( MouseEvent event )
{
}

void kandidatenApp::update()
{
}

void kandidatenApp::draw()
{
	gl::clear( Color( 0, 50, 0 ) );
	gl::drawSolidCircle(getWindowCenter(), 200);
}

CINDER_APP( kandidatenApp, RendererGl )

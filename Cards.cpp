#include "Cards.h"

using namespace ci;
using namespace ci::app;
using namespace std;

Cards::Cards()
{
	string par = "more long text with a lot of important words for testing and stuff. more long text with a lot of important words for testing and stuff. more long text with a lot of important words for testing and stuff. more long text with a lot of important words for testing and stuff. more long text with a lot of important words for testing and stuff. more long text with a lot of important words for testing and stuff. ";

	float x1 = rand() % 400;
	float y1 = rand() % 400;
	testkort = Card(300.0f, 320.0f, "test", "more long text");
	rectKort = Card(x1,y1,"A pretty title", par);

}

Cards::Cards(std::vector<std::pair<std::string, std::string>>* titles, std::vector<std::pair<std::string, std::string>>* bodyText)
{
	float x1 = rand() % 400;
	float y1 = rand() % 400;

	std::vector<std::pair<std::string, std::string>>::iterator titleIt = titles->begin();
	std::vector<std::pair<std::string, std::string>>::iterator bodyIt = bodyText->begin();


	while(titleIt != titles->end() - 70) {
		allcards.push_back( Card(x1, y1, titleIt->first, bodyIt->first) );
		titleIt++;
		bodyIt++;
	}

}

void Cards::renderCards() {
   /* gl::pushModelMatrix();
    gl::rotate*/
	rectKort.renderCard();
	//testkort.renderCard();
}

Cards::~Cards()
{
}

void Cards::sort()
{
}

void Cards::categorize()
{
}

void Cards::collision()
{
}

void Cards::setPath(Card &inst)
{
}

void Cards::search()
{
}
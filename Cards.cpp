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

Cards::Cards(vector<pair<string, string>>* titles, vector<pair<string, string>>* bodyText)
{
	loaded = false;
	allcards.reserve(20);
	float x1 = rand() % 400;
	float y1 = rand() % 400;

	vector<pair<string, string>>::iterator titleIt = titles->begin();
	vector<pair<string, string>>::iterator bodyIt = bodyText->begin();


	while(titleIt != titles->end() - 70) {
		allcards.push_back( new Card(x1, y1, titleIt->first, bodyIt->first) );

		x1 = rand() % 400;
		y1 = rand() % 400;

		titleIt++;
		bodyIt++;
	}

	loaded = true;

}

Cards::Cards(vector<pair<string, string>>* titles, vector<pair<string, string>>* bodyText, vector<pair<string, string>>* categories, vector<pair<string, string>>* cardCat, bool createAll)
{
	vector<pair<string, string>>::iterator catIt = categories->begin();

	vector< pair<string, Cards*> > categorizedCards;

	// insert all categorie names
	while (catIt != categories->end())
	{
		categorizedCards.push_back( make_pair(catIt->first, nullptr) );
		catIt++;
	}
	categorize(categorizedCards, cardCat, titles, bodyText);
}

void Cards::renderCards() 
{
	for (int i = 0; i < allcards.size(); i++)
	{
		allcards[i]->renderCard();
	}
}

Cards::~Cards()
{
}

void Cards::sort()
{
}

void Cards::categorize(vector< pair<string, Cards*> >& categorizedCards, vector<pair<string, string>>* cardCat, vector<pair<string, string>>* titles, vector<pair<string, string>>* bodyText)
{
	vector< pair<string, Cards*> >::iterator sortedCardsIt;

	vector<pair<string, string>>::iterator cardCatIt = titles->begin();
	vector<pair<string, string>>::iterator titleIt = titles->begin();
	vector<pair<string, string>>::iterator bodyIt = bodyText->begin();

	for (int i = 0; i < categorizedCards.size(); i++)
	{
		while (cardCatIt != cardCat->end())
		{
			if (categorizedCards[i].first == cardCatIt->first) {
				categorizedCards[i].second = new Cards();
			}
		}
	}



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

void Cards::mouseDown(MouseEvent event)
{
	for (int i = 0; i < allcards.size(); i++)
	{
		allcards[i]->mouseDown(event);
	}
}

void Cards::mouseDrag(MouseEvent event)
{
	for (int i = 0; i < allcards.size(); i++)
	{
		allcards[i]->mouseDrag(event);
	}
}

void Cards::mouseUp(MouseEvent event)
{
	for (int i = 0; i < allcards.size(); i++)
	{
		allcards[i]->mouseUp(event);
	}
}
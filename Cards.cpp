#include "Cards.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace bluecadet::core;
using namespace bluecadet::views;
using namespace bluecadet::touch;

Cards::Cards()
{
	//render the instace of card by default
	render = true;
	view = make_shared<BaseView>();
	view->setSize({ 1920, 1080 });

}

Cards::Cards(string categorie)
{
	this->categorie = categorie;
	render = true;

	view = make_shared<BaseView>();
	view->setSize({ 1920, 1080 });	
}

Cards::~Cards()
{
}

vector< pair<string, Cards*> > Cards::categorize(vector<pair<string, string>>* titles, vector<pair<string, string>>* bodyText, vector<string>* imgPath, vector<string>* categories, vector<vector<string>>* cardCat)
{
	vector<string>::iterator catIt = categories->begin();
	vector< pair<string, Cards*> > categorizedCards;
	categorizedCards.reserve(8);

	// insert all categorie names
	while (catIt != categories->end())
	{
		categorizedCards.push_back( make_pair(*catIt, new Cards(*catIt) ) ); // set the instance var categorie 
		catIt++;
	}
	return *sort(&categorizedCards, cardCat, titles, bodyText, imgPath); // dereference to create a copy to main
}

vector<pair<string, Cards*>>* Cards::sort(vector< pair<string, Cards*> >* categorizedCards, vector<vector<string>>* cardCat, vector<pair<string, string>>* titles, vector<pair<string, string>>* bodyText, vector<string>* imgPath)
{
	for (int i = 0; i < categorizedCards->size(); i++)
	{
		for (int j = 0; j < cardCat->size(); j++)
		{
			string searched = categorizedCards->at(i).first;
			if (findCat(cardCat->at(j).begin(), cardCat->at(j).end(), searched)) { // help function to search vector for string
				float x1 = rand() % 400;
				float y1 = rand() % 400;
				categorizedCards->at(i).second->addCard(new Card(x1, y1, titles->at(j), bodyText->at(j), imgPath->at(j)));
			}
		}
	}
	return categorizedCards; // return pointer to avoid making an extra copy locally in Cards
}

void Cards::renderCards()
{
	// only render if true
	if (render) {
		for (int i = 0; i < allcards.size(); i++)
		{
			addView(allcards[i]->object);
		}
	}
}

void Cards::tangLayout(const vec2 xy, const Rectf rect)
{
	for (auto card : allcards)
	{
		vec2 cardXy = card->object->convertLocalToGlobal( card->object->getGlobalPosition() );
	
		// if the card is within the story area 
		if (rect.contains(cardXy))
		{
			vec2 size = vec2{ rect.getWidth() / 2, rect.getHeight() / 2 };

			/* create areas for upper left, upper right, bottom left and bottom right */

			Rectf ul = Rectf(xy, size);
			Rectf ur = Rectf({ xy.x + size.x, xy.y }, { 2.0f*size.x, size.y });
			Rectf bl = Rectf({ xy.x, xy.y + size.y }, { size.x, 2.0f*size.y });
			Rectf br = Rectf(xy + size, 2.0f*size);
			
			// common vars for calc new position
			float offsetY = card->object->getHeight() + rand() % 20;
			float offsetX = card->object->getWidth() + rand() % 20;
			vec2 newPos = cardXy; // set the current position as default

			/* calc new position outside the story bounds */

			if (ul.contains(cardXy))
			{
				vec2 delta = abs(cardXy - rect.getUpperLeft());

				if (delta.x < delta.y) {
					newPos = { cardXy.x - delta.x - offsetX, cardXy.y };
				}
				else {
					newPos = { cardXy.x, cardXy.y - delta.y - offsetY };
				}
			}
			else if (ur.contains(cardXy))
			{
				vec2 delta = abs(cardXy - rect.getUpperRight());

				if (delta.x < delta.y) {
					newPos = { cardXy.x + delta.x + offsetX, cardXy.y };
				}
				else {
					newPos = { cardXy.x, cardXy.y - delta.y - offsetY };
				}
			}
			else if (bl.contains(cardXy))
			{
				vec2 delta = abs(cardXy - rect.getLowerLeft() );

				if (delta.x < delta.y) {
					newPos = { cardXy.x - delta.x - offsetX, cardXy.y };
				}
				else {
					newPos = { cardXy.x, cardXy.y + delta.y + offsetY };
				}
			}
			else if (br.contains(cardXy))
			{
				vec2 delta = abs(cardXy - rect.getLowerRight());

				if (delta.x < delta.y) {
					vec2 newPos = { cardXy.x + delta.x + offsetX, cardXy.y };
				}
				else {
					vec2 newPos = { cardXy.x, cardXy.y + delta.y + offsetY };
				}
			}
			card->animateTranslate(newPos);
		}
	}
}

void Cards::addView(BaseViewRef view)
{
	this->view->addChild(view);
}

void Cards::addCard(Card *card)
{
	allcards.push_back(card);
}

bool Cards::findCat(vector<string>::iterator first, vector<string>::iterator last, string searched)
{
	for (; first != last; ++first)
		if (*first == searched) return true;
	
	return false;
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

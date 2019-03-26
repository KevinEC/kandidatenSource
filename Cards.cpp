#include "Cards.h"

using namespace ci;

Cards::Cards()
{
	float x1 = rand() % 400;
	float y1 = rand() % 400;
	testkort = Card(300.0f, 320.0f);
	rectKort = Card(x1,y1);

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
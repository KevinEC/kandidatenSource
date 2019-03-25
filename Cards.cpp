#include "Cards.h"

using namespace ci;

Cards::Cards()
{
	testkort = Card(300.0f, 120.0f, 300.0f, 100.0f);
	rectKort = Card(200, rand() %400, rand() %400, 200); //Xupper, Yupper, Xwidth, Yheight
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
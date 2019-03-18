#include "Cards.h"
#include "Path.h"
#include "Transform.h"

using namespace ci;

Cards::Cards()
{
	testkort = Card();
	rectKort = Card(rand() %300, rand() %300, rand()% 200, rand() %400);

	Transform::translate(testkort);
	Transform::translate(rectKort, 100, 100);
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
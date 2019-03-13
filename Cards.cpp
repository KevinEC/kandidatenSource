#include "Cards.h"
#include "Path.h"
#include "Transform.h"

using namespace ci;

Cards::Cards()
{
	testkort = Card();
	rectKort = Card(50, 100, 66, 100);

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

#include "Knight.h"

#include "BlackKing.h"

void Knight::_register_methods()
{
	register_method((char*)"_init", &Knight::_init);
}

void Knight::_init()
{

}

void Knight::takeTurn()
{
	if (--currTurnCount <= 0)
	{
		//knightAI();
		currTurnCount = TurnCount;
	}
}

void Knight::moveTo(int x, int y)
{
	__super::moveTo(x, y);
}

void Knight::knightAI()
{
	if(!tryToCheckmate())
	{
		if(!tryToCheck())
		{

		}
	}
}

bool Knight::tryToCheckmate()
{
	if (((pX + pY) + (BlackKing::blackKingInstance->pX + BlackKing::blackKingInstance->pY)) % 2 == 1)
	{
		return false;
	}


	
}

bool Knight::tryToCheck()
{
	return false;
}
#include "Rook.h"

#include "BlackKing.h"


void Rook::_register_methods()
{
	register_method((char*)"_init", &Rook::_init);
}

void Rook::_init()
{

}

void Rook::takeTurn()
{
	if (--currTurnCount <= 0)
	{
		rookAI();
		currTurnCount = TurnCount;
	}
}

void Rook::moveTo(int x, int y)
{
	__super::moveTo(x, y);
}

void Rook::changeSquareThreatCount(int mod)
{
	int start;

	for (start = pX + 1; start < 8; start += 1)
	{
		if (!ChessBoard::chessBoardInstance->isSquareEmpty(start, pY))
		{
			ChessBoard::chessBoardInstance->getSquare(start, pY)->threatCount += mod;
			break;
		}
		else
		{
			ChessBoard::chessBoardInstance->getSquare(start, pY)->threatCount += mod;
		}
	}

	for (start = pX - 1; start >= 0; start += -1)
	{
		if (!ChessBoard::chessBoardInstance->isSquareEmpty(start, pY))
		{
			ChessBoard::chessBoardInstance->getSquare(start, pY)->threatCount += mod;
			break;
		}
		else
		{
			ChessBoard::chessBoardInstance->getSquare(start, pY)->threatCount += mod;
		}
	}

	for (start = pY + 1; start < 8; start += 1)
	{
		if (!ChessBoard::chessBoardInstance->isSquareEmpty(pX, start))
		{
			ChessBoard::chessBoardInstance->getSquare(pX, start)->threatCount += mod;
			break;
		}
		else
		{
			ChessBoard::chessBoardInstance->getSquare(pX, start)->threatCount += mod;
		}
	}

	for (start = pY - 1; start >= 0; start += -1)
	{
		if (!ChessBoard::chessBoardInstance->isSquareEmpty(pX, start))
		{
			ChessBoard::chessBoardInstance->getSquare(pX, start)->threatCount += mod;
			break;
		}
		else
		{
			ChessBoard::chessBoardInstance->getSquare(pX, start)->threatCount += mod;
		}
	}
}

void Rook::rookAI()
{
	if(!tryToCheckmate())
	{
		if(!tryToCheck(BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY))
		{
			if(!narrowBlackKingArea())
			{
				runAway();
			}
		}
	}
}

bool Rook::tryToMoveSquare(int fX, int fY, int sX, int sY)
{
	int count;

	if(fX == sX)
	{
		count = fY > sY ? -1 : 1;

		for(int start = fY + count; start < sY; start += count)
		{
			if(!ChessBoard::chessBoardInstance->isSquareEmpty(fX, start))
			{
				return false;
			}
		}

		if(!ChessBoard::chessBoardInstance->isSquareEmpty(sX, sY))
		{
			if (BlackKing::blackKingInstance != ChessBoard::chessBoardInstance->getSquare(sX, sY)->currChessPiece)
			{
				return false;
			}
		}

		return true;
	}
	else if(fY == sY)
	{
		count = fX > sX ? -1 : 1;

		for (int start = fX + count; start < sX; start += count)
		{
			if (!ChessBoard::chessBoardInstance->isSquareEmpty(start, fY))
			{
				return false;
			}
		}

		if (!ChessBoard::chessBoardInstance->isSquareEmpty(sX, sY))
		{
			if (BlackKing::blackKingInstance != ChessBoard::chessBoardInstance->getSquare(sX, sY)->currChessPiece)
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

bool Rook::tryToCheckmate()
{
	bool res = tryToMoveSquare(pX, pY, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY);

	if(res)
	{
		moveTo(BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY);
		BlackKing::blackKingInstance->die();
	}

	return res;
}

bool Rook::tryToCheck(int targetX, int targetY)
{
	int xDiff = pX - targetX;
	int yDiff = pY - targetY;

	if(abs(xDiff) > abs(yDiff))
	{
		if(tryToMoveSquare(pX, pY, pX, pY-yDiff))
		{
			if(tryToMoveSquare(pX, pY-yDiff, targetX, targetY))
			{
				moveTo(pX, pY-yDiff);
				return true;
			}
		}

		if (tryToMoveSquare(pX, pY, pX-xDiff, pY))
		{
			if (tryToMoveSquare(pX-xDiff, pY, targetX, targetY))
			{
				moveTo(pX-xDiff, pY);
				return true;
			}
		}
	}
	else
	{
		if (tryToMoveSquare(pX, pY, pX-xDiff, pY))
		{
			if (tryToMoveSquare(pX-xDiff, pY, targetX, targetY))
			{
				moveTo(pX-xDiff, pY);
				return true;
			}
		}

		if (tryToMoveSquare(pX, pY, pX, pY-yDiff))
		{
			if (tryToMoveSquare(pX, pY-yDiff, targetX, targetY))
			{
				moveTo(pX, pY-yDiff);
				return true;
			}
		}
	}

	return false;
}

bool Rook::narrowBlackKingArea()
{
	if(tryToCheck(BlackKing::blackKingInstance->pX + 1, BlackKing::blackKingInstance->pY + 1))
	{
		return true;
	}

	if (tryToCheck(BlackKing::blackKingInstance->pX - 1, BlackKing::blackKingInstance->pY - 1))
	{
		return true;
	}

	if (tryToCheck(BlackKing::blackKingInstance->pX - 1, BlackKing::blackKingInstance->pY + 1))
	{
		return true;
	}

	if (tryToCheck(BlackKing::blackKingInstance->pX + 1, BlackKing::blackKingInstance->pY - 1))
	{
		return true;
	}

	return false;
}

void Rook::runAway()
{
	
}

void Rook::die()
{
	changeSquareThreatCount(-1);
	__super::die();
}

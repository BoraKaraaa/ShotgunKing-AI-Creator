#include "Knight.h"

#include "BlackKing.h"

void Knight::_register_methods()
{
	register_method((char*)"_init", &Knight::_init);
}

void Knight::_init()
{
	chessPieceType = ChessPieceType::KNIGHT;
}

void Knight::takeTurn()
{
	if (--currTurnCount <= 0)
	{
		knightAI();
		currTurnCount = TurnCount;
	}
}

void Knight::moveTo(int x, int y)
{
	__super::moveTo(x, y);
}

void Knight::changeSquareThreatCount(int mod)
{
	for (int k = 0, i = 1, j = 2; k < 8; k++)
	{
		if (k % 4 == 1) i *= -1;
		else if (k % 4 == 2)
		{
			i *= -1;
			j *= -1;
		}
		else if (k % 4 == 3) i *= -1;
		else if (k == 4)
		{
			i = 2;
			j = 1;
		}

		if(ChessBoard::chessBoardInstance->isSquareValid(pX + i, pY + j))
		{
			ChessBoard::chessBoardInstance->getSquare(pX + i, pY + j)->threatCount += mod;
		}
	}
}

void Knight::knightAI()
{
	if(!tryToCheckmate())
	{
		if(!tryToCheck(BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY))
		{
			if(!narrowBlackKingArea())
			{
				tryToCheck(pX, pY);
			}
		}
	}
}

bool Knight::tryToMoveSquare(int fX, int fY, int sX, int sY) 
{
	if(!ChessBoard::chessBoardInstance->isSquareEmpty(sX, sY))
	{
		if(ChessBoard::chessBoardInstance->getSquare(sX, sY)->currChessPiece != BlackKing::blackKingInstance)
		{
			return false;
		}
	}

	return abs((fX - sX) * (fY - sY)) == 2;
}

bool Knight::tryToCheckmate()
{
	if (((pX + pY) + (BlackKing::blackKingInstance->pX + BlackKing::blackKingInstance->pY)) % 2 == 0)
	{
		return false;
	}

	if(tryToMoveSquare(pX, pY, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY))
	{
		moveTo(BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY);
		BlackKing::blackKingInstance->die();
		return true;
	}
	else
	{
		return false;
	}
}

bool Knight::tryToCheck(int targetX, int targetY)
{
	if (((pX + pY) + (targetX + targetY)) % 2 == 1)
	{
		return false;
	}

	for(int k = 0, i = 1, j = 2; k < 8; k++)
	{
		if (k % 4 == 1) i *= -1;
		else if (k % 4 == 2)
		{
			i *= -1;
			j *= -1;
		}
		else if (k % 4 == 3) i *= -1;
		else if (k == 4) 
		{
			i = 2;
			j = 1;
		}

		if(!ChessBoard::chessBoardInstance->isSquareValid(targetX + i, targetY + j))
		{
			continue;
		}

		if(tryToMoveSquare(pX, pY, targetX + i, targetY + j))
		{
			moveTo(targetX + i, targetY + j);
			return true;
		}
	}


	return false;
}

bool Knight::narrowBlackKingArea()
{
	if (((pX + pY) + (BlackKing::blackKingInstance->pX + BlackKing::blackKingInstance->pY)) % 2 == 0) // same color 
	{
		for (int k = 0, i = 1, j = 1; k < 4; k++)
		{
			if (k == 1) i *= -1;
			else if (k == 2)
			{
				i *= -1;
				j *= -1;
			}
			else if (k == 3) i *= -1;

			if (!ChessBoard::chessBoardInstance->isSquareValid(BlackKing::blackKingInstance->pX + i, BlackKing::blackKingInstance->pY + j))
			{
				continue;
			}

			if (tryToCheck(BlackKing::blackKingInstance->pX + i, BlackKing::blackKingInstance->pY + j))
			{
				return true;
			}
		}
	}
	else // diff color
	{
		for (int k = 0, i = 1, j = 0; k < 4; k++)
		{
			if (k == 1) i *= -1;
			else if (k == 2)
			{
				i = 0;
				j = 1;
			}
			else if (k == 3) j *= -1;

			if (!ChessBoard::chessBoardInstance->isSquareValid(BlackKing::blackKingInstance->pX + i, BlackKing::blackKingInstance->pY + j))
			{
				continue;
			}

			if (tryToCheck(BlackKing::blackKingInstance->pX + i, BlackKing::blackKingInstance->pY + j))
			{
				return true;
			}
		}
	}

	return false;
}

void Knight::die()
{
	changeSquareThreatCount(-1);
	__super::die();
}
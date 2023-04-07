#include "Queen.h"

#include "BlackKing.h"

void Queen::_register_methods()
{
	register_method((char*)"_init", &Queen::_init);
}

void Queen::_init()
{

}

void Queen::takeTurn()
{
	if (--currTurnCount <= 0)
	{
		queenAI();
		currTurnCount = TurnCount;
	}
}

void Queen::moveTo(int x, int y)
{
	__super::moveTo(x, y);
}

void Queen::changeSquareThreatCount(int mod)
{
	changeSquareThreatCountLikeRook(mod);
	changeSquareThreatCountLikeBishop(mod);
}

void Queen::changeSquareThreatCountLikeRook(int mod)
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

void Queen::changeSquareThreatCountLikeBishop(int mod)
{
	int startX, startY;
	int operation = 1;

	for (startX = pX + operation, startY = pY + operation; startX < 8 && startY < 8; startX += operation, startY += operation)
	{
		if (!ChessBoard::chessBoardInstance->isSquareEmpty(startX, startY))
		{
			ChessBoard::chessBoardInstance->getSquare(startX, startY)->threatCount += mod;
			break;
		}
		else
		{
			ChessBoard::chessBoardInstance->getSquare(startX, startY)->threatCount += mod;
		}
	}

	operation = -1;

	for (startX = pX + operation, startY = pY + operation; startX >= 0 && startY >= 0; startX += operation, startY += operation)
	{
		if (!ChessBoard::chessBoardInstance->isSquareEmpty(startX, startY))
		{
			ChessBoard::chessBoardInstance->getSquare(startX, startY)->threatCount += mod;
			break;
		}
		else
		{
			ChessBoard::chessBoardInstance->getSquare(startX, startY)->threatCount += mod;
		}
	}

	operation = 1;

	for (startX = pX + operation, startY = pY - operation; startX < 8 && startY >= 0; startX += operation, startY -= operation)
	{
		if (!ChessBoard::chessBoardInstance->isSquareEmpty(startX, startY))
		{
			ChessBoard::chessBoardInstance->getSquare(startX, startY)->threatCount += mod;
			break;
		}
		else
		{
			ChessBoard::chessBoardInstance->getSquare(startX, startY)->threatCount += mod;
		}
	}

	operation = -1;

	for (startX = pX + operation, startY = pY - operation; startX >= 0 && startY < 8; startX += operation, startY -= operation)
	{
		if (!ChessBoard::chessBoardInstance->isSquareEmpty(startX, startY))
		{
			ChessBoard::chessBoardInstance->getSquare(startX, startY)->threatCount += mod;
			break;
		}
		else
		{
			ChessBoard::chessBoardInstance->getSquare(startX, startY)->threatCount += mod;
		}
	}
}

void Queen::queenAI()
{
	if (!tryToCheckmate())
	{
		if (!tryToCheck(BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY))
		{
			if (!narrowBlackKingArea())
			{
				runAway();
			}
		}
	}
}

bool Queen::tryToMoveSquare(int fX, int fY, int sX, int sY) // not used
{
	return false;
}

bool Queen::isHorizontalFree(int fX, int fY, int sX, int sY)
{
	int count;

	if (fX == sX)
	{
		count = fY > sY ? -1 : 1;

		for (int start = fY + count; start < sY; start += count)
		{
			if (!ChessBoard::chessBoardInstance->isSquareEmpty(fX, start))
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
	else if (fY == sY)
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

bool Queen::isDiagonalFree(int fX, int fY, int sX, int sY)
{
	if (sX < 0 || sX > 7 || sY < 0 || sY > 7)
	{
		return false;
	}

	int xDiff = fX - sX;
	int yDiff = fY - sY;

	if (abs(xDiff) != abs(yDiff))
	{
		return false;
	}

	for (int i = 0, x = fX, y = fY; abs(xDiff) - i != 0; i++)
	{
		x += xDiff > 0 ? -1 : 1;
		y += yDiff > 0 ? -1 : 1;

		Square* s = ChessBoard::chessBoardInstance->getSquare(x, y);

		if (!s->isSquareEmpty())
		{
			if(s->currChessPiece == BlackKing::blackKingInstance)
			{
				return true;
			}

			return false;
		}
	}

	return true;
}

bool Queen::tryToCheckmate()
{
	bool res = isHorizontalFree(pX, pY, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY)
		|| isDiagonalFree(pX, pY, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY);

	if (res)
	{
		moveTo(BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY);
		BlackKing::blackKingInstance->die();
	}

	return res;
}

bool Queen::tryToCheck(int targetX, int targetY)
{
	if(tryToCheckLikeBishop(targetX, targetY))
	{
		return true;
	}

	if (tryToCheckLikeRook(targetX, targetY))
	{
		return true;
	}

	return false;
}

bool Queen::tryToCheckLikeRook(int targetX, int targetY)
{
	int xDiff = pX - targetX;
	int yDiff = pY - targetY;

	if (abs(xDiff) > abs(yDiff))
	{
		if (isHorizontalFree(pX, pY, pX, pY - yDiff))
		{
			if (isHorizontalFree(pX, pY - yDiff, targetX, targetY))
			{
				moveTo(pX, pY - yDiff);
				return true;
			}
		}

		if (isHorizontalFree(pX, pY, pX - xDiff, pY))
		{
			if (isHorizontalFree(pX - xDiff, pY, targetX, targetY))
			{
				moveTo(pX - xDiff, pY);
				return true;
			}
		}
	}
	else
	{
		if (isHorizontalFree(pX, pY, pX - xDiff, pY))
		{
			if (isHorizontalFree(pX - xDiff, pY, targetX, targetY))
			{
				moveTo(pX - xDiff, pY);
				return true;
			}
		}

		if (isHorizontalFree(pX, pY, pX, pY - yDiff))
		{
			if (isHorizontalFree(pX, pY - yDiff, targetX, targetY))
			{
				moveTo(pX, pY - yDiff);
				return true;
			}
		}
	}

	return false;
}

bool Queen::tryToCheckLikeBishop(int targetX, int targetY)
{
	if (((pX + pY) + (targetX + targetY)) % 2 == 1)
	{
		return false;
	}

	int xDiff = pX - targetX;
	int yDiff = pY - targetY;

	int minDiff = 0;

	if (xDiff > 0)
	{
		// Right Area 

		if (abs(xDiff) > abs(yDiff))
		{
			minDiff = (abs(xDiff) - abs(yDiff)) / 2;

			if (yDiff > 0)
			{
				if (isDiagonalFree(pX, pY, pX - minDiff, pY + minDiff))
				{
					if (isDiagonalFree(pX - minDiff, pY + minDiff, targetX + 1, targetY + 1))
					{
						moveTo(pX - minDiff, pY + minDiff);
						return true;
					}
				}

				if (isDiagonalFree(pX, pY, targetX + minDiff, targetY - minDiff))
				{
					if (isDiagonalFree(targetX + minDiff, targetY - minDiff,
						targetX + 1, targetY - 1))
					{
						moveTo(targetX + minDiff, targetY - minDiff);
						return true;
					}
				}

				return false;
			}
			else
			{
				if (isDiagonalFree(pX, pY, pX - minDiff, pY - minDiff))
				{
					if (isDiagonalFree(pX - minDiff, pY - minDiff, targetX + 1, targetY - 1))
					{
						moveTo(pX - minDiff, pY - minDiff);
						return true;
					}
				}

				if (isDiagonalFree(pX, pY, targetX + minDiff, targetY + minDiff))
				{
					if (isDiagonalFree(targetX + minDiff, targetY + minDiff,
						targetX + 1, targetY + 1))
					{
						moveTo(targetX + minDiff, targetY + minDiff);
						return true;
					}
				}

				return false;
			}
		}
	}
	else if (xDiff < 0)
	{
		// Left Area

		if (abs(xDiff) > abs(yDiff))
		{
			minDiff = (abs(xDiff) - abs(yDiff)) / 2;

			if (yDiff > 0)
			{


				if (isDiagonalFree(pX, pY, pX + minDiff, pY + minDiff))
				{
					if (isDiagonalFree(pX + minDiff, pY + minDiff, targetX - 1, targetY + 1))
					{
						moveTo(pX + minDiff, pY + minDiff);
						return true;
					}
				}

				if (isDiagonalFree(pX, pY, targetX - minDiff, targetY - minDiff))
				{
					if (isDiagonalFree(targetX - minDiff, targetY - minDiff,
						targetX - 1, targetY - 1))
					{
						moveTo(targetX - minDiff, targetY - minDiff);
						return true;
					}
				}

				return false;
			}
			else
			{
				if (isDiagonalFree(pX, pY, pX + minDiff, pY - minDiff))
				{
					if (isDiagonalFree(pX + minDiff, pY - minDiff, targetX - 1, targetY - 1))
					{
						moveTo(pX + minDiff, pY - minDiff);
						return true;
					}
				}

				if (isDiagonalFree(pX, pY, targetX - minDiff, targetY + minDiff))
				{
					if (isDiagonalFree(targetX - minDiff, targetY + minDiff,
						targetX - 1, targetY + 1))
					{
						moveTo(targetX - minDiff, targetY + minDiff);
						return true;
					}
				}

				return false;
			}
		}
	}

	if (yDiff > 0)
	{
		// Down Area

		if (abs(yDiff) > abs(xDiff))
		{
			minDiff = (abs(yDiff) - abs(xDiff)) / 2;

			if (xDiff > 0)
			{
				if (isDiagonalFree(pX, pY, pX + minDiff, pY - minDiff))
				{
					if (isDiagonalFree(pX + minDiff, pY - minDiff, targetX + 1, targetY + 1))
					{
						moveTo(pX + minDiff, pY - minDiff);
						return true;
					}
				}

				if (isDiagonalFree(pX, pY, targetX - minDiff, targetY + minDiff))
				{
					if (isDiagonalFree(targetX - minDiff, targetY + minDiff,
						targetX - 1, targetY + 1))
					{
						moveTo(targetX - minDiff, targetY + minDiff);
						return true;
					}
				}

				return false;
			}
			else
			{
				if (isDiagonalFree(pX, pY, pX - minDiff, pY - minDiff))
				{
					if (isDiagonalFree(pX - minDiff, pY - minDiff, targetX - 1, targetY + 1))
					{
						moveTo(pX - minDiff, pY - minDiff);
						return true;
					}
				}

				if (isDiagonalFree(pX, pY, targetX + minDiff, targetY + minDiff))
				{
					if (isDiagonalFree(targetX + minDiff, targetY + minDiff,
						targetX + 1, targetY + 1))
					{
						moveTo(targetX + minDiff, targetY + minDiff);
						return true;
					}
				}

				return false;
			}
		}
	}
	else if (yDiff < 0)
	{
		// Up Area

		if (abs(yDiff) > abs(xDiff))
		{
			minDiff = (abs(yDiff) - abs(xDiff)) / 2;

			if (xDiff > 0)
			{
				if (isDiagonalFree(pX, pY, pX + minDiff, pY + minDiff))
				{
					if (isDiagonalFree(pX + minDiff, pY + minDiff, targetX + 1, targetY - 1))
					{
						moveTo(pX + minDiff, pY + minDiff);
						return true;
					}
				}

				if (isDiagonalFree(pX, pY, targetX - minDiff, targetY - minDiff))
				{
					if (isDiagonalFree(targetX - minDiff, targetY - minDiff,
						targetX - 1, targetY - 1))
					{
						moveTo(targetX - minDiff, targetY - minDiff);
						return true;
					}
				}

				return false;
			}
			else
			{
				if (isDiagonalFree(pX, pY, pX - minDiff, pY + minDiff))
				{
					if (isDiagonalFree(pX - minDiff, pY + minDiff, targetX - 1, targetY - 1))
					{
						moveTo(pX - minDiff, pY + minDiff);
						return true;
					}
				}

				if (isDiagonalFree(pX, pY, targetX + minDiff, targetY - minDiff))
				{
					if (isDiagonalFree(targetX + minDiff, targetY - minDiff,
						targetX + 1, targetY - 1))
					{
						moveTo(targetX + minDiff, targetY - minDiff);
						return true;
					}
				}

				return false;
			}
		}
	}
}

bool Queen::narrowBlackKingArea()
{
	if(narrowBlackKingAreaLikeBishop())
	{
		return true;
	}

	if(narrowBlackKingAreaLikeRook())
	{
		return true;
	}

	return false;
}

bool Queen::narrowBlackKingAreaLikeRook()
{
	if (tryToCheckLikeRook(BlackKing::blackKingInstance->pX + 1, BlackKing::blackKingInstance->pY + 1))
	{
		return true;
	}

	if (tryToCheckLikeRook(BlackKing::blackKingInstance->pX - 1, BlackKing::blackKingInstance->pY - 1))
	{
		return true;
	}

	if (tryToCheckLikeRook(BlackKing::blackKingInstance->pX - 1, BlackKing::blackKingInstance->pY + 1))
	{
		return true;
	}

	if (tryToCheckLikeRook(BlackKing::blackKingInstance->pX + 1, BlackKing::blackKingInstance->pY - 1))
	{
		return true;
	}

	return false;
}

bool Queen::narrowBlackKingAreaLikeBishop()
{
	if (BlackKing::blackKingInstance->pX > 3)
	{
		if (!tryToCheckLikeBishop(BlackKing::blackKingInstance->pX - 1, BlackKing::blackKingInstance->pY) && BlackKing::blackKingInstance->pX < 7)
		{
			return tryToCheckLikeBishop(BlackKing::blackKingInstance->pX + 1, BlackKing::blackKingInstance->pY);
		}
	}
	else
	{
		if (!tryToCheckLikeBishop(BlackKing::blackKingInstance->pX + 1, BlackKing::blackKingInstance->pY) && BlackKing::blackKingInstance->pX > 0)
		{
			return tryToCheckLikeBishop(BlackKing::blackKingInstance->pX - 1, BlackKing::blackKingInstance->pY);
		}
	}
}

void Queen::runAway()
{
	
}

void Queen::die()
{
	changeSquareThreatCount(-1);
	__super::die();
}
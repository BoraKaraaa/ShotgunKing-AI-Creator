#include "Bishop.h"

#include "BlackKing.h"

#include<iostream>

void Bishop::_register_methods()
{
	register_method((char*)"_init", &Bishop::_init);
}

void Bishop::_init()
{

}

void Bishop::takeTurn()
{
	if (--currTurnCount <= 0)
	{
		bishopAI();
		currTurnCount = TurnCount;
	}
}

void Bishop::moveTo(int x, int y)
{
	__super::moveTo(x,y);
}

void Bishop::changeSquareThreatCount(int mod)
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

void Bishop::bishopAI()
{
	// First Check Diagonals for Killing BlackKing

	if(!tryToCheckmate()) // Try to checkmate
	{
		if (!checkDiagonalsForCheck(BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY)) // Try to check the BlackKing
		{
			if (!narrowBlackKingArea()) // Try to narrow BlackKing's area 
			{
				runAway();
			}
		}
	}


}

bool Bishop::isDiagonalFree(int fX, int fY, int sX, int sY)
{
	if(sX < 0 || sX > 7 || sY < 0 || sY > 7)
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
			return false;
		}
	}

	return true;
}

bool Bishop::tryToCheckmate()
{
	int xDiff = pX - BlackKing::blackKingInstance->pX;
	int yDiff = pY - BlackKing::blackKingInstance->pY;

	if(abs(xDiff) != abs(yDiff))
	{
		return false;
	}

	for (int i = 0, x = pX, y = pY; abs(xDiff)-i != 0; i++)
	{
		x += xDiff > 0 ? -1 : 1;
		y += yDiff > 0 ? -1 : 1;

		Square* s = ChessBoard::chessBoardInstance->getSquare(x, y);

		if (!s->isSquareEmpty())
		{
			if (s->currChessPiece == BlackKing::blackKingInstance)
			{
				moveTo(x, y);
				BlackKing::blackKingInstance->die();
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}

void Bishop::runAway()
{
	int x1 = pX, y1 = pY, x2 = pX, y2 = pY, x3 = pX, y3 = pY, x4 = pX, y4 = pY;
	float g1 = 0, g2 = 0, g3 = 0, g4 = 0;
	bool lock1 = false, lock2 = false, lock3 = false, lock4 = false;
	bool diffOccur = false;

	float holder = 0;

	float max = 0;

	for(int i = 1; i < 7; i++)
	{
		if(!lock1 && pX+i <= 7 && pY+i <= 7 && ChessBoard::chessBoardInstance->isSquareEmpty(pX+i, pY+i) 
			&& ChessBoard::chessBoardInstance->twoSquareDistance(pX+i, pY+i, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY) >=
			ChessBoard::chessBoardInstance->twoSquareDistance(pX, pY, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY))
		{
			diffOccur = true;

			holder = ChessBoard::chessBoardInstance->twoSquareDistance(pX + i, pY + i, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY);

			if (max < holder)
			{
				g1 = holder;
				max = g1;

				x1 = pX + i;
				y1 = pY + i;
			}

		}
		else
		{
			lock1 = true;
		}

		if (!lock2 && pX-i >= 0 && pY+i <= 7 && ChessBoard::chessBoardInstance->isSquareEmpty(pX - i, pY + i)
			&& ChessBoard::chessBoardInstance->twoSquareDistance(pX - i, pY + i, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY) >=
			ChessBoard::chessBoardInstance->twoSquareDistance(pX, pY, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY))
		{
			diffOccur = true;
			 
			holder = ChessBoard::chessBoardInstance->twoSquareDistance(pX - i, pY + i, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY);

			if (max < holder)
			{
				g2 = holder;
				max = g2;

				x2 = pX - i;
				y2 = pY + i;
			}

		}
		else
		{
			lock2 = true;
		}

		if (!lock3 && pX + i <= 7 && pY - i >= 0 && ChessBoard::chessBoardInstance->isSquareEmpty(pX + i, pY - i)
			&& ChessBoard::chessBoardInstance->twoSquareDistance(pX + i, pY - i, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY) >=
			ChessBoard::chessBoardInstance->twoSquareDistance(pX, pY, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY))
		{
			diffOccur = true;
			
			holder = ChessBoard::chessBoardInstance->twoSquareDistance(pX + i, pY - i, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY);

			if (max < holder)
			{
				g3 = holder;
				max = g3;

				x3 = pX + i;
				y3 = pY - i;
			}

		}
		else
		{
			lock3 = true;
		}

		if (!lock4 && pX - i >= 0 && pY - i >= 0 && ChessBoard::chessBoardInstance->isSquareEmpty(pX - i, pY - i)
			&& ChessBoard::chessBoardInstance->twoSquareDistance(pX - i, pY - i, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY) >=
			ChessBoard::chessBoardInstance->twoSquareDistance(pX, pY, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY))
		{
			diffOccur = true;

			holder = ChessBoard::chessBoardInstance->twoSquareDistance(pX - i, pY - i, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY);

			if (max < holder)
			{
				g4 = holder;
				max = g4;

				x4 = pX - i;
				y4 = pY - i;
			}

		}
		else
		{
			lock4 = true;
		}

		if(!diffOccur)
		{
			break;
		}
		else
		{
			diffOccur = false;
		}
	}

	if(max == 0)
	{
		return;
	}

	if(max == g1)
	{
		moveTo(x1, y1);
	}
	else if(max == g2)
	{
		moveTo(x2, y2);
	}
	else if(max == g3)
	{
		moveTo(x3, y3);
	}
	else if(max == g4)
	{
		moveTo(x4, y4);
	}

}

bool Bishop::narrowBlackKingArea()
{
	if(BlackKing::blackKingInstance->pX > 3)
	{
		if(!checkDiagonalsForCheck(BlackKing::blackKingInstance->pX - 1, BlackKing::blackKingInstance->pY) && BlackKing::blackKingInstance->pX < 7)
		{
			return checkDiagonalsForCheck(BlackKing::blackKingInstance->pX + 1, BlackKing::blackKingInstance->pY);
		}
	}
	else
	{
		if(!checkDiagonalsForCheck(BlackKing::blackKingInstance->pX + 1, BlackKing::blackKingInstance->pY) && BlackKing::blackKingInstance->pX > 0)
		{
			return checkDiagonalsForCheck(BlackKing::blackKingInstance->pX - 1, BlackKing::blackKingInstance->pY);
		}
	}
}

bool Bishop::checkDiagonalsForCheck(int targetX, int targetY)
{
	if( ((pX+pY) + (targetX + targetY)) % 2 == 1 )
	{
		return false;
	}

	int xDiff = pX - targetX;
	int yDiff = pY - targetY;

	int minDiff = 0;

	if(xDiff > 0)
	{
		// Right Area 

		if(abs(xDiff) > abs(yDiff))
		{
			minDiff = (abs(xDiff) - abs(yDiff)) / 2;

			if(yDiff > 0)
			{
				if(isDiagonalFree(pX, pY, pX - minDiff, pY + minDiff))
				{
					if(isDiagonalFree(pX - minDiff, pY + minDiff, targetX+1, targetY+1))
					{
						moveTo(pX - minDiff, pY + minDiff);
						return true;
					}
				}
				
				if (isDiagonalFree(pX, pY, targetX + minDiff, targetY - minDiff))
				{
					if (isDiagonalFree(targetX + minDiff, targetY - minDiff,
						targetX +1, targetY -1))
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
					if (isDiagonalFree(pX - minDiff, pY - minDiff, targetX+1, targetY -1))
					{
						moveTo(pX - minDiff, pY - minDiff);
						return true;
					}
				}

				if (isDiagonalFree(pX, pY, targetX + minDiff, targetY + minDiff))
				{
					if (isDiagonalFree(targetX + minDiff, targetY + minDiff,
						targetX+1, targetY +1))
					{
						moveTo(targetX + minDiff, targetY + minDiff);
						return true;
					}
				}

				return false;
			}
		}
	}
	else if(xDiff < 0)
	{
		// Left Area

		if(abs(xDiff) > abs(yDiff))
		{
			minDiff = (abs(xDiff) - abs(yDiff)) / 2;

			if (yDiff > 0)
			{


				if (isDiagonalFree(pX, pY, pX + minDiff, pY + minDiff))
				{
					if (isDiagonalFree(pX + minDiff, pY + minDiff, targetX -1, targetY +1))
					{
						moveTo(pX + minDiff, pY + minDiff);
						return true;
					}
				}

				if (isDiagonalFree(pX, pY, targetX - minDiff, targetY - minDiff))
				{
					if (isDiagonalFree(targetX - minDiff, targetY - minDiff,
						targetX -1, targetY -1))
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
					if (isDiagonalFree(pX + minDiff, pY - minDiff, targetX -1, targetY -1))
					{
						moveTo(pX + minDiff, pY - minDiff);
						return true;
					}
				}

				if (isDiagonalFree(pX, pY, targetX - minDiff, targetY + minDiff))
				{
					if (isDiagonalFree(targetX - minDiff, targetY + minDiff,
						targetX -1, targetY +1))
					{
						moveTo(targetX - minDiff, targetY + minDiff);
						return true;
					}
				}

				return false;
			}
		}
	}

	if(yDiff > 0)
	{
		// Down Area

		if (abs(yDiff) > abs(xDiff))
		{
			minDiff = (abs(yDiff) - abs(xDiff)) / 2;

			if (xDiff > 0)
			{
				if (isDiagonalFree(pX, pY, pX + minDiff, pY - minDiff))
				{
					if (isDiagonalFree(pX + minDiff, pY - minDiff, targetX+1, targetY +1))
					{
						moveTo(pX + minDiff, pY - minDiff);
						return true;
					}
				}

				if (isDiagonalFree(pX, pY, targetX - minDiff, targetY + minDiff))
				{
					if (isDiagonalFree(targetX - minDiff, targetY + minDiff,
						targetX -1, targetY +1))
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
					if (isDiagonalFree(pX - minDiff, pY - minDiff, targetX -1, targetY +1))
					{
						moveTo(pX - minDiff, pY - minDiff);
						return true;
					}
				}

				if (isDiagonalFree(pX, pY, targetX + minDiff, targetY + minDiff))
				{
					if (isDiagonalFree(targetX + minDiff, targetY + minDiff,
						targetX +1, targetY +1))
					{
						moveTo(targetX + minDiff, targetY + minDiff);
						return true;
					}
				}

				return false;
			}
		}
	}
	else if(yDiff < 0)
	{
		// Up Area

		if (abs(yDiff) > abs(xDiff))
		{
			minDiff = (abs(yDiff) - abs(xDiff)) / 2;

			if (xDiff > 0)
			{
				if (isDiagonalFree(pX, pY, pX + minDiff, pY + minDiff))
				{
					if (isDiagonalFree(pX + minDiff, pY + minDiff, targetX+1, targetY-1))
					{
						moveTo(pX + minDiff, pY + minDiff);
						return true;
					}
				}

				if (isDiagonalFree(pX, pY, targetX - minDiff, targetY - minDiff))
				{
					if (isDiagonalFree(targetX - minDiff, targetY - minDiff,
						targetX -1, targetY -1))
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
					if (isDiagonalFree(pX - minDiff, pY + minDiff, targetX -1, targetY-1))
					{
						moveTo(pX - minDiff, pY + minDiff);
						return true;
					}
				}

				if (isDiagonalFree(pX, pY, targetX + minDiff, targetY - minDiff))
				{
					if (isDiagonalFree(targetX + minDiff, targetY - minDiff,
						targetX+1, targetY -1))
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

void Bishop::die()
{
	changeSquareThreatCount(-1);
	__super::die();
}

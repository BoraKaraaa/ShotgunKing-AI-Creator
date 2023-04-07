#include "WhiteKing.h"

#include "BlackKing.h"

WhiteKing* WhiteKing::whiteKingInstance = NULL;

void WhiteKing::_register_methods()
{
	register_method((char*)"_init", &WhiteKing::_init);
}

void WhiteKing::_init()
{
	whiteKingInstance = this;
}

void WhiteKing::takeTurn()
{
	if (--currTurnCount <= 0)
	{
		whiteKingAI();
		currTurnCount = TurnCount;
	}
}

void WhiteKing::moveTo(int x, int y)
{
	__super::moveTo(x, y);
}

void WhiteKing::changeSquareThreatCount(int mod)
{
	if(ChessBoard::chessBoardInstance->isSquareValid(pX+1, pY+1)) 
		ChessBoard::chessBoardInstance->getSquare(pX + 1, pY + 1)->threatCount += mod;
	if (ChessBoard::chessBoardInstance->isSquareValid(pX + 1, pY - 1))
		ChessBoard::chessBoardInstance->getSquare(pX + 1, pY - 1)->threatCount += mod;
	if (ChessBoard::chessBoardInstance->isSquareValid(pX - 1, pY - 1))
		ChessBoard::chessBoardInstance->getSquare(pX - 1, pY - 1)->threatCount += mod;
	if (ChessBoard::chessBoardInstance->isSquareValid(pX - 1, pY + 1))
		ChessBoard::chessBoardInstance->getSquare(pX - 1, pY + 1)->threatCount += mod;

	if (ChessBoard::chessBoardInstance->isSquareValid(pX, pY + 1))
		ChessBoard::chessBoardInstance->getSquare(pX, pY + 1)->threatCount += mod;
	if (ChessBoard::chessBoardInstance->isSquareValid(pX, pY - 1))
		ChessBoard::chessBoardInstance->getSquare(pX, pY - 1)->threatCount += mod;

	if (ChessBoard::chessBoardInstance->isSquareValid(pX + 1, pY))
		ChessBoard::chessBoardInstance->getSquare(pX + 1, pY)->threatCount += mod;
	if (ChessBoard::chessBoardInstance->isSquareValid(pX - 1, pY))
		ChessBoard::chessBoardInstance->getSquare(pX - 1, pY)->threatCount += mod;
}

void WhiteKing::whiteKingAI()
{
	runAway();
}

void WhiteKing::runAway()
{
	int x1 = pX, y1 = pY, x2 = pX, y2 = pY, x3 = pX, y3 = pY, x4 = pX, y4 = pY;
	float g1 = 0, g2 = 0, g3 = 0, g4 = 0;

	float holder = 0;

	float max = 0;

	for (int i = 1, j = 1, k = 1; k < 3; k++)
	{
		if(k == 2)
		{
			i = 0;
		}

		if (pX + i <= 7 && pY + j <= 7 && ChessBoard::chessBoardInstance->isSquareEmpty(pX + i, pY + j)
			&& ChessBoard::chessBoardInstance->twoSquareDistance(pX + i, pY + j, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY) >=
			ChessBoard::chessBoardInstance->twoSquareDistance(pX, pY, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY))
		{
			holder = ChessBoard::chessBoardInstance->twoSquareDistance(pX + i, pY + j, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY);

			if (max < holder)
			{
				g1 = holder;
				max = g1;

				x1 = pX + i;
				y1 = pY + j;
			}

		}


		if (pX - i >= 0 && pY + j <= 7 && ChessBoard::chessBoardInstance->isSquareEmpty(pX - i, pY + j)
			&& ChessBoard::chessBoardInstance->twoSquareDistance(pX - i, pY + j, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY) >=
			ChessBoard::chessBoardInstance->twoSquareDistance(pX, pY, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY))
		{
			holder = ChessBoard::chessBoardInstance->twoSquareDistance(pX - i, pY + j, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY);

			if (max < holder)
			{
				g2 = holder;
				max = g2;

				x2 = pX - i;
				y2 = pY + j;
			}
		}

		if(k == 2)
		{
			i = 1;
			j = 0;
		}

		if (pX + i <= 7 && pY - j >= 0 && ChessBoard::chessBoardInstance->isSquareEmpty(pX + i, pY - j)
			&& ChessBoard::chessBoardInstance->twoSquareDistance(pX + i, pY - j, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY) >=
			ChessBoard::chessBoardInstance->twoSquareDistance(pX, pY, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY))
		{
			holder = ChessBoard::chessBoardInstance->twoSquareDistance(pX + i, pY - j, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY);

			if (max < holder)
			{
				g3 = holder;
				max = g3;

				x3 = pX + i;
				y3 = pY - j;
			}

		}

		if (pX - i >= 0 && pY - j >= 0 && ChessBoard::chessBoardInstance->isSquareEmpty(pX - i, pY - j)
			&& ChessBoard::chessBoardInstance->twoSquareDistance(pX - i, pY - j, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY) >=
			ChessBoard::chessBoardInstance->twoSquareDistance(pX, pY, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY))
		{
			holder = ChessBoard::chessBoardInstance->twoSquareDistance(pX - i, pY - j, BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY);

			if (max < holder)
			{
				g4 = holder;
				max = g4;

				x4 = pX - i;
				y4 = pY - j;
			}

		}

	}

	if (max == 0)
	{
		return;
	}

	if (max == g1)
	{
		moveTo(x1, y1);
	}
	else if (max == g2)
	{
		moveTo(x2, y2);
	}
	else if (max == g3)
	{
		moveTo(x3, y3);
	}
	else if(max == g4)
	{
		moveTo(x4, y4);
	}

}

void WhiteKing::die()
{
	Godot::print("YOU WIN THE GAME");

	TurnController::turnControllerInstance->stopTurn();

	//changeSquareThreatCount(-1);
	__super::die();
}
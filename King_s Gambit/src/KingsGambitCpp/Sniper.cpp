#include "Sniper.h"

#include "ChessBoard.h"
#include "BlackKing.h"

Sniper::Sniper(int areaW)
{
	areaWidth = areaW;
}

Sniper::~Sniper()
{

}

bool Sniper::tryToHitPiece(int x, int y)
{
	return areaWidth >= ChessBoard::chessBoardInstance->twoSquareDistanceWithSquare
	(BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY, x, y);
}
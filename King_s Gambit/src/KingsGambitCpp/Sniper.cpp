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

void Sniper::searchHittableArea()
{
	hittablePieces.clear();

	for (ChessPiece* whitePiece : TurnController::turnControllerInstance->whitePieces)
	{
		if(areaWidth >= ChessBoard::chessBoardInstance->twoSquareDistanceWithSquare
			(BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY, whitePiece->pX, whitePiece->pY))
		{
			hittablePieces.push_back(whitePiece);
		}
	}
}

void Sniper::createAllPossibleHitNodes(Snapshot* currSS)
{
	searchHittableArea();
	__super::createAllPossibleHitNodes(currSS);
}

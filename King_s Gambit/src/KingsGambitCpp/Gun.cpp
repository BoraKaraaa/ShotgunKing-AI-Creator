#include "Gun.h"

#include "ChessBoard.h"

#include "BlackKing.h"
#include "WhiteKing.h"


Gun::Gun() {}

Gun::~Gun() {}

void Gun::hitPiece(ChessPiece* chessPiece)
{
	//CameraNoise::cameraNoiseInstance->doCameraShake(120, 0.3, 120);

	chessPiece->takeDamage(gunDamage);
}

void Gun::createAllPossibleHitNodes(Snapshot* currSS)
{
	/*
	if(ChessBoard::chessBoardInstance->getSquare(BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY)->threatCount > 0)
	{
		// Threat Count calculations
		for (ChessPiece* chessPiece : hittablePieces)
		{
			if(chessPiece == WhiteKing::whiteKingInstance)
			{
				if(chessPiece->canKill(gunDamage))
				{
					createHitSnapshots(currSS, chessPiece);
					break;
				}
			}

			chessPiece->changeSquareThreatCount(-1);

			// add extra check for killing WhiteKing with shot
			if(ChessBoard::chessBoardInstance->getSquare(BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY)->threatCount <= 0)
			{
				chessPiece->changeSquareThreatCount(1);
				createHitSnapshots(currSS, chessPiece);
				break;
			}
				
			chessPiece->changeSquareThreatCount(1);
		}
	}
	else
	{
		for(ChessPiece* chessPiece : hittablePieces)
		{
			createHitSnapshots(currSS, chessPiece);
		}
	}
	*/
	
	for (ChessPiece* chessPiece : hittablePieces)
	{
		createHitSnapshots(currSS, chessPiece);
	}
	
}

void Gun::createHitSnapshots(Snapshot* currSS, ChessPiece* chessPiece)
{
	Snapshot newSnapshot;

	newSnapshot.parentSS = (currSS == NULL) ? NULL : currSS;

	newSnapshot.isMoving = false;

	newSnapshot.hitPiece = chessPiece;

	newSnapshot.chessPiecePositionDictioanry[BlackKing::blackKingInstance] = Vector2(BlackKing::blackKingInstance->pX, BlackKing::blackKingInstance->pY);

	for (ChessPiece* chessPiece : TurnController::turnControllerInstance->whitePieces)
	{
		newSnapshot.chessPiecePositionDictioanry[chessPiece] = Vector2(chessPiece->pX, chessPiece->pY);
	}

	newSnapshot.totalMoveCount = BlackKing::blackKingInstance->counter + 1;
	newSnapshot.heuristicValue = BlackKing::blackKingInstance->heuristicStartingValue + chessPiece->hitHeuristicCalculator(gunDamage);
	newSnapshot.heuristicValue -= BlackKing::blackKingInstance->eachMoveMinusValue;

	BlackKing::blackKingInstance->snapshotQueue.push(newSnapshot);
}

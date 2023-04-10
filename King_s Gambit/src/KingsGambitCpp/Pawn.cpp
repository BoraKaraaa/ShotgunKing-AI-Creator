#include "Pawn.h"

#include "BlackKing.h"
#include <string>

void Pawn::_register_methods()
{
	register_method((char*)"_init", &Pawn::_init);
	register_method((char*)"_ready", &Pawn::_ready);
}

void Pawn::_init()
{
	__super::_init();
	queenScene = ResourceLoader::get_singleton()->load("res://Nodes/Pieces/Queen.tscn");
}

void Pawn::_ready()
{
	pieceHolder = (Node2D*)get_node("/root/MainScene/PieceHolder");
}

void Pawn::takeTurn()
{
	if(--currTurnCount <= 0)
	{
		pawnAI();
		currTurnCount = TurnCount;
	}
}

void Pawn::moveTo(int x, int y)
{
	__super::moveTo(x, y);
}

void Pawn::changeSquareThreatCount(int mod)
{
	if(ChessBoard::chessBoardInstance->isSquareValid(pX-1, pY+1))
	{
		ChessBoard::chessBoardInstance->getSquare(pX - 1, pY + 1)->threatCount += mod;
	}
	
	if(ChessBoard::chessBoardInstance->isSquareValid(pX+1, pY+1))
	{
		ChessBoard::chessBoardInstance->getSquare(pX + 1, pY + 1)->threatCount += mod;
	}
	
}

void Pawn::pawnAI()
{
	// First Check for killing BlackKing
	if( BlackKing::blackKingInstance->pX == pX - 1  && BlackKing::blackKingInstance->pY == pY + 1 )
	{
		moveTo(pX - 1, pY + 1);
		BlackKing::blackKingInstance->die();
		return;
	}

	if( BlackKing::blackKingInstance->pX == pX + 1 && BlackKing::blackKingInstance->pY == pY + 1 )
	{
		moveTo(pX + 1, pY + 1);
		BlackKing::blackKingInstance->die();
		return;
	}

	// First move's length can be 2 ( pY == 1 check for pawn hasnt moved )
	if(pY == 1 && ChessBoard::chessBoardInstance->isSquareEmpty(pX, pY+2))
	{
		moveTo(pX, pY + 2);
	}
	else if( ChessBoard::chessBoardInstance->isSquareEmpty(pX, pY+1) )
	{
		moveTo(pX, pY + 1);
	}

	// Pawn Promotion
	if(pY == 7)
	{

		// Queen Parameters is not true
		spawnQueenToPosition(pX, pY);

		// destroy the pawn
		TurnController::turnControllerInstance->deleteWhitePiece(this);

		isDied = true;
		set_visible(false);
		//get_parent()->remove_child(this);
	}

}


void Pawn::spawnQueenToPosition(int x, int y)
{
	KinematicBody2D* queenBody = (KinematicBody2D*)queenScene->instance();
	ChessPiece* queenSC = Object::cast_to<ChessPiece>(queenBody);
	queenSC->assignInitialDirections(x, y);
	pieceHolder->add_child(queenBody);

	TurnController::turnControllerInstance->setCreatedWhitePieces(queenSC);
}

void Pawn::die()
{
	changeSquareThreatCount(-1);
	__super::die();
}
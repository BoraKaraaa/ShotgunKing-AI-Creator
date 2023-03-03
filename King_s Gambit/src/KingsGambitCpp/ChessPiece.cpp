#include "ChessPiece.h"
#include "ChessBoard.h"

#include "TurnController.h"

void ChessPiece::_register_methods()
{
	register_method((char*)"_init", &ChessPiece::_init);
	register_method((char*)"_ready", &ChessPiece::_ready);

	register_method((char*)"moveTweenCompleted", &ChessPiece::moveTweenCompleted);

	register_property<ChessPiece, int>((char*)"TurnCount", &ChessPiece::TurnCount, 0);
	register_property<ChessPiece, int>((char*)"HealthAmount", &ChessPiece::HealthAmount, 0);
	register_property<ChessPiece, int>((char*)"moveDuration", &ChessPiece::moveDuration, 0);
}

void ChessPiece::_init()
{
	currTurnCount = TurnCount;
}

void ChessPiece::_ready()
{
	const godot::String tweenName = "Tween";
	Node* n;
	godot::Array array = get_children();

	int64_t childCount = get_child_count();

	for(int64_t x = 0; x < childCount; x++)
	{
		n = get_child(x);

		if(tweenName == n->get_name())
		{
			tween = (Tween*)n;
		}
	}
}

void ChessPiece::assignInitialDirections(int x, int y) 
{
	pX = x;
	pY = y;

	set_position(ChessBoard::chessBoardInstance->getSquarePivotPosition(pX, pY));
	ChessBoard::chessBoardInstance->setChessPieceToSquare(this, pX, pY);
}

void ChessPiece::takeTurn() { }

void ChessPiece::moveTo(int x, int y) 
{
	tween->connect("tween_completed", this, "moveTweenCompleted");
	tween->interpolate_property(this, "position", get_position(), ChessBoard::chessBoardInstance->getSquarePivotPosition(x, y), moveDuration, 10, 1);
	tween->start();

	ChessBoard::chessBoardInstance->getSquare(pX, pY)->setPieceToSquare(nullptr);
	ChessBoard::chessBoardInstance->getSquare(x, y)->setPieceToSquare(this);

	pX = x;
	pY = y;
}

void ChessPiece::moveTweenCompleted()
{
	tween->disconnect("tween_completed", this, "moveTweenCompleted");
}

void ChessPiece::takeDamage()
{
	//TODO: Add Visual Effect

	if(--HealthAmount <= 0)
	{
		die();
	}
}

void ChessPiece::die()
{
	TurnController::turnControllerInstance->deleteWhitePiece(this);
	get_parent()->remove_child(this);
}



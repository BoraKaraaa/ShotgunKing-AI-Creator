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

	register_property<ChessPiece, int>((char*)"takeDamageHeuristicValue", &ChessPiece::takeDamageHeuristicValue, 0);
	register_property<ChessPiece, int>((char*)"dieHeuristicValue", &ChessPiece::dieHeuristicValue, 0);
}

void ChessPiece::_init()
{
	currTurnCount = TurnCount;
	isDied = false;
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

	deadParticleScene = ResourceLoader::get_singleton()->load("res://Nodes/DeadParticle.tscn");
}

void ChessPiece::assignInitialDirections(int x, int y) 
{
	pX = x;
	pY = y;

	set_position(ChessBoard::chessBoardInstance->getSquarePivotPosition(pX, pY));
	ChessBoard::chessBoardInstance->setChessPieceToSquare(this, pX, pY);
}

void ChessPiece::takeTurn() 
{
	if(currTurnCount == 1)
	{
		lastOneCountToPlay();
	}
}

void ChessPiece::lastOneCountToPlay()
{
	
}

void ChessPiece::moveTo(int x, int y) 
{
	tween->connect("tween_completed", this, "moveTweenCompleted");
	tween->interpolate_property(this, "position", get_position(), ChessBoard::chessBoardInstance->getSquarePivotPosition(x, y), 0.002, 10, 1);
	tween->start();

	ChessBoard::chessBoardInstance->getSquare(pX, pY)->setPieceToSquare(nullptr);
	ChessBoard::chessBoardInstance->getSquare(x, y)->setPieceToSquare(this);

	pX = x;
	pY = y;
}

void ChessPiece::moveImmediately(int x, int y)
{
	set_position(ChessBoard::chessBoardInstance->getSquarePivotPosition(x,y));

	ChessBoard::chessBoardInstance->getSquare(pX, pY)->setPieceToSquare(nullptr);
	ChessBoard::chessBoardInstance->getSquare(x, y)->setPieceToSquare(this);

	pX = x;
	pY = y;
}

void ChessPiece::moveTweenCompleted()
{
	tween->disconnect("tween_completed", this, "moveTweenCompleted");
}

void ChessPiece::changeSquareThreatCount(int mod) // mod = 1 for add mod = -1 for subtract
{

}

void ChessPiece::takeDamage(int damageAmount)
{
	//TODO: Add Visual Effect

	HealthAmount -= damageAmount;

	if(HealthAmount <= 0)
	{
		die();
	}
}

void ChessPiece::die()
{
	//playDeadParticle();

	TurnController::turnControllerInstance->deleteWhitePiece(this);

	//get_parent()->remove_child(this);

	isDied = true;
	set_visible(false);
}

bool ChessPiece::canKill(int damageAmount)
{
	return HealthAmount <= damageAmount;
}

void ChessPiece::setParameters(int healthA, int turnC, int takeDamageH, int dieH)
{
	if(healthA != -1)
		HealthAmount = healthA;
	if(turnC != -1)
		TurnCount = turnC;
	if(takeDamageH != -1)
		takeDamageHeuristicValue = takeDamageH;
	if(dieH != -1)
		dieHeuristicValue = dieH;
}

int ChessPiece::hitHeuristicCalculator(int damageAmount)
{
	if(HealthAmount > damageAmount)
	{
		return takeDamageHeuristicValue;
	}
	else
	{
		return dieHeuristicValue;
	}
}

void ChessPiece::playDeadParticle()
{
	Particles2D* deadParticle = (Particles2D*)deadParticleScene->instance();
	get_parent()->add_child(deadParticle);
	deadParticle->set_position(ChessBoard::chessBoardInstance->getSquarePivotPosition(pX, pY));
	deadParticle->set_emitting(true);
}

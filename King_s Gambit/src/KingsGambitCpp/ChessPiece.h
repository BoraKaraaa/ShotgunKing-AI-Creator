#pragma once
#include <Godot.hpp>
#include <KinematicBody2D.hpp>
#include <Tween.hpp>

#include "ChessPieceType.h"

using namespace godot;

class ChessPiece : public KinematicBody2D
{
	GODOT_CLASS(ChessPiece, KinematicBody2D);

public:
	int HealthAmount;
	int TurnCount;

	int currTurnCount;

	int moveDuration;

	int pX;
	int pY;

	Tween* tween;

	static void _register_methods();
	void _init();
	void _ready();

	virtual void assignInitialDirections(int, int);
	virtual void takeTurn();
	virtual void moveTo(int, int);
	virtual void moveTweenCompleted();
	virtual void takeDamage();
	virtual void die();

private:
	ChessPieceType chessPieceType;
};


#pragma once
#include <Godot.hpp>
#include <KinematicBody2D.hpp>

#include "ChessPieceType.h"


using namespace godot;

class ChessPiece : public KinematicBody2D
{
	GODOT_CLASS(ChessPiece, KinematicBody2D);

public:
	int HealthAmount;
	int TurnCount;

	static void _register_methods();
	void _init();

	virtual void takeTurn() = 0;
	virtual void moveTo(int, int) = 0;

private:
	ChessPieceType chessPieceType;

};


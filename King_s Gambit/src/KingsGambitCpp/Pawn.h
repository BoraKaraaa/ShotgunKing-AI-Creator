#pragma once
#include "ChessPiece.h"


class Pawn : public ChessPiece
{
	GODOT_CLASS(Pawn, ChessPiece);

public:
	static void _register_methods();
	void _init();

	void takeTurn() override;
	void moveTo(int, int) override;
};


#pragma once
#include "ChessPiece.h"

class WhiteKing : public ChessPiece
{
	GODOT_CLASS(WhiteKing, ChessPiece);

public:
	static void _register_methods();
	void _init();

	void takeTurn() override;
	void moveTo(int, int) override;
};


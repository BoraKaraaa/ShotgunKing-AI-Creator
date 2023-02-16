#pragma once
#include "ChessPiece.h"

class BlackKing : public ChessPiece
{
	GODOT_CLASS(BlackKing, ChessPiece);

public:
	static void _register_methods();
	void _init();

	void takeTurn() override;
	void moveTo(int, int) override;

};


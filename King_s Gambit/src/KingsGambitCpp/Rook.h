#pragma once
#include "ChessPiece.h"

class Rook : public ChessPiece
{
	GODOT_CLASS(Rook, ChessPiece);

public:
	static void _register_methods();
	void _init();

	void takeTurn() override;
	void moveTo(int, int) override;
};


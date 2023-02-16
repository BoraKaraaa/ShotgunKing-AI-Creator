#pragma once
#include "ChessPiece.h"

class Knight : public ChessPiece
{
	GODOT_CLASS(Knight, ChessPiece);

public:
	static void _register_methods();
	void _init();

	void takeTurn() override;
	void moveTo(int, int) override;
};


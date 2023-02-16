#pragma once
#include "ChessPiece.h"

class Queen : public ChessPiece
{
	GODOT_CLASS(Queen, ChessPiece);

public:
	static void _register_methods();
	void _init();

	void takeTurn() override;
	void moveTo(int, int) override;
};


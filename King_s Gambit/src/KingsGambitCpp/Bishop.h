#pragma once
#include "ChessPiece.h"

class Bishop : public ChessPiece
{
	GODOT_CLASS(Bishop, ChessPiece);

public:
	static void _register_methods();
	void _init();

	void takeTurn() override;
	void moveTo(int, int) override;

};


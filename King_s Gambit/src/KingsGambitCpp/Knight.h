#pragma once
#include "ChessPiece.h"
#include "Square.h"

class Knight : public ChessPiece
{
	GODOT_CLASS(Knight, ChessPiece);

public:
	static void _register_methods();
	void _init();

	void takeTurn() override;
	void moveTo(int, int) override;

private:
	void knightAI();
	bool tryToCheckmate();
	bool tryToCheck();

	Square** freeSquares;

};


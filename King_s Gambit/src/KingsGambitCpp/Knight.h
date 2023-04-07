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

	void changeSquareThreatCount(int) override;

	void die() override;

private:
	void knightAI();
	bool tryToMoveSquare(int, int, int, int);

	bool tryToCheckmate();
	bool tryToCheck(int, int);

	bool narrowBlackKingArea();

};


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

	void changeSquareThreatCount(int) override;

	void die() override;

private:
	void rookAI();

	bool tryToMoveSquare(int, int, int, int);

	bool tryToCheckmate();
	bool tryToCheck(int, int);
	bool narrowBlackKingArea();
	void runAway();
};


#pragma once
#include "ChessPiece.h"

#include "ChessBoard.h"

class Bishop : public ChessPiece
{
	GODOT_CLASS(Bishop, ChessPiece);

public:
	static void _register_methods();
	void _init();

	void takeTurn() override;
	void moveTo(int, int) override;
private:
	void bishopAI();
	bool tryToCheckmate();
	bool checkDiagonalsForCheck(int targetX, int targetY);

	bool narrowBlackKingArea();

	void runAway();

	bool isDiagonalFree(int, int, int, int);
};


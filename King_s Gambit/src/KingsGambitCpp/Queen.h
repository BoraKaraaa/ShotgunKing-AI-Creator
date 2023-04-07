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

	void changeSquareThreatCount(int) override;

	void die() override;

private:

	bool isHFree;
	bool isDFree;

	void queenAI();

	bool tryToMoveSquare(int, int, int, int);

	bool isDiagonalFree(int, int, int, int);
	bool isHorizontalFree(int, int, int, int);

	bool tryToCheckmate();

	bool tryToCheck(int, int);
	bool tryToCheckLikeRook(int, int);
	bool tryToCheckLikeBishop(int, int);

	bool narrowBlackKingArea();
	bool narrowBlackKingAreaLikeRook();
	bool narrowBlackKingAreaLikeBishop();

	void changeSquareThreatCountLikeRook(int);
	void changeSquareThreatCountLikeBishop(int);

	void runAway();
};


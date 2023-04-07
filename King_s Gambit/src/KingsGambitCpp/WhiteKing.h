#pragma once
#include "ChessPiece.h"

#include "ChessBoard.h"

class WhiteKing : public ChessPiece
{
	GODOT_CLASS(WhiteKing, ChessPiece);

public:
	static WhiteKing* whiteKingInstance;

	static void _register_methods();
	void _init();

	void takeTurn() override;
	void moveTo(int, int) override;

	void changeSquareThreatCount(int) override;

	void die() override;

private:
	void whiteKingAI();
	void runAway();
};


#pragma once
#include "ChessPiece.h"

#include "ChessBoard.h"

#include <Label.hpp>

class WhiteKing : public ChessPiece
{
	GODOT_CLASS(WhiteKing, ChessPiece);

public:
	static WhiteKing* whiteKingInstance;

	static void _register_methods();
	void _init();
	void _ready();

	void takeTurn() override;
	void moveTo(int, int) override;

	void changeSquareThreatCount(int) override;

	void die() override;

private:
	Label* moveCounter;

	void whiteKingAI();
	void runAway();
};


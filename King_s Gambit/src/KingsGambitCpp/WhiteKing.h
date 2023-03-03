#pragma once
#include "ChessPiece.h"

#include "ChessBoard.h"

class WhiteKing : public ChessPiece
{
	GODOT_CLASS(WhiteKing, ChessPiece);

public:
	static void _register_methods();
	void _init();

	void takeTurn() override;
	void moveTo(int, int) override;

private:
	void whiteKingAI();
	void runAway();
};


#pragma once
#include "ChessPiece.h"

#include <ResourceLoader.hpp>
#include <PackedScene.hpp>

#include "ChessBoard.h"
#include "TurnController.h"


class Pawn : public ChessPiece
{
	GODOT_CLASS(Pawn, ChessPiece);

public:
	static void _register_methods();
	void _init();
	void _ready();

	void takeTurn() override;
	void moveTo(int, int) override;

	void changeSquareThreatCount(int) override;

	void die() override;

private:
	void pawnAI();

	void spawnQueenToPosition(int, int);

	Ref<PackedScene> queenScene;
	Node2D* pieceHolder;
};


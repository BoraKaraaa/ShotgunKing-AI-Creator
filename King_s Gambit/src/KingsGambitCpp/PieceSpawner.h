#pragma once
#include <Godot.hpp>
#include <Timer.hpp>
#include <Node2D.hpp>

#include <KinematicBody2D.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>

#include "TurnController.h"

#include "ChessBoard.h"

using namespace godot;

class PieceSpawner : public Timer
{
	GODOT_CLASS(PieceSpawner, Timer);

public:
	static PieceSpawner* pieceSpawnerInstance;

	static void _register_methods();
	void _init();
	void _ready();

	PieceSpawner();
	~PieceSpawner();

	void spawnQueenToPosition(int, int);

private:

	Node2D* pieceHolder;

	// Piece Scene References
	Ref<PackedScene> pawnScene;
	Ref<PackedScene> knightScene;
	Ref<PackedScene> bishopScene;
	Ref<PackedScene> rookScene;
	Ref<PackedScene> queenScene;
	Ref<PackedScene> whiteKingScene;
	Ref<PackedScene> blackKingScene;

	int pawnAmount;
	int knightAmount;
	int bishopAmount;
	int rookAmount;
	int queenAmount;

	void spawnPieces();

	void waitNSecond(int, String);

};


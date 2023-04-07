#pragma once
#include <Godot.hpp>
#include <Timer.hpp>
#include <Node2D.hpp>

#include <KinematicBody2D.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <TextureButton.hpp>
#include <Control.hpp>
#include <LineEdit.hpp>
#include <CheckButton.hpp>
#include <AudioStreamPlayer2D.hpp>
#include <TextEdit.hpp>

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

	void spawnBlackKingToPosition(int, int);
	void spawnWhiteKingPosition(int, int);
	void spawnQueenToPosition(int, int);
	void spawnRookToPosition(int, int);
	void spawnBishopToPosition(int, int);
	void spawnKnightToPosition(int, int);
	void spawnPawnToPosition(int, int);

	void spawnChessPieceWhitType(ChessPiece*, int, int);

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

	TextureButton* playButton;
	AudioStreamPlayer2D* pressAudio;

	Control* preGameUI;
	Control* chessPieceInfoUIHolder;

	TextEdit* fenNotationText;

	CheckButton* usePrepHeuriscticValButton;

	String FEN;

	// 0 -> Pawn | 1 -> Knight | 2 -> Bishop | 3 -> Rook | 4 -> Queen | 5 -> WhiteKing | 6 -> BlackKing 
	//std::vector<std::vector<int>> chessPieceParameters;

	void spawnPieces();

	void startGame();

	void setChessPieceAmounts();
	void setChessPieceParametersToArray();

	void setChessPieceAmountsWithFenNotation();

	void closeUI();

	bool isFenNotationValid(String);

	void waitNSecond(int, String);

};


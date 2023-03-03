#pragma once
#include <Godot.hpp>
#include <Timer.hpp>

#include <vector>

#include "TurnType.h"
#include "ChessPiece.h"

using namespace godot;

class TurnController : public Timer 
{
	GODOT_CLASS(TurnController, Timer);

public:
	static TurnController* turnControllerInstance;
	static TurnType pTurnType;

	static void _register_methods();
	void _init();

	TurnController();
	~TurnController();

	void startTurn();

	void setWhitePiece(ChessPiece*);
	void setBlackPiece(ChessPiece*);

	void deleteWhitePiece(ChessPiece*);

	void setCreatedWhitePieces(ChessPiece*);

private:
	std::vector<ChessPiece*> whitePieces;
	std::vector<ChessPiece*> createdWhitePieces;
	std::vector<int> deletedIndexes;

	ChessPiece* blakcPiece;


	void blackTurn();
	void whiteTurn();

	void waitNSecond(float, String);

	void updateWhitePieces();
};


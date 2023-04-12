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

	std::vector<ChessPiece*> whitePieces;

	bool isGameStarted = false;
	bool isTurnFinished = false;

	static void _register_methods();
	void _init();
	void _process(float);

	TurnController();
	~TurnController();

	void startTurn();
	void stopTurn();

	void setWhitePiece(ChessPiece*);
	void setBlackPiece(ChessPiece*);

	void deleteWhitePiece(ChessPiece*);

	void setCreatedWhitePieces(ChessPiece*);

private:
	std::vector<ChessPiece*> createdWhitePieces;
	std::vector<int> deletedIndexes;

	bool isStart = true;

	ChessPiece* blakcPiece;


	void blackTurn();
	void whiteTurn();

	void waitNSecond(float, String);

	void updateWhitePieces();
};


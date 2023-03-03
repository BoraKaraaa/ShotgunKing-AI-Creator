#include "TurnController.h"

TurnController* TurnController::turnControllerInstance;
TurnType TurnController::pTurnType = TurnType::BLACK;

void TurnController::_register_methods()
{
	register_method((char*)"_init", &TurnController::_init);
	register_method((char*)"blackTurn", &TurnController::blackTurn);
	register_method((char*)"whiteTurn", &TurnController::whiteTurn);
}

void TurnController::_init()
{
	turnControllerInstance = this;
}

TurnController::TurnController() { }

TurnController::~TurnController() { }


void TurnController::startTurn()
{
	/*
	if(pTurnType == TurnType::BLACK)
	{
		waitNSecond(1, "blackTurn");
	}
	else
	{
		waitNSecond(1, "whiteTurn");
	}
	*/
}

void TurnController::blackTurn()
{
	blakcPiece->takeTurn();

	Godot::print("Black Turn");

	disconnect("timeout", this, "blackTurn");
	waitNSecond(1, "whiteTurn");
}

void TurnController::whiteTurn()
{
	for(ChessPiece* chessPiece : whitePieces)
	{
		chessPiece->takeTurn();
	}
	
	Godot::print("White Turn");

	disconnect("timeout", this, "whiteTurn");
	waitNSecond(1, "blackTurn");

	updateWhitePieces();
}


void TurnController::waitNSecond(float second, String callbackFunc)
{
	set_wait_time(second);
	set_one_shot(true);
	connect("timeout", this, callbackFunc);
	start();
}

void TurnController::setWhitePiece(ChessPiece* whitePiece)
{
	whitePieces.push_back(whitePiece);
}

void TurnController::setCreatedWhitePieces(ChessPiece* createdWhitePiece)
{
	createdWhitePieces.push_back(createdWhitePiece);
}

void TurnController::setBlackPiece(ChessPiece* blackKing)
{
	blakcPiece = blackKing;
}

void TurnController::deleteWhitePiece(ChessPiece* whitePiece)
{
	for (int i = 0; i < whitePieces.size(); i++)
	{
		if(whitePieces.at(i)->get_instance_id() == whitePiece->get_instance_id())
		{
			deletedIndexes.insert(deletedIndexes.begin(), i);
			return;
		}
	}
	
}

void TurnController::updateWhitePieces()
{

	for(ChessPiece* createdPiece : createdWhitePieces)
	{
		whitePieces.push_back(createdPiece);
	}

	createdWhitePieces.clear();

	for(int index : deletedIndexes)
	{
		whitePieces.erase(whitePieces.begin() + index);
	}

	deletedIndexes.clear();
}
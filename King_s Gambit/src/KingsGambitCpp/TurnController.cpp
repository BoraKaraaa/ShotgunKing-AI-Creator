#include "TurnController.h"

TurnController* TurnController::turnControllerInstance = NULL;
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
	if(pTurnType == TurnType::BLACK)
	{
		waitNSecond(0.5, "blackTurn");
	}
	else
	{
		waitNSecond(0.5, "whiteTurn");
	}
}

void TurnController::stopTurn()
{
	get_parent()->remove_child(this);
	//disconnect("timeout", this, "blackTurn");
	//disconnect("timeout", this, "whiteTurn");
}

void TurnController::blackTurn()
{
	blakcPiece->takeTurn();

	Godot::print("Black Turn");

	disconnect("timeout", this, "blackTurn");
	waitNSecond(0.5, "whiteTurn");
}

void TurnController::whiteTurn()
{
	/*
	if(!isStart)
	{
		for (ChessPiece* chessPiece : whitePieces)
		{
			chessPiece->changeSquareThreatCount(-1);
		}
	}
	*/

	for(ChessPiece* chessPiece : whitePieces)
	{
		chessPiece->takeTurn();
	}

	/*
	for (ChessPiece* chessPiece : whitePieces)
	{
		chessPiece->changeSquareThreatCount(1);
	}
	*/

	//isStart = false;

	Godot::print("White Turn");

	disconnect("timeout", this, "whiteTurn");
	waitNSecond(0.5, "blackTurn");

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
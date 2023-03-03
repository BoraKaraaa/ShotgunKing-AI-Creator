#include "PieceSpawner.h"


PieceSpawner* PieceSpawner::pieceSpawnerInstance = NULL;

void PieceSpawner::_register_methods()
{
	register_method((char*)"_init", &PieceSpawner::_init);
	register_method((char*)"_ready", &PieceSpawner::_ready);
	register_method((char*)"spawnPieces", &PieceSpawner::spawnPieces);

	register_property<PieceSpawner, int>((char*)"pawnAmount", &PieceSpawner::pawnAmount, 0);
	register_property<PieceSpawner, int>((char*)"knightAmount", &PieceSpawner::knightAmount, 0);
	register_property<PieceSpawner, int>((char*)"bishopAmount", &PieceSpawner::bishopAmount, 0);
	register_property<PieceSpawner, int>((char*)"rookAmount", &PieceSpawner::rookAmount, 0);
	register_property<PieceSpawner, int>((char*)"queenAmount", &PieceSpawner::queenAmount, 0);
}

void PieceSpawner::_init()
{	
	pieceSpawnerInstance = this;

	pawnScene = ResourceLoader::get_singleton()->load("res://Nodes/Pieces/Pawn.tscn");
	knightScene = ResourceLoader::get_singleton()->load("res://Nodes/Pieces/Knight.tscn");
	bishopScene = ResourceLoader::get_singleton()->load("res://Nodes/Pieces/Bishop.tscn");
	rookScene = ResourceLoader::get_singleton()->load("res://Nodes/Pieces/Rook.tscn");
	queenScene = ResourceLoader::get_singleton()->load("res://Nodes/Pieces/Queen.tscn");
	whiteKingScene = ResourceLoader::get_singleton()->load("res://Nodes/Pieces/WhiteKing.tscn");
	blackKingScene = ResourceLoader::get_singleton()->load("res://Nodes/Pieces/BlackKing.tscn");
}

void PieceSpawner::_ready()
{
	pieceHolder = (Node2D*)get_node("/root/MainScene/PieceHolder");

	waitNSecond(1, "spawnPieces");
	Godot::print("Wait For Spawning Pieces");
}

PieceSpawner::PieceSpawner() { }

PieceSpawner::~PieceSpawner() { }


void PieceSpawner::spawnPieces()
{
	queue_free();

	int firstRowIndex = 0;
	int secondRowIndex = 1;

	int firstRowMiddleIndex = 3;
	int firstRowMiddleIndexPawn = 3;

	int index = 1;
	int indexPawn = 0;

	// WhiteKing Amount always == 1
	for(int i = 0; i < 1; i++)
	{
		KinematicBody2D* kingBody = (KinematicBody2D*)whiteKingScene->instance();
		ChessPiece* kingSC = Object::cast_to<ChessPiece>(kingBody);
		kingSC->assignInitialDirections(firstRowMiddleIndex, firstRowIndex);
		//kingSC->assignInitialDirections(4, 5);
		pieceHolder->add_child(kingBody);

		TurnController::turnControllerInstance->setWhitePiece(kingSC);
	}

	for(int i = 0; i < queenAmount; i++)
	{
		if (index % 2 == 0)
			firstRowMiddleIndex -= index++;
		else
			firstRowMiddleIndex += index++;

		spawnQueenToPosition(firstRowMiddleIndex, firstRowIndex);
	}

	for (int i = 0; i < rookAmount; i++)
	{
		if (index % 2 == 0)
			firstRowMiddleIndex -= index++;
		else
			firstRowMiddleIndex += index++;

		KinematicBody2D* rookBody = (KinematicBody2D*)rookScene->instance();
		ChessPiece* rookSC = Object::cast_to<ChessPiece>(rookBody);
		rookSC->assignInitialDirections(firstRowMiddleIndex, firstRowIndex);
		pieceHolder->add_child(rookBody);

		TurnController::turnControllerInstance->setWhitePiece(rookSC);
	}

	for (int i = 0; i < bishopAmount; i++)
	{
		if (index % 2 == 0)
			firstRowMiddleIndex -= index++;
		else
			firstRowMiddleIndex += index++;

		KinematicBody2D* bishopBody = (KinematicBody2D*)bishopScene->instance();
		ChessPiece* bishopSC = Object::cast_to<ChessPiece>(bishopBody);
		bishopSC->assignInitialDirections(firstRowMiddleIndex, firstRowIndex);
		pieceHolder->add_child(bishopBody);

		TurnController::turnControllerInstance->setWhitePiece(bishopSC);
	}
	
	for (int i = 0; i < knightAmount; i++)
	{
		if (index % 2 == 0)
			firstRowMiddleIndex -= index++;
		else
			firstRowMiddleIndex += index++;

		KinematicBody2D* knightBody = (KinematicBody2D*)knightScene->instance();
		ChessPiece* knightSC = Object::cast_to<ChessPiece>(knightBody);
		knightSC->assignInitialDirections(firstRowMiddleIndex, firstRowIndex);
		pieceHolder->add_child(knightBody);

		TurnController::turnControllerInstance->setWhitePiece(knightSC);
	}

	for (int i = 0; i < pawnAmount; i++)
	{
		if (indexPawn % 2 == 0)
			firstRowMiddleIndexPawn -= indexPawn++;
		else
			firstRowMiddleIndexPawn += indexPawn++;

		KinematicBody2D* pawnBody = (KinematicBody2D*)pawnScene->instance();
		ChessPiece* pawnSC = Object::cast_to<ChessPiece>(pawnBody);
		pawnSC->assignInitialDirections(firstRowMiddleIndexPawn, secondRowIndex);
		pieceHolder->add_child(pawnBody);

		TurnController::turnControllerInstance->setWhitePiece(pawnSC);
	}

	// BlackKing Amount always == 1
	for (int i = 0; i < 1; i++)
	{

		KinematicBody2D* blackKingBody = (KinematicBody2D*)blackKingScene->instance();
		ChessPiece* blackKingSC = Object::cast_to<ChessPiece>(blackKingBody);
		blackKingSC->assignInitialDirections(3, 7);
		pieceHolder->add_child(blackKingBody);

		TurnController::turnControllerInstance->setBlackPiece(blackKingSC);
	}

	TurnController::turnControllerInstance->startTurn();
	disconnect("timeout", this, "spawnPieces");
}

void PieceSpawner::spawnQueenToPosition(int x, int y)
{
	KinematicBody2D* queenBody = (KinematicBody2D*)queenScene->instance();
	ChessPiece* queenSC = Object::cast_to<ChessPiece>(queenBody);
	queenSC->assignInitialDirections(x, y);
	pieceHolder->add_child(queenBody);

	TurnController::turnControllerInstance->setWhitePiece(queenSC);
}


void PieceSpawner::waitNSecond(int second, String callbackFunc)
{
	set_wait_time(second);
	set_one_shot(true);
	connect("timeout", this, callbackFunc);
	start();
}
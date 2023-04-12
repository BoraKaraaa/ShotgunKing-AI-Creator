#include "PieceSpawner.h"

#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "WhiteKing.h"

#include "BlackKing.h"

#include "CountDown.h"

#include <iostream>

PieceSpawner* PieceSpawner::pieceSpawnerInstance = NULL;

std::vector<std::vector<int>> chessPieceParameters(7, std::vector<int>(4, -1));

void PieceSpawner::_register_methods()
{
	register_method((char*)"_init", &PieceSpawner::_init);
	register_method((char*)"_ready", &PieceSpawner::_ready);
	register_method((char*)"spawnPieces", &PieceSpawner::spawnPieces);
	register_method((char*)"setChessPieceAmountsWithFenNotation", &PieceSpawner::setChessPieceAmountsWithFenNotation);

	register_method((char*)"startGame", &PieceSpawner::startGame);

	// Piece Amount Parameters
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

	playButton = (TextureButton*)get_node("/root/MainScene/PlayButton");
	pressAudio = (AudioStreamPlayer2D*)get_node("/root/MainScene/PlayButton/pressAudio");

	preGameUI = (Control*)get_node("/root/MainScene/PreGameUI");
	chessPieceInfoUIHolder = (Control*)get_node("/root/MainScene/ChessPieceInfoUIHolder");

	fenNotationText = (TextEdit*)get_node("/root/MainScene/FenNotationText");
	leaderBoard = (TextEdit*)get_node("/root/MainScene/LeaderBoard");

	usePrepHeuriscticValButton = (CheckButton*)get_node("/root/MainScene/PrepHeuristicValueButton");

	countDown = (Label*)get_node("/root/MainScene/CountDown");

	totalTryText = (Label*)get_node("/root/MainScene/GameUI2/TotalPossibilities");
	totalQueuedPosText = (Label*)get_node("/root/MainScene/GameUI2/QueuedPositions");
	pauseText = (Label*)get_node("/root/MainScene/GameUI2/PauseText");

	playButton->connect("pressed", this, "startGame");
}

PieceSpawner::PieceSpawner() { }

PieceSpawner::~PieceSpawner() { }

void PieceSpawner::startGame()
{
	playButton->disconnect("pressed", this, "startGame");

	closeUI();
	openUI();

	pressAudio->play();

	// Spawn Pieces

	if (isFenNotationValid(fenNotationText->get_text()))
	{
		waitNSecond(1, "setChessPieceAmountsWithFenNotation");
	}
	else
	{
		setChessPieceAmounts();

		//waitNSecond(1, "spawnPieces");
		spawnPieces();
	}

	Godot::print("Wait For Spawning Pieces");
}

void PieceSpawner::spawnPieces()
{
	//queue_free();

	setChessPieceParametersToArray();

	int firstRowIndex = 0;
	int secondRowIndex = 1;

	int firstRowMiddleIndex = 3;
	int firstRowMiddleIndexPawn = 3;

	int index = 1;
	int indexPawn = 0;


	// WhiteKing Amount always == 1
	for(int i = 0; i < 1; i++)
	{
		spawnWhiteKingPosition(firstRowMiddleIndex, firstRowIndex);
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

		spawnRookToPosition(firstRowMiddleIndex, firstRowIndex);
	}

	for (int i = 0; i < bishopAmount; i++)
	{
		if (index % 2 == 0)
			firstRowMiddleIndex -= index++;
		else
			firstRowMiddleIndex += index++;

		spawnBishopToPosition(firstRowMiddleIndex, firstRowIndex);
	}
	
	for (int i = 0; i < knightAmount; i++)
	{
		if (index % 2 == 0)
			firstRowMiddleIndex -= index++;
		else
			firstRowMiddleIndex += index++;

		spawnKnightToPosition(firstRowMiddleIndex, firstRowIndex);
	}

	for (int i = 0; i < pawnAmount; i++)
	{
		if (indexPawn % 2 == 0)
			firstRowMiddleIndexPawn -= indexPawn++;
		else
			firstRowMiddleIndexPawn += indexPawn++;

		spawnPawnToPosition(firstRowMiddleIndexPawn, secondRowIndex);
	}

	// BlackKing Amount always == 1
	for (int i = 0; i < 1; i++)
	{
		spawnBlackKingToPosition(3, 7);
	}

	//disconnect("timeout", this, "spawnPieces");

	CountDown::countDownInstance->set_visible(true);
	CountDown::countDownInstance->startCountDown();
	
	TurnController::turnControllerInstance->startTurn();
}

void PieceSpawner::setChessPieceAmountsWithFenNotation()
{
	queue_free();

	setChessPieceParametersToArray();

	int rowIndex = 7; 
	int columnIndex = 7;

	for (int i = 0; i < FEN.length(); i++)
	{
		if (FEN[i] == '/')
		{
			rowIndex--;
			columnIndex = 7;
		}
		else if (FEN[i] >= '1' && FEN[i] <= '8')
		{ 
			columnIndex -= FEN[i] - '0';
		}
		else 
		{
			switch (FEN[i])
			{
				case 'k':
					spawnBlackKingToPosition(columnIndex, rowIndex);
					break;
				case 'K':
					spawnWhiteKingPosition(columnIndex, rowIndex);
					break;
				case 'Q':
					spawnQueenToPosition(columnIndex, rowIndex);
					break;
				case 'R':
					spawnRookToPosition(columnIndex, rowIndex);
					break;
				case 'B':
					spawnBishopToPosition(columnIndex, rowIndex);
					break;
				case 'N':
					spawnKnightToPosition(columnIndex, rowIndex);
					break;
				case 'P':
					spawnPawnToPosition(columnIndex, rowIndex);
					break;
			}

			columnIndex--;
		}
	}

	CountDown::countDownInstance->set_visible(true);
	CountDown::countDownInstance->startCountDown();

	TurnController::turnControllerInstance->startTurn();
	disconnect("timeout", this, "setChessPieceAmountsWithFenNotation");
}

void PieceSpawner::spawnQueenToPosition(int x, int y)
{
	KinematicBody2D* queenBody = (KinematicBody2D*)queenScene->instance();
	ChessPiece* queenSC = Object::cast_to<ChessPiece>(queenBody);
	queenSC->assignInitialDirections(x, y);
	pieceHolder->add_child(queenBody);

	queenSC->setParameters(chessPieceParameters[4][0], chessPieceParameters[4][1],
		chessPieceParameters[4][2], chessPieceParameters[4][3]);

	TurnController::turnControllerInstance->setWhitePiece(queenSC);
}

void PieceSpawner::spawnBlackKingToPosition(int x, int y)
{
	KinematicBody2D* blackKingBody = (KinematicBody2D*)blackKingScene->instance();
	ChessPiece* blackKingSC = Object::cast_to<ChessPiece>(blackKingBody);
	blackKingSC->assignInitialDirections(x, y);
	pieceHolder->add_child(blackKingBody);

	((BlackKing*)blackKingSC)->setBlackKingParameters(chessPieceParameters[6][0], chessPieceParameters[6][1],
		chessPieceParameters[6][2]);

	TurnController::turnControllerInstance->setBlackPiece(blackKingSC);
}

void PieceSpawner::spawnWhiteKingPosition(int x, int y)
{
	KinematicBody2D* kingBody = (KinematicBody2D*)whiteKingScene->instance();
	ChessPiece* kingSC = Object::cast_to<ChessPiece>(kingBody);
	kingSC->assignInitialDirections(x, y);

	pieceHolder->add_child(kingBody);

	kingSC->setParameters(chessPieceParameters[5][0], chessPieceParameters[5][1],
		chessPieceParameters[5][2], chessPieceParameters[5][3]);
	
	TurnController::turnControllerInstance->setWhitePiece(kingSC);
}

void PieceSpawner::spawnRookToPosition(int x, int y)
{
	KinematicBody2D* rookBody = (KinematicBody2D*)rookScene->instance();
	ChessPiece* rookSC = Object::cast_to<ChessPiece>(rookBody);
	rookSC->assignInitialDirections(x, y);
	pieceHolder->add_child(rookBody);

	rookSC->setParameters(chessPieceParameters[3][0], chessPieceParameters[3][1],
		chessPieceParameters[3][2], chessPieceParameters[3][3]);

	TurnController::turnControllerInstance->setWhitePiece(rookSC);
}

void PieceSpawner::spawnBishopToPosition(int x, int y)
{
	KinematicBody2D* bishopBody = (KinematicBody2D*)bishopScene->instance();
	ChessPiece* bishopSC = Object::cast_to<ChessPiece>(bishopBody);
	bishopSC->assignInitialDirections(x, y);
	pieceHolder->add_child(bishopBody);

	bishopSC->setParameters(chessPieceParameters[2][0], chessPieceParameters[2][1],
		chessPieceParameters[2][2], chessPieceParameters[2][3]);

	TurnController::turnControllerInstance->setWhitePiece(bishopSC);
}

void PieceSpawner::spawnKnightToPosition(int x, int y)
{
	KinematicBody2D* knightBody = (KinematicBody2D*)knightScene->instance();
	ChessPiece* knightSC = Object::cast_to<ChessPiece>(knightBody);
	knightSC->assignInitialDirections(x, y);
	pieceHolder->add_child(knightBody);

	knightSC->setParameters(chessPieceParameters[1][0], chessPieceParameters[1][1],
		chessPieceParameters[1][2], chessPieceParameters[1][3]);

	TurnController::turnControllerInstance->setWhitePiece(knightSC);
}

void PieceSpawner::spawnPawnToPosition(int x, int y)
{
	KinematicBody2D* pawnBody = (KinematicBody2D*)pawnScene->instance();
	ChessPiece* pawnSC = Object::cast_to<ChessPiece>(pawnBody);
	pawnSC->assignInitialDirections(x, y);
	pieceHolder->add_child(pawnBody);

	pawnSC->setParameters(chessPieceParameters[0][0], chessPieceParameters[0][1],
		chessPieceParameters[0][2], chessPieceParameters[0][3]);

	TurnController::turnControllerInstance->setWhitePiece(pawnSC);
}


void PieceSpawner::waitNSecond(int second, String callbackFunc)
{
	set_wait_time(second);
	set_one_shot(true);
	connect("timeout", this, callbackFunc);
	start();
}

void PieceSpawner::spawnChessPieceWhitType(ChessPiece* chessPiece, int x, int y)
{
	if(dynamic_cast<Pawn*>(chessPiece))
	{
		spawnPawnToPosition(x, y);
	}
	else if(dynamic_cast<Knight*>(chessPiece))
	{
		spawnKnightToPosition(x, y);
	}
	else if (dynamic_cast<Bishop*>(chessPiece))
	{
		spawnBishopToPosition(x, y);
	}
	else if (dynamic_cast<Rook*>(chessPiece))
	{
		spawnRookToPosition(x, y);
	}
	else if (dynamic_cast<Queen*>(chessPiece))
	{
		spawnQueenToPosition(x, y);
	}
	else if (dynamic_cast<WhiteKing*>(chessPiece))
	{
		spawnWhiteKingPosition(x, y);
	}
}

void PieceSpawner::closeUI()
{
	playButton->set_visible(false);
	preGameUI->set_visible(false);
	chessPieceInfoUIHolder->set_visible(false);
	usePrepHeuriscticValButton->set_visible(false);
	fenNotationText->set_visible(false);
}

void PieceSpawner::openUI()
{
	totalTryText->set_visible(true);
	totalQueuedPosText->set_visible(true);
	pauseText->set_visible(true);
	leaderBoard->set_visible(true);
}

void PieceSpawner::setChessPieceParametersToArray()
{
	const String LineEditName = "LineEdit";

	Node* node;

	int childCount = chessPieceInfoUIHolder->get_child_count();

	for(int i = 0; i < childCount; i++)
	{
		node = chessPieceInfoUIHolder->get_child(i);

		int insideChildCount = node->get_child_count();

		int foundedIndex = 0;

		for(int j = 0; j < insideChildCount; j++)
		{
			node = chessPieceInfoUIHolder->get_child(i)->get_child(j);
			
			if(node->get_class() == LineEditName)
			{
				godot::String str = (((LineEdit*)node)->get_text());

				if (str.is_valid_integer() && str.to_int() != 0)
				{
					chessPieceParameters[i][foundedIndex] = str.to_int();
				}
				else
				{
					chessPieceParameters[i][foundedIndex] = -1;
				}
				
				foundedIndex++;
			}
			
		}
		
	}

	if (usePrepHeuriscticValButton->is_pressed())
	{
		setEfficentHeuristicParameters();
		return;
	}
}

void PieceSpawner::setChessPieceAmounts()
{
	const String LineEditName = "LineEdit";

	Node* node;

	int childCount = preGameUI->get_child_count();

	for (int i = 0; i < childCount; i++)
	{
		node = preGameUI->get_child(i);

		if (node->get_class() == LineEditName)
		{
			godot::String str = (((LineEdit*)node)->get_text());

			if (str.is_valid_integer())
			{
				switch (i)
				{
				case 1: // Pawn
					if (i <= 8)
						pawnAmount = str.to_int();
					else
						pawnAmount = 8;
					break;
				case 2: // Knight
					if (i <= 7)
						knightAmount = str.to_int();
					else
						knightAmount = 7;
					break;
				case 3: // Bishop
					if (i <= 7)
						bishopAmount = str.to_int();
					else
						bishopAmount = 7;
					break;
				case 4: // Rook
					if (i <= 7)
						rookAmount = str.to_int();
					else
						rookAmount = 7;
					break;
				case 5: // Queen
					if (i <= 7)
						queenAmount = str.to_int();
					else
						queenAmount = 7;
					break;
				}
			}
		}
	}

	if (knightAmount + bishopAmount + rookAmount + queenAmount > 7)
	{
		knightAmount = 1;
		bishopAmount = 1;
		rookAmount = 1;
		queenAmount = 1;
	}
}

bool PieceSpawner::isFenNotationValid(String fenNotation)
{
	int rows = 1;
	int cols = 0;

	const int EXPECTED_ROWS = 8;

	// LowerCase chars represent black pieces
	// UpperCase chars represent white pieces
	const String VALID_PIECES = "KkQRBNP";
	
	for (int i = 0; i < fenNotation.length(); i++) 
	{
		if (fenNotation[i] == '/') 
		{ 
			if (cols != 8) 
			{ 
				return false;
			}
			rows++;
			cols = 0;
		}
		else if (fenNotation[i] >= '1' && fenNotation[i] <= '8') 
		{ 
			cols += fenNotation[i] - '0';
		}
		else if (VALID_PIECES.find(fenNotation[i]) != std::string::npos) 
		{
			cols++;
		}
		else if (fenNotation[i] == ' ') 
		{ 
			break;
		}
		else 
		{ 
			return false;
		}
	}
	

	if (rows != EXPECTED_ROWS)
	{
		return false;
	}

	FEN = fenNotation;
	return true;
}

void PieceSpawner::setEfficentHeuristicParameters()
{
	// Pawn Parameters
	chessPieceParameters[0][2] = 1;
	chessPieceParameters[0][3] = 3;

	// Knight Parameters
	chessPieceParameters[1][2] = 2;
	chessPieceParameters[1][3] = 4;

	// Bishop Parameters
	chessPieceParameters[2][2] = 2;
	chessPieceParameters[2][3] = 4;

	// Rook Parameters
	chessPieceParameters[3][2] = 3;
	chessPieceParameters[3][3] = 5;

	// Queen Parameters
	chessPieceParameters[4][2] = 3;
	chessPieceParameters[4][3] = 6;

	// WhiteKing Parameters
	chessPieceParameters[5][2] = 15;
	chessPieceParameters[5][3] = 1000;

	// BlackKing Parameters
	chessPieceParameters[6][0] = 10;
	chessPieceParameters[6][1] = 5;
	chessPieceParameters[6][2] = 2;

}
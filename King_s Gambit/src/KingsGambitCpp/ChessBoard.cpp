#include "ChessBoard.h"
#include <iostream>

// Singleton
ChessBoard* ChessBoard::chessBoardInstance = NULL;

Vector2 ChessBoard::boardDimensions = Vector2(8, 8);

void ChessBoard::_register_methods()
{
	register_method((char*)"_init", &ChessBoard::_init);
	register_property<ChessBoard, Vector2>((char*)"boardDimensions", &ChessBoard::setBoardDimensions, &ChessBoard::getBoardDimensions, ChessBoard::boardDimensions);

	register_property<ChessBoard, float>((char*)"firstSquareXtransform", &ChessBoard::firstSquareXtransform, 0.); // 240
	register_property<ChessBoard, float>((char*)"firstSquareYTransform", &ChessBoard::firstSquareYTransform, 0.); // 167

	register_property<ChessBoard, float>((char*)"twoSquareXDistanceDiff", &ChessBoard::twoSquareXDistanceDiff, 0.); // 48
	register_property<ChessBoard, float>((char*)"twoSquareYDistanceDiff", &ChessBoard::twoSquareYDistanceDiff, 0.); // 47
}

void ChessBoard::_init()
{
	chessBoardInstance = this;

	firstSquareXtransform = 240;
	firstSquareYTransform = 167;

	twoSquareXDistanceDiff = 48;
	twoSquareYDistanceDiff = 47;
}

ChessBoard::ChessBoard()
{
	initSquares();
}

ChessBoard::~ChessBoard()
{
	destroySquares();
}

void ChessBoard::initSquares()
{
	boardDimensions = Vector2(8, 8);

	squareArray = new Square ** [boardDimensions.x];

	for (int i = 0; i < boardDimensions.x; i++)
	{
		squareArray[i] = new Square * [boardDimensions.y];

		for(int j = 0; j < boardDimensions.y; j++)
		{
			squareArray[i][j] = new Square(i, j);
		}
	}
}

void ChessBoard::destroySquares()
{
	if(squareArray)
	{
		for (int i = 0; i < boardDimensions.x; i++)
		{
			for (int j = 0; j < boardDimensions.y; j++)
			{
				delete squareArray[i][j];
			}

			delete squareArray[i];
		}
	}

	delete squareArray;
	squareArray = nullptr;
}

void ChessBoard::setBoardDimensions(Vector2 v)
{
	boardDimensions = v;
}

Vector2 ChessBoard::getBoardDimensions()
{
	return boardDimensions;
}

Square* ChessBoard::getSquare(int x, int y)
{
	checkProperDirection(&x, &y);

	return squareArray[x][y];
}

Vector2 ChessBoard::getSquarePivotPosition(int x, int y)
{
	checkProperDirection(&x, &y);

	return Vector2(firstSquareXtransform + twoSquareXDistanceDiff * x, firstSquareYTransform + twoSquareYDistanceDiff * y);
}

void ChessBoard::setChessPieceToSquare(ChessPiece* chessPiece, int x, int y)
{
	checkProperDirection(&x, &y);

	getSquare(x, y)->setPieceToSquare(chessPiece);
}

bool ChessBoard::isSquareEmpty(int x, int y)
{
	checkProperDirection(&x, &y);

	return getSquare(x, y)->isSquareEmpty();
}

void ChessBoard::clearBoardThreatCount()
{
	for (int i = 0; i < boardDimensions.x; i++)
	{
		for (int j = 0; j < boardDimensions.y; j++)
		{
			getSquare(i, j)->threatCount = 0;
		}
	}
}

float ChessBoard::twoSquareDistance(int fX, int fY, int sX, int sY)
{
	return sqrt((pow(abs(sX - fX), 2) + pow(abs(sY - fY), 2)));
}


int ChessBoard::twoSquareDistanceWithSquare(int fX, int fY, int sX, int sY)
{
	return std::max(abs(fX - sX), abs(fY - sY));
}


bool ChessBoard::isSquareValid(int x, int y)
{
	if (x < 0 || x > 7 || y < 0 || y > 7) return false;
	return true;
}

void ChessBoard::checkProperDirection(int* x, int* y)
{
	if (*x < 0) *x = 0;
	if (*x > 7) *x = 7;
	if (*y < 0) *y = 0;
	if (*y > 7) *y = 7;
}


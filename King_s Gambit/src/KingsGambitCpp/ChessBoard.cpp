#include "ChessBoard.h"
#include <string>

Vector2 ChessBoard::boardDimensions = Vector2(8, 8);

void ChessBoard::_register_methods()
{
	register_method((char*)"_init", &ChessBoard::_init);
	register_property<ChessBoard, Vector2>((char*)"boardDimensions", &ChessBoard::setBoardDimensions, &ChessBoard::getBoardDimensions, ChessBoard::boardDimensions);

	register_property<ChessBoard, float>((char*)"firstSquareXtransform", &ChessBoard::firstSquareXtransform, 0.);
	register_property<ChessBoard, float>((char*)"firstSquareYTransform", &ChessBoard::firstSquareYTransform, 0.);

	register_property<ChessBoard, float>((char*)"twoSquareDistanceDiff", &ChessBoard::twoSquareDistanceDiff, 0.);
}

void ChessBoard::_init()
{

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
	return squareArray[x][y];
}





#pragma once

#include <Godot.hpp>
#include <TileMap.hpp>

#include "Square.h"

using namespace godot;

class ChessBoard : public TileMap
{
	GODOT_CLASS(ChessBoard, TileMap);

public:

	static ChessBoard* chessBoardInstance;

	Square*** squareArray;

	static void _register_methods();
	void _init();

	ChessBoard();
	~ChessBoard();

	Vector2 getBoardDimensions();
	void setBoardDimensions(Vector2);

	Square* getSquare(int, int);

	Vector2 getSquarePivotPosition(int, int);

	void setChessPieceToSquare(ChessPiece*, int, int);

	bool isSquareEmpty(int, int);

	void clearBoardThreatCount();

	float twoSquareDistance(int, int, int, int);
	int twoSquareDistanceWithSquare(int, int, int, int);

	bool isSquareValid(int, int);


private:
	static Vector2 boardDimensions;

	float firstSquareXtransform;
	float firstSquareYTransform;

	float twoSquareXDistanceDiff;
	float twoSquareYDistanceDiff;

	void initSquares();
	void destroySquares();

	void checkProperDirection(int*, int*);
};


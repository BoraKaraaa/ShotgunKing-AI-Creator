#pragma once

#include <Godot.hpp>
#include <TileMap.hpp>

#include "Square.h"

using namespace godot;

class ChessBoard : public TileMap
{
	GODOT_CLASS(ChessBoard, TileMap);

public:
	Square*** squareArray;

	static void _register_methods();
	void _init();

	ChessBoard();
	~ChessBoard();

	Vector2 getBoardDimensions();
	void setBoardDimensions(Vector2);

	Square* getSquare(int, int);


private:
	static Vector2 boardDimensions;

	float firstSquareXtransform;
	float firstSquareYTransform;

	float twoSquareDistanceDiff;

	void initSquares();
	void destroySquares();

};


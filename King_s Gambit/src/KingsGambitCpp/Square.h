#pragma once

#include "ChessPiece.h"

class Square
{
public:
	Square(int, int);
	~Square();

	bool isSquareThreatened();
	bool isSquareEmpty();

	void setSquareThreatCount(int);
	void setSquareEmpty(bool);

	int getX();
	int getY();

	void setX(int);
	void setY(int);

	void setPieceToSquare(ChessPiece*);

private:
	int x;
	int y;

	int threatCount;
	bool isEmpty;

	ChessPiece* currChessPiece;

};


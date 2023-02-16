#include "Square.h"
#include <stdio.h>

Square::Square(int xIndex, int yIndex)
{
	x = xIndex;
	y = yIndex;

	setSquareEmpty(true);
	setSquareThreatCount(0);
}

Square::~Square()
{

}

bool Square::isSquareThreatened()
{
	return threatCount > 0;
}

bool Square::isSquareEmpty()
{
	return isEmpty;
}

void Square::setPieceToSquare(ChessPiece* chessPiece)
{
	currChessPiece = chessPiece;
}

void Square::setSquareEmpty(bool isSquareEmpty)
{
	isEmpty = isSquareEmpty;
}

void Square::setSquareThreatCount(int squareThreatCount)
{
	threatCount = squareThreatCount;
}

int Square::getX()
{
	return x;
}

int Square::getY()
{
	return y;
}

void Square::setX(int xIndex)
{
	x = xIndex;
}

void Square::setY(int yIndex)
{
	y = yIndex;
}


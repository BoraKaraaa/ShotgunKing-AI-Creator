#pragma once
#include <Vector2.hpp>
#include "ChessPiece.h"
#include <map>

struct Snapshot
{
	Snapshot* parentSS;

	bool isMoving;
	Vector2 movePosition; // if BlackKing will move

	ChessPiece* hitPiece; // if BlackKing will hit

	std::map<ChessPiece*, Vector2> chessPiecePositionDictioanry; // first piece always BlackKing
	int totalMoveCount; // equal to tree depth
	int heuristicValue;

	bool operator<(const Snapshot& other) const
	{
		return heuristicValue < other.heuristicValue; // arrenge for prioraty queue prioritization
	}

	bool operator==(const Snapshot& other) const
	{
		return this == &other;
	}
};
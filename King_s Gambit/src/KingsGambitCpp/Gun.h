#pragma once

#include "ChessPiece.h"
#include <vector>

#include "TurnController.h"

#include "CameraNoise.h"
#include "Snapshot.h"

class Gun 
{

public:
	int gunDamage = 1;

	Gun();
	~Gun();

	std::vector<ChessPiece*> hittablePieces;

	void virtual searchHittableArea() = 0;
	void virtual createAllPossibleHitNodes(Snapshot*);

	void virtual createHitSnapshots(Snapshot*, ChessPiece*);

	void hitPiece(ChessPiece*);

	
};


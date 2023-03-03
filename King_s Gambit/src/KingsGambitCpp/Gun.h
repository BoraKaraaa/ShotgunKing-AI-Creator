#pragma once

class Gun 
{

public:
	Gun();
	~Gun();

	bool virtual tryToHitPiece(int, int) = 0;
	
};


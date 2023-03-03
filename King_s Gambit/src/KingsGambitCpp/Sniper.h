#pragma once

#include "Gun.h"

class Sniper : public Gun
{
public:
	Sniper(int);
	~Sniper();

	bool tryToHitPiece(int, int) override;

private:
	int areaWidth;
};


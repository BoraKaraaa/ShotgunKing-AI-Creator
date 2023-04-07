#pragma once

#include "Gun.h"

class Sniper : public Gun
{
public:
	Sniper(int);
	~Sniper();

	void searchHittableArea() override;
	void createAllPossibleHitNodes() override;

private:
	int areaWidth;
};


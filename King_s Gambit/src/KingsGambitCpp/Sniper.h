#pragma once

#include "Gun.h"

class Sniper : public Gun
{
public:
	Sniper(int, int);
	~Sniper();

	void searchHittableArea() override;
	void createAllPossibleHitNodes(Snapshot*) override;
};


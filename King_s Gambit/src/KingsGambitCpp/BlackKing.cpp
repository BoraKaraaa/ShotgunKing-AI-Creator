#include "BlackKing.h"

#include <string> 

#include "Sniper.h"

BlackKing* BlackKing::blackKingInstance = NULL;

void BlackKing::_register_methods()
{
	register_method((char*)"_init", &BlackKing::_init);
	register_method((char*)"_ready", &BlackKing::_ready);

	register_property<BlackKing, int>((char*)"gunType", &BlackKing::setGunType, &BlackKing::getGunType, (int)GunType::SNIPER); // Default SNIPER
}

void BlackKing::_init()
{
	blackKingInstance = this;

	switch (gunType)
	{
		case GunType::SNIPER:
			gun = new Sniper(5); // sniper shot area 5*5
			break;
	}
}

void BlackKing::_ready()
{
	moveCounter = (Label*)get_node("/root/MainScene/GameUI/MoveCountAmount");
}

BlackKing::~BlackKing()
{
	delete gun;
	gun = nullptr;
}

void BlackKing::takeTurn()
{
	
	if(counter % 4 == 0)
	{
		moveTo(pX + 3, pY);
	}
	else if(counter % 4 == 1)
	{
		moveTo(pX, pY + 2);
	}
	else if(counter % 4 == 2)
	{
		moveTo(pX - 3, pY);
	}
	else if(counter % 4 == 3)
	{
		moveTo(pX, pY - 2);
	}

	counter++;
	moveCounter->set_text(String::num_int64(counter));
}

void BlackKing::moveTo(int x, int y)
{
	__super::moveTo(x, y);
}

void BlackKing::setGunType(int gunT)
{
	gunType = (GunType)gunT;
}

int BlackKing::getGunType()
{
	return (int)gunType;
}

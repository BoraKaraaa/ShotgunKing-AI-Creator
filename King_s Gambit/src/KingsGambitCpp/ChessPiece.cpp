#include "ChessPiece.h"


void ChessPiece::_register_methods()
{
	register_method((char*)"_init", &ChessPiece::_init);
	register_property<ChessPiece, int>((char*)"TurnCount", &ChessPiece::TurnCount, 1);
	register_property<ChessPiece, int>((char*)"HealthAmount", &ChessPiece::HealthAmount, 1);
}

void ChessPiece::_init()
{

}




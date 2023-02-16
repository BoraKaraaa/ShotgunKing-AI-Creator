#include "TurnController.h"

TurnType TurnController::pTurnType;

void TurnController::_register_methods()
{
	register_method((char*)"_init", &TurnController::_init);
}

void TurnController::_init()
{

}

TurnController::TurnController() { }

TurnController::~TurnController() { }
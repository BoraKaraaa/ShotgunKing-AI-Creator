#pragma once
#include "ChessPiece.h"
#include <Label.hpp>

#include "ChessBoard.h"
#include "Gun.h"
#include "GunType.h"


class BlackKing : public ChessPiece
{
	GODOT_CLASS(BlackKing, ChessPiece);

public:
	static BlackKing* blackKingInstance;

	static void _register_methods();
	void _init();
	void _ready();

	~BlackKing();

	void takeTurn() override;
	void moveTo(int, int) override;

protected:
	inline int getGunType();
	void setGunType(int);


private:
	int counter = 0;

	Label* moveCounter;

	Gun* gun;
	GunType gunType;


};


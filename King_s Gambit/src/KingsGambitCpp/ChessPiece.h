#pragma once
#include <Godot.hpp>
#include <KinematicBody2D.hpp>
#include <Tween.hpp>
#include <Particles2D.hpp>

#include "ChessPieceType.h"
#include "ChessPieceColor.h"

#include <ResourceLoader.hpp>
#include <PackedScene.hpp>

using namespace godot;

class ChessPiece : public KinematicBody2D
{
	GODOT_CLASS(ChessPiece, KinematicBody2D);

public:
	int HealthAmount;
	int TurnCount;

	int currTurnCount;

	int moveDuration;

	bool isDied;

	int pX;
	int pY;

	Tween* tween;

	static void _register_methods();
	void _init();
	void _ready();

	virtual void assignInitialDirections(int, int);
	virtual void takeTurn();

	virtual void moveTo(int, int);
	void moveImmediately(int, int);

	virtual void moveTweenCompleted();
	virtual void changeSquareThreatCount(int);
	virtual void takeDamage(int);
	virtual void die();

	bool canKill(int);

	void setParameters(int, int, int, int);

	// Heuristic Parameters
	int takeDamageHeuristicValue;
	int dieHeuristicValue;

	int hitHeuristicCalculator(int);

protected:
	ChessPieceType chessPieceType;
	ChessPieceColor chessPieceColor;

private:
	Ref<PackedScene> deadParticleScene;

	void playDeadParticle();
	void lastOneCountToPlay();
};


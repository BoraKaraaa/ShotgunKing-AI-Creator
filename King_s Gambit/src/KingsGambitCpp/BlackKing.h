#pragma once
#include "ChessPiece.h"
#include <Label.hpp>
#include <AnimatedSprite.hpp>
#include <Sprite.hpp>
#include <AudioStreamPlayer2D.hpp>
#include <Particles2D.hpp>

#include <queue>
#include <map>

#include "ChessBoard.h"
#include "Gun.h"
#include "GunType.h"

struct Snapshot
{
	bool isMoving;
	Vector2 movePosition; // if BlackKing will move

	ChessPiece* hitPiece; // if BlackKing will hit

	std::map<ChessPiece*, Vector2> chessPiecePositionDictioanry; // first piece always BlackKing
	int totalMoveCount; // equal to tree depth
	int heuristicValue;

	bool operator<(const Snapshot& other) const
	{
		return heuristicValue < other.heuristicValue; // arrenge for prioraty queue prioritization
	}
};

class BlackKing : public ChessPiece
{
	GODOT_CLASS(BlackKing, ChessPiece);

public:
	static BlackKing* blackKingInstance;
	std::priority_queue<Snapshot> snapshotQueue;

	int counter = 0;

	// Heuristic Parameters
	int heuristicStartingValue;
	int eachMoveMinusValue;

	int heuristicDistanceMultiplier;

	static void _register_methods();
	void _init();
	void _ready();

	~BlackKing();

	void takeTurn() override;
	void moveTo(int, int) override;

	void die() override;

	void setBlackKingParameters(int, int, int);

protected:
	inline int getGunType();
	void setGunType(int);

	void createAllPossibleSnapshots();
	void createMoveSnapshots(int, int);
	int moveHeuristicValueCalculator(int, int);
	void playBestMove();
	void goSnapshot(Snapshot);


private:
	Label* moveCounter;

	Snapshot currSS;

	Gun* gun;
	GunType gunType;

	AnimatedSprite* animatedSprite;
	Sprite* gunSprite;

	Sprite* targetIcon;

	Sprite* straightArrow;
	Sprite* crossArrow;

	AudioStreamPlayer2D* jumpAudio;
	AudioStreamPlayer2D* shotAudio;

	Particles2D* bulletParticle;

	Timer* shotTimer;

	bool isDead = false;

	String current_animation;

	void calculateGunRotation(int, int);
	void lookPosition(int, int);

	void showArrow(int, int);
	void unshowArrowAndMove();

	void moveRandom();

	void gunHitPiece();
	void waitNSecond(float, String);
};

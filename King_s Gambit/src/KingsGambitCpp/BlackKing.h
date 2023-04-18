#pragma once
#include "ChessPiece.h"
#include <Label.hpp>
#include <AnimatedSprite.hpp>
#include <Sprite.hpp>
#include <AudioStreamPlayer2D.hpp>
#include <Particles2D.hpp>
#include <TextEdit.hpp>
#include <LineEdit.hpp>

#include <algorithm>
#include <stack>
#include <queue>
#include <map>

#include "ChessBoard.h"
#include "Gun.h"
#include "GunType.h"

#include "Snapshot.h"

class BlackKing : public ChessPiece
{
	GODOT_CLASS(BlackKing, ChessPiece);

public:
	static BlackKing* blackKingInstance;
	std::priority_queue<Snapshot> snapshotQueue;

	std::vector<Snapshot> visitedSnapshots;

	Gun* gun;

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

	void initGunParameters();

	void setBlackKingParameters(int, int, int);

protected:
	inline int getGunType();
	void setGunType(int);

	bool winGame = false;

	int minSuccessfulPathLength = 99999;

	int firstMove = true;
	int minMoveToKillWhiteKing;

	void setMinMoveToKillWhiteKing();

	// Search AI Functions
	void createAllPossibleSnapshots();
	void createMoveSnapshots(int, int);
	int moveHeuristicValueCalculator(int, int);
	void playBestMove();
	void goSnapshot(Snapshot*);

	Snapshot* isTwoSnapshotsEquals(Snapshot*, Snapshot*); // if equal return min totalMoveCount Snapshot if not equal return NULL

private:

	std::stack<Snapshot*> bestGameSnapshotStack;
	Snapshot* bestGameLastSS;

	int totalTry = 0;

	Label* moveCounter;
	Label* totalTryAmountText;
	Label* gameResultText;

	Label* queuedPositionAmountText;

	TextEdit* leaderBoard;

	LineEdit* gunDamage;
	LineEdit* gunRange;

	Snapshot* currSS;

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

	void playBestGame();
	void bestGameMoves();

	void calculateGunRotation(int, int);
	void lookPosition(int, int);

	void showArrow(int, int);
	void unshowArrowAndMove();

	void moveRandom();

	void gunHitPiece();
	void waitNSecond(float, String);

	void updateLeaderBoard(int);
};

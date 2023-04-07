#include "BlackKing.h"

#include <string> 

#include "Sniper.h"
#include "WhiteKing.h"
#include "PieceSpawner.h"

BlackKing* BlackKing::blackKingInstance = NULL;

void BlackKing::_register_methods()
{
	register_method((char*)"_init", &BlackKing::_init);
	register_method((char*)"_ready", &BlackKing::_ready);
	register_method((char*)"gunHitPiece", &BlackKing::gunHitPiece);
	register_method((char*)"unshowArrowAndMove", &BlackKing::unshowArrowAndMove);

	register_property<BlackKing, int>((char*)"gunType", &BlackKing::setGunType, &BlackKing::getGunType, (int)GunType::SNIPER); // Default SNIPER

	register_property<BlackKing, int>((char*)"heuristicStartingValue", &BlackKing::heuristicStartingValue, 0);
	register_property<BlackKing, int>((char*)"eachMoveMinusValue", &BlackKing::eachMoveMinusValue, 0); 
	register_property<BlackKing, int>((char*)"heuristicDistanceMultiplier", &BlackKing::heuristicDistanceMultiplier, 0);
}

void BlackKing::_init()
{
	blackKingInstance = this;

	switch (gunType)
	{
		case GunType::SNIPER:
			gun = new Sniper(2); // sniper shot area 2*2
			break;
	}
}

void BlackKing::_ready()
{
	moveCounter = (Label*)get_node("/root/MainScene/GameUI/MoveCountAmount");

	animatedSprite = (AnimatedSprite*)get_node("/root/MainScene/PieceHolder/BlackKing/KingSprite/KingAnimation");
	gunSprite = (Sprite*)get_node("/root/MainScene/PieceHolder/BlackKing/Gun");

	targetIcon = (Sprite*)get_node("/root/MainScene/targetIcon");

	straightArrow = (Sprite*)get_node("/root/MainScene/PieceHolder/BlackKing/ArrowHolder/StraightArrow");
	crossArrow = (Sprite*)get_node("/root/MainScene/PieceHolder/BlackKing/ArrowHolder/CrossArrow");

	jumpAudio = (AudioStreamPlayer2D*)get_node("/root/MainScene/PieceHolder/BlackKing/AudioHolder/JumpAudio");
	shotAudio = (AudioStreamPlayer2D*)get_node("/root/MainScene/PieceHolder/BlackKing/AudioHolder/ShotAudio");

	bulletParticle = (Particles2D*)get_node("/root/MainScene/PieceHolder/BlackKing/BulletParticle");

	shotTimer = (Timer*)get_node("/root/MainScene/PieceHolder/BlackKing/ShotTimer");

	current_animation = "Idle";
}

BlackKing::~BlackKing()
{
	delete gun;
	gun = nullptr;
}

void BlackKing::takeTurn()
{
	if(!isDead)
	{
		createAllPossibleSnapshots();
	}

	playBestMove();

	isDead = false;
}

void BlackKing::moveRandom()
{
	if (counter % 4 == 0)
	{
		moveTo(pX + 3, pY);
	}
	else if (counter % 4 == 1)
	{
		moveTo(pX, pY + 2);
	}
	else if (counter % 4 == 2)
	{
		moveTo(pX - 3, pY);
	}
	else if (counter % 4 == 3)
	{
		moveTo(pX, pY - 2);
	}
}

void BlackKing::moveTo(int x, int y)
{
	__super::moveTo(x, y);
}

void BlackKing::createAllPossibleSnapshots()
{
	// Creating All Possible Hit Snapsohts

	gun->createAllPossibleHitNodes();

	// Creating All Possible Move Snapshots

	if (ChessBoard::chessBoardInstance->isSquareValid(pX + 1, pY + 1) && ChessBoard::chessBoardInstance->getSquare(pX + 1, pY + 1)->isSquareEmpty())
		//&& ChessBoard::chessBoardInstance->getSquare(pX + 1, pY + 1)->threatCount <= 0)
		createMoveSnapshots(pX + 1, pY + 1);
	if (ChessBoard::chessBoardInstance->isSquareValid(pX + 1, pY - 1) && ChessBoard::chessBoardInstance->getSquare(pX + 1, pY - 1)->isSquareEmpty())
		//&& ChessBoard::chessBoardInstance->getSquare(pX + 1, pY - 1)->threatCount <= 0)
		createMoveSnapshots(pX + 1, pY - 1);
	if (ChessBoard::chessBoardInstance->isSquareValid(pX - 1, pY - 1) && ChessBoard::chessBoardInstance->getSquare(pX - 1, pY - 1)->isSquareEmpty())
		//&& ChessBoard::chessBoardInstance->getSquare(pX - 1, pY - 1)->threatCount <= 0)
		createMoveSnapshots(pX - 1, pY - 1);
	if (ChessBoard::chessBoardInstance->isSquareValid(pX - 1, pY + 1) && ChessBoard::chessBoardInstance->getSquare(pX - 1, pY + 1)->isSquareEmpty())
		//&& ChessBoard::chessBoardInstance->getSquare(pX - 1, pY + 1)->threatCount <= 0)
		createMoveSnapshots(pX - 1, pY + 1);

	if (ChessBoard::chessBoardInstance->isSquareValid(pX, pY + 1) && ChessBoard::chessBoardInstance->getSquare(pX, pY + 1)->isSquareEmpty())
		//&& ChessBoard::chessBoardInstance->getSquare(pX, pY + 1)->threatCount <= 0)
		createMoveSnapshots(pX, pY + 1);
	if (ChessBoard::chessBoardInstance->isSquareValid(pX, pY - 1) && ChessBoard::chessBoardInstance->getSquare(pX, pY - 1)->isSquareEmpty())
		//&& ChessBoard::chessBoardInstance->getSquare(pX, pY - 1)->threatCount <= 0)
		createMoveSnapshots(pX, pY - 1);

	if (ChessBoard::chessBoardInstance->isSquareValid(pX + 1, pY) && ChessBoard::chessBoardInstance->getSquare(pX + 1, pY)->isSquareEmpty())
		//&& ChessBoard::chessBoardInstance->getSquare(pX + 1, pY)->threatCount <= 0)
		createMoveSnapshots(pX + 1, pY);
	if (ChessBoard::chessBoardInstance->isSquareValid(pX - 1, pY) && ChessBoard::chessBoardInstance->getSquare(pX - 1, pY)->isSquareEmpty())
		//&& ChessBoard::chessBoardInstance->getSquare(pX - 1, pY)->threatCount <= 0)
		createMoveSnapshots(pX - 1, pY);
}

void BlackKing::createMoveSnapshots(int lastX, int lastY)
{
	Snapshot newSnapshot;

	newSnapshot.isMoving = true;

	newSnapshot.movePosition = Vector2(lastX, lastY);

	newSnapshot.chessPiecePositionDictioanry[this] = Vector2(pX, pY);

	for (ChessPiece* chessPiece : TurnController::turnControllerInstance->whitePieces)
	{
		newSnapshot.chessPiecePositionDictioanry[chessPiece] = Vector2(chessPiece->pX, chessPiece->pY);
	}

	newSnapshot.totalMoveCount = counter + 1;
	newSnapshot.heuristicValue = heuristicStartingValue + moveHeuristicValueCalculator(lastX, lastY);
	newSnapshot.heuristicValue -= eachMoveMinusValue;

	snapshotQueue.push(newSnapshot);
}

int BlackKing::moveHeuristicValueCalculator(int lastX, int lastY)
{
	return (8 - (int)ChessBoard::chessBoardInstance->twoSquareDistance(lastX, lastY, WhiteKing::whiteKingInstance->pX, WhiteKing::whiteKingInstance->pY)) // will change
		* heuristicDistanceMultiplier;
}

void BlackKing::playBestMove()
{
	if(snapshotQueue.size() == 0)
	{
		Godot::print("YOU CANT WIN");
		TurnController::turnControllerInstance->stopTurn();
		return;
	}

	Snapshot currSnapshot = snapshotQueue.top();
	snapshotQueue.pop();

	currSS = currSnapshot;

	goSnapshot(currSnapshot);

	// TODO: PAWN ISSUE 

	if(currSnapshot.isMoving)
	{
		lookPosition(currSnapshot.movePosition.x, currSnapshot.movePosition.y);

		showArrow(currSnapshot.movePosition.x, currSnapshot.movePosition.y);
		waitNSecond(0.2, "unshowArrowAndMove");
	}
	else
	{
		lookPosition(currSnapshot.hitPiece->pX, currSnapshot.hitPiece->pY);

		Vector2 targetPivotPoint = ChessBoard::chessBoardInstance->getSquarePivotPosition(currSnapshot.hitPiece->pX, currSnapshot.hitPiece->pY);
		targetPivotPoint.y -= 20;

		targetIcon->set_position(targetPivotPoint);

		waitNSecond(0.3, "gunHitPiece");
	}

}

void BlackKing::goSnapshot(Snapshot currSnapshot)
{
	std::map<ChessPiece*, Vector2>::iterator iter = currSnapshot.chessPiecePositionDictioanry.begin();

	while (iter != currSnapshot.chessPiecePositionDictioanry.end())
	{
		if(iter->first->isDied)
		{
			iter->first->isDied = false;
			iter->first->set_visible(true);

			TurnController::turnControllerInstance->setWhitePiece(iter->first);

			//PieceSpawner::pieceSpawnerInstance->spawnChessPieceWhitType(iter->first, iter->second.x, iter->second.y); // create and move
		}

		iter->first->moveImmediately(iter->second.x, iter->second.y); // instant movement

		++iter;
	}

	heuristicStartingValue = currSnapshot.heuristicValue;
	counter = currSnapshot.totalMoveCount;
	moveCounter->set_text(String::num_int64(counter));

	ChessBoard::chessBoardInstance->clearBoardThreatCount();
}

void BlackKing::setGunType(int gunT)
{
	gunType = (GunType)gunT;
}

int BlackKing::getGunType()
{
	return (int)gunType;
}

void BlackKing::die()
{
	isDead = true;
	Godot::print("--------------YOU LOST-------------");
}

void BlackKing::setBlackKingParameters(int heuristicStartingVal, int eachMoveMinusVal, int heuristicDistanceMultip)
{
	heuristicStartingValue = heuristicStartingVal;
	eachMoveMinusValue = eachMoveMinusVal;
	heuristicDistanceMultiplier = heuristicDistanceMultip;
}

void BlackKing::calculateGunRotation(int x, int y)
{
	Vector2 newVector = Vector2(x - pX, y - pY);

	int dot = newVector.x * Vector2::RIGHT.x + newVector.y * Vector2::RIGHT.y;
	int det = newVector.x * Vector2::RIGHT.y - newVector.y * Vector2::RIGHT.x;

	float angle = atan2(det, dot);

	angle *= -1;
	angle *= 180 / 3.14;

	gunSprite->set_rotation_degrees(angle);

	if (angle < 0)
	{
		angle += 360;
	}

	if (angle > 270)
	{
		gunSprite->set_z_index(4);
		gunSprite->set_flip_v(false);
	}
	else if (angle > 180)
	{
		gunSprite->set_z_index(4);
		gunSprite->set_flip_v(true);
	}
	else if (angle > 90)
	{
		gunSprite->set_z_index(6);
		gunSprite->set_flip_v(true);
	}
	else if (angle > 0)
	{
		gunSprite->set_z_index(6);
		gunSprite->set_flip_v(false);
	}

}

void BlackKing::lookPosition(int x, int y)
{
	calculateGunRotation(x, y);

	current_animation = "Idle";
	String postfix;

	if (x > pX && y == pY)
		postfix = "0";
	else if (x > pX && y > pY)
		postfix = "1";
	else if (x == pX && y > pY)
		postfix = "2";
	else if (x < pX && y > pY)
		postfix = "3";
	else if (x < pX && y == pY)
		postfix = "4";
	else if (x < pX && y < pY)
		postfix = "5";
	else if (x == pX && y < pY)
		postfix = "6";
	else
		postfix = "7";

	current_animation += postfix;
	animatedSprite->set_animation(current_animation);
}

void BlackKing::showArrow(int x, int y)
{
	Vector2 pos = ChessBoard::chessBoardInstance->getSquarePivotPosition(x, y);
	pos.y -= 10;
	straightArrow->set_global_position(pos);
	crossArrow->set_global_position(pos);

	if (x > pX && y == pY)
	{
		straightArrow->set_rotation_degrees(90);
		straightArrow->set_visible(true);
	}
	else if (x > pX && y > pY)
	{
		crossArrow->set_rotation_degrees(-90);
		crossArrow->set_visible(true);
	}
	else if (x == pX && y > pY)
	{
		straightArrow->set_rotation_degrees(180);
		straightArrow->set_visible(true);
	}
	else if (x < pX && y > pY)
	{
		crossArrow->set_rotation_degrees(0);
		crossArrow->set_visible(true);
	}
	else if (x < pX && y == pY)
	{
		straightArrow->set_rotation_degrees(-90);
		straightArrow->set_visible(true);
	}
	else if (x < pX && y < pY)
	{
		crossArrow->set_rotation_degrees(90);
		crossArrow->set_visible(true);
	}
	else if (x == pX && y < pY)
	{
		straightArrow->set_rotation_degrees(0);
		straightArrow->set_visible(true);
	}
	else
	{
		crossArrow->set_rotation_degrees(180);
		crossArrow->set_visible(true);
	}
}

void BlackKing::unshowArrowAndMove()
{
	shotTimer->disconnect("timeout", this, "unshowArrowAndMove");

	straightArrow->set_visible(false);
	crossArrow->set_visible(false);

	jumpAudio->play();

	moveTo(currSS.movePosition.x, currSS.movePosition.y);
}

void BlackKing::gunHitPiece()
{
	shotTimer->disconnect("timeout", this, "gunHitPiece");

	shotAudio->play();

	bulletParticle->set_emitting(true);

	gun->hitPiece(currSS.hitPiece);
	targetIcon->set_position(Vector2(-50, -50));
}

void BlackKing::waitNSecond(float second, String callbackFunc)
{
	shotTimer->set_wait_time(second);
	shotTimer->set_one_shot(true);
	shotTimer->connect("timeout", this, callbackFunc);
	shotTimer->start();
}
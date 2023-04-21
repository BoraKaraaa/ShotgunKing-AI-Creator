#include "BlackKing.h"

#include <string> 

#include "Sniper.h"
#include "WhiteKing.h"
#include "PieceSpawner.h"

#include "CountDown.h"

BlackKing* BlackKing::blackKingInstance = NULL;

void BlackKing::_register_methods()
{
	register_method((char*)"_init", &BlackKing::_init);
	register_method((char*)"_ready", &BlackKing::_ready);
	register_method((char*)"gunHitPiece", &BlackKing::gunHitPiece);
	register_method((char*)"unshowArrowAndMove", &BlackKing::unshowArrowAndMove);

	register_method((char*)"bestGameMoves", &BlackKing::bestGameMoves);


	register_property<BlackKing, int>((char*)"gunType", &BlackKing::setGunType, &BlackKing::getGunType, (int)GunType::SNIPER); // Default SNIPER

	register_property<BlackKing, int>((char*)"heuristicStartingValue", &BlackKing::heuristicStartingValue, 0);
	register_property<BlackKing, int>((char*)"eachMoveMinusValue", &BlackKing::eachMoveMinusValue, 0); 
	register_property<BlackKing, int>((char*)"heuristicDistanceMultiplier", &BlackKing::heuristicDistanceMultiplier, 0);
}

void BlackKing::_init()
{
	blackKingInstance = this;

	currSS = NULL;

	switch (gunType)
	{
		case GunType::SNIPER:
			gun = new Sniper(2, 1); // sniper shot area 2*2 damage -> 1 (Default Values)
		break;
	}
}

void BlackKing::_ready()
{
	moveCounter = (Label*)get_node("/root/MainScene/GameUI/MoveCountAmount");
	totalTryAmountText = (Label*)get_node("/root/MainScene/GameUI2/TriedPositions");
	gameResultText = (Label*)get_node("/root/MainScene/GameUI2/GameResult");

	queuedPositionAmountText = (Label*)get_node("/root/MainScene/GameUI2/QueuedPosAmount");

	leaderBoard = (TextEdit*)get_node("/root/MainScene/LeaderBoard");

	animatedSprite = (AnimatedSprite*)get_node("/root/MainScene/PieceHolder/BlackKing/KingSprite/KingAnimation");
	gunSprite = (Sprite*)get_node("/root/MainScene/PieceHolder/BlackKing/Gun");

	targetIcon = (Sprite*)get_node("/root/MainScene/targetIcon");

	straightArrow = (Sprite*)get_node("/root/MainScene/PieceHolder/BlackKing/ArrowHolder/StraightArrow");
	crossArrow = (Sprite*)get_node("/root/MainScene/PieceHolder/BlackKing/ArrowHolder/CrossArrow");

	jumpAudio = (AudioStreamPlayer2D*)get_node("/root/MainScene/PieceHolder/BlackKing/AudioHolder/JumpAudio");
	shotAudio = (AudioStreamPlayer2D*)get_node("/root/MainScene/PieceHolder/BlackKing/AudioHolder/ShotAudio");

	bulletParticle = (Particles2D*)get_node("/root/MainScene/PieceHolder/BlackKing/BulletParticle");

	shotTimer = (Timer*)get_node("/root/MainScene/PieceHolder/BlackKing/ShotTimer");

	gunDamage = (LineEdit*)get_node("/root/MainScene/GameUI2/Sniper/GunDamage");
	gunRange = (LineEdit*)get_node("/root/MainScene/GameUI2/Sniper/GunRange");

	current_animation = "Idle";

	initGunParameters();
}

void BlackKing::initGunParameters()
{
	godot::String gunDamageString = (((LineEdit*)gunDamage)->get_text());
	godot::String gunRangeString = (((LineEdit*)gunRange)->get_text());

	if ((gunDamageString.is_valid_integer() && gunDamageString.to_int() != 0))
	{
		gun->gunDamage = gunDamageString.to_int();
	}

	if ((gunRangeString.is_valid_integer() && gunRangeString.to_int() != 0))
	{
		gun->gunRange = gunRangeString.to_int();
	}
}

BlackKing::~BlackKing()
{
	delete gun;
	gun = nullptr;
}

void BlackKing::takeTurn()
{
	if(firstMove)
	{
		setMinMoveToKillWhiteKing();
		firstMove = false;
	}

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

	gun->createAllPossibleHitNodes(currSS);

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

	if(currSS == NULL)
	{
		newSnapshot.parentSS = NULL;
	}
	else
	{
		newSnapshot.parentSS = currSS;
	}

	newSnapshot.isMoving = true;

	newSnapshot.movePosition = Vector2(lastX, lastY); 

	newSnapshot.chessPiecePositionDictioanry[this] = Vector2(lastX, lastY); 

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
	//Godot::print(String::num_int64(snapshotQueue.size()));

	totalTryAmountText->set_text(String::num_int64(++totalTry));
	queuedPositionAmountText->set_text(String::num_int64(snapshotQueue.size()));

	// Base Case

	if(snapshotQueue.size() == 0)
	{
		if(winGame)
		{
			gameResultText->set_text("YOU WON THE GAME");
			Godot::print("GAME FINISHED -> YOU WIN");

			playBestGame();
		}
		else
		{
			gameResultText->set_text("YOU LOST THE GAME");
			Godot::print("GAME FINISHED -> YOU LOSE");
		}

		TurnController::turnControllerInstance->stopTurn();
		CountDown::countDownInstance->stopCountDown();

		return;
	}

	Snapshot currSnapshot = snapshotQueue.top();
	snapshotQueue.pop();

	currSS = &currSnapshot;

	// Tree pruning with looking min successful path

	if(currSnapshot.totalMoveCount >= minSuccessfulPathLength)
	{
		// Kill the long path
		playBestMove();
		return;
	}

	// --------------------------------------------


	// Handle Endless Loops & Same Positions

	bool isSameSnapshotIncluded = false;

	for (Snapshot visitedSnapshot : visitedSnapshots)
	{
		Snapshot* res = isTwoSnapshotsEquals(&visitedSnapshot, &currSnapshot);

		if (res == &visitedSnapshot)
		{
			// Kill the endless loop
			playBestMove();

			return;
		}
		else if (res == &currSnapshot)
		{
			visitedSnapshots.erase(std::remove(visitedSnapshots.begin(), visitedSnapshots.end(), visitedSnapshot), visitedSnapshots.end());
			visitedSnapshots.push_back(*res);
			isSameSnapshotIncluded = true;
			break;
		}
	}

	if (!isSameSnapshotIncluded)
	{
		visitedSnapshots.push_back(currSnapshot);
	}

	// -----------------------

	goSnapshot(&currSnapshot);


	if(currSnapshot.isMoving)
	{
		lookPosition(currSnapshot.movePosition.x, currSnapshot.movePosition.y);

		showArrow(currSnapshot.movePosition.x, currSnapshot.movePosition.y);
		//waitNSecond(0.004, "unshowArrowAndMove");
		unshowArrowAndMove();
	}
	else
	{
		lookPosition(currSnapshot.hitPiece->pX, currSnapshot.hitPiece->pY);

		Vector2 targetPivotPoint = ChessBoard::chessBoardInstance->getSquarePivotPosition(currSnapshot.hitPiece->pX, currSnapshot.hitPiece->pY);
		targetPivotPoint.y -= 20;

		targetIcon->set_position(targetPivotPoint);

		//waitNSecond(0.005, "gunHitPiece");
		gunHitPiece();
	}

}

void BlackKing::goSnapshot(Snapshot* currSnapshot)
{
	std::map<ChessPiece*, Vector2>::iterator iter = currSnapshot->chessPiecePositionDictioanry.begin();

	for (ChessPiece* chessPiece : TurnController::turnControllerInstance->whitePieces)
	{
		chessPiece->isDied = true;
		chessPiece->set_visible(false);
	}

	TurnController::turnControllerInstance->whitePieces.clear();

	while (iter != currSnapshot->chessPiecePositionDictioanry.end())
	{
		if(iter->first == this) // Skip BlackKing Position
		{
			iter++;
			continue;
		}

		if(iter->first->isDied)
		{
			iter->first->isDied = false;
			iter->first->set_visible(true);

			TurnController::turnControllerInstance->setWhitePiece(iter->first);
		}

		iter->first->moveImmediately(iter->second.x, iter->second.y); // instant movement

		++iter;
	}

	heuristicStartingValue = currSnapshot->heuristicValue;
	counter = currSnapshot->totalMoveCount;
	moveCounter->set_text(String::num_int64(counter));

	ChessBoard::chessBoardInstance->clearBoardThreatCount();
}

Snapshot* BlackKing::isTwoSnapshotsEquals(Snapshot* s1, Snapshot* s2)
{

	if(s1->isMoving != s2->isMoving)
	{
		return nullptr;
	}

	std::map<ChessPiece*, Vector2>::iterator iter1 = s1->chessPiecePositionDictioanry.begin();

	while (iter1 != s1->chessPiecePositionDictioanry.end())
	{
		std::map<ChessPiece*, Vector2>::iterator iter2 = s2->chessPiecePositionDictioanry.begin();

		bool sameChessPiece = false;

		while (iter2 != s2->chessPiecePositionDictioanry.end())
		{

			if( (iter2->first == iter1->first) && (iter2->second.x == iter1->second.x) && 
				(iter2->second.y == iter1->second.y))
			{
				sameChessPiece = true;
				break;
			}

			++iter2;
		}

		if(!sameChessPiece)
		{
			return nullptr;
		}

		++iter1;
	}

	if(s1->totalMoveCount > s2->totalMoveCount)
	{
		return s2;
	}
	else if(s1->totalMoveCount <= s2->totalMoveCount)
	{
		return s1; 
	}

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
	//shotTimer->disconnect("timeout", this, "unshowArrowAndMove");

	straightArrow->set_visible(false);
	crossArrow->set_visible(false);

	//jumpAudio->play();

	moveTo((*currSS).movePosition.x, (*currSS).movePosition.y);
}

void BlackKing::gunHitPiece()
{
	//shotTimer->disconnect("timeout", this, "gunHitPiece");

	//shotAudio->play();

	bulletParticle->set_emitting(true);

	if((*currSS).hitPiece == WhiteKing::whiteKingInstance)
	{
		if(WhiteKing::whiteKingInstance->canKill(gun->gunDamage))
		{
			winGame = true;
			minSuccessfulPathLength = (*currSS).totalMoveCount;
			Godot::print(String::num_int64(minSuccessfulPathLength));

			updateLeaderBoard(minSuccessfulPathLength);

			bestGameLastSS = currSS;
			
			if(minSuccessfulPathLength <= minMoveToKillWhiteKing)
			{
				Godot::print("GAME FINISHED -> YOU WIN");
				gameResultText->set_text("YOU WON THE GAME");
				TurnController::turnControllerInstance->stopTurn();
				CountDown::countDownInstance->stopCountDown();

				playBestGame();
			}
			
			
		}
	}

	gun->hitPiece((*currSS).hitPiece);
	targetIcon->set_position(Vector2(-50, -50));
}

void BlackKing::playBestGame()
{
	/*
	Snapshot* iter = bestGameLastSS;
	while(iter != NULL)
	{
		bestGameSnapshotStack.push(iter);
		iter = iter->parentSS;
	}
	*/

	//bestGameMoves();
}

void BlackKing::bestGameMoves()
{
	shotTimer->disconnect("timeout", this, "bestGameMoves");

	if(bestGameLastSS->parentSS == NULL)
	{
		return;
	}

	goSnapshot(bestGameLastSS);

	bestGameLastSS = bestGameLastSS->parentSS;

	//goSnapshot(bestGameSnapshotStack.top());
	//bestGameSnapshotStack.pop();

	waitNSecond(1, "bestGameMoves");
}

void BlackKing::updateLeaderBoard(int moveCount)
{
	String currText = leaderBoard->get_text();
	currText += String::num_int64(moveCount) + "M " + String::num_int64(CountDown::countDownInstance->time) + "S\n";
	leaderBoard->set_text(currText);
}

void BlackKing::setMinMoveToKillWhiteKing()
{
	minMoveToKillWhiteKing = ChessBoard::chessBoardInstance->twoSquareDistanceWithSquare(this->pX, this->pY,
		WhiteKing::whiteKingInstance->pX, WhiteKing::whiteKingInstance->pY) - gun->gunRange + 1;

	if (minMoveToKillWhiteKing < 1)
	{
		minMoveToKillWhiteKing = 1;
	}

	Godot::print(String::num_int64(minMoveToKillWhiteKing));
}

void BlackKing::waitNSecond(float second, String callbackFunc)
{
	shotTimer->set_wait_time(second);
	shotTimer->set_one_shot(true);
	shotTimer->connect("timeout", this, callbackFunc);
	shotTimer->start();
}
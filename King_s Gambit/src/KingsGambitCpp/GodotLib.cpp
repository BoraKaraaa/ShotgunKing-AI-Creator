#include "TurnController.h"
#include "ChessBoard.h"

#include "PieceSpawner.h"

#include "CameraNoise.h"

#include "ChessPiece.h"

#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "WhiteKing.h"

#include "BlackKing.h"

using namespace godot;

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options * o)
{
	Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options * o)
{
	Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void* handle)
{
	Godot::nativescript_init(handle);

	register_class<TurnController>();
	register_class<ChessBoard>();

	register_class<PieceSpawner>();

	register_class<CameraNoise>();

	// Pieces
	register_class<ChessPiece>();

	register_class<Pawn>();
	register_class<Knight>();
	register_class<Bishop>();
	register_class<Rook>();
	register_class<Queen>();
	register_class<WhiteKing>();

	register_class<BlackKing>();

}
#pragma once
#include <Godot.hpp>
#include <Node2d.hpp>

using namespace godot;

enum class TurnType
{
	BLACK = 0,
	WHITE = 1
};

class TurnController : public Node2D 
{
	GODOT_CLASS(TurnController, Node2D);

public:
	static TurnType pTurnType;

	static void _register_methods();
	void _init();

	TurnController();
	~TurnController();

};


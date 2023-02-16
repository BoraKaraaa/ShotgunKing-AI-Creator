#pragma once

#include <Godot.hpp>
#include <Node2D.hpp>

using namespace godot;

class Test : public Node2D
{
	GODOT_CLASS(Test, Node2D);

public:
	static void _register_methods();
	void _init();
	void _process(float delta);

	Test();
	~Test();


};


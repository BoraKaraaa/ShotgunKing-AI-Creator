#pragma once

#include <Godot.hpp>
#include <Label.hpp>

using namespace godot;


class CountDown : public Label
{
	GODOT_CLASS(CountDown, Label);

public:

	static CountDown* countDownInstance;

	float time = 0;
	bool timer_on = false;

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float);

	CountDown();
	~CountDown();

	void startCountDown();
	void stopCountDown();


};


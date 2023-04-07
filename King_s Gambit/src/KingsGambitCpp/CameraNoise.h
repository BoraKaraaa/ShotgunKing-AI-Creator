#pragma once

#include <Godot.hpp>

#include <Camera2D.hpp>

#include <Tween.hpp>
#include <Timer.hpp>

using namespace godot;

class CameraNoise : public Camera2D
{
	GODOT_CLASS(CameraNoise, Camera2D);

public:
	static CameraNoise* cameraNoiseInstance;
	static void _register_methods();
	
	void _init();
	
	void _ready();
	
	void _process(float delta);

	CameraNoise();
	~CameraNoise();
	
	void doCameraShake(int, float, int);
	void endCameraShake();
	
private:
	Tween* tween;
	Timer* timer;
	
	float shake_amount;
	Vector2 default_offet;

	int rand_range(int, int);
	
};


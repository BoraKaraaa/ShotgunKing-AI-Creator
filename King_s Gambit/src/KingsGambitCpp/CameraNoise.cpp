#include "CameraNoise.h"

CameraNoise* CameraNoise::cameraNoiseInstance = NULL;

void CameraNoise::_register_methods()
{
	register_method((char*)"_init", &CameraNoise::_init);
	register_method((char*)"_ready", &CameraNoise::_ready);
	register_method((char*)"_process", &CameraNoise::_process);
	register_method((char*)"endCameraShake", &CameraNoise::endCameraShake);
}


CameraNoise::CameraNoise() { }

CameraNoise::~CameraNoise() { }

void CameraNoise::_ready()
{
	tween = (Tween*)get_node("/root/MainScene/Camera2D/Tween");
	timer = (Timer*)get_node("/root/MainScene/Camera2D/Timer");

	set_process(false);
}

void CameraNoise::_init()
{
	cameraNoiseInstance = this;

	shake_amount = 0;
	default_offet = get_offset();
}

void CameraNoise::_process(float delta)
{
	set_offset(Vector2(rand_range(-shake_amount, shake_amount), rand_range(shake_amount, -shake_amount)) * delta + default_offet);
}

int CameraNoise::rand_range(int min, int max)
{
	return min + (rand() % static_cast<int>(max - min + 1));
}

void CameraNoise::doCameraShake(int new_shake, float shake_time = 0.2, int shake_limit = 100)
{
	shake_amount += new_shake;

	if(shake_amount > shake_limit)
	{
		shake_amount = shake_limit;
	}

	timer->set_wait_time(shake_time);

	tween->stop_all();

	set_process(true);

	timer->connect("timeout", this, "endCameraShake");
	timer->start();
}

void CameraNoise::endCameraShake()
{
	shake_amount = 0;
	set_process(false);

	tween->interpolate_property(this, "offset", get_offset(), default_offet, 0.1, Tween::TRANS_QUAD, Tween::EASE_IN_OUT);
	tween->start();

	timer->disconnect("timeout", this, "endCameraShake");
}

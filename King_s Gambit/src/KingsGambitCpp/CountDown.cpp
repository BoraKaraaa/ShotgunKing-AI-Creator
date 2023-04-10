#include "CountDown.h"

CountDown* CountDown::countDownInstance = NULL;

void CountDown::_register_methods()
{
	register_method((char*)"_init", &CountDown::_init);
	register_method((char*)"_ready", &CountDown::_ready);
	register_method((char*)"_process", &CountDown::_process);
}

CountDown::CountDown() {}

CountDown::~CountDown() {}

void CountDown::_init()
{
	countDownInstance = this;
}

void CountDown::_ready()
{
}

void CountDown::_process(float delta)
{
	if(timer_on)
	{
		time += delta;

		int second = (int)time % 60;
		int minute = ((int)time / 60) % 60;
		String lastTime = String::num_int64(minute) + ":" + String::num_int64(second);
		set_text(lastTime);
	}
}

void CountDown::startCountDown()
{
	timer_on = true;
}

void CountDown::stopCountDown()
{
	timer_on = false;
}
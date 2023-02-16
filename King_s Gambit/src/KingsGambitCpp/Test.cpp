#include "Test.h"
#include <iostream>

using namespace godot;

void Test::_register_methods()
{
	register_method((char*)"_init", &Test::_init);
}

Test::Test() { }

Test::~Test() { }

void Test::_init()
{
	Godot::print("COK MANTIKLI");
}

void Test::_process(float delta) { }


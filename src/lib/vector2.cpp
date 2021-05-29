#include "vector2.h"

vector2::vector2(float x, float y) : X(x), Y(y) {}

void vector2::set_vector(float x, float y)
{
	X = x;
	Y = y;
}

void vector2::move_vector(float x, float y) {
	X += x;
	Y += y;
}
#include "math.h"
#include <corecrt_math.h>

float Math::LinearMapToRange(float value, float inputMin, float inputMax, float outputMin, float outputMax)
{
	return (value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin;
}

bool Math::Between(float value, float min, float max)
{
	return (value > min && value < max);
}

float Math::EaseInOutQuart(float x) {
	return x < 0.5f ? 8.0f * x * x * x * x : 1.0f - pow(-2.0f * x + 2.0f, 4.0f) / 2.0f;
}

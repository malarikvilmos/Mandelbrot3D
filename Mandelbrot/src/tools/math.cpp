#include "math.h"

float Math::LinearMapToRange(float value, float inputMin, float inputMax, float outputMin, float outputMax)
{
	return (value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin;
}

bool Math::Between(float value, float min, float max)
{
	return (value > min && value < max);
}

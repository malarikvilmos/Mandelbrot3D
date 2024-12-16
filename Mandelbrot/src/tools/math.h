#pragma once

namespace Math
{
	float LinearMapToRange(float value, float inputMin, float inputMax, float outputMin, float outputMax);
	bool Between(float value, float min, float max);
	float EaseInOutQuart(float x);
}


#pragma once
#include <string>

enum ColorScheme
{
	Hue,
	Grayscale,
	Scary,
	Fourth
};

std::string GetColorName(ColorScheme cScheme);

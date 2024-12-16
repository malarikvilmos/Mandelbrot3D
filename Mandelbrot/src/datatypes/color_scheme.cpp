#include "color_scheme.h"

std::string GetColorName(ColorScheme cScheme)
{
	switch (cScheme)
	{
	case ColorScheme::Hue:
		return "hue";
		break;
	case ColorScheme::Grayscale:
		return "grayscale";
		break;
	case ColorScheme::Scary:
		return "scary";
		break;
	case ColorScheme::Fourth:
		return "fourth";
		break;
	default:
		return "hue";
	}
};

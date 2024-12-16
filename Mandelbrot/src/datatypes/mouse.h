#pragma once

struct Mouse
{
	int32_t x = 0;
	int32_t y = 0;
	bool heldLeft   = false;
	bool heldMiddle = false;
	bool heldRight  = false;
	int32_t scroll = 0;
};

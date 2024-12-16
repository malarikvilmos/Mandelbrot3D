#pragma once
#include <cstdint>

class Image
{
public:
	inline static void* app = nullptr;
	uint8_t* data = nullptr;
	int width, height, channels;

	Image(int32_t width, int32_t height, int32_t channels);
	Image(Image* image);
	Image(const char* filename, bool flipVertical = false, bool rgba = false);
	~Image();

	void Save(const char* path);
};

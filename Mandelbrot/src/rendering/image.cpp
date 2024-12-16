#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <iostream>
#include <glad4_3/glad.h>

Image::Image(int32_t width, int32_t height, int32_t channels)
{
	this->width = width;
	this->height = height;
	this->channels = channels;
	this->data = new uint8_t[width * height * channels];
}

Image::Image(Image* image)
{
	width = image->width;
	height = image->height;
	channels = image->channels;
	this->data = new uint8_t[width * height * channels];
	memcpy(this->data, image->data, width * height * channels);
}

Image::Image(const char* filename, bool flip, bool rgba)
{
	stbi_set_flip_vertically_on_load(flip);

	if (rgba)
		data = stbi_load(filename, &width, &height, &channels, 4);
	else
		data = stbi_load(filename, &width, &height, &channels, 0);
	if (data == nullptr) {
		std::cout << "Error loading image at: " << filename << std::endl;
		return;
	}
}

Image::~Image()
{
	stbi_image_free(data);
}

void Image::Save(const char* path)
{
	if (this == nullptr) return;
	stbi_flip_vertically_on_write(1);
	stbi_write_png(path, width, height, channels, data, width * channels);
}

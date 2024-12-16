#pragma once
#include <string>

class Config
{
public:
    inline static int32_t width;
    inline static int32_t height;
    inline static int32_t fullscreen;
    inline static int32_t vsync;
    inline static int32_t precision;
    inline static float sensitivity;

    static int32_t Read();
    static int32_t Save();
};

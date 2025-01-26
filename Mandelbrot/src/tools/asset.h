#pragma once
#include <string>
#include <map>
#include <cstdint>

struct Asset
{
	uint32_t begin;
	uint32_t end;
	uint8_t* content;
};

class AssetManager
{
public:
	inline static std::map<const std::string, Asset> files;
	static void Load(const char* path);
};

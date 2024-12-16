#include "asset.h"
#include <fstream>
#include <cassert>

void AssetManager::Load(const char* path)
{
	std::ifstream asset_file(path, std::ios_base::binary);
	std::string line;
	for (;;) {
		std::getline(asset_file, line, (char)'&');
		if (line == "@META_END") break;
		Asset asset;
		std::string filepath = strtok(line.data(), "$");
		asset.begin = strtoul(strtok(nullptr, "$"), nullptr, 10);
		asset.end = strtoul(strtok(nullptr, "$"), nullptr, 10);
		files[filepath] = asset;
	}
	uint32_t pos = asset_file.tellg();
	asset_file.close();

	FILE* fptr = fopen(path, "rb");
	fseek(fptr, pos, SEEK_SET);
	for (auto& [filename, asset] : files) {
		const uint32_t ssize = asset.end - asset.begin;
		uint8_t* buf = new uint8_t[ssize];
		fread(buf, sizeof(uint8_t), ssize, fptr);
		asset.content = (uint8_t*)malloc(ssize);
		if (asset.content != 0) memcpy(asset.content, buf, ssize);
	}
	fclose(fptr);
}

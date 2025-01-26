#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <filesystem>
#include <iostream>
#include "../datatypes/precision.h"

int32_t Config::Read()
{
    Config::width = 800;
    Config::height = 600;
    Config::vsync = 1;
    Config::fullscreen = 0;
    Config::precision = Precision::Automatic;
    Config::sensitivity = 0.5f;

    const char* filepath = "config.ini";
    FILE* file = fopen(filepath, "r");

    if (file == nullptr) {
        perror("Error opening config file for read.");
        Config::Save();
        return 1;
    }

    char buffer[1024];
    char* token = nullptr;
    while (!feof(file) && fgets(buffer, 512, file)) {
        token = strtok(buffer, "= ");
        if (strcmp(token, "width") == 0) {
            token = strtok(nullptr, "= ");
            int32_t parsedValue = atoi(token);
            Config::width = parsedValue != 0 ? parsedValue : 800;
        }
        else if (strcmp(token, "height") == 0) {
            token = strtok(nullptr, "= ");
            int32_t parsedValue = atoi(token);
            Config::height = parsedValue != 0 ? parsedValue : 600;
        }
        else if (strcmp(token, "fullscreen") == 0) {
            token = strtok(nullptr, "= ");
            int32_t parsedValue = atoi(token);
            Config::fullscreen = parsedValue != 0 ? parsedValue : 0;
        }
        else if (strcmp(token, "vsync") == 0) {
            token = strtok(nullptr, "= ");
            int32_t parsedValue = atoi(token);
            Config::vsync = parsedValue != 0 ? parsedValue : 0;
        }
    }

    fclose(file);
    return 0;
}

int32_t Config::Save()
{
    const char* filepath = "config.ini";
    FILE* file = fopen(filepath, "w");

    if (file == nullptr) {
        perror("Error opening config file for save.");
        return 1;
    }

    fputs("width = ", file);
    fputs(std::to_string(Config::width).c_str(), file);
    fputs("\n", file);

    fputs("height = ", file);
    fputs(std::to_string(Config::height).c_str(), file);
    fputs("\n", file);

    fputs("fullscreen = ", file);
    fputs(std::to_string(Config::fullscreen).c_str(), file);
    fputs("\n", file);

    fputs("vsync = ", file);
    fputs(std::to_string(Config::vsync).c_str(), file);
    fputs("\n", file);

    fclose(file);
    return 0;
}

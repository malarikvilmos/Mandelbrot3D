#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <filesystem>
#include <iostream>
#include "../datatypes/precision.h"


int32_t Config::Read()
{
    const char* filepath = "config.ini";
    FILE* file = fopen(filepath, "r");

    if (file == nullptr) {
        perror("Error opening config file for read.");
        Config::width = 800;
        Config::height = 600;
        Config::vsync = 1;
        Config::fullscreen = 0;
        Config::precision = Precision::Automatic;
        Config::sensitivity = 0.5f;
        Config::Save();
        return 1;
    }

    char buffer[320];
    char* token = nullptr;
    while (!feof(file) && fgets(buffer, 300, file)) {
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
        else if (strcmp(token, "sensitivity") == 0) {
            token = strtok(nullptr, "= ");
            float parsedValue = atof(token);
            Config::sensitivity = parsedValue != 0 ? parsedValue : 0.5f;
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

    char vbuffer[120];
    _itoa(Config::width, vbuffer, 10);
    fputs("width = ", file);
    fputs(vbuffer, file);
    fputs("\n", file);

    _itoa(Config::height, vbuffer, 10);
    fputs("height = ", file);
    fputs(vbuffer, file);
    fputs("\n", file);

    _itoa(Config::fullscreen, vbuffer, 10);
    fputs("fullscreen = ", file);
    fputs(vbuffer, file);
    fputs("\n", file);

    _itoa(Config::vsync, vbuffer, 10);
    fputs("vsync = ", file);
    fputs(vbuffer, file);
    fputs("\n", file);

    snprintf(vbuffer, sizeof(vbuffer), "%.3g", Config::sensitivity);
    fputs("sensitivity = ", file);
    fputs(vbuffer, file);
    fputs("\n", file);

    fclose(file);
    return 0;
}

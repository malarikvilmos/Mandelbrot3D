#include "benchmark.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <thread>


void Benchmark::DoBenchmark()
{
	lastFrame = now;
	++framecount;

	if (now - previousTime >= 1.0) {
		framerate = framecount;
		framecount = 0;
		previousTime = now;
	}

	now = SDL_GetTicks64() * 0.001f;
	deltaTime = now - lastFrame;
}

void Benchmark::MeasureFrametimeBegin()
{
	frameStart = std::chrono::steady_clock::now();
}

void Benchmark::MeasureFrametimeEnd()
{
	frameEnd = std::chrono::steady_clock::now();
	frametime = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart).count() * 0.001f;
	leftoverFrametime = targetFrametime - frametime;
}

void Benchmark::MeasureBegin()
{
	t1 = std::chrono::steady_clock::now();
}

float Benchmark::MeasureEnd()
{
	t2 = std::chrono::steady_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	return (float)duration.count();
}

#pragma once
#include <chrono>

class Benchmark
{
public:
	inline static double deltaTime = 0.0f;
	inline static int32_t framerate = 0;
	inline static float frametime = 0.0f;

	inline static std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - std::chrono::high_resolution_clock::now());

	static void DoBenchmark();
	static void MeasureFrametimeBegin();
	static void MeasureFrametimeEnd();
	static void MeasureBegin();
	static float MeasureEnd();

private:
	inline static std::chrono::steady_clock::time_point t1, t2;
	inline static std::chrono::steady_clock::time_point frameStart, frameEnd;
	inline static int32_t framecount = 0;
	inline static float lastFrame = 0.0f;
	inline static float now = 0.0f;
	inline static float previousTime = 0.0f;
	inline static float targetFrametime = 0.0f;
	inline static float leftoverFrametime = 0.0f;
};

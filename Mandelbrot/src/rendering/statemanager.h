#pragma once
#include "../tools/config.h"

enum class MSAA_Samples
{
	MSAA_OFF  = 0,
	MSAA_2	  = 2,
	MSAA_4	  = 4,
	MSAA_8	  = 8
};

class StateManager
{
public:
	inline static bool isWireframe = false;
	inline static MSAA_Samples msaa_count = MSAA_Samples::MSAA_OFF;
	inline static bool drawFrameStats = true;
	inline static bool minimized = false;

	static void Init();
	static void Blending(bool value);
	static void DepthTest(bool value);
	static void FaceCulling(bool value);
	static void Multisampling(bool value);
	static void MultisamplingCount(MSAA_Samples count);
	static void ClearColor(float r, float g, float b, float a);
	static void Wireframe(bool value);
	static void FrameStats(bool drawFrameStats);
	static void Minimize(bool value);
};

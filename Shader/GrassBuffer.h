#pragma once
#include "ShaderBuffer.h"

class GrassBuffer : public ShaderBuffer
{
public:
	GrassBuffer()
		: ShaderBuffer(0,0,0, sizeof(GS_DATA), 0)
	{	
		gsData._grassHeight = 0.7f;
		gsData._grassWidth = 0.04f;
		gsData._timer = 0;
		gsData._wind = 0.05f;
		UpdateGSBuffer(&gsData, sizeof(GS_DATA));
	}

	void SetGrassData(float height, float width, float wind) {
		gsData._grassHeight = height;
		gsData._grassWidth = width;
		gsData._wind = wind;
		UpdateGSBuffer(&gsData, sizeof(GS_DATA));
	}

	struct GS_DATA
	{
		float _grassHeight;
		float _grassWidth;
		float _timer;
		float _wind;
	};
	void Update()
	{
		DWORD time = timeGetTime();

		float timer = (float)(time) / 1000;
		gsData._timer = timer;
		UpdateGSBuffer(&gsData, sizeof(GS_DATA));
	}


private:

	GS_DATA gsData;
};
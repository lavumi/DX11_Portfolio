#pragma once
#include "ShaderBuffer.h"

class RainBuffer : public ShaderBuffer
{
public:
	RainBuffer()
		: ShaderBuffer(0,0, 0,0, sizeof(PS_DATA))
	{
	}

	void Update()
	{
		DWORD time = timeGetTime();

		float timer = (float)(time) / 1000;
		psData.time.x = timer;
		UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}


	struct PS_DATA
	{
		D3DXVECTOR4 time;
	};


private:

	PS_DATA psData;
};
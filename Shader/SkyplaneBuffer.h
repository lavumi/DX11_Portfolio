#pragma once
#include "ShaderBuffer.h"

class SkyplaneBuffer : public ShaderBuffer
{
public:
	SkyplaneBuffer()
		: ShaderBuffer(0,0,0, 0,sizeof(PS_DATA))
	{
		psData.scale = 0.3f;
		psData.translation = 0.0f;
		
		speed = 0.0001f;
		UserInterface::AddSkyplane(&psData.scale, &speed);
		UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}



private:
	void Update()
	{
		psData.translation += speed / 4;
		if (psData.translation >= 1.0f) {
			psData.translation -= 1.0f;
		}
		UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}


	struct PS_DATA
	{
		float translation;
		float scale;
		D3DXVECTOR2 pad;
	};

	float speed;
	PS_DATA psData;
};
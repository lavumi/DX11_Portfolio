#pragma once
#include "ShaderBuffer.h"

class SkydomeBuffer : public ShaderBuffer
{
public:
	SkydomeBuffer()
		: ShaderBuffer(0, 0, sizeof(PS_DATA))
	{

		//UpdateVSBuffer(&vsData, 0);
		//UpdatePSBuffer(&psData, 0);
	}







private:
	struct PS_DATA
	{
		D3DXVECTOR3 lightDir;
		float padding;
	};
	void Update()
	{
		LightManager::Get()->GetLightDirection(&psData.lightDir);
		UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}
	PS_DATA psData;
};
#pragma once
#include "ShaderBuffer.h"

class DepthShadowBuffer : public ShaderBuffer
{
public:
	DepthShadowBuffer()
		: ShaderBuffer(sizeof(VS_DATA), 0, 0)
	{
		Update();
		//UpdateVSBuffer(&vsData, 0);
		//UpdatePSBuffer(&psData, 0);
	}







private:
	void Update()
	{
		LightManager::Get()->GetView(&vsData.lightViewXProjection);
		LightManager::Get()->GetProjection(&vsData.proj);
		UpdateVSBuffer(&vsData, sizeof(VS_DATA));
	}

	struct VS_DATA
	{
		D3DXMATRIX lightViewXProjection;
		D3DXMATRIX proj;
	};
	VS_DATA vsData;

};
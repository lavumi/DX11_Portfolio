#pragma once
#include "ShaderBuffer.h"

class ShadowBuffer : public ShaderBuffer
{
public:
	ShadowBuffer()
		: ShaderBuffer(sizeof(VS_DATA), 0, sizeof(PS_DATA))
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

		LightManager::Get()->GetLightDirection(&psData.lightDir);
		UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}

	struct VS_DATA
	{
		D3DXMATRIX lightViewXProjection;
		D3DXMATRIX proj;
	};
	VS_DATA vsData;


	struct PS_DATA
	{
		D3DXVECTOR3 lightDir;
		float padding;
	};
	PS_DATA psData;
};
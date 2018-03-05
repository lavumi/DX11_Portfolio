#pragma once
#include "ShaderBuffer.h"

class TerrainBuffer : public ShaderBuffer
{
public:
	TerrainBuffer()
		: ShaderBuffer(sizeof(VS_DATA), 0, sizeof(PS_DATA))
	{
		psData.ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		psData.diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
		psData.globalAmbient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);

		vsData.clipPlane = D3DXPLANE(0, 0, 0, 0);
	}

	void SetPLANE(D3DXPLANE plane) {
		vsData.clipPlane = plane;
	}

	void SetMaterial(D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR globalAmbient) {
		psData.ambient = ambient;
		psData.diffuse = diffuse;
		psData.globalAmbient = globalAmbient;
	}

	void Update(D3DXMATRIX world)
	{
		vsData.lightDirection = LightManager::Get()->GetLightData()->lightDirection;

		D3DXMATRIX invWorld;
		D3DXMatrixInverse(&invWorld, nullptr, &world);
		D3DXMatrixTranspose(&vsData.invTransWorld, &invWorld);
	}

	struct VS_DATA
	{
		D3DXVECTOR3 lightDirection;
		D3DXMATRIX invTransWorld;
		D3DXPLANE clipPlane;
		float padding;
	};
	struct PS_DATA
	{
		D3DXCOLOR ambient;
		D3DXCOLOR diffuse;
		D3DXCOLOR globalAmbient;
		D3DXMATRIX cameraSpaceBuffer;
	};


private:
	
	VS_DATA vsData;
	PS_DATA psData;
};
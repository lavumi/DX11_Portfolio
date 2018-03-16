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
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		vsData.invTransWorld = world;
		vsData.clipPlane = D3DXPLANE(0, 1, 0, 7.9f);


		UpdateVSBuffer(&vsData, sizeof(VS_DATA));
		UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}

	void SetPLANE(D3DXPLANE& plane) {
		vsData.clipPlane = plane;
		UpdateVSBuffer(&vsData, sizeof(VS_DATA));
	}

	void SetMaterial(D3DXCOLOR& ambient, D3DXCOLOR& diffuse, D3DXCOLOR& globalAmbient) {
		psData.ambient = ambient;
		psData.diffuse = diffuse;
		psData.globalAmbient = globalAmbient;

		UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}

	void SetWorld(D3DXMATRIX& world) {
	//	ShaderBuffer::SetWorld(world);

		D3DXMATRIX invWorld;
		D3DXMatrixInverse(&invWorld, nullptr, &world);
		D3DXMatrixTranspose(&vsData.invTransWorld, &invWorld);

		UpdateVSBuffer(&vsData, sizeof(VS_DATA));
	}

	void Update()
	{
		vsData.lightDirection = LightManager::Get()->GetLightData()->lightDirection;

		UpdateVSBuffer(&vsData, sizeof(VS_DATA));
	}

	struct VS_DATA
	{
		D3DXPLANE clipPlane;
		D3DXMATRIX invTransWorld;
		D3DXVECTOR3 lightDirection;
		float renderReverse;
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
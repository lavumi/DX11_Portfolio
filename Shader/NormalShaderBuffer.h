#pragma once
#include "ShaderBuffer.h"

class NormalShaderBuffer : public ShaderBuffer
{
public:
	NormalShaderBuffer()
		: ShaderBuffer(sizeof(VS_DATA), 0, sizeof(PS_DATA))
	{
		psData.ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		psData.diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
		psData.specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		psData.shininess = 70;
		psData.globalAmbient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
		psData.scale = 0.1f;
		psData.layer = 32;

		UpdateVSBuffer(&vsData, sizeof(VS_DATA));
		UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}
	void SetMaterial(D3DXCOLOR& ambient, D3DXCOLOR& diffuse, D3DXCOLOR& globalAmbient) {
		psData.ambient = ambient;
		psData.diffuse = diffuse;
		psData.globalAmbient = globalAmbient;

		UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}

	void SetWorld(D3DXMATRIX& world) {
		ShaderBuffer::SetWorld(world);

		D3DXMATRIX invWorld;
		D3DXMatrixInverse(&invWorld, nullptr, &world);
		D3DXMatrixTranspose(&vsData.invTransWorld, &invWorld);

		//UpdateVSBuffer(&vsData, sizeof(VS_DATA));
	}
	void Update()
	{
		vsData.lightDirection = LightManager::Get()->GetLightData()->lightDirection;
		Camera::Get()->GetPosition(&(vsData.cameraPos));
		UpdateVSBuffer(&vsData, sizeof(VS_DATA));
	}

	struct VS_DATA
	{
		D3DXVECTOR3 cameraPos;
		float padding;
		D3DXMATRIX invTransWorld;
		D3DXVECTOR3 lightDirection;
		float padding2;

	};
	struct PS_DATA
	{
		D3DXCOLOR ambient;
		D3DXCOLOR diffuse;
		D3DXCOLOR specular;
		D3DXCOLOR globalAmbient;
		float shininess;
		float scale;
		float layer;
		float padding;
	};


private:
	
	VS_DATA vsData;
	PS_DATA psData;
};
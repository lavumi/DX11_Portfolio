#pragma once
#include "ShaderBuffer.h"

class TerrainBuffer : public ShaderBuffer
{
public:
	TerrainBuffer()
		: ShaderBuffer(0,sizeof(HS_DATA),sizeof(DS_Data), 0, sizeof(PS_DATA))
	{
		psData.ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		psData.diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
		psData.globalAmbient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		gdData.invTransWorld = world;
		gdData.clipPlane = D3DXPLANE(0, 1, 0, 7.9f);


		UpdateDSBuffer(&gdData, sizeof(DS_Data));
		UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}

	void SetPLANE(D3DXPLANE& plane) {
		gdData.clipPlane = plane;
		UpdateDSBuffer(&gdData, sizeof(DS_Data));
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
		D3DXMatrixTranspose(&gdData.invTransWorld, &invWorld);

		UpdateDSBuffer(&gdData, sizeof(DS_Data));
	}

	void Update()
	{
		Camera::Get()->GetPosition(&hsData.cameraPos);
		UpdateHSBuffer(&hsData, sizeof(HS_DATA));
	}

	struct DS_Data
	{
		D3DXPLANE clipPlane;
		D3DXMATRIX invTransWorld;
	};
	struct PS_DATA
	{
		D3DXCOLOR ambient;
		D3DXCOLOR diffuse;
		D3DXCOLOR globalAmbient;
	//	D3DXMATRIX cameraSpaceBuffer;
	};
	struct HS_DATA {
		D3DXVECTOR3 cameraPos;
		float padding;
	};

private:
	HS_DATA hsData;
	DS_Data gdData;
	PS_DATA psData;
};
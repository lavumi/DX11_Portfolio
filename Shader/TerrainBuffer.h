#pragma once
#include "ShaderBuffer.h"

class TerrainBuffer : public ShaderBuffer
{
public:
	TerrainBuffer()
		: ShaderBuffer(0,sizeof(DS_Data), 0, sizeof(PS_DATA))
	{
		psData.ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		psData.diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
		psData.globalAmbient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		gdsData.invTransWorld = world;
		gdsData.clipPlane = D3DXPLANE(0, 1, 0, 7.9f);


		UpdateDSBuffer(&gdsData, sizeof(DS_Data));
		UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}

	void SetPLANE(D3DXPLANE& plane) {
		gdsData.clipPlane = plane;
		UpdateDSBuffer(&gdsData, sizeof(DS_Data));
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
		D3DXMatrixTranspose(&gdsData.invTransWorld, &invWorld);

		UpdateDSBuffer(&gdsData, sizeof(DS_Data));
	}

	void Update()
	{
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


private:
	
	DS_Data gdsData;
	PS_DATA psData;
};
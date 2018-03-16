#pragma once
#include "ShaderBuffer.h"

class CascadeShadowBuffer : public ShaderBuffer
{
public:
	CascadeShadowBuffer()
		: ShaderBuffer(0, 0, 0, sizeof(GS_DATA), 0)
	{
		D3DXMatrixIdentity(&gsData.cropMatrix[0]);
		D3DXMatrixIdentity(&gsData.cropMatrix[1]);
		D3DXMatrixIdentity(&gsData.cropMatrix[2]);
	}

	void UpdateMatrix(D3DXMATRIX cropMatrix[3]) {
		for (UINT i = 0; i < 3; i++) {
			gsData.cropMatrix[i] = cropMatrix[i];
		}
		UpdateGSBuffer(&gsData, sizeof(GS_DATA));
	}
	void Update() {

	}

	struct GS_DATA
	{
		D3DXMATRIX cropMatrix[3];
	};



private:

	GS_DATA gsData;

};
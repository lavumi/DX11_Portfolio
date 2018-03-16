#pragma once
#include "ShaderBuffer.h"

class BaseBuffer : public ShaderBuffer
{
public:
	BaseBuffer()
		: ShaderBuffer(0, 0,0, sizeof(PS_DATA))
	{
		SetColor(0, 0, 0, 1);
		//UpdateVSBuffer(&vsData, 0);
		//UpdatePSBuffer(&psData, 0);
	}

	void SetColor(D3DXCOLOR& color) {
		psData.color = color;
		UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}

	void SetColor(float r, float g, float b, float a) {
		psData.color = D3DXCOLOR(r, g, b, a);
		UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}

	void Update()
	{
	}

	struct VS_DATA
	{

	};
	struct PS_DATA
	{
		D3DXCOLOR color;
	};


private:
	
	VS_DATA vsData;
	PS_DATA psData;
};
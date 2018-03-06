#pragma once
#include "ShaderBuffer.h"

class BaseBuffer : public ShaderBuffer
{
public:
	BaseBuffer()
		: ShaderBuffer(0, 0, 0)
	{

		//UpdateVSBuffer(&vsData, 0);
		//UpdatePSBuffer(&psData, 0);
	}

	//void SetWorld(D3DXMATRIX& world) {
	//	ShaderBuffer::SetWorld(world);
	//}

	void Update()
	{
	}

	struct VS_DATA
	{

	};
	struct PS_DATA
	{

	};


private:
	
	VS_DATA vsData;
	PS_DATA psData;
};
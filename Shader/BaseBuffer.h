#pragma once
#include "ShaderBuffer.h"

class BaseBuffer : public ShaderBuffer
{
public:
	BaseBuffer()
		: ShaderBuffer(sizeof(VS_DATA), sizeof(VS_DATA), sizeof(VS_DATA), sizeof(VS_DATA), sizeof(VS_DATA))
	{

	}

	void Update()
	{
	}

	struct VS_DATA
	{
		D3DXVECTOR4 empty;
	};


private:
	
	VS_DATA vsData;
};
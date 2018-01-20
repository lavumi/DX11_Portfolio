#pragma once
#include "ShaderBuffer.h"

class LightBuffer : public ShaderBuffer
{
public:
	LightBuffer()
		: ShaderBuffer(sizeof(Data))
	{
		data.direction = D3DXVECTOR3(0,-1, 1);
		data.padding = 0;

		UserInterface::AddSun(&data.direction, nullptr);
	}

	void SetDirection(D3DXVECTOR3& direction)
	{
		data.direction = direction;
	}

	void Update()
	{
		UpdateBuffer(&data, sizeof(Data));
	}

	struct Data
	{
		D3DXVECTOR3 direction;
		float padding;
	};

private:
	Data data;
};
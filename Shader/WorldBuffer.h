#pragma once
#include "ShaderBuffer.h"

class WorldBuffer : public ShaderBuffer
{
public:
	WorldBuffer()
		: ShaderBuffer(sizeof(Data))
	{
		D3DXMatrixIdentity(&data.world);
		UpdateBuffer(&data.world, sizeof(Data));
	}

	void SetWorld(D3DXMATRIX& world)
	{
		data.world = world;
		Data temp;
		D3DXMatrixTranspose(&temp.world, &data.world);

		UpdateBuffer(&temp, sizeof(Data));
	}

	void Update()
	{

	}

	struct Data
	{
		D3DXMATRIX world;
	};

private:
	Data data;
};
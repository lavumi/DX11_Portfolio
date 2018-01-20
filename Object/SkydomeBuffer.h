#pragma once
#include "../Shader/ShaderBuffer.h"

class SkydomeBuffer : public ShaderBuffer
{
public:
	SkydomeBuffer()
		: ShaderBuffer(sizeof(Data))
	{

	}

	void Update()
	{
		UpdateBuffer(&data, sizeof(data));
	}

	struct Data
	{
		D3DXCOLOR center;
		D3DXCOLOR apex;
	};

private:
	Data data;
};
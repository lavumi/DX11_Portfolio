#pragma once
#include "Shader.h"

class SkydomeShader : public Shader {
public:
	SkydomeShader();
	~SkydomeShader();


	virtual void Update();
	virtual void Render(UINT indexCount, D3DXMATRIX world);



private:
	void CreateBuffers();


	D3D11_BUFFER_DESC desc;
	ID3D11Buffer* buffer;



	struct SkyColor
	{
		D3DXCOLOR center;
		D3DXCOLOR apex;
	} data;

};
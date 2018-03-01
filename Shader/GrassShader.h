#pragma once
#include "Shader.h"

class GrassShader : public Shader {
public:
	GrassShader();
	~GrassShader();


	virtual void Update();
	virtual void Render(UINT instanceCount,
		D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* perlin);



private:

	void CreateInputLayout();
	void CreateBuffers();


	ID3D11Buffer* buffer;
	D3DXVECTOR4 data;

};
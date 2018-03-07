#pragma once
#include "Shader.h"

class GrassBuffer;
class GrassShader : public Shader {
public:
	GrassShader();
	~GrassShader();


	virtual void Update();
	virtual void Render(UINT instanceCount,
		ID3D11ShaderResourceView* perlin);



private:

	//void CreateInputLayout();

	GrassBuffer* buffer;
	//void CreateBuffers();
	//
	//
	//ID3D11Buffer* buffer;
	//D3DXVECTOR4 data;

};
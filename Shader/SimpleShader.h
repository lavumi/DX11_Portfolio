#pragma once
#include "Shader.h"

class SimpleShader : public Shader {
public:
	SimpleShader();
	~SimpleShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world, ID3D11ShaderResourceView* diffuse);



private:
	void CreateBuffers();


	ID3D11Buffer* lightBuffer;



};
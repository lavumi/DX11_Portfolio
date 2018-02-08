#pragma once
#include "Shader.h"

class SkydomeShader : public Shader {
public:
	SkydomeShader();
	~SkydomeShader();


	virtual void Update();
	virtual void Render(UINT indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection);



private:
	void CreateBuffers();


	D3D11_BUFFER_DESC desc;
	ID3D11Buffer* LightBuffer;




};
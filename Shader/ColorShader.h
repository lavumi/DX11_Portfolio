#pragma once
#include "Shader.h"

class ColorShader : public Shader {
public:
	ColorShader();
	~ColorShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world, D3DXCOLOR color);



private:
	void CreateBuffers();


	ID3D11Buffer* colorBuffer;



};
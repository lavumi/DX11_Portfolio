#pragma once
#include "Shader.h"

class RainShader : public Shader {
public:
	RainShader();
	~RainShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* depthMap,  D3DXCOLOR color);



private:
	void CreateBuffers();

	float Data;
	ID3D11Buffer* colorBuffer;



};
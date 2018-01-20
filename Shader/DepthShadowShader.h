#pragma once
#include "Shader.h"

class DepthShadowShader : public Shader {
public:
	DepthShadowShader();
	~DepthShadowShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world);



private:
	void CreateBuffers();


	ID3D11Buffer* lightBuffer;


	//float lightNear = 0.1f;
	//float lightDepth = 1000;


};
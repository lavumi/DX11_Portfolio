#pragma once
#include "Shader.h"
class DepthShadowBuffer;
class DepthShadowShader : public Shader {
public:
	DepthShadowShader();
	~DepthShadowShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world);



private:


	DepthShadowBuffer * buffer;



};
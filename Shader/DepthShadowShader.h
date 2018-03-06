#pragma once
#include "Shader.h"
class LightViewBuffer;
class DepthShadowShader : public Shader {
public:
	DepthShadowShader();
	~DepthShadowShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world);



private:


	LightViewBuffer * buffer;



};
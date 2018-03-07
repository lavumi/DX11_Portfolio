#pragma once
#include "Shader.h"
class LightViewBuffer;
class DepthShadowShader : public Shader {
public:
	DepthShadowShader();
	~DepthShadowShader();


	virtual void Update();
	virtual void Render();



private:


	LightViewBuffer * buffer;



};
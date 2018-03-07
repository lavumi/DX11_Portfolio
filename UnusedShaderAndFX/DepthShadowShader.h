#pragma once
#include "Shader.h"
class DepthShadowShader : public Shader {
public:
	DepthShadowShader();
	~DepthShadowShader();


	virtual void Update();
	virtual void Render();



private:


};
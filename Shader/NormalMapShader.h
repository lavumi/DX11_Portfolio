#pragma once
#include "Shader.h"


class NormalMapShader : public Shader {
public:
	NormalMapShader();
	~NormalMapShader();


	virtual void Update();
	virtual void Render();
};
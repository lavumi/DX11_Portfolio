#pragma once
#include "Shader.h"


class BlurShader : public Shader {
public:
	BlurShader();
	~BlurShader();


	virtual void Update();

	
	virtual void Render();



private:

};
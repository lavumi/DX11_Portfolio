#pragma once
#include "Shader.h"

class BlurBuffer;
class BlurShader : public Shader {
public:
	BlurShader();
	~BlurShader();


	virtual void Update();

	
	virtual void Render(ID3D11ShaderResourceView* diffuse);



private:


	BlurBuffer * buffer;
};
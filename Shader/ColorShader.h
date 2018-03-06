#pragma once
#include "Shader.h"

class ColorShader : public Shader {
public:
	ColorShader();
	~ColorShader();


	virtual void Update();
	virtual void Render(UINT indexCount
		);



private:
	void CreateBuffers();


	ID3D11Buffer* colorBuffer;



};
#pragma once
#include "Shader.h"

class GrassShader : public Shader {
public:
	GrassShader();
	~GrassShader();


	virtual void Update();
	virtual void Render(UINT instanceCount,
		D3DXMATRIX view, D3DXMATRIX projection);



private:

	void CreateInputLayout();
	void CreateBuffers();
	



};
#pragma once
#include "Shader.h"

class InstanceTextureShader : public Shader {
public:
	InstanceTextureShader();
	~InstanceTextureShader();


	virtual void Update();
	virtual void Render(UINT indexCount, UINT instanceCount,
		ID3D11ShaderResourceView* diffuse);



private:

	void CreateInputLayout();

	


};
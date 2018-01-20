#pragma once
#include "Shader.h"

class BlurShader : public Shader {
public:
	BlurShader();
	~BlurShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world, ID3D11ShaderResourceView* diffuse);



private:
	void CreateBuffers();

	ID3D11Buffer* screenSizeBuffer;


};
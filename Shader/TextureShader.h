#pragma once
#include "Shader.h"

class BaseBuffer;
class TextureShader : public Shader {
public:
	TextureShader();
	~TextureShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world, ID3D11ShaderResourceView* diffuse);



private:

	BaseBuffer * buffer;




};
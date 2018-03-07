#pragma once
#include "Shader.h"

class RainBuffer;
class RainShader : public Shader {
public:
	RainShader();
	~RainShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* depthMap,  D3DXCOLOR color);



private:

	RainBuffer * buffer;





};
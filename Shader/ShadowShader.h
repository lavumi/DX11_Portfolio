#pragma once
#include "Shader.h"

class ShadowBuffer;
class ShadowShader : public Shader {
public:
	ShadowShader();
	~ShadowShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world, 
		ID3D11ShaderResourceView * lightMap);




	ShadowBuffer* buffer;

	//void CreateBuffers();
	//
	//ID3D11Buffer* LightBuffer;




};
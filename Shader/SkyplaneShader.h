#pragma once
#include "Shader.h"

class SkyplaneShader : public Shader {
public:
	SkyplaneShader();
	~SkyplaneShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* diffuseMap, ID3D11ShaderResourceView* perlin);



private:

	struct SkyplaneData {
		float translation;
		float scale;
		float brightness;
		float padding;
	} data;

	void CreateBuffers();
	ID3D11Buffer* skyplaneBuffer;

	float speed;

};
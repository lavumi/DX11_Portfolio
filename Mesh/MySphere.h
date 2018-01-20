#pragma once
#include "../Shader/Shader.h"

class MySphere : public Shader
{
public:
	MySphere();
	~MySphere();

	void Update();
	void Render();
	UINT vari;
	float radius;

	void RedrawSphere(void*);

private:
	struct BlendShaderDesc
	{
		float selected;
		float linearblend;
		float gammablend;
		float padding;
	} blendData;


	void CreateInputLayout();

	void CreateData();
	void CreateBuffer();

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* blendBuffer;
 
	VertexColorTextureNormal* vertexData;
	UINT* indexData;

	ID3D11ShaderResourceView** texture;

	D3DXVECTOR2 scale;

	UINT indexCount;
	UINT vertexCount;
};
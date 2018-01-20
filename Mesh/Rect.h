#pragma once
#include "../Shader/Shader.h"

class MaterialBuffer;
class Rect : public Shader
{
public:
	Rect(D3DXVECTOR2 scale);
	~Rect();

	void Update();
	void Render();

private:
	struct BlendShaderDesc
	{
		float selected;
		float linearblend;
		float gammablend;
		float padding;
	} blendData;




	void CreateData();
	void CreateBuffer();

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* blendBuffer;
 
	VertexColorTextureNormal* vertexData;
	UINT* indexData;

	ID3D11ShaderResourceView** texture;

	D3DXVECTOR2 scale;

	MaterialBuffer* materialBuffer;
};
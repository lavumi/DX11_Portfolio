#pragma once


class TexturePlane
{
public:
	TexturePlane();
	~TexturePlane();

	void Update();
	void Render();

	void SetWorld(D3DXMATRIX world);
	void SetTexture(wstring texture);




private:



	UINT indexCount;
	D3DXMATRIX world;
	ID3D11ShaderResourceView* diffuseMap;


	void CreateBuffer();



	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	WorldBuffer* wBuffer;


	UINT vertexCount;


};

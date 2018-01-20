#pragma once


class Skydome
{
public:
	Skydome();
	~Skydome();

	void Update();
	void Render();

	UINT indexCount;
	D3DXMATRIX world;
private:
	void CreateBuffer();

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	

	UINT vertexCount;
	
};
#pragma once


class Skydome
{
public:
	Skydome();
	~Skydome();

	void Update();
	void Render();

	UINT getIndexCount() {
		return indexCount;
	}

	D3DXMATRIX getWorld() {
		return world;
	}
	
private:
	void CreateBuffer();

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	

	UINT vertexCount;
	UINT indexCount;
	D3DXMATRIX world;
	WorldBuffer* wBuffer;
};
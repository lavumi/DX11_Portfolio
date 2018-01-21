#pragma once

class Water {
public:

	Water();
	~Water();

	void Update();
	void Render();

	UINT indexCount;

	D3DXMATRIX world;


private:






	void CreateBuffer();



	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT vertexCount;



	D3DXMATRIX Rworld;
	ID3D11Buffer* worldBuffer;

};
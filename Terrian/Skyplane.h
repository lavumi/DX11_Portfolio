#pragma once


class Skyplane
{
public:
	Skyplane();
	~Skyplane();

	void Update();
	void Render();

	UINT indexCount;
	D3DXMATRIX world;
private:
	void CreateVertexData();
	void CreateIndexData();
	void CreateBuffer();

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;


	UINT vertexCount;
	VertexTexture* vertexData;
	vector<UINT> indexData;


	int skyPlaneResolution;
	float skyPlaneWidth, skyPlaneTop, skyPlaneBottom;
};
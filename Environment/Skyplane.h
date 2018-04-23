#pragma once

class PerlinNoise;
class SkyplaneBuffer;
class Skyplane{
public:
	Skyplane();
	~Skyplane();


	void Initialize();
	void Update();
	void Render();

	void MakeCloudPerlin();


private:
	void CreateVertexData();
	void CreateIndexData();
	void CreateBuffer();

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;


	UINT vertexCount;
	UINT indexCount;
	VertexTexture* vertexData;
	vector<UINT> indexData;


	UINT skyPlaneResolution;
	float skyPlaneWidth, skyPlaneTop, skyPlaneBottom;

//	PerlinNoise* perlinNoise;
//	ID3D11ShaderResourceView* perlin;
	D3DXMATRIX world;


	WorldBuffer* wBuffer;
	SkyplaneBuffer * buffer;
};
#pragma once

class PerlinNoise;
class Skyplane{
public:
	Skyplane();
	~Skyplane();


	void Initialize();
	void Update();
	void Render();

	void MakeCloudPerlin();

	UINT getIndexCount() {
		return indexCount;
	}

	ID3D11ShaderResourceView* getDiffuseMap() {
		return diffuse;
	}

	ID3D11ShaderResourceView* getPerlinMap() {
		return perlin;
	}

	D3DXMATRIX getWorld() {
		return world;
	}
	
	
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

	
	ID3D11ShaderResourceView* diffuse;


	PerlinNoise* perlinNoise;
	ID3D11ShaderResourceView* perlin;
	D3DXMATRIX world;


	PerlinNoise* noise;
};
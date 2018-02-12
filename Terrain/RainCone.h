#pragma once

class PerlinNoise;
class RainCone{
public:
	RainCone();
	~RainCone();


	void Initialize();
	void Update();
	void Render();

	void MakeCloudPerlin();

	UINT getIndexCount() {
		return indexCount;
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
	VertexTexture3* vertexData;
	vector<UINT> indexData;

	
	ID3D11ShaderResourceView* diffuse;



	D3DXMATRIX world;
	float radius;
};
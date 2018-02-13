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
	ID3D11ShaderResourceView* diffuse;
	
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

	




	D3DXMATRIX world;
	float radius;
};
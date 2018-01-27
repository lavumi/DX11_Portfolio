#pragma once


class TerrainGrass
{
public:
	TerrainGrass();
	~TerrainGrass();

	void Update();
	void Render();

	UINT getIndexCount() {
		return indexCount;
	}

	 void setDiffuseMap(ID3D11ShaderResourceView* texture) {
		// diffuse = texture;
	}


	ID3D11ShaderResourceView* getDiffuseMap() {
		return diffuse;
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
	VertexTexture* vertexData;
	UINT* indexData;


	
	ID3D11ShaderResourceView* diffuse;

	D3DXMATRIX world;
};
#pragma once

class Landscape;
class TerrainGrass
{
public:
	TerrainGrass();
	~TerrainGrass();

	void Initialize(Landscape* land);

	void Update();
	void Render();

	UINT getIndexCount() {
		return indexCount;
	}

	 void setDiffuseMap(ID3D11ShaderResourceView* texture) {
		 diffuse = texture;
	}


	ID3D11ShaderResourceView* getDiffuseMap() {
		return diffuse;
	}


	UINT getInstanceCount() {
		return instanceData.size();
	}
	
	
private:
	void CreateBuffer();
	void CreateInstanceData();

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* instanceBuffer;
	ID3D11Buffer* indexBuffer;


	UINT vertexCount;
	UINT indexCount;

	VertexTexture* vertexData;
	UINT* indexData;


	
	ID3D11ShaderResourceView* diffuse;

	vector<D3DXMATRIX> instanceData;

	Landscape* land;

};
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

	//UINT getIndexCount() {
	//	return indexCount;
	//}

	 void setDiffuseMap(ID3D11ShaderResourceView* texture) {
		// diffuse = texture;
	}


	ID3D11ShaderResourceView* getDiffuseMap() {
		return diffuse;
	}


	UINT getInstanceCount() {
		return instanceData.size();
	}
	
	
private:
	void CreateBufferTemp();
	void CreateInstanceData();

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* instanceBuffer;


	UINT vertexCount;

	VertexTexture* vertexData;
	
	ID3D11ShaderResourceView* diffuse;

	vector<D3DXMATRIX> instanceData;

	Landscape* land;

};
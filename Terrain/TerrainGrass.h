#pragma once

class Landscape;
class GrassBuffer;
class TerrainGrass
{
public:
	TerrainGrass();
	~TerrainGrass();

	void Initialize(Landscape* land);

	void Update();
	void Render();




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
	vector<D3DXMATRIX> instanceData;

	Landscape* land;

	GrassBuffer* buffer;
};
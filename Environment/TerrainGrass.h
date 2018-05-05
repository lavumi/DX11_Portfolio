#pragma once

class Landscape;
class GrassBuffer;
class Frustum;
class Character;
class TerrainGrass
{
public:
	TerrainGrass();
	~TerrainGrass();

	void Initialize(Landscape* land);

	void Update();
	void Render();

	void SetCharacterPos(D3DXVECTOR3* pos);

	void SetPosByFrustum(Frustum* frustum);


	
	
private:


	void CreateBuffer();
	void CreateBufferTemp();
	void CreateInstanceData();

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* instanceBuffer;


	ID3D11ShaderResourceView* heightMap;

	UINT vertexCount;


	//TODO 고치자 이거
	friend class Environment;
	vector<D3DXVECTOR2> instanceData;


	Landscape* land;
	GrassBuffer* buffer;
	Character* player;
};
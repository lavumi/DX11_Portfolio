#pragma once


class Frustum;
class QuadTree;
class TerrainGrass;
class Landscape : public Renderer
{
public:
	Landscape();
	~Landscape();


	void Initialize();


	void LoadHeightMap();
	void CreateVertexData();
	void CreateIndexData();
	void CreateNormalData();
	void CreateBuffer();


	void Update();
	void Render();
	void GetY(D3DXVECTOR3 &position);
	//Brush* brush;

	void SetTexture(ID3D11ShaderResourceView* diffuse, ID3D11ShaderResourceView* normal, ID3D11ShaderResourceView* specular) {
		diffuseMap[0] = diffuse;
		specularMap = specular;
	}



	UINT getvertexCount() {
		return vertexCount;
	}


	UINT getIndexCount() {
		return indexCount;
	}

	ID3D11ShaderResourceView** getDiffuseMap() {
		return diffuseMap;
	}



	ID3D11ShaderResourceView* getNormalMap() {
		return normalMap;
	}

	D3DXMATRIX getWorld() {
		return world;
	}

	 void getVertexData(VertexTextureNormalTangent* vertices) {

		memcpy(vertices, vertexData, sizeof(VertexTextureNormalTangent) * vertexCount);

		return;
	};


	 void changeLOD(Frustum* frustum);

	 void GetGroundPos(vector<D3DXMATRIX>& grassGround) {
		 grassGround = this->grassGround;
	 }
private:

	void CheckGround();


	//MaterialBuffer* materialBuffer;

	wstring heightMapFile;
	BYTE* heightData;


	vector<D3DXMATRIX> grassGround;




	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT width, height;
	VertexTextureNormalTangent* vertexData;
	vector<UINT> indexData;

	UINT vertexCount;
	UINT indexCount;
	ID3D11ShaderResourceView** diffuseMap;
	ID3D11ShaderResourceView* normalMap;
	ID3D11ShaderResourceView* specularMap;
	D3DXMATRIX world;
	WorldBuffer* wBuffer;

	QuadTree* quadTree;
	TerrainGrass* grass;
};
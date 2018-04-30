#pragma once

//static UINT landscapeScale = 3;
class Frustum;
class QuadTree;
class TerrainBuffer;
class Landscape
{
public:
	Landscape();
	~Landscape();


	void Initialize();





	void Update();
	void Render();
	void RenderShadow();
	void GetY(D3DXVECTOR3 &position);
	//Brush* brush;

	void SetTexture(ID3D11ShaderResourceView* diffuse) {
		diffuseMap[0] = diffuse;
	}



	void getVertexData(VertexTextureNormalTangent* vertices) {
		memcpy(vertices, vertexData, sizeof(VertexTextureNormalTangent) * vertexCount);
	};


	 void changeLOD(Frustum* frustum);



	 


	 void SetPlane(D3DXPLANE& plane);

	 void GetHeightMap(ID3D11ShaderResourceView*& heightMap) {
		 heightMap = this->heightMap;
	 }





	 vector<D3DXVECTOR3> treePos;
private:
	int testNumber = 0;


	void LoadHeightMap();
	void CreateVertexData();
	void CreateIndexData();

	void CreateFullVertexData();
	void CreateFullIndexData();
	void CreateNormalData();


	void MakeTreePosition();

	
	void CreateBuffer();
	void LoadTextures();
	

	D3DXMATRIX world;

	float* heightData;









	UINT width, height;
	VertexTexture* vertexData;
	vector<UINT> indexData;

	UINT vertexCount;
	UINT indexCount;







	UINT worldWidth, worldHeight;
	VertexTextureNormalTangent* fullVertexData;
	vector<UINT> fullIndexData;

	UINT fullVertexCount, fullIndexCount;





	WorldBuffer* wBuffer;
	TerrainBuffer* buffer;


	QuadTree* quadTree;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	ID3D11Buffer* fullvertexBuffer;
	ID3D11Buffer* fullindexBuffer;

	ID3D11ShaderResourceView** diffuseMap;
	ID3D11ShaderResourceView* normalMap;
	ID3D11ShaderResourceView* worldNormalMap;
	ID3D11ShaderResourceView* specularMap;
	ID3D11ShaderResourceView* heightMap;

};
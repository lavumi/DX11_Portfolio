#pragma once


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

	 void GetGroundPos(vector<D3DXMATRIX>& grassGround) {
		 grassGround = this->grassGround;
	 }


	 void SetPlane(D3DXPLANE& plane);

	 void GetHeightMap(ID3D11ShaderResourceView*& heightMap) {
		 heightMap = this->heightMap;
	 }


private:



	void LoadHeightMap();
	void CreateVertexData();
	void CreateQuadIndexData();



	void CreateFullVertexData();
	void CreateIndexData();
	void CreateNormalData();
	void CheckGround();

	
	
	
	void CreateBuffer();
	void LoadTextures();
	


	//MaterialBuffer* materialBuffer;

	
	float* heightData;


	vector<D3DXMATRIX> grassGround;




	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT width, height;
	VertexTexture* vertexData;
	vector<UINT> indexData;

	UINT vertexCount;
	UINT indexCount;



	ID3D11Buffer* fullvertexBuffer;
	ID3D11Buffer* fullindexBuffer;


	UINT worldWidth, worldHeight;
	VertexTextureNormalTangent* fullVertexData;
	vector<UINT> fullIndexData;

	UINT fullVertexCount, fullIndexCount;




	//UINT tessWidth, tessHeight;
	//VertexTextureNormalTangent* tessVertexData;
	//vector<UINT> tessIndexData;
	//
	//UINT tessVertexCount;
	//UINT tessIndexCount;




	ID3D11ShaderResourceView** diffuseMap;
	ID3D11ShaderResourceView* normalMap;
	ID3D11ShaderResourceView* worldNormalMap;
	ID3D11ShaderResourceView* specularMap;
	ID3D11ShaderResourceView* heightMap;
	D3DXMATRIX world;
	WorldBuffer* wBuffer;

	QuadTree* quadTree;



	TerrainBuffer* buffer;
};
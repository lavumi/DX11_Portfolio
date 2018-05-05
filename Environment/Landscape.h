#pragma once


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

	void SetTexture(ID3D11ShaderResourceView* diffuse) {
		diffuseMap[0] = diffuse;
	}

	void getVertexData(VertexTextureNormalTangent* vertices) {
		memcpy(vertices, vertexData, sizeof(VertexTextureNormalTangent) * vertexCount);
	};

	//CPU lod system
	//void changeLOD(Frustum* frustum);

	//호수 평면 세팅
	void SetPlane(D3DXPLANE& plane);

	void GetHeightMap(ID3D11ShaderResourceView*& heightMap) {
		 heightMap = this->heightMap;
	 }

	vector<D3DXVECTOR3> treePos;

	void GetSize(UINT& width, UINT& height) {
		width = worldWidth;
		height = worldHeight;
			
	}
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
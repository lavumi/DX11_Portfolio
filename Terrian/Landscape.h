#pragma once


class Frustum;
class QuadTree;
class Landscape 
{
public:
	struct VertexTextureNormalTangent
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 uv;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
	};


	Landscape();
	~Landscape();


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
		diffuseMap = diffuse;
		specularMap = specular;
	}



	int getvertexCount() {
		return vertexCount;
	}


	 void getVertexData(VertexTextureNormalTangent* vertices) {

		memcpy(vertices, vertexData, sizeof(VertexTextureNormalTangent) * vertexCount);

		return;
	};


	 void changeLOD(Frustum* frustum);


	UINT indexCount;
	ID3D11ShaderResourceView* diffuseMap;
	ID3D11ShaderResourceView* normalMap;
	ID3D11ShaderResourceView* specularMap;
	D3DXMATRIX world;
private:




	//MaterialBuffer* materialBuffer;

	wstring heightMapFile;
	BYTE* heightData;





	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT width, height;
	VertexTextureNormalTangent* vertexData;
	vector<UINT> indexData;

	UINT vertexCount;



	QuadTree* quadTree;
};
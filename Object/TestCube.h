#pragma once

class NormalShaderBuffer;
class TestCube
{
public:
	TestCube();
	~TestCube();

	void Update();
	void Render();


	UINT indexCount;
	
	ID3D11ShaderResourceView* diffuseMap;
	ID3D11ShaderResourceView* normalMap;
	ID3D11ShaderResourceView* heightMap;

private:
	bool LoadResourcesFiles();



	WorldBuffer* wBuffers[6];
	D3DXMATRIX world[6];


	NormalShaderBuffer* buffer;

	void CreateBuffer();
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT vertexCount;
	float rotSpeed = 0.00f;
};

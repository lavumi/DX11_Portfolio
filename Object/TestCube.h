#pragma once


class TestCube 
{
public:
	TestCube();
	~TestCube();

	void Update();
	void Render();


	UINT indexCount;
	D3DXMATRIX world[6];
	ID3D11ShaderResourceView* diffuseMap;
	ID3D11ShaderResourceView* normalMap;
	ID3D11ShaderResourceView* heightMap;

private:






	void CreateBuffer();




	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;





	UINT vertexCount;




	int world_count = 6;

	// , world2, world3, world4, world5;
	D3DXMATRIX Rworld[6];// , Rworld2, Rworld3, Rworld4, Rworld5;
	ID3D11Buffer* worldBuffer[6];


	float rotSpeed = 0.00f;



};

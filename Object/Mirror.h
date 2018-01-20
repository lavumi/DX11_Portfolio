#pragma once


class Mirror
{
public:
	Mirror();
	~Mirror();

	void Update();
	void Render();
	void RenderMirror();

	UINT indexCountMirror;
	UINT indexCount;
	D3DXMATRIX world;
	ID3D11ShaderResourceView* diffuseMap;


private:






	void CreateBuffer();



	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	ID3D11Buffer* vertexBufferMirror;
	ID3D11Buffer*  indexBufferMirror;


	UINT vertexCount;
	UINT vertexCountMirror;


	// , world2, world3, world4, world5;
	D3DXMATRIX Rworld;// , Rworld2, Rworld3, Rworld4, Rworld5;
	ID3D11Buffer* worldBuffer;


	float rotSpeed = 0.00f;



};

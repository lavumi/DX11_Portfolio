#pragma once

class Water {
public:

	Water();
	~Water();

	void Update();
	void Render();

	UINT indexCount;

	D3DXMATRIX world;

	ID3D11ShaderResourceView* getNormalTexture() {
		return normal;
	}

	float GetWaterHeigh() {
		return waterHeight;
	}

	D3DXPLANE getwaterPlane() {
		return waterPlane;
	}
private:

	D3DXPLANE waterPlane;
	float waterHeight;




	void CreateBuffer();



	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT vertexCount;



	D3DXMATRIX Rworld;
	ID3D11Buffer* worldBuffer;


	ID3D11ShaderResourceView* normal;
};
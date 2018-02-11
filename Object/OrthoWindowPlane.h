#pragma once


class OrthoWindowPlane
{
public:
	OrthoWindowPlane();
	~OrthoWindowPlane();

	void Update();
	void Render();



	UINT GetIndexCount() {
		return indexCount;
	}
	D3DXMATRIX GetWorld() {
		return world;
	}


private:






	void CreateBuffer();



	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;




	UINT vertexCount;



	// , world2, world3, world4, world5;
	D3DXMATRIX Rworld;// , Rworld2, Rworld3, Rworld4, Rworld5;
	ID3D11Buffer* worldBuffer;


	float rotSpeed = 0.00f;


	UINT indexCount;
	D3DXMATRIX world;

};

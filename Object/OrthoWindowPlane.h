#pragma once

class OrthoWindowBuffer;
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
//	D3DXMATRIX GetWorld();


private:

	WorldBuffer * wBuffer;
	OrthoWindowBuffer* buffer;



	void CreateBuffer();



	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;




	UINT vertexCount;

	float rotSpeed = 0.00f;


	UINT indexCount;
	D3DXMATRIX world, translation;

};

#pragma once
#include "../Shader/Shader.h"

class ObjectMesh : public Shader
{
public:
	ObjectMesh(int index);
	~ObjectMesh();

	void Render();
	void Update();

private:

	void CreateBuffer();



	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;


	int type;
	UINT vertexCount;
	UINT indexCount;

	D3DXMATRIX world;

};
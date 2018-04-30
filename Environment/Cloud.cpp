#include "../stdafx.h"
#include "Cloud.h"
#include "../ProceduralGenerator/PerlinNoise.h"

Cloud::Cloud()
{

}

Cloud::~Cloud()
{


	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_DELETE(perlinNoise);
}

void Cloud::Initialize()
{

	CreateBuffer();

	perlinNoise = new PerlinNoise();


	MakeCloudPerlin();

	D3DXMatrixScaling(&scale, 128, 2, 128);

}

void Cloud::Update()
{
	D3DXVECTOR3  position;
	Camera::Get()->GetPosition(&position);


	D3DXMatrixTranslation(&world, position.x, position.y+40, position.z);
	world = scale * world;
}

void Cloud::Render()
{

	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Cloud::MakeCloudPerlin()
{
	perlinNoise->MakePerlinNoise();

	perlin = *perlinNoise->GetPerlinNoise();

	D3DXMatrixIdentity(&world);
	
}





void Cloud::CreateBuffer()
{
	HRESULT hr;

//	vector<UINT> indexData;


	vertexCount = 24;
	indexCount = 36;


	VertexTexture* vertexData = new VertexTexture[24];
	int i = 0;

	vertexData[i].position = D3DXVECTOR3(-1.0, -1.0, 1.0);			vertexData[i++].uv = D3DXVECTOR2(0, 0);
	vertexData[i].position = D3DXVECTOR3(1.0, -1.0, 1.0);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(1.0, 1.0, 1.0);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(-1.0, 1.0, 1.0);			vertexData[i++].uv = D3DXVECTOR2(0, 1);

	vertexData[i].position = D3DXVECTOR3(1.0, 1.0, 1.0);			vertexData[i++].uv = D3DXVECTOR2(0, 1);
	vertexData[i].position = D3DXVECTOR3(1.0, 1.0, -1.0);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(1.0, -1.0, -1.0);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(1.0, -1.0, 1.0);			vertexData[i++].uv = D3DXVECTOR2(0, 0);

	vertexData[i].position = D3DXVECTOR3(-1.0, -1.0, -1.0);			vertexData[i++].uv = D3DXVECTOR2(0, 1);
	vertexData[i].position = D3DXVECTOR3(1.0, -1.0, -1.0);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(1.0, 1.0, -1.0);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(-1.0, 1.0, -1.0);			vertexData[i++].uv = D3DXVECTOR2(0, 0);

	vertexData[i].position = D3DXVECTOR3(-1.0, -1.0, -1.0);			vertexData[i++].uv = D3DXVECTOR2(0, 0);
	vertexData[i].position = D3DXVECTOR3(-1.0, -1.0, 1.0);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(-1.0, 1.0, 1.0);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(-1.0, 1.0, -1.0);			vertexData[i++].uv = D3DXVECTOR2(0, 1);

	vertexData[i].position = D3DXVECTOR3(1.0, 1.0, 1.0);			vertexData[i++].uv = D3DXVECTOR2(0, 1);
	vertexData[i].position = D3DXVECTOR3(-1.0, 1.0, 1.0);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(-1.0, 1.0, -1.0);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(1.0, 1.0, -1.0);			vertexData[i++].uv = D3DXVECTOR2(0, 0);

	vertexData[i].position = D3DXVECTOR3(-1.0, -1.0, -1.0);			vertexData[i++].uv = D3DXVECTOR2(0, 0);
	vertexData[i].position = D3DXVECTOR3(1.0, -1.0, -1.0);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(1.0, -1.0, 1.0);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(-1.0, -1.0, 1.0);			vertexData[i++].uv = D3DXVECTOR2(0, 1);









	UINT* indexData = new UINT[36]{
		0, 1, 3, 2, 3, 1,
		5, 4, 6, 4, 7, 6,
		8, 11, 9, 11, 10, 9,
		12, 13, 15, 13, 14, 15,
		17, 16, 19, 17, 19, 18,
		20, 21, 23,22, 23, 21 };

	

	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;


	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexTexture) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertexData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));



	//2. Index Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &indexData[0];

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));



	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);
}

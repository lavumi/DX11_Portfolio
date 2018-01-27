#include "../stdafx.h"
#include "TerrainGrass.h"

TerrainGrass::TerrainGrass()
{

	CreateBuffer();


	D3DXMatrixIdentity(&world);


	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), L"./Terrain/grass.dds", nullptr, nullptr, &diffuse, nullptr);
	assert(SUCCEEDED(hr));
}

TerrainGrass::~TerrainGrass()
{


	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void TerrainGrass::Update()
{
	D3DXVECTOR3 position;
	Camera::Get()->GetPosition(&position);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

}

void TerrainGrass::Render()
{

	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}


void TerrainGrass::CreateBuffer()
{
	HRESULT hr;

	vertexCount = 4;
	indexCount = 6;
	VertexTexture* vertexData = new VertexTexture[vertexCount];
	int i = 0;
	vertexData[i].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			vertexData[i++].uv = D3DXVECTOR2(0, 1);
	vertexData[i].position = D3DXVECTOR3( 1.0f, 0.0f, 0.0f);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3( 1.0f,  1.0f, 0.0f);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(0.0f,  1.0f, 0.0f);			vertexData[i++].uv = D3DXVECTOR2(0, 0);

	UINT* indexData = new UINT[indexCount]{
		0, 3, 1, 2, 1, 3,
	};

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

}

#include "../stdafx.h"
#include "Skyplane.h"

Skyplane::Skyplane()
{

	skyPlaneResolution = 50;
	skyPlaneWidth = 10.0f;
	skyPlaneTop = 0.5f;
	skyPlaneBottom = 0.0f;


	CreateVertexData();
	CreateIndexData();
	CreateBuffer();


	D3DXMatrixIdentity(&world);
}

Skyplane::~Skyplane()
{


	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void Skyplane::Update()
{
	D3DXVECTOR3 position;
	Camera::Get()->GetPosition(&position);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

}

void Skyplane::Render()
{

	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Skyplane::CreateVertexData()
{

	vertexCount = (skyPlaneResolution + 1) * (skyPlaneResolution + 1);

	float positionX, positionY, positionZ;

	vertexData = new VertexTexture[vertexCount];
	for (UINT z = 0; z <= skyPlaneResolution; z++)
	{
		for (UINT x = 0; x <= skyPlaneResolution; x++)
		{
			int index = (skyPlaneResolution + 1) * z + x;

			positionX = (-0.5f * skyPlaneWidth) + ((float)x * skyPlaneWidth / skyPlaneResolution);
			positionZ = (-0.5f * skyPlaneWidth) + ((float)z * skyPlaneWidth / skyPlaneResolution);

			vertexData[index].position.x = positionX;
			vertexData[index].position.z = positionZ;
			vertexData[index].position.y =  skyPlaneTop - ((skyPlaneTop - skyPlaneBottom) / (skyPlaneWidth * skyPlaneWidth / 4) * ((positionX * positionX) + (positionZ * positionZ)));

			vertexData[index].uv.x = (float)(x) / skyPlaneResolution;
			vertexData[index].uv.y = (float)(z) / skyPlaneResolution;
		}
	}




}

void Skyplane::CreateIndexData()
{
	indexCount =  skyPlaneResolution * skyPlaneResolution * 6;


	UINT count = 0;
	for (UINT z = 0; z < skyPlaneResolution; z++)	{
		for (UINT x = 0; x < skyPlaneResolution; x++)		{
			indexData.push_back((skyPlaneResolution + 1) * z + x);
			indexData.push_back((skyPlaneResolution + 1) * z + x + 1);
			indexData.push_back((skyPlaneResolution + 1) * (z + 1) + x);

			indexData.push_back((skyPlaneResolution + 1) * z + x + 1);
			indexData.push_back((skyPlaneResolution + 1) * (z + 1) + (x + 1));
			indexData.push_back((skyPlaneResolution + 1) * (z + 1) + x);
		}//for(x)
	}//for(z)

	if (indexData.size() != indexCount)		
		assert(0);
}


void Skyplane::CreateBuffer()
{
	HRESULT hr;



	

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
	indexData.clear();
}

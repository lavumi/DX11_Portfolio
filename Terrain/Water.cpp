#include "../stdafx.h"
#include "Water.h"





Water::Water()
	: vertexBuffer(nullptr), indexBuffer(nullptr)
{

	waterHeight = -7.9f;

	D3DXMatrixTranslation(&world, 0.0f, waterHeight, 0.0f);

	waterPlane = D3DXPLANE(0, 1, 0, -waterHeight);
	CreateBuffer();

	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), L"./Terrain/water_normal.jpg", nullptr, nullptr, &normal, nullptr);
	assert(SUCCEEDED(hr)); 

}

Water::~Water()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}


void Water::Update()
{
}

void Water::Render()
{

	UINT stride = sizeof(VertexTextureNormalTangent);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


}

















void Water::CreateBuffer()
{
	HRESULT hr;

	vertexCount = 4;
	indexCount = 6;

	//VertexTextureNormalTangent* vertexData = new VertexTextureNormalTangent[vertexCount];
	//int i = 0;
	//vertexData[i].position = D3DXVECTOR3(0,0,0);			vertexData[i++].uv = D3DXVECTOR2(0, 0);
	//vertexData[i].position = D3DXVECTOR3(256,0,0);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	//vertexData[i].position = D3DXVECTOR3(256, 0, 256);		vertexData[i++].uv = D3DXVECTOR2(1, 1);
	//vertexData[i].position = D3DXVECTOR3(0,0,256);			vertexData[i++].uv = D3DXVECTOR2(0, 1);



	//UINT* indexData = new UINT[indexCount]{
	//	1, 0, 3, 1, 3, 2,
	//};



	UINT width =1, height =1;
	

	vertexCount = (width + 1) * (height + 1);

	UINT heightIndex = 0;
	VertexTextureNormalTangent* vertexData = new VertexTextureNormalTangent[vertexCount];
	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			int index = (width + 1) * z + x;

			vertexData[index].position.x = (float)x * 255 / (width);
			vertexData[index].position.y = 0;
			vertexData[index].position.z = (float)z * 255 / (height);

			vertexData[index].uv.x = (float)(x);// (float)width;
			vertexData[index].uv.y = (float)(z);// (float)height;

			vertexData[index].normal = D3DXVECTOR3(0, 0, 0);
		}
	}


	indexCount = width * height * 6;
	UINT* indexData = new UINT[indexCount];

	UINT count = 0;
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			indexData[count + 0] = (width + 1) * z + x;
			indexData[count + 1] = (width + 1) * (z + 1) + x;
			indexData[count + 2] = (width + 1) * z + x + 1;
			indexData[count + 3] = (width + 1) * z + x + 1;
			indexData[count + 4] = (width + 1) * (z + 1) + x;
			indexData[count + 5] = (width + 1) * (z + 1) + (x + 1);
			
			count += 6;
		}//for(x)
	}//for(z)

	for (UINT i = 0; i < vertexCount; i++) {
		vertexData[i].normal = vertexData[i].tangent = D3DXVECTOR3(0, 0, 0);
	}



	for (UINT i = 0; i < (indexCount / 3); i++)
	{
		UINT index0 = indexData[i * 3 + 0];
		UINT index1 = indexData[i * 3 + 1];
		UINT index2 = indexData[i * 3 + 2];

		VertexTextureNormalTangent v0 = vertexData[index0];
		VertexTextureNormalTangent v1 = vertexData[index1];
		VertexTextureNormalTangent v2 = vertexData[index2];

		D3DXVECTOR3 deltaPos1 = v1.position - v0.position;
		D3DXVECTOR3 deltaPos2 = v2.position - v0.position;

		D3DXVECTOR2 deltaUV1 = v1.uv - v0.uv;
		D3DXVECTOR2 deltaUV2 = v2.uv - v0.uv;

		D3DXVECTOR3 normal;
		D3DXVec3Cross(&normal, &deltaPos1, &deltaPos2);

		vertexData[index0].normal += normal;
		vertexData[index1].normal += normal;
		vertexData[index2].normal += normal;

		float det = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		vertexData[index0].tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*det;
		vertexData[index1].tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*det;
		vertexData[index2].tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*det;
	}



	for (UINT i = 0; i < vertexCount; i++)
	{
		D3DXVec3Normalize(&vertexData[i].normal, &vertexData[i].normal);
		D3DXVec3Normalize(&vertexData[i].tangent, &vertexData[i].tangent);
	}


	VertexTextureNormalTangent lastPoint = vertexData[3];
	VertexTextureNormalTangent firstPoint = vertexData[0];
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;


	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexTextureNormalTangent) * vertexCount;
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
	data.pSysMem = indexData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));




	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);
}


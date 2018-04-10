#include "../stdafx.h"
#include "TestCube.h"

#include "../Shader/NormalShaderBuffer.h"


TestCube::TestCube()
	: vertexBuffer(nullptr), indexBuffer(nullptr)
	{

	for (int i = 0; i < 6; i++) {
		//D3DXMatrixTranslation(&world[i], 10 + 4* (float)(i%3) , 5, 10 * (float)(i/3));


		D3DXMatrixTranslation(&world[i], 128 + 10 + 4* (float)(i%3) , 5, 188 + 10 * (float)(i/3));
		wBuffers[i] = new WorldBuffer();
		wBuffers[i]->SetWorld(world[i]);
	}


	buffer = new NormalShaderBuffer();
	

	CreateBuffer();
	


	LoadResourcesFiles();


}

TestCube::~TestCube()
{
	for (int i = 0; i < 6; i++) {
		SAFE_DELETE(wBuffers[i])
	}
	SAFE_DELETE(buffer);
}

void TestCube::Update()
{


}

void TestCube::Render()
{
	UINT stride = sizeof(VertexTextureNormalTangent);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);




	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &diffuseMap);
	D3D::GetDeviceContext()->PSSetShaderResources(1, 1, &normalMap);
	D3D::GetDeviceContext()->PSSetShaderResources(2, 1, &heightMap);

	

	for (int i = 0; i < 6; i++) {
		wBuffers[i]->SetBuffer();
		buffer->SetWorld(world[i]);
		buffer->SetBuffers();
		D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
	}

}

bool TestCube::LoadResourcesFiles()
{
	bool diffuse, normal, height;
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
		D3D::GetDevice(),
		L"./Contents/Textures/wood.jpg",
		NULL,
		NULL,
		&diffuseMap,
		NULL
	);
	diffuse = SUCCEEDED(hr);


	hr = D3DX11CreateShaderResourceViewFromFile(
		D3D::GetDevice(),
		L"./Contents/Textures/normal.png",
		NULL,
		NULL,
		&normalMap,
		NULL
	);
	normal = SUCCEEDED(hr);

	hr = D3DX11CreateShaderResourceViewFromFile(
		D3D::GetDevice(),
		L"./Contents/Textures/depth.png",
		NULL,
		NULL,
		&heightMap,
		NULL
	);
	height = SUCCEEDED(hr);

	return diffuse || normal || height;
}
















void TestCube::CreateBuffer()
{
	HRESULT hr;

	vertexCount = 24;
	indexCount = 36;


	VertexTextureNormalTangent* vertexData = new VertexTextureNormalTangent[24];
	int i = 0;

	vertexData[i].position = D3DXVECTOR3(-1.0, -1.0, 1.0);			vertexData[i++].uv = D3DXVECTOR2(0, 0);
	vertexData[i].position = D3DXVECTOR3(1.0, -1.0, 1.0);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(1.0, 1.0, 1.0)	;			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(-1.0, 1.0, 1.0);			vertexData[i++].uv = D3DXVECTOR2(0, 1);

	vertexData[i].position = D3DXVECTOR3(1.0, 1.0, 1.0)	;			vertexData[i++].uv = D3DXVECTOR2(0, 1);
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

	vertexData[i].position = D3DXVECTOR3(1.0, 1.0, 1.0)	;			vertexData[i++].uv = D3DXVECTOR2(0, 1);
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
}


#include "../stdafx.h"
#include "RainCone.h"
#include "../ProceduralTexture/PerlinNoise.h"

RainCone::RainCone()
{



	

	//hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), L"./Terrain/perturb001.dds", nullptr, nullptr, &perlin, nullptr);
	//
	//assert(SUCCEEDED(hr));
}

RainCone::~RainCone()
{


	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void RainCone::Initialize()
{
	radius = 1;
	CreateVertexData();
	CreateIndexData();
	CreateBuffer();


	D3DXMatrixIdentity(&world);

	

}

void RainCone::Update()
{
	D3DXVECTOR3 position;
	Camera::Get()->GetPosition(&position);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

}

void RainCone::Render()
{

	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void RainCone::MakeCloudPerlin()
{

	D3DXMatrixIdentity(&world);
}

void RainCone::CreateVertexData()
{

	vertexCount = 4 * 4 * 7;
	
	vertexData = new VertexTexture[vertexCount];

	for (int i = 0; i < 7; i++) { //7층 구조
		for (int j = 0; j < 4; j++) { //원을 4개로 나눈것
			for (int k = 0; k < 4; k++) { //4분원 내의 점
				float stage = (float)i - 3;
				float currentRadius = radius * (1 - stage*stage/9);

				vertexData[i * 4 * 4 + j * 4 + k].position.x = currentRadius * sinf(3.141592f / 8 * (4 * j + k));
				vertexData[i * 4 * 4 + j * 4 + k].position.z = currentRadius * cosf(3.141592f / 8 * (4 * j + k));
				vertexData[i * 4 * 4 + j * 4 + k].position.y = stage ;
				vertexData[i * 4 * 4 + j * 4 + k].uv = D3DXVECTOR2(k % 2,(4-i));
			}
		}
	}




}

void RainCone::CreateIndexData()
{
	indexCount = 6 * 4 * 4 * 6;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				UINT startIndex = i * 4 * 4 + j * 4 + k;
				indexData.push_back(startIndex);
				indexData.push_back(startIndex + 16);
				if(j *k == 9){
					indexData.push_back(startIndex -15);

					indexData.push_back(startIndex -15);
					indexData.push_back(startIndex + 16);
					indexData.push_back(startIndex + 1);
				}
				else {
					indexData.push_back(startIndex + 1);

					indexData.push_back(startIndex + 1);
					indexData.push_back(startIndex + 16);
					indexData.push_back(startIndex + 17);
				}

			}
		}
	}




	if (indexData.size() != indexCount)		
		assert(0);
}


void RainCone::CreateBuffer()
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

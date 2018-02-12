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

	UINT stride = sizeof(VertexTexture3);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);


}

void RainCone::MakeCloudPerlin()
{

	D3DXMatrixIdentity(&world);
}

void RainCone::CreateVertexData()
{

	//vertexCount = 4 * 4 * 7;
	//
	//vertexData = new VertexTexture3[vertexCount];
   //
	//for (int i = 0; i < 7; i++) { //7층 구조
	//	for (int j = 0; j < 4; j++) { //원을 4개로 나눈것
	//		for (int k = 0; k < 4; k++) { //4분원 내의 점
	//			float stage = (float)i - 3;
	//			float currentRadius = radius * (1 - stage*stage/9);
	//
	//			vertexData[i * 4 * 4 + j * 4 + k].position.x = currentRadius * sinf(3.141592f / 8 * (4 * j + k));
	//			vertexData[i * 4 * 4 + j * 4 + k].position.z = currentRadius * cosf(3.141592f / 8 * (4 * j + k));
	//			vertexData[i * 4 * 4 + j * 4 + k].position.y = stage *2;
	//			vertexData[i * 4 * 4 + j * 4 + k].uvq = D3DXVECTOR3(((float)k)/4,-(float)i/6,);
	//		}
	//	}
	//}

	vertexCount = 4 * 16 * 6;  //사각형, 16개, 6층
	
	vertexData = new VertexTexture3[vertexCount];

	int index = 0;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 16; j++) {
			float stage = (float)i - 3;
			float currentRadius = radius * (1 - stage*stage/9);
			vertexData[index].position.x = currentRadius * sinf(3.141592f / 8 * j);
			vertexData[index].position.z = currentRadius * cosf(3.141592f / 8 * j);
			vertexData[index].position.y = stage *2;
			vertexData[index].uvq = D3DXVECTOR3(((float)j)/4,-(float)i/6,1);

			index++;
			stage = (float)i - 3 + 1;
			currentRadius = radius * (1 - stage * stage / 9);
			vertexData[index].position.x = currentRadius * sinf(3.141592f / 8 * j);
			vertexData[index].position.z = currentRadius * cosf(3.141592f / 8 * j);
			vertexData[index].position.y = stage * 2;
			vertexData[index].uvq = D3DXVECTOR3(((float)j) / 4, -(float)i / 6, 1);
			index++;
		}
	}



}

void RainCone::CreateIndexData()
{
	//indexCount = 6 * 4 * 4 * 6;
	//for (int i = 0; i < 6; i++) {
	//	for (int j = 0; j < 4; j++) {
	//		for (int k = 0; k < 4; k++) {
	//			UINT startIndex = i * 4 * 4 + j * 4 + k;
	//
	//			indexData.push_back(startIndex);
	//			indexData.push_back(startIndex + 16);
	//		/*	if(j *k == 9){
	//				indexData.push_back(startIndex + 1);
	//				indexData.push_back(startIndex -15);
	//			}
	//			else {
	//				indexData.push_back(startIndex + 17);
	//				indexData.push_back(startIndex + 1);					
	//			}*/
	//			if(j *k == 9){
	//				indexData.push_back(startIndex -15);
	//			
	//				indexData.push_back(startIndex -15);
	//				indexData.push_back(startIndex + 16);
	//				indexData.push_back(startIndex + 1);
	//			}
	//			else {
	//				indexData.push_back(startIndex + 1);
	//			
	//				indexData.push_back(startIndex + 1);
	//				indexData.push_back(startIndex + 16);
	//				indexData.push_back(startIndex + 17);
	//			}
	//
	//		}
	//	}
	//}



	indexCount = 16 * 6 * 6;

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 16; j++) {
			UINT startIndex = i * 32 + j * 2;
			indexData.push_back(startIndex);
			indexData.push_back(startIndex+1);
			indexData.push_back(startIndex+2);

			indexData.push_back(startIndex+2);
			indexData.push_back(startIndex+1);
			indexData.push_back(startIndex+3);
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
	desc.ByteWidth = sizeof(VertexTexture3) * vertexCount;
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

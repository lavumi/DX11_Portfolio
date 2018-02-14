#include "../stdafx.h"
#include "RainCone.h"
#include "../ProceduralTexture/PerlinNoise.h"

RainCone::RainCone()
{



	

	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), L"./Terrain/UV_mapper.jpg", nullptr, nullptr, &diffuse, nullptr);
	
	assert(SUCCEEDED(hr));
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

}

void RainCone::MakeCloudPerlin()
{

	D3DXMatrixIdentity(&world);
}

void RainCone::CreateVertexData()
{
	//vertexCount = 4;
	//vertexData = new VertexTexture3[vertexCount];
	//
	//int i = 0;
	//

	//vertexData[i++].position = D3DXVECTOR3(-4 , -5.0f, 0.1f);
	//vertexData[i++].position = D3DXVECTOR3(7 , -9.0f, 0.1f);
	//vertexData[i++].position = D3DXVECTOR3(11 , 5.0f, 0.1f);
	//vertexData[i++].position = D3DXVECTOR3(-6 , 11.0f, 0.1f);


	//D3DXVECTOR3 ac, bd, ad;

	//ac = vertexData[2].position - vertexData[0].position;
	//bd = vertexData[3].position - vertexData[1].position;
	//ad = vertexData[3].position - vertexData[0].position;

	//float trapezoid[4];

	//i = 0;

	//trapezoid[0] = (bd.x * ad.y - bd.y * ad.x) / (bd.x * ac.y - ac.x * bd.y); // alpha
	//trapezoid[1] = (ac.x * ad.y - ad.x * ac.y) / (ac.x * bd.y - bd.x * ac.y); // beta


	//trapezoid[2] = 1 - trapezoid[0];
	//trapezoid[3] = 1 - trapezoid[1];


	//trapezoid[0] = 1 / trapezoid[0];		  //깊이값에 역수를 취해 uv에 곱해준다
	//trapezoid[1] = 1 / trapezoid[1];		  //깊이값에 역수를 취해 uv에 곱해준다
	//trapezoid[2] = 1 / trapezoid[2];		  //깊이값에 역수를 취해 uv에 곱해준다
	//trapezoid[3] = 1 / trapezoid[3];		  //깊이값에 역수를 취해 uv에 곱해준다


	//vertexData[i++].uvq = D3DXVECTOR3(0, trapezoid[2], trapezoid[2]); // 그리고 uv와 같이 그 점에 대한 깊이값도 같이 넣어줌 ( 깊이도 보정해주어야 하기 때문)
	//vertexData[i++].uvq = D3DXVECTOR3(trapezoid[1], trapezoid[1], trapezoid[1]);
	//vertexData[i++].uvq = D3DXVECTOR3(trapezoid[0], 0, trapezoid[0]);
	//vertexData[i++].uvq = D3DXVECTOR3(0, 0, trapezoid[3]);



	//return;

	vertexCount = 4 * 17 * 3;  

	vertexData = new VertexTexture3[vertexCount];

	int index = 0;
	for (int i = 0; i < 6; i++) {
		float stage = (float)i - 3;
		float currentRadius = radius * (1 - stage * stage / 9 + 0.01f);
	
	
		float stage2 = (float)i - 3 + 1;
		float currentRadius2 = radius * (1 - stage2 * stage2 / 9+0.01f);
	
	
		float trapezoidDegree = (currentRadius + currentRadius2) / currentRadius2;
		float trapezoidDegree2 = (currentRadius + currentRadius2) / currentRadius;
		for (int j = 0; j < 17; j++) {
	
			vertexData[index].position.x = currentRadius * sinf(3.141592f / 8 * j);
			vertexData[index].position.z = currentRadius * cosf(3.141592f / 8 * j);
			vertexData[index].position.y = stage *2;
			vertexData[index].uvq = D3DXVECTOR3(
				(float)j / 4 * trapezoidDegree, 
				stage*-1 , 
				trapezoidDegree);
	
			index++;
						
			vertexData[index].position.x = currentRadius2 * sinf(3.141592f / 8 * j);
			vertexData[index].position.z = currentRadius2 * cosf(3.141592f / 8 * j);
			vertexData[index].position.y = stage2 * 2;
			vertexData[index].uvq = D3DXVECTOR3(
		
			(float)j / 4 * trapezoidDegree2, 
			stage2  * -1 , 
			trapezoidDegree2);
	
			index++;
		}
	}
	
	vertexData[0].uvq;
	vertexData[2].uvq;
	if (vertexCount != index)
		assert(0);

}

void RainCone::CreateIndexData()
{
	//indexCount = 18;
	//
	//indexData.push_back(0);
	//indexData.push_back(3);
	//indexData.push_back(1);
	//indexData.push_back(2);
	//indexData.push_back(1);
	//indexData.push_back(3);


	//return;

	indexCount = 16 * 6 * 6;

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 16; j++) {
			UINT startIndex = i * 34 + j * 2;
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

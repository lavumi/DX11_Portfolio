#include "../stdafx.h"
#include "OrthoWindowPlane.h"




OrthoWindowPlane::OrthoWindowPlane()
	: vertexBuffer(nullptr), indexBuffer(nullptr)
{
	D3DXMatrixIdentity(&world);

	CreateBuffer();

}

OrthoWindowPlane::~OrthoWindowPlane()
{

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);


	
}

void OrthoWindowPlane::Update()
{




}

void OrthoWindowPlane::Render()
{


	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
















void OrthoWindowPlane::CreateBuffer()
{
	HRESULT hr;

	vertexCount =4;
	indexCount = 6;

	VertexTexture* vertexData = new VertexTexture[vertexCount];

	D3DInfo info;
	D3D::GetInfo(&info);

	float screenx, screeny;
	screenx =(float)info.screenWidth / 2;
	screeny =(float)info.screenHeight / 2;
	int i = 0;

	vertexData[i].position = D3DXVECTOR3(screenx * -1	, screeny * -1	, 0.0f);			vertexData[i++].uv = D3DXVECTOR2(0, 1);
	vertexData[i].position = D3DXVECTOR3(screenx		, screeny * -1	, 0.0f);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(screenx		, screeny		, 0.0f);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(screenx * -1	, screeny		, 0.0f);			vertexData[i++].uv = D3DXVECTOR2(0, 0);



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
	data.pSysMem = indexData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));


	


}


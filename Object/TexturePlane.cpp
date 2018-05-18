#include "../stdafx.h"
#include "TexturePlane.h"


TexturePlane::TexturePlane()
	: vertexBuffer(nullptr), indexBuffer(nullptr)
{
	diffuseMap = 0;
	//D3DXMatrixTranslation(&world, 10.0f, 0.0f, 20.0f);
	CreateBuffer();

	wBuffer = new WorldBuffer();
	wBuffer->SetWorld(world);

}

TexturePlane::~TexturePlane()
{

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

	SAFE_DELETE(wBuffer);
	
}

void TexturePlane::Update()
{


}

void TexturePlane::Render()
{


	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;




	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	wBuffer->SetBuffer();

	if (diffuseMap != 0)		D3D::GetDeviceContext()->PSSetShaderResources(10, 1, &diffuseMap);

	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}

void TexturePlane::SetWorld(D3DXMATRIX world)
{

	wBuffer->SetWorld(world);
}

void TexturePlane::SetTexture(wstring texture)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
		D3D::GetDevice(),
		texture.c_str(),
		NULL,
		NULL,
		&diffuseMap,
		NULL
	);
	assert(SUCCEEDED(hr));
}
















void TexturePlane::CreateBuffer()
{
	HRESULT hr;

	vertexCount =4;
	indexCount = 6;

	VertexTexture* vertexData = new VertexTexture[vertexCount];
	int i = 0;

	vertexData[i].position = D3DXVECTOR3(-1.0f, -1.0f,0.0f);			vertexData[i++].uv =D3DXVECTOR2(0, 1);
	vertexData[i].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);			vertexData[i++].uv =D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);				vertexData[i++].uv =D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);			vertexData[i++].uv =D3DXVECTOR2(0, 0);


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


	
	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);
}


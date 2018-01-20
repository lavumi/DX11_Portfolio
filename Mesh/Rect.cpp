#include "../stdafx.h"
#include "Rect.h"
#include "../Shader/MaterialBuffer.h"

Rect::Rect(D3DXVECTOR2 scale)
	: Shader(L"./Mesh/Mesh.fx")
	, scale(scale)
{
	
	
	CreateData();
	CreateBuffer();


	CreateInputLayout(VertexColorTextureNormal::desc, VertexColorTextureNormal::count);


	texture = new ID3D11ShaderResourceView*[2];


	HRESULT hr;
	hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(), L"./Contents/Textures/Dirt.png"
		, NULL, NULL, &texture[0], NULL
	);
	assert(SUCCEEDED(hr));
	hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(), L"./Contents/Textures/Wall.png"
		, NULL, NULL, &texture[1], NULL
	);
	assert(SUCCEEDED(hr));
}

Rect::~Rect()
{
	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	for (UINT i = 0; i < 2; i++)
		SAFE_RELEASE(texture[i]);
}

void Rect::Update()
{
	D3D11_MAPPED_SUBRESOURCE resc;
	HRESULT hr = D3D::GetDeviceContext()->Map(
		blendBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resc
	);

	BlendShaderDesc* data = (BlendShaderDesc*)resc.pData;
	memcpy(data, &blendData, sizeof(BlendShaderDesc));
	D3D::GetDeviceContext()->Unmap(blendBuffer, 0);



	Shader::Update();
}

void Rect::Render()
{
	ID3D11DeviceContext* dc = D3D::GetDeviceContext();

	UINT stride = sizeof(VertexColorTextureNormal);
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDeviceContext()->PSSetShaderResources(0, 2, texture);
	D3D::GetDeviceContext()->PSSetConstantBuffers(2, 1, &blendBuffer);
	Shader::Render();

	dc->DrawIndexed(6, 0, 0);
}



void Rect::CreateData()
{
	vertexData = new VertexColorTextureNormal[4];
	
	vertexData[0].position = D3DXVECTOR3(0, 0, 0);
	vertexData[0].uv = D3DXVECTOR2(0, 1);
	
	vertexData[1].position = D3DXVECTOR3(0, 1, 0);
	vertexData[1].uv = D3DXVECTOR2(0, 0);

	vertexData[2].position = D3DXVECTOR3(1, 0, 0);
	vertexData[2].uv = D3DXVECTOR2(1, 1);

	vertexData[3].position = D3DXVECTOR3(1, 1, 0);
	vertexData[3].uv = D3DXVECTOR2(1, 0);

	vertexData[1].color = vertexData[0].color = D3DXCOLOR(0, 0, 0, 0);
	vertexData[2].color = vertexData[3].color = D3DXCOLOR(1, 0, 0, 0);

	D3DXVECTOR3 temp1, temp2;
	
	temp1 = vertexData[1].position - vertexData[0].position;
	temp2 = vertexData[2].position - vertexData[0].position;
	D3DXVec3Cross(&vertexData[0].normal, &temp1, &temp2);

	temp1 = vertexData[3].position - vertexData[1].position;
	temp2 = vertexData[0].position - vertexData[1].position;
	D3DXVec3Cross(&vertexData[1].normal, &temp1, &temp2);

	temp1 = vertexData[0].position - vertexData[2].position;
	temp2 = vertexData[3].position - vertexData[2].position;
	D3DXVec3Cross(&vertexData[2].normal, &temp1, &temp2);

	temp1 = vertexData[2].position - vertexData[3].position;
	temp2 = vertexData[1].position - vertexData[3].position;
	D3DXVec3Cross(&vertexData[3].normal, &temp1, &temp2);

	indexData = new UINT[6]{ 0, 1, 2, 2, 1, 3 };
}

void Rect::CreateBuffer()
{
	HRESULT hr;

	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;


	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexColorTextureNormal) * 4;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertexData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));


	//2. Index Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * 6;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = indexData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));


	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(BlendShaderDesc);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;


	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &blendBuffer);
	assert(SUCCEEDED(hr));

	blendData.selected = 0;
	blendData.linearblend = 0.5f;
	blendData.gammablend = 2.2f;

}

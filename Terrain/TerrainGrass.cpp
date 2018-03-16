#include "../stdafx.h"
#include "TerrainGrass.h"
#include "Landscape.h"
#include "../Shader/GrassBuffer.h"

TerrainGrass::TerrainGrass()
{
	vertexBuffer = 0;
	instanceBuffer = 0;

	buffer = new GrassBuffer();
}

TerrainGrass::~TerrainGrass()
{


	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(instanceBuffer);
}

void TerrainGrass::Initialize(Landscape* land)
{
	this->land = land;
	CreateInstanceData();
	CreateBufferTemp();
	
}

void TerrainGrass::Update()
{

}

void TerrainGrass::Render()
{

	UINT stride[2];
	stride[0] = sizeof(Vertex);
	stride[1] = sizeof(D3DXMATRIX);


	UINT offset[2];
	offset[0] = 0;
	offset[1] = 0;

	ID3D11Buffer* bufferPointers[2];
	bufferPointers[0] = vertexBuffer;
	bufferPointers[1] = instanceBuffer;


	buffer->SetBuffers();
	D3D::GetDeviceContext()->IASetVertexBuffers(0, 2, bufferPointers, stride, offset);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	D3D::GetDeviceContext()->DrawInstanced(1, instanceData.size(), 0, 0);
}

void TerrainGrass::CreateBufferTemp()
{
	HRESULT hr;

	vertexCount = 1;
	Vertex* vertexData = new Vertex[vertexCount];
	int i = 0;
	vertexData[i++].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;


	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(Vertex) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertexData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));

	SAFE_DELETE_ARRAY(vertexData);



	if (instanceData.size() == 0)
		return;

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(D3DXMATRIX) * instanceData.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &instanceData[0];
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;



	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &instanceBuffer);
	assert(SUCCEEDED(hr));
}

void TerrainGrass::CreateInstanceData()
{
	//land->GetGroundPos(instanceData);

}

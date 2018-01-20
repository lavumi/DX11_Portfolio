#include "../stdafx.h"
#include "ObjectMesh.h"
#include "Mesh.h"

ObjectMesh::ObjectMesh(int type)
	:Shader(L"./Mesh/Mesh.fx"), type(type)
{
	CreateBuffer();
	CreateInputLayout(VertexTextureNormal::desc, VertexTextureNormal::count);
	D3DXMatrixIdentity(&world);
}

ObjectMesh::~ObjectMesh()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

}

void ObjectMesh::Render()
{

	UINT stride = sizeof(VertexTextureNormal);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	

	Shader::Render(indexCount);


}

void ObjectMesh::Update()
{
	Shader::Update();
}

void ObjectMesh::Move(D3DXVECTOR3 move)
{
	D3DXMatrixTranslation(&world, move.x, move.y, move.z);
	worldBuffer->SetWorld(world);

}


void ObjectMesh::CreateBuffer()
{
	HRESULT hr;

	
	Mesh* mesh;
	if(type == 0)
		mesh = new Mesh(L"Cube");
	else if (type == 1)
		mesh = new Mesh(L"Sphere");
	else if (type == 2)
		mesh = new Mesh(L"Pyramid");
	else if (type == 3)
		mesh = new Mesh(L"Cylinder");



	
	vertexCount = mesh->vertexCount;
	indexCount = mesh->indexCount;


	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;


	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexTextureNormal) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = mesh->vertexData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));



	//2. Index Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = mesh->indexData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));



	SAFE_DELETE(mesh);




}

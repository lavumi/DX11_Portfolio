#include "../stdafx.h"
#include "Mirror.h"

Mirror::Mirror()
	: vertexBuffer(nullptr), indexBuffer(nullptr)
{



	D3DXMatrixTranslation(&world, 10.0f, 5.0f, 20.0f);




	//	TwBar* bar = TweakBar::Get()->GetBar();
	//	TwAddVarRW(bar, "BoxPosition", TW_TYPE_DIR3F, &position, "group='Box'");

	//	CreateShader();
	CreateBuffer();





	//light.lightdirection = D3DXVECTOR3(0,0,1);
	//light.baselight = 0.45f;
	//UserInterface::AddSun(&light.lightdirection, &light.baselight);








	//	UserInterface::AddMaterialData(&material.ambient, &material.diffuse, &material.specular, &material.shininess, &parallexData.drawTexture);

}

Mirror::~Mirror()
{

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);


	
}

void Mirror::Update()
{


}

void Mirror::Render()
{

	UINT stride = sizeof(VertexTextureNormalTangent);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


}

void Mirror::RenderMirror()
{
	UINT stride = sizeof(VertexTextureNormalTangent);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBufferMirror, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBufferMirror, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
















void Mirror::CreateBuffer()
{
	HRESULT hr;

	vertexCount =24;
	indexCount = 36;

	vertexCountMirror = 4;
	indexCountMirror = 6;

	VertexTextureNormalTangent* vertexData = new VertexTextureNormalTangent[vertexCount];
	int i = 0;

	vertexData[i].position = D3DXVECTOR3(-4.2f, -4.2f, -0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 1);
	vertexData[i].position = D3DXVECTOR3(4.2f, -4.2f, -0.1f);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(4.2f, 4.2f, -0.1f);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(-4.2f, 4.2f, -0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 0);




	vertexData[i].position = D3DXVECTOR3(4.2f, 4.2f,  0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 1);
	vertexData[i].position = D3DXVECTOR3(4.2f, 4.2f, -0.1f);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(4.2f, -4.2f, -0.1f);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(4.2f, -4.2f,  0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 0);

	vertexData[i].position = D3DXVECTOR3(-4.2f, -4.2f, 0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 0);
	vertexData[i].position = D3DXVECTOR3(4.2f, -4.2f, 0.1f);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(4.2f, 4.2f, 0.1f);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(-4.2f, 4.2f, 0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 1);

	vertexData[i].position = D3DXVECTOR3(-4.2f, -4.2f, -0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 0);
	vertexData[i].position = D3DXVECTOR3(-4.2f, -4.2f, 0.1f);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(-4.2f, 4.2f, 0.1f);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(-4.2f, 4.2f, -0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 1);

	vertexData[i].position = D3DXVECTOR3(4.2f, 4.2f, 0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 1);
	vertexData[i].position = D3DXVECTOR3(-4.2f, 4.2f, 0.1f);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(-4.2f, 4.2f, -0.1f);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(4.2f, 4.2f, -0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 0);

	vertexData[i].position = D3DXVECTOR3(-4.2f, -4.2f, -0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 0);
	vertexData[i].position = D3DXVECTOR3(4.2f, -4.2f, -0.1f);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3(4.2f, -4.2f, 0.1f);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(-4.2f, -4.2f, 0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 1);

	//vertexData[i].position = D3DXVECTOR3(-4.2f, -4.2f, 0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 1);
	//vertexData[i].position = D3DXVECTOR3( 4.2f, -4.2f, 0.1f);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	//vertexData[i].position = D3DXVECTOR3( 4.2f,  4.2f, 0.1f);				vertexData[i++].uv = D3DXVECTOR2(1, 0);
	//vertexData[i].position = D3DXVECTOR3(-4.2f,  4.2f, 0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 0);
	//
	//vertexData[i].position = D3DXVECTOR3(-4.2f, -4.2f, -0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 1);
	//vertexData[i].position = D3DXVECTOR3(4.2f, -4.2f,  -0.1f);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	//vertexData[i].position = D3DXVECTOR3(4.2f, 4.2f,   -0.1f);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	//vertexData[i].position = D3DXVECTOR3(-4.2f, 4.2f,  -0.1f);			vertexData[i++].uv = D3DXVECTOR2(0, 0);



	UINT* indexData = new UINT[indexCount]{
		0, 3, 1, 3, 2, 1,
		
		5, 4, 6, 4, 7, 6,
		8, 9, 11, 10, 11, 9,
		12, 13, 15, 13, 14, 15,
		17, 16, 19, 17, 19, 18,
		20, 21, 23,22, 23, 21
	};


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
		D3DXVec3Normalize
		(
			&vertexData[i].normal
			, &vertexData[i].normal
		);
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


	



	i = 0;

	vertexData[i].position = D3DXVECTOR3(-4.0f, -4.0f, -0.11f);			vertexData[i++].uv = D3DXVECTOR2(0, 0);
	vertexData[i].position = D3DXVECTOR3 (4.0f, -4.0f, -0.11f);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	vertexData[i].position = D3DXVECTOR3( 4.0f,  4.0f, -0.11f);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	vertexData[i].position = D3DXVECTOR3(-4.0f,  4.0f, -0.11f);			vertexData[i++].uv = D3DXVECTOR2(0, 1);

	//vertexData[i].position = D3DXVECTOR3(-4.0f, -4.0f,-0.111f);			vertexData[i++].uv = D3DXVECTOR2(0, 1);
	//vertexData[i].position = D3DXVECTOR3(4.0f, -4.0f, -0.111f);			vertexData[i++].uv = D3DXVECTOR2(1, 1);
	//vertexData[i].position = D3DXVECTOR3(4.0f, 4.0f,  -0.111f);			vertexData[i++].uv = D3DXVECTOR2(1, 0);
	//vertexData[i].position = D3DXVECTOR3(-4.0f, 4.0f, -0.111f);			vertexData[i++].uv = D3DXVECTOR2(0, 0);



	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexTextureNormalTangent) * vertexCountMirror;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertexData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBufferMirror);
	assert(SUCCEEDED(hr));



	//2. Index Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indexCountMirror;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = indexData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBufferMirror);
	assert(SUCCEEDED(hr));



	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);
}


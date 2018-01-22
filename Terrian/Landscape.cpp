#include "../stdafx.h"
#include "Landscape.h"
#include "QuadTree.h"

//#include "../ProceduralTexture/RNDNoise.h"

Landscape::Landscape()
	:heightMapFile(L"./Terrian/heightmap.jpg")

{
	LoadHeightMap();
	CreateVertexData();
	CreateIndexData();
	CreateNormalData();
	CreateBuffer();

	D3DXMatrixIdentity(&world);
	diffuseMap = 0;
	normalMap = 0;
	specularMap = 0;


	quadTree = new QuadTree(width+1, height+1);
	quadTree->CreateTree();


	diffuseMap = new ID3D11ShaderResourceView*[3];

	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), L"./Terrian/normal.jpg", nullptr, nullptr, &normalMap, nullptr);
	



	assert(SUCCEEDED(hr));
	hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), L"./Terrian/sand.jpg", nullptr, nullptr, &diffuseMap[1], nullptr);
	assert(SUCCEEDED(hr));
	hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), L"./Terrian/mountain.jpg", nullptr, nullptr, &diffuseMap[2], nullptr);
	assert(SUCCEEDED(hr));

}



Landscape::~Landscape()
{
	SAFE_DELETE(quadTree);
	SAFE_DELETE_ARRAY(vertexData);
	
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);
}

void Landscape::LoadHeightMap()
{
	ID3D11Texture2D* texture;
	Texture::LoadTexture(heightMapFile, &texture);

	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);

	UINT* colors = new UINT[desc.Width * desc.Height];
	Texture::LoadPixel(texture, &colors);

	width = desc.Width;
	height = desc.Height;

	heightData = new BYTE[width * height];
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT tempY = height - z - 1;

			UINT color = colors[tempY * width + x];
			BYTE r = ((color & 0x00FF0000) >> 16);
			heightData[z * width + x] = r;
		}
	}

	width--;
	height--;
}

void Landscape::CreateVertexData()
{
	vertexCount = (width + 1) * (height + 1);

	UINT heightIndex = 0;
	vertexData = new VertexTextureNormalTangent[vertexCount];
	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			int index = (width + 1) * z + x;

			vertexData[index].position.x = (float)x -0.1f;
			vertexData[index].position.y = (float)heightData[index] / 7.5f - 18;
			vertexData[index].position.z = (float)z -0.1f;

			vertexData[index].uv.x = (float)(x );// (float)width;
			vertexData[index].uv.y = (float)(z );// (float)height;

			vertexData[index].normal = D3DXVECTOR3(0, 0, 0);
		}
	}
}

void Landscape::CreateIndexData()
{
	indexCount = width * height * 6;
	indexData;// = new UINT[indexCount];

	UINT count = 0;
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{

			indexData.push_back((width + 1) * z + x);
			indexData.push_back((width + 1) * (z + 1) + x);
			indexData.push_back((width + 1) * z + x + 1);
			indexData.push_back((width + 1) * z + x + 1);
			indexData.push_back((width + 1) * (z + 1) + x);
			indexData.push_back((width + 1) * (z + 1) + (x + 1));
			//indexData[count + 0] = (width + 1) * z + x;
			//indexData[count + 1] = (width + 1) * (z + 1) + x;
			//indexData[count + 2] = (width + 1) * z + x + 1;
			//indexData[count + 3] = (width + 1) * z + x + 1;
			//indexData[count + 4] = (width + 1) * (z + 1) + x;
			//indexData[count + 5] = (width + 1) * (z + 1) + (x + 1);
			//
			//count += 6;
		}//for(x)
	}//for(z)

	if (indexData.size() != indexCount)
		assert(0);
}

void Landscape::CreateNormalData()
{
	for (UINT i = 0; i < vertexCount; i++)
		vertexData[i].normal = D3DXVECTOR3(0, 0, 0);

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
}

void Landscape::CreateBuffer()
{

	HRESULT hr;
	D3D11_BUFFER_DESC vertexDesc = { 0 };
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.ByteWidth = sizeof(VertexTextureNormalTangent) * vertexCount;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	D3D11_SUBRESOURCE_DATA vertexSubResource = { 0 };
	vertexSubResource.pSysMem = vertexData;


	hr = D3D::GetDevice()->CreateBuffer(&vertexDesc, &vertexSubResource, &vertexBuffer);
	assert(SUCCEEDED(hr));






	D3D11_BUFFER_DESC indexDesc = { 0 };

	ZeroMemory(&indexDesc, sizeof(D3D11_BUFFER_DESC));
	indexDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexDesc.ByteWidth = sizeof(UINT) * indexCount;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexDesc.MiscFlags = 0;
	indexDesc.StructureByteStride = 0;


	hr = D3D::GetDevice()->CreateBuffer(&indexDesc, NULL, &indexBuffer);
	assert(SUCCEEDED(hr));



	D3D11_MAPPED_SUBRESOURCE subResource = { 0 };

	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &indexData[0], sizeof(UINT) * indexData.size());

	D3D::GetDeviceContext()->Unmap(indexBuffer, 0);
}

void Landscape::Update()
{

}

void Landscape::Render()
{
	UINT stride = sizeof(VertexTextureNormalTangent);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Landscape::GetY(D3DXVECTOR3 &position)
{
	UINT x = (int)position.x;
	UINT z = (int)position.z;

	if (x < 0 || x >= width)
		return;
	if (z < 0 || z >= width)
		return;

	UINT index0 = (width + 1)*z + x;
	UINT index1 = (width + 1)*(z + 1) + x;
	UINT index2 = (width + 1)*z + x + 1;
	UINT index3 = (width + 1)*(z + 1) + x + 1;

	D3DXVECTOR3 v0 = vertexData[index0].position;
	D3DXVECTOR3 v1 = vertexData[index1].position;
	D3DXVECTOR3 v2 = vertexData[index2].position;
	D3DXVECTOR3 v3 = vertexData[index3].position;

	//float nx = vertexData[index0].normal.x;

	float deltaX = (position.x - v0.x);
	float deltaZ = (position.z - v0.z);
	D3DXVECTOR3 temp;
	if (deltaX + deltaZ <= 1)
		temp = v0 + (v2 - v0) * deltaX + (v1 - v0)* deltaZ;
	else
		deltaX = 1 - deltaX;
	deltaZ = 1 - deltaZ;
	temp = v3 + (v2 - v3) * deltaX + (v1 - v3)* deltaZ;
	position.y = temp.y;
}

void Landscape::changeLOD(Frustum* frustum)
{

	vector<UINT> indexDataTemp;
	quadTree->setIndexByLOD(indexDataTemp, frustum);

	D3D11_MAPPED_SUBRESOURCE subResource = { 0 };

	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	if (indexDataTemp.size() == 0) {
		D3D::GetDeviceContext()->Unmap(indexBuffer, 0);
		indexCount = indexDataTemp.size();
		return;
	}
		
	memcpy(subResource.pData, &indexDataTemp[0], sizeof(UINT) * indexDataTemp.size());

	D3D::GetDeviceContext()->Unmap(indexBuffer, 0);
	indexCount = indexDataTemp.size();

}

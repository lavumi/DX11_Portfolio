#include "../stdafx.h"
#include "Landscape.h"
#include "QuadTree.h"

#include "../Shader/TerrainBuffer.h"
//#include "../ProceduralTexture/PerlinNoise.h"

Landscape::Landscape()
{
	D3DXMatrixIdentity(&world);
	diffuseMap = 0;
	normalMap = 0;
	specularMap = 0;
	LoadTextures();


	indexBuffer = 0;
	vertexBuffer = 0;

	fullindexBuffer = 0;
	fullvertexBuffer = 0;
}



Landscape::~Landscape()
{
//	SAFE_DELETE(quadTree);
	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(fullVertexData);
	
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);

	SAFE_RELEASE(fullindexBuffer);
	SAFE_RELEASE(fullvertexBuffer);



	SAFE_DELETE(wBuffer);
	SAFE_DELETE(buffer);
}

void Landscape::Initialize()
{
	LoadHeightMap();
	height = 8;
	width = 8;
	CreateVertexData();
	//CreateQuadVertexData();
	CreateQuadIndexData();
	


	CreateFullVertexData();
	CreateIndexData();
	CreateNormalData();
	CheckGround();



	CreateBuffer();
	//quadTree = new QuadTree(width + 1, height + 1);
	//quadTree->CreateTree();

	wBuffer = new WorldBuffer();
	wBuffer->SetWorld(world);
	buffer = new TerrainBuffer();

}

void Landscape::LoadHeightMap()
{
	ID3D11Texture2D* texture;
	Texture::LoadTexture(L"./Terrain/heightmap.jpg", &texture);

	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);

	UINT* colors = new UINT[desc.Width * desc.Height];
	Texture::LoadPixel(texture, &colors);

	worldWidth = desc.Width;
	worldHeight = desc.Height;

	heightData = new float[worldWidth * worldHeight];
	for (UINT z = 0; z < worldHeight; z++)
	{
		for (UINT x = 0; x < worldWidth; x++)
		{
			UINT tempY = worldHeight - z - 1;

			UINT color = colors[tempY * worldWidth + x];
			BYTE r = ((color & 0x00FF0000) >> 16);
			heightData[z * worldWidth + x] = (float)r / 255.0f * 34.0f - 18.0f;
		}
	}

	worldWidth--;
	worldHeight--;
}

void Landscape::CreateVertexData()
{
	vertexCount = (width + 1) * (height + 1);

	UINT heightIndex = 0;
	vertexData = new VertexTexture[vertexCount];
	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			int index = (width + 1) * z + x;

			vertexData[index].position.x = (float)x * 32;
			vertexData[index].position.y = 0; //(float)heightData[index] / 7.5f - 18;
			vertexData[index].position.z = (float)z * 32;
			
			vertexData[index].uv.x = (float)(x) * 32;// (float)width;
			vertexData[index].uv.y = (float)(z) * 32;// (float)height;


			//vertexData[index].position.x = (float)x ;
			//vertexData[index].position.y = (float)heightData[index];
			//vertexData[index].position.z = (float)z;
			//
			//vertexData[index].uv.x = (float)(x);// (float)width;
			//vertexData[index].uv.y = (float)(z);// (float)height;
			//
			//vertexData[index].normal = D3DXVECTOR3(0, 0, 0);
		}
	}
}

void Landscape::CreateIndexData()
{
	//indexCount = width * height * 6;
	//indexData;// = new UINT[indexCount];

	UINT count = 0;
	for (UINT z = 0; z < worldHeight; z++)
	{
		for (UINT x = 0; x < worldWidth; x++)
		{

			fullIndexData.push_back((worldWidth + 1) * z + x);
			fullIndexData.push_back((worldWidth + 1) * (z + 1) + x);
			fullIndexData.push_back((worldWidth + 1) * z + x + 1);
			fullIndexData.push_back((worldWidth + 1) * z + x + 1);
			fullIndexData.push_back((worldWidth + 1) * (z + 1) + x);
			fullIndexData.push_back((worldWidth + 1) * (z + 1) + (x + 1));
			//count += 6;
		}//for(x)
	}//for(z)

	fullIndexCount = fullIndexData.size();
}
void Landscape::CreateFullVertexData()
{
	fullVertexCount = (worldWidth + 1) * (worldHeight + 1);

	UINT heightIndex = 0;
	fullVertexData = new VertexTextureNormalTangent[fullVertexCount];
	for (UINT z = 0; z <= worldHeight; z++)
	{
		for (UINT x = 0; x <= worldWidth; x++)
		{
			int index = (worldWidth + 1) * z + x;

			fullVertexData[index].position.x = (float)x ;
			fullVertexData[index].position.y = heightData[index];
			fullVertexData[index].position.z = (float)z;


			float temp = fullVertexData[index].position.y;
			fullVertexData[index].uv.x = (float)(x);// (float)width;
			fullVertexData[index].uv.y = (float)(z);// (float)height;
		}
	}


}
/*
void Landscape::CreateQuadVertexData()
{
	tessVertexCount = (tessWidth + 1) * (tessHeight + 1);

	UINT heightIndex = 0;
	tessVertexData = new VertexTextureNormalTangent[tessVertexCount];
	for (UINT z = 0; z <= tessHeight; z++)
	{
		for (UINT x = 0; x <= tessWidth; x++)
		{
			int index = (tessWidth + 1) * z + x;

			tessVertexData[index].position.x = (float)x * 32;
			tessVertexData[index].position.y = 0; //(float)heightData[index] / 7.5f - 18;
			tessVertexData[index].position.z = (float)z * 32;

			tessVertexData[index].uv.x = (float)(x) * 32;// (float)width;
			tessVertexData[index].uv.y = (float)(z) * 32;// (float)height;
		}
	}
}
*/
void Landscape::CreateQuadIndexData()
{
	indexCount = width * height * 4;
	

	UINT count = 0;
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			indexData.push_back((width + 1) * (z + 1) + x);
			indexData.push_back((width + 1) * (z + 1) + (x + 1));
			
			indexData.push_back((width + 1) * z + x);
			indexData.push_back((width + 1) * z + x + 1);
		}//for(x)
	}//for(z)

	if (indexData.size() != indexCount)
		assert(0);
}

void Landscape::CreateNormalData()
{
	for (UINT i = 0; i < fullVertexCount; i++)
		fullVertexData[i].normal = D3DXVECTOR3(0, 0, 0);
	
	for (UINT i = 0; i < (fullIndexCount / 3); i++)
	{
		UINT index0 = fullIndexData[i * 3 + 0];
		UINT index1 = fullIndexData[i * 3 + 1];
		UINT index2 = fullIndexData[i * 3 + 2];
	
		VertexTextureNormalTangent v0 = fullVertexData[index0];
		VertexTextureNormalTangent v1 = fullVertexData[index1];
		VertexTextureNormalTangent v2 = fullVertexData[index2];
	
		D3DXVECTOR3 deltaPos1 = v1.position - v0.position;
		D3DXVECTOR3 deltaPos2 = v2.position - v0.position;
	
		D3DXVECTOR2 deltaUV1 = v1.uv - v0.uv;
		D3DXVECTOR2 deltaUV2 = v2.uv - v0.uv;
	
		D3DXVECTOR3 normal;
		D3DXVec3Cross(&normal, &deltaPos1, &deltaPos2);
	
		fullVertexData[index0].normal += normal;
		fullVertexData[index1].normal += normal;
		fullVertexData[index2].normal += normal;
	
		float det = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		fullVertexData[index0].tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*det;
		fullVertexData[index1].tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*det;
		fullVertexData[index2].tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*det;
	}
	
	for (UINT i = 0; i < vertexCount; i++)
	{
		D3DXVec3Normalize(&fullVertexData[i].normal, &fullVertexData[i].normal);
		D3DXVec3Normalize(&fullVertexData[i].tangent, &fullVertexData[i].tangent);
	}
}

void Landscape::CreateBuffer()
{

	HRESULT hr;
	D3D11_BUFFER_DESC vertexDesc = { 0 };
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.ByteWidth = sizeof(VertexTexture) * vertexCount;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem =vertexData;


	hr = D3D::GetDevice()->CreateBuffer(&vertexDesc, &subResource, &vertexBuffer);
	assert(SUCCEEDED(hr));






	D3D11_BUFFER_DESC indexDesc = { 0 };

	ZeroMemory(&indexDesc, sizeof(D3D11_BUFFER_DESC));
	indexDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexDesc.ByteWidth = sizeof(UINT) * indexCount;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexDesc.MiscFlags = 0;
	indexDesc.StructureByteStride = 0;

	subResource = { 0 };
	subResource.pSysMem = &indexData[0];

	hr = D3D::GetDevice()->CreateBuffer(&indexDesc,&subResource, &indexBuffer);
	assert(SUCCEEDED(hr));



	//D3D11_MAPPED_SUBRESOURCE isubResource = { 0 };
	//
	//ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	//D3D::GetDeviceContext()->Map
	//(
	//	indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &isubResource
	//);
	//
	//memcpy(isubResource.pData, &indexData[0], sizeof(UINT) * indexData.size());
	//
	//D3D::GetDeviceContext()->Unmap(indexBuffer, 0);





	vertexDesc = { 0 };
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.ByteWidth = sizeof(VertexTextureNormalTangent) * fullVertexCount;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	subResource = { 0 };
	subResource.pSysMem = fullVertexData;

	hr = D3D::GetDevice()->CreateBuffer(&vertexDesc, &subResource, &fullvertexBuffer);
	assert(SUCCEEDED(hr));






	indexDesc = { 0 };

	ZeroMemory(&indexDesc, sizeof(D3D11_BUFFER_DESC));
	indexDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexDesc.ByteWidth = sizeof(UINT) * fullIndexCount;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexDesc.MiscFlags = 0;
	indexDesc.StructureByteStride = 0;

	subResource = { 0 };
	subResource.pSysMem = &fullIndexData[0];

	hr = D3D::GetDevice()->CreateBuffer(&indexDesc, &subResource, &fullindexBuffer);
	assert(SUCCEEDED(hr));






}

void Landscape::Update()
{

}

void Landscape::Render()
{
	//return;
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	//D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	wBuffer->SetBuffer();
	buffer->SetBuffers();

	D3D::GetDeviceContext()->DSSetShaderResources(0, 1, &heightMap);
	D3D::GetDeviceContext()->DSSetShaderResources(1, 1, &worldNormalMap);

	D3D::GetDeviceContext()->PSSetShaderResources(0, 3, diffuseMap);
	D3D::GetDeviceContext()->PSSetShaderResources(5, 1, &normalMap);

	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);

}

void Landscape::RenderShadow()
{
	UINT stride = sizeof(VertexTextureNormalTangent);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &fullvertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(fullindexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	//D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	wBuffer->SetBuffer();


	D3D::GetDeviceContext()->DrawIndexed(fullIndexCount, 0, 0);


}

void Landscape::GetY(D3DXVECTOR3 &position)
{
	UINT x = (int)position.x;
	UINT z = (int)position.z;

	float deltaX = position.x - (float)x;
	float deltaZ = position.z - (float)z;


	if (x < 0 || x >= 256)
		return;
	if (z < 0 || z >= 256)
		return;



	UINT index0 = (255 + 1) * z + x;
	UINT index1 = (255 + 1) *(z + 1) + x;
	UINT index2 = (255 + 1) * z + x + 1;
	UINT index3 = (255 + 1) *(z + 1) + x + 1;



	//float height1 = heightData[index0] * (1 - deltax) + heightData[index1] * deltax;
	//float height2 = heightData[index2] * (1 - deltax) + heightData[index3] * deltax;
	//
	//position.y = height1 * (1 - deltaz) + height2 * deltaz;



	float v0 = heightData[index0];
	float v1 = heightData[index1];
	float v2 = heightData[index2];
	float v3 = heightData[index3];
	
	////float nx = vertexData[index0].normal.x;
	//
	//float deltaX = (position.x - v0.x);
	//float deltaZ = (position.z - v0.z);
	float temp;
	if (deltaX + deltaZ <= 1)
		temp = v0 + (v2 - v0) * deltaX + (v1 - v0)* deltaZ;
	else {
		deltaX = 1 - deltaX;
		deltaZ = 1 - deltaZ;
		temp = v3 + (v2 - v3) * deltaX + (v1 - v3)* deltaZ;
	}

	
	position.y = temp;
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

void Landscape::SetPlane(D3DXPLANE & plane)
{
	buffer->SetPLANE(plane);
}

void Landscape::LoadTextures()
{
	diffuseMap = new ID3D11ShaderResourceView*[3];

	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), L"./Terrain/normal.jpg", nullptr, nullptr, &normalMap, nullptr);
	assert(SUCCEEDED(hr));
	hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), L"./Terrain/underwater.jpg", nullptr, nullptr, &diffuseMap[1], nullptr);
	assert(SUCCEEDED(hr));
	hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), L"./Terrain/mountain.jpg", nullptr, nullptr, &diffuseMap[2], nullptr);
	assert(SUCCEEDED(hr));

	hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), L"./Terrain/heightmap.jpg", nullptr, nullptr, &heightMap, nullptr);
	assert(SUCCEEDED(hr));
	hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), L"./Terrain/worldNormalMap.png", nullptr, nullptr, &worldNormalMap, nullptr);
	assert(SUCCEEDED(hr));
}

void Landscape::CheckGround()
{
	//D3DXMatrixTranslation(&world, 0, 0, 0);
	//grassGround.push_back(world);


	D3DXMATRIX position, rotation, world;
	for(UINT i = 0;i<fullVertexCount;i++){
		if (fullVertexData[i].normal.y >= 0.95f){// && fullVertexData[i].position.y > -7.0f && fullVertexData[i].position.y <0.0f) {
			
			D3DXMatrixTranslation(&position, fullVertexData[i].position.x, fullVertexData[i].position.y, fullVertexData[i].position.z);
	
			//D3DXMatrixRotationY(&rotation, (float)D3DX_PI / 180 *0);
			world = position;
			grassGround.push_back(world);
			
			//D3DXMatrixRotationY(&rotation, (float)D3DX_PI / 180 * 120);
			//world = rotation * position;
			//grassGround.push_back(world);
			//
			//D3DXMatrixRotationY(&rotation, (float)D3DX_PI / 180 * 240);
			//world = rotation * position;
			//grassGround.push_back(world);
		}
			
	}
	int a = 10;
}

#include "../stdafx.h"
#include "MySphere.h"

MySphere::MySphere()
	: Shader(L"./Mesh/MySphere.fx")
	, scale(scale), vari(6), radius(10)
{
	D3DXMatrixIdentity(&matrixData.world);
	
	D3DXMATRIX rotation, scaling;
	//D3DXMatrixScaling(&matrixData.world, scale.x, scale.y, 1);

	materialData.diffuse = D3DXCOLOR(1, 1, 1, 1);
	materialData.ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1);

	
	CreateData();
	CreateBuffer();
	CreateMaterialBuffer();
	CreateInputLayout();

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


	UserInterface::AddSphereParam(&radius, (int*)&vari);



	function<void(void *)> func = bind
	(
		&MySphere::RedrawSphere
		, this
		, placeholders::_1
	);
	TweakBar::Add(&func, func);

	TwAddButton(TweakBar::Get()->GetBar(), "RedrawMySphere", Notified, &func, "group = 'MySphere'");
}

MySphere::~MySphere()
{

	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	for (UINT i = 0; i < 2; i++)
		SAFE_RELEASE(texture[i]);
}

void MySphere::Update()
{
	D3D11_MAPPED_SUBRESOURCE resc;
	HRESULT hr = D3D::GetDeviceContext()->Map(
		blendBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resc
	);

	BlendShaderDesc* data = (BlendShaderDesc*)resc.pData;
	memcpy(data, &blendData, sizeof(BlendShaderDesc));
	D3D::GetDeviceContext()->Unmap(blendBuffer, 0);



	UpdateMaterialBuffer();

	Shader::Update();
}

void MySphere::Render()
{
	ID3D11DeviceContext* dc = D3D::GetDeviceContext();

	UINT stride = sizeof(VertexColorTextureNormal);
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDeviceContext()->PSSetShaderResources(0, 2, texture);
	D3D::GetDeviceContext()->PSSetConstantBuffers(1, 1, &materialBuffer);
	D3D::GetDeviceContext()->PSSetConstantBuffers(2, 1, &blendBuffer);
	Shader::Render();

	dc->DrawIndexed(indexCount, 0, 0);
}

void MySphere::RedrawSphere(void *)
{

	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);

	CreateData();

	D3D::GetDeviceContext()->UpdateSubresource
	(
		vertexBuffer
		, 0
		, NULL
		, vertexData
		, sizeof(VertexTextureNormal) * vertexCount
		, 0
	);

	D3D::GetDeviceContext()->UpdateSubresource
	(
		indexBuffer
		, 0
		, NULL
		, indexData
		, sizeof(VertexTextureNormal) * vertexCount
		, 0
	);

}

void MySphere::CreateInputLayout()
{
	HRESULT hr;
	hr = D3D::GetDevice()->CreateInputLayout
	(
		VertexColorTextureNormal::desc
		, VertexColorTextureNormal::count
		, vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, &layout
	);
	assert(SUCCEEDED(hr));
}

void MySphere::CreateData()
{



	vertexCount = (vari * 2 + 1) * (vari+1) * 4 + 2;
	vertexData = new VertexColorTextureNormal[ vertexCount];
	
	
	
	///Position
	int vtxidx = 1;
	vertexData[0].position = D3DXVECTOR3(0, radius, 0);
	//north
	for (UINT i = 0; i < vari; i++) {
		float y = radius * std::cosf(D3DX_PI / 2  / (vari + 1) * ((float)i + 1));
		float xz_radius = radius * std::sinf(D3DX_PI / 2  / (vari + 1) * ((float)i + 1));
		for (UINT j = 0; j < (vari + 1)*4; j++)
		{
			float x = xz_radius * std::cosf(D3DX_PI / 2 / (vari + 1) * (float)j);
			float z = xz_radius * std::sinf(D3DX_PI / 2 / (vari + 1) * (float)j);
			vertexData[vtxidx++].position = D3DXVECTOR3(x, y, z);
		}
	}
	//middle
	for (UINT i = 0; i < 1; i++) {
		float y = 0;
		float xz_radius = radius;
		for (UINT j = 0; j < (vari + 1) * 4; j++)
		{
			float x = xz_radius * std::cosf(D3DX_PI / 2 / (vari + 1) * (float)j);
			float z = xz_radius * std::sinf(D3DX_PI / 2 / (vari + 1) * (float)j);
			vertexData[vtxidx++].position = D3DXVECTOR3(x, y, z);
		}
	}

	//south
	for (UINT i = 0; i < vari; i++) {
		float y = radius * std::sinf(D3DX_PI / 2 / (vari + 1) * ((float)i + 1)) * -1;
		float xz_radius =  radius * std::cosf(D3DX_PI / 2 / (vari + 1) * ((float)i + 1));
		for (UINT j = 0; j < (vari + 1) * 4; j++)
		{
			float x = xz_radius * std::cosf(D3DX_PI / 2 / (vari + 1) * (float)j);
			float z = xz_radius * std::sinf(D3DX_PI / 2 / (vari + 1) * (float)j);
			vertexData[vtxidx++].position = D3DXVECTOR3(x, y, z);
		}
	}


	vertexData[vertexCount - 1].position = D3DXVECTOR3(0, -1 * (float)radius, 0);



	///Normal
	for (UINT i = 0; i < vertexCount; i++) {
		vertexData[i].normal = vertexData[i].position;
	}
	






	///Index
	indexCount = (vari + 1) * 4 * (vari * 2 * 2 + 2)*3;
	indexData = new UINT[indexCount];

	UINT idxidx = 0;
	UINT vertexnumber = 1;
	//첫줄
	for (UINT i = 0; i < 1; i++) {


		for (UINT j = 0; j < (vari+1)*4-1; j++) {
			indexData[idxidx++] = (vari + 1) * 4 * i + 1+j;
			indexData[idxidx++] = 0;
			indexData[idxidx++] = (vari + 1) * 4 * i + 2 + j;
			indexData[idxidx++] = (vari + 1) * 4 * i + 1 + j;
			indexData[idxidx++] = (vari + 1) * 4 * i + 2 + j;
			indexData[idxidx++] = (vari + 1) * 4 * (i + 1) + 1 + j;
		}

		UINT j = (vari + 1) * 4 - 1;
		indexData[idxidx++] = (vari + 1) * 4 * i + 1 + j;
		indexData[idxidx++] = 0;
		indexData[idxidx++] = 1;
		indexData[idxidx++] = (vari + 1) * 4 * i + 1 + j;
		indexData[idxidx++] = 1;
		indexData[idxidx++] = (vari + 1) * 4 * (i + 1) + 1 + j;

	}
	//그외
	for (UINT i = 1; i < vari * 2 ; i++) {
		for (UINT j = 0; j < (vari + 1) * 4 - 1; j++) {
			indexData[idxidx++] = (vari + 1) * 4 * i + 1 + j;
			indexData[idxidx++] = (vari + 1) * 4 * (i - 1) + 2+j;
			indexData[idxidx++] = (vari + 1) * 4 * i + 2 + j;
			indexData[idxidx++] = (vari + 1) * 4 * i + 1 + j;
			indexData[idxidx++] = (vari + 1) * 4 * i + 2 + j;
			indexData[idxidx++] = (vari + 1) * 4 * (i + 1) + 1 + j;
		}
		UINT j = (vari + 1) * 4 - 1;
		indexData[idxidx++] = (vari + 1) * 4 * i + 1 + j;
		indexData[idxidx++] = (vari + 1) * 4 * (i - 1) + 1;
		indexData[idxidx++] = (vari + 1) * 4 * i + 1;
		indexData[idxidx++] = (vari + 1) * 4 * i + 1 + j;
		indexData[idxidx++] = (vari + 1) * 4 * i + 1;
		indexData[idxidx++] = (vari + 1) * 4 * (i + 1) + 1 + j;

	}
	//끝줄
	for (UINT i = vari * 2 ; i < vari * 2 + 1; i++) {
		for (UINT j = 0; j <(vari + 1) * 4 - 1; j++) {
			indexData[idxidx++] = (vari + 1) * 4 * i + 1 + j;
			indexData[idxidx++] = (vari + 1) * 4 * (i - 1) + 2 + j;
			indexData[idxidx++] = (vari + 1) * 4 * i + 2 + j;
			indexData[idxidx++] = (vari + 1) * 4 * i + 1 + j;
			indexData[idxidx++] = (vari + 1) * 4 * i + 2 + j;
			indexData[idxidx++] = vertexCount-1;
		}
		UINT j = (vari + 1) * 4 - 1;
		indexData[idxidx++] = (vari + 1) * 4 * i + 1 + j;
		indexData[idxidx++] = (vari + 1) * 4 * (i - 1) + 1;
		indexData[idxidx++] = (vari + 1) * 4 * i + 1;
		indexData[idxidx++] = (vari + 1) * 4 * i + 1 + j;
		indexData[idxidx++] = (vari + 1) * 4 * i + 1;
		indexData[idxidx++] = vertexCount - 1;
	}

}

void MySphere::CreateBuffer()
{

	HRESULT hr;

	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;


	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexColorTextureNormal) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertexData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));


	//2. Index Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = indexData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));


	//3. Blend Buffer
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

	UserInterface::AddBlendValue(&blendData.selected, &blendData.linearblend, &blendData.gammablend);
}

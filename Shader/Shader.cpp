#include "../stdafx.h"
#include "Shader.h"



void Shader::Update()
{


}


void Shader::SetMatrix(D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection)
{
	D3D11_MAPPED_SUBRESOURCE subResource = { 0 };

	wData.world = world;
	vpData.view = view;
	vpData.projection = projection;

	D3DXMatrixTranspose(&wData.world, &wData.world);
	D3DXMatrixTranspose(&vpData.view, &vpData.view);
	D3DXMatrixTranspose(&vpData.projection, &vpData.projection);

	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		wBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &wData, sizeof(WorldMatrix));
	D3D::GetDeviceContext()->Unmap(wBuffer, 0);


	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		vpBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &vpData, sizeof(ViewProjectionMatrix));
	D3D::GetDeviceContext()->Unmap(vpBuffer, 0);
}

Shader::Shader(wstring shaderFile)
	: shaderFile(shaderFile)
{
	CreateVertexShader();
	CreatePixelShader();
	CreateMATRIXBuffer();
	geoBlob = 0;
	geoShader = 0;
}

Shader::~Shader()
{


	SAFE_RELEASE(layout);

	SAFE_RELEASE(vertexBlob);
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(wBuffer);
	SAFE_RELEASE(vpBuffer);

	SAFE_RELEASE(pixelBlob);
	SAFE_RELEASE(pixelShader);

	SAFE_RELEASE(geoBlob);
	SAFE_RELEASE(geoShader);
}

void Shader::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC * desc, UINT count)
{
	HRESULT hr = D3D::GetDevice()->CreateInputLayout
	(
		desc
		, count
		, vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, &layout
	);
	if(!SUCCEEDED(hr))
		assert(0);
}

void Shader::CreateMATRIXBuffer()
{
	D3D11_BUFFER_DESC desc;
	HRESULT hr;


	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(WorldMatrix);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &wBuffer);
	assert(SUCCEEDED(hr));

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(ViewProjectionMatrix);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &vpBuffer);
	assert(SUCCEEDED(hr));

}

void Shader::CreateVertexShader()
{
	ID3D10Blob* error;

	HRESULT hr = D3DX10CompileFromFile
	(
		shaderFile.c_str(), NULL, NULL, "VS", "vs_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &vertexBlob, &error, NULL
	);
	CheckShaderError(hr, error);

	hr = D3D::GetDevice()->CreateVertexShader
	(
		vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, NULL
		, &vertexShader
	);
	assert(SUCCEEDED(hr));
}

void Shader::CreatePixelShader()
{
	ID3D10Blob* error;
	HRESULT hr = D3DX10CompileFromFile
	(
		shaderFile.c_str(), NULL, NULL, "PS", "ps_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &pixelBlob, &error, NULL
	);
	CheckShaderError(hr, error);
	
	hr = D3D::GetDevice()->CreatePixelShader
	(
		pixelBlob->GetBufferPointer()
		, pixelBlob->GetBufferSize()
		, NULL
		, &pixelShader
	);
	assert(SUCCEEDED(hr));
}



void Shader::CreateGeometryShader()
{
	ID3D10Blob* error;
	HRESULT hr = D3DX10CompileFromFile
	(
		shaderFile.c_str(), NULL, NULL, "GS", "gs_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &geoBlob, &error, NULL
	);
	CheckShaderError(hr, error);

	hr = D3D::GetDevice()->CreateGeometryShader
	(
		geoBlob->GetBufferPointer()
		, geoBlob->GetBufferSize()
		, NULL
		, &geoShader
	);
	assert(SUCCEEDED(hr));
}

void Shader::CheckShaderError(HRESULT hr, ID3D10Blob * error)
{
	if (FAILED(hr))
	{
		if (error != NULL)
		{
			string str = (const char *)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}
}
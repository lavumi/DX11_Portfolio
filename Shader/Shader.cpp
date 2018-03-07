#include "../stdafx.h"
#include "Shader.h"



void Shader::SetShader()
{
	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);
}

Shader::Shader(wstring shaderFile)
	: shaderFile(shaderFile)
{
	CreateVertexShader();
	CreatePixelShader();
	//CreateGeometryShader();

	geoBlob = 0;
	geoShader = 0;
}

Shader::~Shader()
{

	SAFE_RELEASE(layout);

	SAFE_RELEASE(vertexBlob);
	SAFE_RELEASE(vertexShader);

	SAFE_RELEASE(pixelBlob);
	SAFE_RELEASE(pixelShader);

	SAFE_RELEASE(geoBlob);
	SAFE_RELEASE(geoShader);
}

bool Shader::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC * desc, UINT count)
{

	void* temp = vertexBlob->GetBufferPointer();
	SIZE_T temp2 = vertexBlob->GetBufferSize();
	HRESULT hr = D3D::GetDevice()->CreateInputLayout
	(
		desc
		, count
		, temp
		, temp2
		, &layout
	);
	if (!SUCCEEDED(hr))
		return false;
	else
		return true;
}

bool Shader::CreateVertexShader()
{
	ID3D10Blob* error;

	HRESULT hr = D3DX10CompileFromFile
	(
		shaderFile.c_str(), NULL, NULL, "VS", "vs_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &vertexBlob, &error, NULL
	);
	if (CheckShaderError(hr, error) == false) {
		return false;
	}
	
	hr = D3D::GetDevice()->CreateVertexShader
	(
		vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, NULL
		, &vertexShader
	);
	return SUCCEEDED(hr);
}

bool Shader::CreatePixelShader()
{
	ID3D10Blob* error;
	HRESULT hr = D3DX10CompileFromFile
	(
		shaderFile.c_str(), NULL, NULL, "PS", "ps_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &pixelBlob, &error, NULL
	);
	if (CheckShaderError(hr, error) == false) {
		return false;
	}
	
	hr = D3D::GetDevice()->CreatePixelShader
	(
		pixelBlob->GetBufferPointer()
		, pixelBlob->GetBufferSize()
		, NULL
		, &pixelShader
	);
	return SUCCEEDED(hr);
}



bool Shader::CreateGeometryShader()
{
	ID3D10Blob* error;
	HRESULT hr = D3DX10CompileFromFile
	(
		shaderFile.c_str(), NULL, NULL, "GS", "gs_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &geoBlob, &error, NULL
	);
	if (CheckShaderError(hr, error) == false) {
		return false;
	}

	hr = D3D::GetDevice()->CreateGeometryShader
	(
		geoBlob->GetBufferPointer()
		, geoBlob->GetBufferSize()
		, NULL
		, &geoShader
	);
	return SUCCEEDED(hr);
}

bool Shader::CheckShaderError(HRESULT hr, ID3D10Blob * error)
{
	if (FAILED(hr))
	{
		if (error != NULL)
		{
			string str = (const char *)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		return false;
	}
	return true;
}
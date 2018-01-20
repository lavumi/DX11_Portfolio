#include "../stdafx.h"
#include "RNDNoise.h"
#include "../Render/RenderTexture.h"

#include "../Object/OrthoWindowPlane.h"

RNDNoise::RNDNoise()
	:vertexBlob(nullptr), pixelBlob(nullptr)
	, vertexShader(nullptr), pixelShader(nullptr)
	, vertexBuffer(nullptr), indexBuffer(nullptr)
{

	basePath = L"./Shader/FX/";
	CreateBuffer();
	CreateShader(L"rndNoise.fx");

	rndNoise = new RenderTexture(256,256);



}

RNDNoise::~RNDNoise()
{
}

void RNDNoise::MakeRNDNoise() {
	rndNoise->SetTarget();
	rndNoise->Clear(1, 1, 1, 1);



	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;
	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);


	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);

	D3D::Get()->SetDefaultRenderView();

	//rndNoise->SaveTexture(L"Test.bmp");
}
ID3D11ShaderResourceView** RNDNoise::GetRNDNoise()
{



	return rndNoise->GetShadowResourceView();

}

void RNDNoise::CreateShader(wstring file)
{
	SAFE_RELEASE(vertexBlob);
	SAFE_RELEASE(vertexShader);

	SAFE_RELEASE(pixelBlob);
	SAFE_RELEASE(pixelShader);
	ID3D10Blob* error;
	HRESULT hr = D3DX10CompileFromFile
	(
		(basePath + file).c_str(), NULL, NULL, "VS", "vs_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &vertexBlob, &error, NULL
	);

	if (FAILED(hr))
	{
		if (error != NULL)
		{
			string str = (const char *)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}

	hr = D3D::GetDevice()->CreateVertexShader
	(
		vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, NULL
		, &vertexShader
	);
	assert(SUCCEEDED(hr));

	hr = D3DX10CompileFromFile
	(
		(basePath + file).c_str(), NULL, NULL, "PS", "ps_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &pixelBlob, &error, NULL
	);

	if (FAILED(hr))
	{
		if (error != NULL)
		{
			string str = (const char *)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}

	hr = D3D::GetDevice()->CreatePixelShader
	(
		pixelBlob->GetBufferPointer()
		, pixelBlob->GetBufferSize()
		, NULL
		, &pixelShader
	);
	assert(SUCCEEDED(hr));






	hr = D3D::GetDevice()->CreateInputLayout
	(
		VertexTexture::desc
		, VertexTexture::count
		, vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, &layout
	);
	if (!SUCCEEDED(hr))
		assert(0);
}

void RNDNoise::CreateBuffer()
{

	VertexTexture* rect = new VertexTexture[4];

	rect[0].position = D3DXVECTOR3(-1, -1, 0);
	rect[1].position = D3DXVECTOR3(-1, 1,  0);
	rect[2].position = D3DXVECTOR3(1, -1,  0);
	rect[3].position = D3DXVECTOR3(1, 1,   0);

	rect[0].uv = D3DXVECTOR2(0, 1);
	rect[1].uv = D3DXVECTOR2(0, 0);
	rect[2].uv = D3DXVECTOR2(1, 1);
	rect[3].uv = D3DXVECTOR2(1, 0);

	UINT* index = new UINT[6]{0,1,2,2,1,3};



	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexTexture) * 4;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = rect;



	HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));


	//2. Index Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * 6;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = index;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));

	SAFE_DELETE_ARRAY(rect);
	SAFE_DELETE_ARRAY(index);

}

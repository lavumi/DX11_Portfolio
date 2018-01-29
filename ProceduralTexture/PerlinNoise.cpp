#include "../stdafx.h"
#include "PerlinNoise.h"
#include "../Render/RenderTexture.h"

#include "../Object/OrthoWindowPlane.h"

PerlinNoise::PerlinNoise()
	:vertexBlob(nullptr), pixelBlob(nullptr)
	, vertexShader(nullptr), pixelShader(nullptr)
	, vertexBuffer(nullptr), indexBuffer(nullptr)
{

	basePath = L"./Shader/FX/";
	CreateBuffer();
	
	CreateShader(L"rndNoise.fx");

	rndNoise = new RenderTexture(1024,1024);


	MakePerlinNoise();
}

PerlinNoise::~PerlinNoise()
{
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(layout);
	SAFE_RELEASE(vertexShaderfinal);
	SAFE_RELEASE(pixelShaderfinal);
	SAFE_RELEASE(layoutfinal);
	SAFE_RELEASE(vertexBlob);
	SAFE_RELEASE(pixelBlob);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);


	SAFE_RELEASE(seedBuffer);

}

void PerlinNoise::MakePerlinNoise() {

	rndNoise->SetTarget();
	rndNoise->Clear(0, 0, 0, 0);




	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;
	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	//시드 세팅
	seed.x = (float)rand()/532;
	seed.y = (float)rand() / 242;
	D3D11_MAPPED_SUBRESOURCE subResource;
	HRESULT hr = D3D::GetDeviceContext()->Map
	(
		seedBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);
	memcpy(subResource.pData, &seed, sizeof(D3DXVECTOR4));
	D3D::GetDeviceContext()->Unmap(seedBuffer, 0);
	
	D3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &seedBuffer);



	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);


	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);

	D3D::Get()->SetDefaultRenderView();
	//CreateShader();
	//rndNoise->SaveTexture(L"Test.png");
}
void PerlinNoise::Render()
{
	D3D::Get()->SetBlender_Off();
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//여기까지 모델에서







	
	D3D::GetDeviceContext()->IASetInputLayout(layoutfinal);
	D3D::GetDeviceContext()->VSSetShader(vertexShaderfinal, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShaderfinal, NULL, 0);
	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, rndNoise->GetShadowResourceView());;
	
	Sampler::Get()->SetDefault();
	
	
	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);
}
ID3D11ShaderResourceView** PerlinNoise::GetPerlinNoise()
{
		return rndNoise->GetShadowResourceView();
}

void PerlinNoise::CreateShader(wstring file)
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




	file = L"finalDraw.fx";

	SAFE_RELEASE(vertexBlob);
	//SAFE_RELEASE(vertexShader);

	SAFE_RELEASE(pixelBlob);
	//SAFE_RELEASE(pixelShader);
	error;
	hr = D3DX10CompileFromFile
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
		, &vertexShaderfinal
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
		, &pixelShaderfinal
	);
	assert(SUCCEEDED(hr));






	hr = D3D::GetDevice()->CreateInputLayout
	(
		VertexTexture::desc
		, VertexTexture::count
		, vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, &layoutfinal
	);
	if (!SUCCEEDED(hr))
		assert(0);
}

void PerlinNoise::CreateBuffer()
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



	//seedBuffer
//	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(D3DXVECTOR4);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &seedBuffer);
	assert(SUCCEEDED(hr));
}

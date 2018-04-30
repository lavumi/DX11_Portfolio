#include "../stdafx.h"
#include "Convert_Normal_Height.h"
#include "../Render/RenderTexture.h"

Convert_Normal_Height::Convert_Normal_Height()
	:vertexBlob(nullptr), pixelBlob(nullptr)
	, vertexShader(nullptr), pixelShader(nullptr)
	, vertexBuffer(nullptr), indexBuffer(nullptr)
{
	target = 0;
	CreateShader(L"./FX/CreateTangentMap.fx");
	CreateBuffer();
}

Convert_Normal_Height::~Convert_Normal_Height()
{
	SAFE_RELEASE(vertexShader );
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(layout);
	SAFE_RELEASE(vertexBlob);
	SAFE_RELEASE(pixelBlob);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_DELETE(target);
}

ID3D11ShaderResourceView * Convert_Normal_Height::Convert(wstring resource, bool HeightMap)
{
	ID3D11ShaderResourceView* temp;

	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), resource.c_str(), nullptr, nullptr, &temp, nullptr);
	assert(SUCCEEDED(hr));


	SAFE_DELETE(target);
	target = new RenderTexture(256, 256);
	target->Initialize();

	target->SetTarget();
	target->Clear(0,0,0,1);



	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;
	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &temp);


	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);


	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);

	D3D::Get()->SetDefaultRenderView();

	target->SaveTexture(L"Test.png");
	SAFE_RELEASE(temp);
	
	return nullptr;
}

void Convert_Normal_Height::CreateShader(wstring file)
{
	SAFE_RELEASE(vertexBlob);
	SAFE_RELEASE(vertexShader);

	SAFE_RELEASE(pixelBlob);
	SAFE_RELEASE(pixelShader);
	ID3D10Blob* error;
	HRESULT hr = D3DX10CompileFromFile
	(
		(file).c_str(), NULL, NULL, "VS", "vs_5_0"
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
		(file).c_str(), NULL, NULL, "PS", "ps_5_0"
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

void Convert_Normal_Height::CreateBuffer()
{
	VertexTexture* rect = new VertexTexture[4];

	rect[0].position = D3DXVECTOR3(-1, -1, 0.5f);
	rect[1].position = D3DXVECTOR3(-1, 1,  0.5f);
	rect[2].position = D3DXVECTOR3(1, -1,  0.5f);
	rect[3].position = D3DXVECTOR3(1, 1,   0.5f);

	rect[0].uv = D3DXVECTOR2(0, 1);
	rect[1].uv = D3DXVECTOR2(0, 0);
	rect[2].uv = D3DXVECTOR2(1, 1);
	rect[3].uv = D3DXVECTOR2(1, 0);

	UINT* index = new UINT[6]{ 0,1,2,2,1,3 };



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

#include "../stdafx.h"
#include "Texture.h"

void Texture::LoadTexture(wstring fileName, ID3D11Texture2D** texture)
{
	HRESULT hr;

	ID3D11Texture2D* srcTexture;
	hr = D3DX11CreateTextureFromFile
	(
		D3D::GetDevice()
		, fileName.c_str()
		, NULL
		, NULL
		, (ID3D11Resource **)&srcTexture
		, NULL
	);
	assert(SUCCEEDED(hr));

	D3D11_TEXTURE2D_DESC srcTextureDesc;
	srcTexture->GetDesc(&srcTextureDesc);


	D3D11_TEXTURE2D_DESC destTextureDesc;
	ZeroMemory(&destTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	destTextureDesc.Width = srcTextureDesc.Width;
	destTextureDesc.Height = srcTextureDesc.Height;
	destTextureDesc.MipLevels = 1;
	destTextureDesc.ArraySize = 1;
	destTextureDesc.Format = srcTextureDesc.Format;
	destTextureDesc.SampleDesc.Count = 1;
	destTextureDesc.SampleDesc.Quality = 0;
	destTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	destTextureDesc.Usage = D3D11_USAGE_STAGING;

	ID3D11Texture2D* destTexture;
	hr = D3D::GetDevice()->CreateTexture2D(&destTextureDesc, NULL, &destTexture);
	assert(SUCCEEDED(hr));

	hr = D3DX11LoadTextureFromTexture
	(
		D3D::GetDeviceContext()
		, srcTexture
		, NULL
		, destTexture
	);
	assert(SUCCEEDED(hr));

	*texture = destTexture;

	SAFE_RELEASE(srcTexture);
}

void Texture::LoadPixel(ID3D11Texture2D * texture, UINT ** colors)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);


	D3D11_MAPPED_SUBRESOURCE mapResource;
	hr = D3D::GetDeviceContext()->Map(texture, 0, D3D11_MAP_READ, NULL, &mapResource);
	assert(SUCCEEDED(hr));

	UINT width = mapResource.RowPitch / sizeof(UINT);
	UINT height = desc.Height;

	*colors = new UINT[width * height];
	memcpy(*colors, mapResource.pData, sizeof(UINT) * width * height);

	D3D::GetDeviceContext()->Unmap(texture, 0);
}

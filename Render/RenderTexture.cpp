#include "../stdafx.h"
#include "RenderTexture.h"

RenderTexture::RenderTexture(UINT width, UINT height)
	:resourceView(NULL), renderView(NULL), texture(NULL)
{


	D3DInfo info;
	D3D::GetInfo(&info);

	if (width < 1  )
		this->width = info.screenWidth;
	else
		this->width = width;


	if (height < 1 )
		this->height = info.screenHeight;
	else
		this->height = height;




	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	textureDesc.Width = this->width;
	textureDesc.Height = this->height;

	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;




	
	HRESULT hr = D3D::GetDevice()->CreateTexture2D(&textureDesc, NULL, &texture);
	assert(SUCCEEDED(hr));

	D3D11_RENDER_TARGET_VIEW_DESC renderDesc;
	ZeroMemory(&renderDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderDesc.Format = textureDesc.Format;
	renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderDesc.Texture2D.MipSlice = 0;

	hr = D3D::GetDevice()->CreateRenderTargetView(texture, &renderDesc, &renderView);
	assert(SUCCEEDED(hr));


	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	viewDesc.Format = textureDesc.Format;
	viewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;

	hr = D3D::GetDevice()->CreateShaderResourceView(texture, &viewDesc, &resourceView);
	assert(SUCCEEDED(hr));



	D3D11_TEXTURE2D_DESC depthBufferDesc;


	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width =  this->width;
	depthBufferDesc.Height = this->height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	D3D::GetDevice()->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);



	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	D3D::GetDevice()->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);



	viewport.Width =  (float)this->width;
	viewport.Height = (float)this->height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;


	D3DXMatrixPerspectiveFovLH(&projectionMatrix, ((float)D3DX_PI / 4.0f), ((float)this->width / (float)this->height), Camera::screenNear, Camera::screenDepth);
	D3DXMatrixOrthoLH(&orthoMatrix,
		(float)this->width, (float)this->height,
		Camera::screenNear, Camera::screenDepth
	);

}

RenderTexture::~RenderTexture()
{
	SAFE_RELEASE(resourceView);
	SAFE_RELEASE(renderView);
	SAFE_RELEASE(texture);
}

void RenderTexture::Clear(float r , float g , float b , float a )
{

	D3DXCOLOR color = D3DXCOLOR(r, g, b, a);
	D3D::GetDeviceContext()->ClearRenderTargetView(renderView, color);

	D3D::GetDeviceContext()->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);

}

void RenderTexture::SetTarget(bool ortho)
{

	D3D::GetDeviceContext()->OMSetRenderTargets(1,		&renderView, depthStencilView);
	D3D::GetDeviceContext()->RSSetViewports(1, &viewport);
	if(ortho)
		D3D::Get()->SetProjection(&orthoMatrix);
	else
		D3D::Get()->SetProjection(&projectionMatrix);
		
}

void RenderTexture::SaveTexture(wstring fileName)
{
	HRESULT hr = D3DX11SaveTextureToFile(
		D3D::GetDeviceContext(),
		texture,
		D3DX11_IFF_PNG,
		fileName.c_str()
	);
	assert(SUCCEEDED(hr));
}

void RenderTexture::ClearDepthStencil(UINT clearFlag, float depth, UINT8 stencil)
{
	D3D::GetDeviceContext()->ClearDepthStencilView(depthStencilView, clearFlag, depth, stencil);
}



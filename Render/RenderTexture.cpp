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


}

RenderTexture::~RenderTexture()
{
	for (UINT i = 0; i < subRT_count; i++){
		SAFE_RELEASE(resourceView[i]);
		SAFE_RELEASE(renderView[i]);
		SAFE_RELEASE(texture[i]);
	}
}


void RenderTexture::Initialize(UINT count)
{
	assert(count >= 1);
	subRT_count = count;


	HRESULT hr;
	texture = new ID3D11Texture2D*[count];
	renderView = new ID3D11RenderTargetView*[count];
	resourceView = new ID3D11ShaderResourceView*[count];


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


	for (UINT i = 0; i < subRT_count; i++) {
		hr = D3D::GetDevice()->CreateTexture2D(&textureDesc, NULL, &texture[i]);
		assert(SUCCEEDED(hr));
	}


	D3D11_RENDER_TARGET_VIEW_DESC renderDesc;
	ZeroMemory(&renderDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderDesc.Format = textureDesc.Format;
	renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderDesc.Texture2D.MipSlice = 0;

	for (UINT i = 0; i < subRT_count; i++) {
		hr = D3D::GetDevice()->CreateRenderTargetView(texture[i], &renderDesc, &renderView[i]);
		assert(SUCCEEDED(hr));
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	viewDesc.Format = textureDesc.Format;
	viewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;

	for (UINT i = 0; i < subRT_count; i++) {
		hr = D3D::GetDevice()->CreateShaderResourceView(texture[i], &viewDesc, &resourceView[i]);
		assert(SUCCEEDED(hr));
	}


	D3D11_TEXTURE2D_DESC depthBufferDesc;


	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = this->width;
	depthBufferDesc.Height = this->height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;


	//shadowMapDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	//shadowMapDesc.ArraySize = count;
	//shadowMapDesc.MipLevels = 1;
	//shadowMapDesc.SampleDesc.Count = 1;
	//shadowMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	//shadowMapDesc.Width = this->width;
	//shadowMapDesc.Height = this->height;


	hr = D3D::GetDevice()->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	assert(SUCCEEDED(hr));


	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = D3D::GetDevice()->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	assert(SUCCEEDED(hr));



	ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	viewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	viewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;

	hr = D3D::GetDevice()->CreateShaderResourceView(depthStencilBuffer, &viewDesc, &dbResourceView);
	assert(SUCCEEDED(hr));



	viewport.Width = (float)this->width;
	viewport.Height = (float)this->height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;



	D3DXMatrixOrthoLH(&orthoMatrix,
		(float)this->width, (float)this->height,
		Camera::screenNear, Camera::screenDepth
	);

	D3DXMatrixPerspectiveFovLH(&projectionMatrix, ((float)D3DX_PI / 4.0f), ((float)this->width / (float)this->height), Camera::screenNear, Camera::screenDepth);

}

void RenderTexture::IntializeShadowTexture(UINT count)
{
	assert(count >= 1);
	subRT_count = 1;


	HRESULT hr;
	resourceView = new ID3D11ShaderResourceView*[1];

	D3D11_TEXTURE2D_DESC shadowMapDesc;
	ZeroMemory(&shadowMapDesc, sizeof(D3D11_TEXTURE2D_DESC));
	shadowMapDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	shadowMapDesc.ArraySize = count;
	shadowMapDesc.MipLevels = 1;
	shadowMapDesc.SampleDesc.Count = 1;
	shadowMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	shadowMapDesc.Width = this->width;
	shadowMapDesc.Height = this->height;
	hr = D3D::GetDevice()->CreateTexture2D(&shadowMapDesc, NULL, &depthStencilBuffer);
	assert(SUCCEEDED(hr));


	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	depthStencilViewDesc.Texture2DArray.ArraySize = count;


	hr = D3D::GetDevice()->CreateDepthStencilView(
		depthStencilBuffer,
		&depthStencilViewDesc,
		&depthStencilView
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.Texture2DArray.ArraySize = count;
	shaderResourceViewDesc.Texture2DArray.MipLevels = 1;

	hr = D3D::GetDevice()->CreateShaderResourceView(
		depthStencilBuffer,
		&shaderResourceViewDesc,
		&resourceView[0]
	);





	renderView = nullptr;	
	subRT_count = 0;
	viewport.Width = (float)this->width;
	viewport.Height = (float)this->height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	D3DXMatrixOrthoLH(&orthoMatrix,
		(float)this->width, (float)this->height,
		Camera::screenNear, Camera::screenDepth
	);
	//orthoMatrix._33 /= Camera::screenDepth;
	//orthoMatrix._43 /= Camera::screenDepth;

	D3DXMatrixPerspectiveFovLH(&projectionMatrix, ((float)D3DX_PI / 4.0f), ((float)this->width / (float)this->height), Camera::screenNear, Camera::screenDepth);
	//projectionMatrix._33 /= Camera::screenDepth;
	//projectionMatrix._43 /= Camera::screenDepth;

}

void RenderTexture::Clear(float r , float g , float b , float a )
{

	D3DXCOLOR color = D3DXCOLOR(r, g, b, a);
	for (UINT i = 0; i < subRT_count; i++) {
		D3D::GetDeviceContext()->ClearRenderTargetView(renderView[i], color);
	}
	D3D::GetDeviceContext()->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);

}

void RenderTexture::SetTarget(bool ortho)
{

	D3D::GetDeviceContext()->OMSetRenderTargets(subRT_count, renderView, depthStencilView);


	D3D::GetDeviceContext()->RSSetViewports(1, &viewport);
	if(ortho)
		D3D::Get()->SetProjection(&orthoMatrix);
	else
		D3D::Get()->SetProjection(&projectionMatrix);
		
}

void RenderTexture::SaveTexture(wstring fileName, UINT i)
{
	HRESULT hr = D3DX11SaveTextureToFile(
		D3D::GetDeviceContext(),
		texture[i],
		D3DX11_IFF_PNG,
		fileName.c_str()
	);
	assert(SUCCEEDED(hr));

}


void RenderTexture::ClearDepthStencil(UINT clearFlag, float depth, UINT8 stencil)
{
	D3D::GetDeviceContext()->ClearDepthStencilView(depthStencilView, clearFlag, depth, stencil);
}



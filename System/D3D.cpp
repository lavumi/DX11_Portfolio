#include "../stdafx.h"
#include "D3D.h"


D3D* D3D::instance = NULL;
ID3D11Device* D3D::device = NULL;
ID3D11DeviceContext* D3D::deviceContext = NULL;
D3DInfo D3D::d3dInfo;

D3D * D3D::Get()
{
	if (instance == NULL)
		instance = new D3D();

	return instance;
}

void D3D::Delete()
{
	SAFE_DELETE(instance);
	
}

void D3D::BeginScene()
{
	deviceContext->ClearRenderTargetView(defaultRenderView, d3dInfo.clearColor);
	deviceContext->ClearDepthStencilView(defaultDepthView, D3D11_CLEAR_DEPTH, 1, 0);
}

void D3D::EndScene()
{
	if (isVsync == true)
		swapChain->Present(1, 0);
	else
		swapChain->Present(0, 0);
}
	
D3D::D3D()
{
	version = D3D_FEATURE_LEVEL_11_0;
	isVsync = d3dInfo.isVsync;

	numerator = 0;
	denominator = 1;
	
	
	CreateAdapter();
	CreateSwapChain();

	CreateDefaultDepthStencilTexture();


	CreateDefaultRenderTarget();
	CreateDefaultDepthView();


	CreateDepthStencil();

	CreateBlenders();



	viewport.Width = (float)d3dInfo.screenWidth;
	viewport.Height = (float)d3dInfo.screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	D3D::GetDeviceContext()->RSSetViewports(1, &viewport);
	
	screenAspect = (float)d3dInfo.screenWidth / (float)d3dInfo.screenHeight;
	
	D3DXMatrixPerspectiveFovLH(&defaultProjection, (float)D3DX_PI/4, screenAspect, Camera::screenNear, Camera::screenDepth);

	D3DXMatrixOrthoLH(&defaultOrthoProjection,
		(float)d3dInfo.screenWidth, (float)d3dInfo.screenHeight,
		Camera::screenNear, Camera::screenDepth
	);




}

D3D::~D3D()
{
	if (swapChain != NULL)
		swapChain->SetFullscreenState(false, NULL);

	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(device);
	SAFE_RELEASE(swapChain);

}

void D3D::CreateAdapter()
{
	HRESULT hr;

	IDXGIFactory* factory;
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&factory);
	assert(SUCCEEDED(hr));

	IDXGIAdapter* adapter;
	hr = factory->EnumAdapters(0, &adapter);
	assert(SUCCEEDED(hr));

	IDXGIOutput* adapterOutput;
	hr = adapter->EnumOutputs(0, &adapterOutput);
	assert(SUCCEEDED(hr));

	UINT modeCount;
	hr = adapterOutput->GetDisplayModeList
	(
		DXGI_FORMAT_R8G8B8A8_UNORM
		, DXGI_ENUM_MODES_INTERLACED
		, &modeCount
		, NULL
	);
	assert(SUCCEEDED(hr));

	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[modeCount];
	hr = adapterOutput->GetDisplayModeList
	(
		DXGI_FORMAT_R8G8B8A8_UNORM
		, DXGI_ENUM_MODES_INTERLACED
		, &modeCount
		, displayModeList
	);
	assert(SUCCEEDED(hr));

	for (UINT i = 0; i < modeCount; i++)
	{
		bool isCheck = true;
		isCheck &= displayModeList[i].Width == d3dInfo.screenWidth;
		isCheck &= displayModeList[i].Height == d3dInfo.screenHeight;

		if (isCheck == true)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	DXGI_ADAPTER_DESC adapterDesc;
	hr = adapter->GetDesc(&adapterDesc);
	assert(SUCCEEDED(hr));

	gpuMemorySize = (UINT)adapterDesc.DedicatedVideoMemory / 1024 / 1024;
	gpuDescription = adapterDesc.Description;

	delete[] displayModeList;
	SAFE_RELEASE(adapterOutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);
}

void D3D::CreateSwapChain()
{
	HRESULT hr;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = d3dInfo.screenWidth;
	swapChainDesc.BufferDesc.Height = d3dInfo.screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (isVsync == true)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = d3dInfo.mainHandle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !d3dInfo.isFullScreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	hr = D3D11CreateDeviceAndSwapChain
	(
		NULL
		, D3D_DRIVER_TYPE_HARDWARE
		, NULL
		, 0
		, &version
		, 1
		, D3D11_SDK_VERSION
		, &swapChainDesc
		, &swapChain
		, &device
		, NULL
		, &deviceContext
	);
	assert(SUCCEEDED(hr));
}





void D3D::SetDefaultRenderView()
{
	deviceContext->OMSetRenderTargets(1, &defaultRenderView, defaultDepthView);
	deviceContext->RSSetViewports(1, &viewport);
	SetProjection(&defaultProjection);
}





void D3D::ClearDepthStencil(UINT clearFlag, float depth, UINT8 stencil)
{
	deviceContext->ClearDepthStencilView(defaultDepthView, clearFlag, depth, stencil);
}





void D3D::CreateDefaultDepthStencilTexture()
{


	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = d3dInfo.screenWidth;
	desc.Height = d3dInfo.screenHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr;
	hr = device->CreateTexture2D(&desc, NULL, &defaultDepthTexture);
	assert(SUCCEEDED(hr));
}

void D3D::CreateDepthStencil()
{
	depthstencilstate = new ID3D11DepthStencilState*[10];


	D3D11_DEPTH_STENCIL_DESC desc = { 0 };
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;

	desc.StencilEnable = false;
	desc.StencilReadMask = 0xFF;
	desc.StencilWriteMask = 0xFF;

	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

	HRESULT hr;
	hr = device->CreateDepthStencilState(&desc, &depthstencilstate[0]);
	assert(SUCCEEDED(hr));

	deviceContext->OMSetDepthStencilState(depthstencilstate[0], 1);


	//스카이 
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	desc.StencilEnable = false;
	desc.StencilReadMask = 0xFF;
	desc.StencilWriteMask = 0xFF;

	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	hr = device->CreateDepthStencilState(&desc, &depthstencilstate[1]);
	assert(SUCCEEDED(hr));




	//반사평면
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_LESS;

	desc.StencilEnable = true;
	desc.StencilReadMask = 0xFF;
	desc.StencilWriteMask = 0xFF;


	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;



	hr = device->CreateDepthStencilState(&desc, &depthstencilstate[2]);
	assert(SUCCEEDED(hr));

	//반사평면 오브젝트
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;

	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	hr = device->CreateDepthStencilState(&desc, &depthstencilstate[3]);
	assert(SUCCEEDED(hr));


	//반사평면 하늘
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	hr = device->CreateDepthStencilState(&desc, &depthstencilstate[4]);
	assert(SUCCEEDED(hr));
}


void D3D::SetDepthStencilState(DS_state state)
{
	deviceContext->OMSetDepthStencilState(depthstencilstate[(int)state], 1);
}

//void D3D::CreateOffState()
//{
//	D3D11_DEPTH_STENCIL_DESC desc = { 0 };
//
//}
//
//void D3D::CreateMirrorStencilTest()
//{
//
//	D3D11_DEPTH_STENCIL_DESC desc = { 0 };
//
//	//D3D::GetDeviceContext()->OMSetDepthStencilState(onState, 1);
//}


void D3D::CreateDefaultRenderTarget()
{
	ID3D11Texture2D* backbufferPointer;
	HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backbufferPointer);
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView(backbufferPointer, NULL, &defaultRenderView);
	assert(SUCCEEDED(hr));
	SAFE_RELEASE(backbufferPointer);
}

void D3D::CreateDefaultDepthView()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;

	HRESULT hr = device->CreateDepthStencilView(defaultDepthTexture, &desc, &defaultDepthView);
	assert(SUCCEEDED(hr));

	
}








//////////////////////////////////////////////////////////////
//////////////////////////////블랜더//////////////////////////
/////////////////////////////////////////////////////////////
void D3D::CreateBlenders()
{
	d3dBlendState = new ID3D11BlendState*[6];


	D3D11_BLEND_DESC omDesc;
	ZeroMemory(&omDesc, sizeof(D3D11_BLEND_DESC));


	omDesc.RenderTarget[0].BlendEnable = true;
	omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	omDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D::GetDevice()->CreateBlendState(&omDesc, &d3dBlendState[0]);



	omDesc.RenderTarget[0].BlendEnable = false;
	D3D::GetDevice()->CreateBlendState(&omDesc, &d3dBlendState[1]);


	omDesc.RenderTarget[0].BlendEnable = true;
	omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	omDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;
	omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
	omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	
	D3D::GetDevice()->CreateBlendState(&omDesc, &d3dBlendState[2]);



	omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	omDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D::GetDevice()->CreateBlendState(&omDesc, &d3dBlendState[3]);



	omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_BLEND_FACTOR;
	omDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;
	omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D::GetDevice()->CreateBlendState(&omDesc, &d3dBlendState[4]);


	omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	omDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D::GetDevice()->CreateBlendState(&omDesc, &d3dBlendState[5]);
}



void D3D::SetBlender_Linear()
{
	deviceContext->OMSetBlendState(d3dBlendState[0], 0, 0xffffffff);
}

void D3D::SetBlender_MaxBlend()
{
	deviceContext->OMSetBlendState(d3dBlendState[2], 0, 0xffffffff);
}

void D3D::SetBlender_Off()
{
	deviceContext->OMSetBlendState(d3dBlendState[1], 0, 0xffffffff);
}

void D3D::SetBlender_None()
{
	deviceContext->OMSetBlendState(d3dBlendState[3], 0, 0xffffffff);
}

void D3D::SetBlender_AddBlend()
{
	deviceContext->OMSetBlendState(d3dBlendState[5], 0, 0xffffffff);
}

void D3D::SetBlender_BlendFacter(const float factor)
{
	deviceContext->OMSetBlendState(d3dBlendState[4], &factor, 0xffffffff);
}
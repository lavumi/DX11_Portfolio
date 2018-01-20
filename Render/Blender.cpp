#include "../stdafx.h"
#include "Blender.h"

Blender* Blender::instance = NULL;

Blender::Blender()
{
	d3dBlendState = new ID3D11BlendState*[4];


	D3D11_BLEND_DESC omDesc;
	ZeroMemory(&omDesc, sizeof(D3D11_BLEND_DESC));
	//omDesc.AlphaToCoverageEnable = true;
	//omDesc.IndependentBlendEnable = true;
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
}

Blender::~Blender()
{

}

Blender * Blender::Get()
{
	if (instance == NULL)
		instance = new Blender();

	return instance;
}

void Blender::Delete()
{
	SAFE_DELETE(instance);
}

void Blender::SetLinear()
{
	D3D::GetDeviceContext()->OMSetBlendState(d3dBlendState[0], 0, 0xffffffff);
}

void Blender::SetMaxBlender()
{
	D3D::GetDeviceContext()->OMSetBlendState(d3dBlendState[2], 0, 0xffffffff);
}

void Blender::SetOff()
{
	D3D::GetDeviceContext()->OMSetBlendState(d3dBlendState[1], 0, 0xffffffff);
}

void Blender::SetNone()
{
	D3D::GetDeviceContext()->OMSetBlendState(d3dBlendState[3], 0, 0xffffffff);
}



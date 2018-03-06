#include "DepthShadowShader.h"
#include "LightViewBuffer.h"

DepthShadowShader::DepthShadowShader()
	:Shader(L"./Shader/FX/LightDepthShader.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);
	buffer = new LightViewBuffer();

}

DepthShadowShader::~DepthShadowShader()
{
	SAFE_DELETE(buffer);
}

void DepthShadowShader::Update()
{

}

void DepthShadowShader::Render(UINT indexCount, D3DXMATRIX world)
{
	buffer->SetWorld(world);
	buffer->SetBuffers();

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}


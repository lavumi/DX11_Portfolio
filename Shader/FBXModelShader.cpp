#include "FBXModelShader.h"

#include "BaseBuffer.h"

FBXModelShader::FBXModelShader()
	:Shader(L"./Shader/FX/FBXModelShader.fx")
{
	CreateInputLayout(VertexTextureNormalTangentBlend::desc, VertexTextureNormalTangentBlend::count);

	buffer = new BaseBuffer();
	CreateBuffers();
}

FBXModelShader::~FBXModelShader()
{
}

void FBXModelShader::Update()
{

}

void FBXModelShader::Render(UINT indexCount, D3DXMATRIX world)
{

	buffer->SetWorld(world);
	buffer->SetBuffers();
	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	

	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}

void FBXModelShader::CreateBuffers()
{

}

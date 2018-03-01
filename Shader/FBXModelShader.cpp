#include "FBXModelShader.h"

FBXModelShader::FBXModelShader()
	:Shader(L"./Shader/FX/FBXModelShader.fx")
{
	CreateInputLayout(VertexTextureNormalTangentBlend::desc, VertexTextureNormalTangentBlend::count);

	CreateBuffers();
}

FBXModelShader::~FBXModelShader()
{
}

void FBXModelShader::Update()
{

}

void FBXModelShader::Render(UINT indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection)
{

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	SetMatrix(world, view, projection);

	D3D::GetDeviceContext()->VSSetConstantBuffers(10, 1, &wBuffer);
	D3D::GetDeviceContext()->VSSetConstantBuffers(11, 1, &vpBuffer);

	//if(diffuse != nullptr)
	//	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &diffuse);

	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}

void FBXModelShader::CreateBuffers()
{

}

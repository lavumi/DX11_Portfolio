#include "FBXModelShader.h"

#include "BaseBuffer.h"

FBXModelShader::FBXModelShader()
	:Shader(L"./Shader/FX/FBXModelShader.fx")
{
	CreateInputLayout(VertexTextureNormalTangentBlend::desc, VertexTextureNormalTangentBlend::count);
}

FBXModelShader::~FBXModelShader()
{
}

void FBXModelShader::Update()
{

}

void FBXModelShader::Render()
{

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	

	
}



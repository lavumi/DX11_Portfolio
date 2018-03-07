#include "DepthShadowShader.h"


DepthShadowShader::DepthShadowShader()
	:Shader(L"./Shader/FX/LightDepthShader.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);


}

DepthShadowShader::~DepthShadowShader()
{
}

void DepthShadowShader::Update()
{

}

void DepthShadowShader::Render()
{


	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	
}


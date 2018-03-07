#include "ShadowShader.h"

ShadowShader::ShadowShader()
	:Shader(L"./Shader/FX/Shadow.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);


}

ShadowShader::~ShadowShader()
{


}

void ShadowShader::Update()
{

}

void ShadowShader::Render()
{



	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);
}


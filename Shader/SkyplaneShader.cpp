#include "SkyplaneShader.h"


SkyplaneShader::SkyplaneShader()
	:Shader(L"./Shader/FX/SkyplaneShader.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);




	
}

SkyplaneShader::~SkyplaneShader()
{

}

void SkyplaneShader::Update()
{

}

void SkyplaneShader::Render()
{
	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

}



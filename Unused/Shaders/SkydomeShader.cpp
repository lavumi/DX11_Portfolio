#include "SkydomeShader.h"

SkydomeShader::SkydomeShader()
	:Shader(L"./Shader/FX/Skydome.fx")
{
	CreateInputLayout(VertexTextureNormal::desc, VertexTextureNormal::count);


}

SkydomeShader::~SkydomeShader()
{

}

void SkydomeShader::Update()
{

}

void SkydomeShader::Render()
{

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);


		
}


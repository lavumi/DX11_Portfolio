#include "NormalMapShader.h"


NormalMapShader::NormalMapShader()
	:Shader(L"./Shader/FX/NormalMap.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);


}

NormalMapShader::~NormalMapShader()
{
}

void NormalMapShader::Update()
{

}

void NormalMapShader::Render()
{
	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);
}

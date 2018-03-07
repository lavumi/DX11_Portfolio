#include "SkydomeShader.h"

#include "SkydomeBuffer.h"
SkydomeShader::SkydomeShader()
	:Shader(L"./Shader/FX/Skydome.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);


	buffer = new SkydomeBuffer();

}

SkydomeShader::~SkydomeShader()
{
	SAFE_DELETE(buffer);
}

void SkydomeShader::Update()
{

}

void SkydomeShader::Render()
{
	//buffer->SetWorld(world);
	buffer->SetBuffers();

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);


		
}


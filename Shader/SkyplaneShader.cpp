#include "SkyplaneShader.h"

#include "SkyplaneBuffer.h"
SkyplaneShader::SkyplaneShader()
	:Shader(L"./Shader/FX/SkyplaneShader.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);


	buffer = new SkyplaneBuffer();

	
}

SkyplaneShader::~SkyplaneShader()
{
	SAFE_DELETE(buffer);
}

void SkyplaneShader::Update()
{

}

void SkyplaneShader::Render(UINT indexCount, D3DXMATRIX world,
	ID3D11ShaderResourceView* perlin)
{
	buffer->SetWorld(world);
	buffer->SetBuffers();

	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &perlin);

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);







	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}



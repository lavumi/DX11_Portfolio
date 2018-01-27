#include "InstanceTextureShader.h"

InstanceTextureShader::InstanceTextureShader()
	:Shader(L"./Shader/FX/InstanceTextureShader.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);

	CreateBuffers();
}

InstanceTextureShader::~InstanceTextureShader()
{
}

void InstanceTextureShader::Update()
{

}

void InstanceTextureShader::Render(UINT indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* diffuse)
{

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	SetMatrix(world, view, projection);

	D3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &wvpBuffer);

	if(diffuse != nullptr)
		D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &diffuse);

	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}

void InstanceTextureShader::CreateBuffers()
{

}

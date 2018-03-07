#include "RainShader.h"

#include "RainBuffer.h"
RainShader::RainShader()
	:Shader(L"./Shader/FX/RainShader.fx")
{
	CreateInputLayout(VertexTexture3::desc, VertexTexture3::count);
	buffer = new RainBuffer();

}

RainShader::~RainShader()
{

}

void RainShader::Update()
{

}

void RainShader::Render(UINT indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* depthMap, D3DXCOLOR color)
{
	buffer->SetWorld(world);
	buffer->SetBuffers();


	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &depthMap);

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);



	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}


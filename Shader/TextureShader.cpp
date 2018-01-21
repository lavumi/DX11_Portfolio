#include "TextureShader.h"

TextureShader::TextureShader()
	:Shader(L"./Shader/FX/TextureShader.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);

	CreateBuffers();
}

TextureShader::~TextureShader()
{
}

void TextureShader::Update()
{

}

void TextureShader::Render(UINT indexCount, D3DXMATRIX world1, ID3D11ShaderResourceView* diffuse)
{

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	cameraBuffer->Update();
	cameraBuffer->SetVSBuffer(0);

	worldBuffer->SetWorld(world1);
	worldBuffer->SetVSBuffer(1);

	if(diffuse != nullptr)
		D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &diffuse);

	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}

void TextureShader::CreateBuffers()
{

}

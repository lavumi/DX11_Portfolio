#include "TextureShader.h"

#include "BaseBuffer.h"
TextureShader::TextureShader()
	:Shader(L"./Shader/FX/TextureShader.fx")
{
	CreateInputLayout(VertexTexture::desc, VertexTexture::count);

	buffer = new BaseBuffer();
}

TextureShader::~TextureShader()
{
}

void TextureShader::Update()
{

}

void TextureShader::Render(ID3D11ShaderResourceView* diffuse)
{

	//buffer->SetWorld(world);
	buffer->SetBuffers();


	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);


	if(diffuse != nullptr)
		D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &diffuse);

	//D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}


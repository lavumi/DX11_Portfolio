#include "ShadowShader.h"
#include "ShadowBuffer.h"

ShadowShader::ShadowShader()
	:Shader(L"./Shader/FX/Shadow.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);

	buffer = new ShadowBuffer();
	//CreateBuffers();

}

ShadowShader::~ShadowShader()
{
	SAFE_DELETE(buffer);
	//SAFE_RELEASE(LightBuffer);
}

void ShadowShader::Update()
{

}

void ShadowShader::Render(ID3D11ShaderResourceView * lightMap)
{
	buffer->SetBuffers();
	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &lightMap);


	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);
}

//void ShadowShader::CreateBuffers()
//{
//	D3D11_BUFFER_DESC desc;
//	HRESULT hr;
//
//	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
//	desc.Usage = D3D11_USAGE_DYNAMIC;
//	desc.ByteWidth = sizeof(LightManager::LightData);
//	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	desc.MiscFlags = 0;
//	desc.StructureByteStride = 0;
//
//	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &LightBuffer);
//	assert(SUCCEEDED(hr));
//
//}

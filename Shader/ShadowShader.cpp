#include "ShadowShader.h"

ShadowShader::ShadowShader()
	:Shader(L"./Shader/FX/Shadow.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);

	CreateBuffers();

}

ShadowShader::~ShadowShader()
{
}

void ShadowShader::Update()
{

}

void ShadowShader::Render(UINT indexCount, D3DXMATRIX world, ID3D11ShaderResourceView * lightMap)
{
	D3D11_MAPPED_SUBRESOURCE subResource = { 0 };

	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, LightManager::Get()->GetLightData(), sizeof(LightManager::LightData));

	D3D::GetDeviceContext()->Unmap(LightBuffer, 0);




	cameraBuffer->Update();
	cameraBuffer->SetVSBuffer(0);

	worldBuffer->SetWorld(world);
	worldBuffer->SetVSBuffer(1);
	D3D::GetDeviceContext()->VSSetConstantBuffers(2, 1, &LightBuffer);


	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &lightMap);

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);



	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
	Sampler::Get()->SetDefault();

}

void ShadowShader::CreateBuffers()
{
	D3D11_BUFFER_DESC desc;
	HRESULT hr;

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(LightManager::LightData);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &LightBuffer);
	assert(SUCCEEDED(hr));

}

#include "DepthShadowShader.h"

DepthShadowShader::DepthShadowShader()
	:Shader(L"./Shader/FX/LightDepthShader.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);

	CreateBuffers();
}

DepthShadowShader::~DepthShadowShader()
{
	SAFE_RELEASE(lightBuffer);
}

void DepthShadowShader::Update()
{

}

void DepthShadowShader::Render(UINT indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection)
{
	SetMatrix(world, view, projection);

	D3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &wvpBuffer);

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);



	
	D3D11_MAPPED_SUBRESOURCE subResource = { 0 };


	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, LightManager::Get()->GetLightData(), sizeof(LightManager::LightData));
	D3D::GetDeviceContext()->Unmap(lightBuffer, 0);

	D3D::GetDeviceContext()->VSSetConstantBuffers(2, 1, &lightBuffer);



	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}

void DepthShadowShader::CreateBuffers()
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

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &lightBuffer);
	assert(SUCCEEDED(hr));
}

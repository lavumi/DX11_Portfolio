#include "SkyplaneShader.h"

SkyplaneShader::SkyplaneShader()
	:Shader(L"./Shader/FX/SkyplaneShader.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);

	CreateBuffers();

	data.brightness = 0.5f;
	data.scale = 0.3f;
	data.translation = 0.0f;

	speed = 0.0001f;
	UserInterface::AddSkyplane(&data.scale, &data.brightness, &speed);
}

SkyplaneShader::~SkyplaneShader()
{
}

void SkyplaneShader::Update()
{

}

void SkyplaneShader::Render(UINT indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* diffuse, ID3D11ShaderResourceView* perlin)
{
	data.translation += speed;
	if (data.translation >= 1.0f) {
		data.translation -= 1.0f;
	}



	D3D11_MAPPED_SUBRESOURCE subResource = { 0 };


	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		skyplaneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &data, sizeof(SkyplaneData));
	D3D::GetDeviceContext()->Unmap(skyplaneBuffer, 0);



	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	SetMatrix(world, view, projection);

	D3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &wvpBuffer);



	D3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &skyplaneBuffer);
	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &diffuse);
	D3D::GetDeviceContext()->PSSetShaderResources(1, 1, &perlin);

	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}

void SkyplaneShader::CreateBuffers()
{
	D3D11_BUFFER_DESC desc;
	HRESULT hr;

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(SkyplaneData);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &skyplaneBuffer);
	assert(SUCCEEDED(hr));
}

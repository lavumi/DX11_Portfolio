#include "SkydomeShader.h"

SkydomeShader::SkydomeShader()
	:Shader(L"./Shader/FX/Skydome.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);


	data.apex = D3DXCOLOR(0.0f, 0.05f, 0.6f, 1.0f);
	data.center = D3DXCOLOR(0.0f, 0.5f, 0.8f, 1.0f);

	UserInterface::AddSkydome(&data.apex, &data.center);

	CreateBuffers();
}

SkydomeShader::~SkydomeShader()
{
}

void SkydomeShader::Update()
{

}

void SkydomeShader::Render(UINT indexCount, D3DXMATRIX world)
{
	D3D11_MAPPED_SUBRESOURCE subResource = { 0 };

	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &data, sizeof(Data));
	D3D::GetDeviceContext()->Unmap(buffer, 0);


	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	cameraBuffer->Update();
	cameraBuffer->SetVSBuffer(0);


	worldBuffer->SetWorld(world);
	worldBuffer->SetVSBuffer(1);

	D3D::GetDeviceContext()->PSSetConstantBuffers(1, 1, &buffer);


		D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);

	
}

void SkydomeShader::CreateBuffers()
{
	D3D11_BUFFER_DESC desc;
	HRESULT hr;

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(Data);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer);
	assert(SUCCEEDED(hr));
}

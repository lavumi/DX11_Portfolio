#include "ColorShader.h"

ColorShader::ColorShader()
	:Shader(L"./Shader/FX/ColorShader.fx")
{
	CreateInputLayout(VertexTexture::desc, VertexTexture::count);

	CreateBuffers();
}

ColorShader::~ColorShader()
{
}

void ColorShader::Update()
{

}

void ColorShader::Render(UINT indexCount, D3DXMATRIX world1, D3DXCOLOR color)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &color, sizeof(D3DXCOLOR));

	D3D::GetDeviceContext()->Unmap(colorBuffer, 0);



	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &colorBuffer);

	cameraBuffer->Update();
	cameraBuffer->SetVSBuffer(0);

	worldBuffer->SetWorld(world1);
	worldBuffer->SetVSBuffer(1);



	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}

void ColorShader::CreateBuffers()
{
	D3D11_BUFFER_DESC desc;
	HRESULT hr;

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(D3DXCOLOR);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &colorBuffer);
	assert(SUCCEEDED(hr));
}

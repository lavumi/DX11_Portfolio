#include "ColorShader.h"

ColorShader::ColorShader()
	:Shader(L"./Shader/FX/ColorShader.fx")
{
	CreateInputLayout(VertexTexture::desc, VertexTexture::count);

	CreateBuffers();
}

ColorShader::~ColorShader()
{
	SAFE_RELEASE(colorBuffer);
}

void ColorShader::Update()
{

}

void ColorShader::Render(UINT indexCount)
{

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);


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

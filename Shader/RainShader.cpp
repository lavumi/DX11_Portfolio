#include "RainShader.h"

RainShader::RainShader()
	:Shader(L"./Shader/FX/RainShader.fx")
{
	CreateInputLayout(VertexTexture::desc, VertexTexture::count);

	CreateBuffers();
}

RainShader::~RainShader()
{
	SAFE_RELEASE(colorBuffer);
}

void RainShader::Update()
{

}

void RainShader::Render(UINT indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* depthMap, D3DXCOLOR color)
{
	SetMatrix(world, view, projection);

	DWORD time = timeGetTime();

	//time %= 5000;
	float timer = (float)(time) / 1000;


	D3DXCOLOR inputData = color;

	inputData.a = timer;

	D3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &wvpBuffer);

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &inputData, sizeof(D3DXCOLOR));

	D3D::GetDeviceContext()->Unmap(colorBuffer, 0);


	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, D3D::Get()->GetBackBufferSubRenderTexture(0));

	if (Keyboard::Get()->KeyUp(VK_SPACE)) {
		D3D::Get()->TestMultiTexture(0);
	}
	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &colorBuffer);


	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}

void RainShader::CreateBuffers()
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

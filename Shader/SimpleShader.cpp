#include "SimpleShader.h"

SimpleShader::SimpleShader()
	:Shader(L"./Shader/FX/SimpleShader.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);

	CreateBuffers();
}

SimpleShader::~SimpleShader()
{
}

void SimpleShader::Update()
{

}

void SimpleShader::Render(UINT indexCount, D3DXMATRIX world1, ID3D11ShaderResourceView* diffuse)
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

void SimpleShader::CreateBuffers()
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

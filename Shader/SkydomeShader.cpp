#include "SkydomeShader.h"

SkydomeShader::SkydomeShader()
	:Shader(L"./Shader/FX/Skydome.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);





	CreateBuffers();
}

SkydomeShader::~SkydomeShader()
{
	SAFE_RELEASE(LightBuffer);
}

void SkydomeShader::Update()
{

}

void SkydomeShader::Render(UINT indexCount, D3DXMATRIX world,D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 lightDir)
{
	D3DXVECTOR3 lightDir;

	LightManager::Get()->GetLightDirection(&lightDir);

	float lightCos = -D3DXVec3Dot(&lightDir, &D3DXVECTOR3(0, 1, 0));




	SetMatrix(world, view, projection);

	D3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &wvpBuffer);


	D3D11_MAPPED_SUBRESOURCE subResource = { 0 };




	

	//LightManager::Get()->GetLightDirection(&lightDir);
	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &lightDir, sizeof(D3DXVECTOR4));
	D3D::GetDeviceContext()->Unmap(LightBuffer, 0);



	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);


	D3D::GetDeviceContext()->VSSetConstantBuffers(1, 1, &LightBuffer);

	


	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);

	
}

void SkydomeShader::CreateBuffers()
{
	D3D11_BUFFER_DESC desc;
	HRESULT hr;



	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(D3DXVECTOR4);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &LightBuffer);
	assert(SUCCEEDED(hr));
}

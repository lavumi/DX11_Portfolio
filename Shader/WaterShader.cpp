#include "WaterShader.h"
#include "WaterBuffer.h"

WaterShader::WaterShader() 
	:Shader(L"./Shader/FX/WaterShader.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);

	buffer = new WaterBuffer();
	//CreateBuffers();
	//
	//
	//
	//material.ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	//material.diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	//material.specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//material.shininess = 200;
	//material.globalAmbient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	//
	//
	//angle = 90;
	//speed = 0.001f;
	//
	//waterData.translation = 0;
	//waterData.scale = 20;
	//waterData.angle =  (float)D3DX_PI / 180 * angle;
	//
	//
	//UserInterface::AddWave(&speed, &waterData.scale, &angle);
}


WaterShader::~WaterShader()
{

	SAFE_DELETE(buffer);
	//SAFE_RELEASE(LightBuffer);
	//SAFE_RELEASE(ExtraBuffer);
	//SAFE_RELEASE(MaterialBuffer);
}

void WaterShader::Update()
{

}
void WaterShader::Render(UINT indexCount, D3DXMATRIX world, 
	ID3D11ShaderResourceView * normalMap, ID3D11ShaderResourceView* perlin, ID3D11ShaderResourceView * reflectionTexture, ID3D11ShaderResourceView * refractionTexture)
{
	buffer->SetWorld(world);
	buffer->SetBuffers();

	D3D::GetDeviceContext()->PSSetShaderResources(1, 1, &normalMap);
	D3D::GetDeviceContext()->PSSetShaderResources(2, 1, &perlin);
	D3D::GetDeviceContext()->PSSetShaderResources(3, 1, &reflectionTexture);
	D3D::GetDeviceContext()->PSSetShaderResources(4, 1, &refractionTexture);

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);


	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
	Sampler::Get()->SetDefault();
}

/*
void WaterShader::Render(UINT indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
	ID3D11ShaderResourceView * normalMap, ID3D11ShaderResourceView* perlin, ID3D11ShaderResourceView * reflectionTexture, ID3D11ShaderResourceView * refractionTexture)
{
	SetMatrix(world, view, projection);

	D3D::GetDeviceContext()->VSSetConstantBuffers(10, 1, &wBuffer);
	D3D::GetDeviceContext()->VSSetConstantBuffers(11, 1, &vpBuffer);

	D3D11_MAPPED_SUBRESOURCE subResource = { 0 };
	D3DXMATRIX invWorld;
	D3DXMatrixInverse(&invWorld, nullptr, &world);

	D3DXMatrixTranspose(&extraData.invTransWorld, &invWorld);
	

	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		ExtraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &extraData, sizeof(ExtraData));
	D3D::GetDeviceContext()->Unmap(ExtraBuffer, 0);

	
	D3DXVECTOR3 cameraPos;
	Camera::Get()->GetPosition(&cameraPos);

	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		cameraPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &cameraPos, sizeof(D3DXVECTOR4));

	D3D::GetDeviceContext()->Unmap(cameraPosBuffer, 0);


	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		MaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &material, sizeof(MaterialData));

	D3D::GetDeviceContext()->Unmap(MaterialBuffer, 0);



	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, LightManager::Get()->GetLightData(), sizeof(LightManager::LightData));

	D3D::GetDeviceContext()->Unmap(LightBuffer, 0);




	waterData.translation += speed;
	waterData.angle = (float)D3DX_PI / 180 * angle;
	//if (waterData.translation >= 1.0f) {
	//	waterData.translation -= 1.0f;
	//}
	


	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		waterBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &waterData, sizeof(WaterData));

	D3D::GetDeviceContext()->Unmap(waterBuffer, 0);





	
	D3D::GetDeviceContext()->PSSetShaderResources(1, 1, &normalMap);
	D3D::GetDeviceContext()->PSSetShaderResources(2, 1, &perlin);
	D3D::GetDeviceContext()->PSSetShaderResources(3, 1, &reflectionTexture);
	D3D::GetDeviceContext()->PSSetShaderResources(4, 1, &refractionTexture);

	D3D::GetDeviceContext()->VSSetConstantBuffers(1, 1, &cameraPosBuffer);
	D3D::GetDeviceContext()->VSSetConstantBuffers(2, 1, &LightBuffer);
	D3D::GetDeviceContext()->VSSetConstantBuffers(3, 1, &ExtraBuffer);

	D3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &MaterialBuffer);
	D3D::GetDeviceContext()->PSSetConstantBuffers(1, 1, &waterBuffer);

	
	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);


	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
	Sampler::Get()->SetDefault();

}

void WaterShader::CreateBuffers()
{
	D3D11_BUFFER_DESC desc;
	HRESULT hr;
	
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(ExtraData);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &ExtraBuffer);
	assert(SUCCEEDED(hr));



	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(MaterialData);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &MaterialBuffer);
	assert(SUCCEEDED(hr));


	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(LightManager::LightData);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &LightBuffer);
	assert(SUCCEEDED(hr));


	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(D3DXVECTOR4);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &cameraPosBuffer);
	assert(SUCCEEDED(hr));


	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(WaterData);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &waterBuffer);
	assert(SUCCEEDED(hr));

}
*/